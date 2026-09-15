
# Sh25-STB

A collection of single header libraries for c.

## Libraries

- **sh25_stack**: A stack implementation using dynamic memory allocation.
- **sh25_utest**: A simple unit testing framework for c.

## Usage

```c
#define SH25_STACK_IMPLEMENTATION
#include "sh25_stack.h"
```

Include the header where using the library and define the implementation macro
only once.


## Example

```c
#define SH25_STACK_IMPLEMENTATION
#include "sh25_stack.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    stack_t stack = {0};
    stack_init(stack, sizeof(int));
    int a = 10;
    stack_push(stack, a);
    int value;
    stack_peek(stack, value);
    printf("Top value: %d\n", value);
    stack_drop(stack);
    stack_destroy(stack);
    return 0;
}
```

