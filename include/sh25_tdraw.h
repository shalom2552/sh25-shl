/*
 * tdraw - singl-header terminal drawing library
 *
 * this library provides small api to draw simple text and boxes using ANSI escape sequences.
 *
 * Usage:
 *   Include this header in any file where you want to use it: #include "sh25_tdraw.h"
 *   Define the implementation once in your project: #define SH25_TDRAW_IMPL
 *   If the project already handles sig exit just add reset call there,
 *     otherwise you can use this library handler by defining TDRAW_SIG_HANDLER
 *     once with the implementation macro: #define TDRAW_SIG_HANDLER
 *
 * API:
 *   tdraw_init                   initialize the terminal drawing library
 *   tdraw_delay                  sleep for a given number of milliseconds
 *   tdraw_clear                  clears the screen
 *   tdraw_clear_line             clears a given line
 *   tdraw_draw_at                draws text at the specified position
 *   tdraw_draw_centered_line     draws a centered line at the specified position
 *   tdraw_draw_frame             draw a frame around two edges
 *   tdraw_flush                  flush the output buffer
 *   tdraw_reset                  reset all styles and modes
 *   tdraw_set_color              draw an a color/style escape sequence
 *   tdraw_term_size              get current terminal height and width
 *   tdraw_term_size_ok           returns 1 if the terminal size is sufficient, 0 otherwise
 *
 * Example:
 *   ```c
 *   #define SH25_TDRAW_IMPL
 *   #define TDRAW_SIG_HANDLER
 *   #include "../include/sh25_tdraw.h"
 *   #include <stdio.h>
 *
 *   int main(void) {
 *       tdraw_init();
 *       int w; int h; tdraw_term_size(&h, &w);
 *       tdraw_draw_frame(1, 1, h, w, "< Title >");
 *       tdraw_draw_at(2, 2, "Hello, World!");
 *       tdraw_draw_centered_line(h/2, "This is a centered line.");
 *       tdraw_flush();
 *       getchar();
 *       return 0;
 *   }
 *  ```
 *
 * Notes:
 *   - The library uses ANSI escape codes to control terminal output.
 *   - The `TDRAW_SIG_HANDLER` macro is optional and can be used to handle terminal signals.
 *
 * author: shalom2552
 * date: 2026-09-27
 */
#ifndef SH25_TDRAW_H_1331063137
#define SH25_TDRAW_H_1331063137

#include <string.h>
void tdraw_init(void);
void tdraw_delay(int ms);
void tdraw_clear(void);
void tdraw_clear_line(int y);
void tdraw_draw_at(int y, int x, const char* format, ...);
void tdraw_draw_centered_line(int y, const char* format, ...);
void tdraw_draw_frame(int y1, int x1, int y2, int x2, char* title);
void tdraw_flush(void);
void tdraw_reset(void);
void tdraw_set_color(const char* c);
void tdraw_term_size(int* h, int* w);
int tdraw_term_size_ok(int req_h, int req_w);

#define SH25_TDRAW_IMPL
#ifdef SH25_TDRAW_IMPL

#include <ctype.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>

// === Functions ==============================================================

void tdraw_term_size(int* h, int* w) {
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    if (h) *h = ws.ws_row;
    if (w) *w = ws.ws_col;
}

void tdraw_flush(void) {
    printf("\033[?2026l"); // unlock output
    fflush(stdout);
    printf("\033[?2026h"); // relock output
}

void tdraw_delay(int ms) {
    tdraw_flush();
    struct timespec ts = {ms / 1000, ms % 1000 * 1000 * 1000};
    thrd_sleep(&ts, NULL);
}

// === Drawing ================================================================

void tdraw_clear(void) {
    printf("\033[H\033[J");
}

void tdraw_clear_line(int y) {
    printf("\033[%d;1H\033[2K", y);
}

void tdraw_set_color(const char* c) {
    printf("%s", c);
}

void tdraw_draw_at(int y, int x, const char* format, ...) {
    char buf[256]; va_list ap;
    va_start(ap, format); vsnprintf(buf, sizeof(buf), format, ap); va_end(ap);
    printf("\033[%d;%dH%s", y, x, buf);
}

void tdraw_draw_centered_line(int y, const char* format, ...) {
    char buf[256]; va_list ap;
    va_start(ap, format); vsnprintf(buf, sizeof(buf), format, ap); va_end(ap);
    int w; tdraw_term_size(NULL, &w);
    int len = 0; for (char* p = buf; *p; p++) if (*p == '\033') { while (*p && !isalpha(*p)) p++; } else len += (*p & 0xC0) != 0x80;
    int x = (w - len) / 2 + 1;
    printf("\033[%d;%dH%s", y, x < 1 ? 1 : x, buf);
}

// === Boarder ================================================================

void tdraw_draw_frame(int y1, int x1, int y2, int x2, char* title) {
    for (int i = y1; i <= y2; ++i) {
        for (int j = x1 + 1; j < x2; ++j) {
            tdraw_draw_at(i, j, " ");
        }
        tdraw_draw_at(i, x1, "│");
        tdraw_draw_at(i, x2, "│");
    }
    for (int i = x1; i <= x2; ++i) {
        tdraw_draw_at(y1, i, "─");
        tdraw_draw_at(y2, i, "─");
    }
    tdraw_draw_at(y1, x1, "╭");
    tdraw_draw_at(y1, x2, "╮");
    tdraw_draw_at(y2, x1, "╰");
    tdraw_draw_at(y2, x2, "╯");
    int len = strlen(title);
    tdraw_draw_at(y1, (x2 - x1 - len) / 2, title);
}

// === Utils ==================================================================

int tdraw_term_size_ok(int req_h, int req_w) {
    int h; int w;
    tdraw_term_size(&h, &w);
    if (h < req_h || w < req_w) {
        tdraw_clear();
        tdraw_draw_centered_line(h / 2 - 1, "Terminal too small.");
        tdraw_draw_centered_line(h / 2 - 0, "Width = \033[3%dm%d\033[0m Height = \033[3%dm%d\033[0m",
                                w < req_w ? 1 : 2, w, h < req_h ? 1 : 2, h);
        tdraw_draw_centered_line(h / 2 + 2, "Needed:");
        tdraw_draw_centered_line(h / 2 + 3, "Width: %d Height: %d", req_w, req_h);
        return 0;
    }
    return 1;
}

// === Init & handlers ========================================================

static char _tdraw_buf[65536];
static int _tdraw_active = 0;

void tdraw_reset(void) {
    if (_tdraw_active) {
        printf("\033[?2026l\033[?25h\033[0m\033[?1049l");
        _tdraw_active = 0;
        fflush(stdout);
    }
}

#ifdef TDRAW_SIG_HANDLER
static void _tdraw_sig_handler(int sig) {
    (void)sig;
    tdraw_reset();
    exit(0);
}
#endif // TDRAW_SIG_HANDLER

void tdraw_init(void) {
#ifdef TDRAW_SIG_HANDLER
    signal(SIGINT, _tdraw_sig_handler);
    signal(SIGTERM, _tdraw_sig_handler);
#endif // TDRAW_SIG_HANDLER
    atexit(tdraw_reset);
    _tdraw_active = 1;
    setvbuf(stdout, _tdraw_buf, _IOFBF, sizeof(_tdraw_buf));
    printf("\033[?1049h\033[?25l\033[H\033[J\033[?2026h");
    fflush(stdout);
}

#endif // SH25_TDRAW_IMPL

#endif // !SH25_TDRAW_H_1331063137

