#define SH25_TDRAW_IMPL
#define TDRAW_SIG_HANDLER
#include "../include/sh25_tdraw.h"
#include <stdio.h>

int main(void) {
    tdraw_init();
    int w; int h; tdraw_term_size(&h, &w);
    tdraw_draw_frame(1, 1, h, w, "< Title >");
    tdraw_draw_at(2, 2, "Hello, World!");
    tdraw_draw_centered_line(h/2, "This is a centered line.");
    tdraw_flush();
    getchar();
    return 0;
}
