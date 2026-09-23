#define SH25_STACK_IMPL
#include "../include/sh25_stack.h"

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

static char* open = "([{";
static char* close = ")]}";

int matching(char open, char close) {
    return ((open == '(' && close == ')') || (open == '[' && close == ']') || (open == '{' && close == '}'));
}

int valid_parentheses(const char* str) {
    Stack s = {0};
    stack_init(s, sizeof(char));

    for (int i = 0; str[i] != '\0'; ++i) {

        if (strchr(open, str[i]) != NULL) {
            stack_push(s, str[i]);
        }

        if (strchr(close, str[i]) != NULL) {
            if (stack_empty(s)) return 0;

            char pop; stack_pop(s, pop);
            if (!matching(pop, str[i])) {
                return 0;
            }
        }
    }

    int result = stack_empty(s);
    stack_destroy(s);
    return result;
}

#ifndef NO_MAIN
typedef struct { int valid; char* str; } Case;
static Case cases[] = {
    { .valid = 1, .str = "" },
    { .valid = 0, .str = "(" },
    { .valid = 1, .str = "()" },
    { .valid = 0, .str = ")(" },
    { .valid = 1, .str = "([])" },
    { .valid = 0, .str = "([)]" },
    { .valid = 1, .str = "{([])}" },
    { .valid = 0, .str = "{([[])}]" },
    { .valid = 1, .str = "()()[()]{{()[()]}}" },
};

int main(void) {
    int n = sizeof(cases) / sizeof(Case);
    for (int i = 0; i < n; ++i) {
        int res = valid_parentheses(cases[i].str);
        char* actual = res ? "Valid" : "Invalid";
        char* expect = cases[i].valid ? "Valid" : "Invalid";
        char* pass = res == cases[i].valid ? "PASS" : "FAIL";
        printf("%s: got: %s, \twant: %s; \tcase: %s\n", pass, actual, expect, cases[i].str);
    }
    return 0;
}
#endif // ifndef NO_MAIN
