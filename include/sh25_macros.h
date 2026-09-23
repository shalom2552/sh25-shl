/*
 * sh25_macros.h - macros header
 *
 * This header provides usfull macros for development.
 *
 * Author: shalom2552
 * License: MIT
 */
#ifndef SH25_MACROS_H_aab4a25dc3054e1418e0d54d2c1343c1
#define SH25_MACROS_H_aab4a25dc3054e1418e0d54d2c1343c1

#include <stdio.h>
#include <stdlib.h>

// #define PRINT_POS

#if defined(__FILE_NAME__)
#define SH25__FILE __FILE_NAME__
#else
#define SH25__FILE __FILE__
#endif

#define PRINT_SRC_LOC fprintf(stderr, "\t%s:%d at %s()\n", SH25__FILE, __LINE__, __func__);

#define DUMP(what, fmt, ...)                                     \
do {                                                   \
    fprintf(stderr, "%s: "fmt"\n", what, ##__VA_ARGS__); \
    PRINT_SRC_LOC;                                     \
    abort();                                           \
} while (0)

#define TODO(fmt, ...)          DUMP("TODO", fmt, ##__VA_ARGS__)
#define UNCREACHABLE(fmt, ...)  DUMP("UNCREACHABLE", fmt, ##__VA_ARGS__)

#endif // SH25_MACROS_H_aab4a25dc3054e1418e0d54d2c1343c1

