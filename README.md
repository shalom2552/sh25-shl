
# Sh25-STB

A collection of single-header libraries for c.

## Libraries

- **sh25_stack**: A stack implementation using dynamic memory allocation.
- **sh25_utest**: A simple unit testing framework for C.

## Install

Download the headers you need into your project.

```Bash
BASE=shalom2552.github.io/sh25-stb/include
wget $BASE/sh25_stack.h
wget $BASE/sh25_utest.h
```

## Usage

```c
#define SH25_STACK_IMPLEMENTATION
#include "sh25_stack.h"
```

Include the header where using the library and define the implementation macro
only once.

## Stack

- Create a stack: `stack_t stack = {0};`
- Must call `stack_init` before any use.
- Call stack_destroy when done to free memory.

### API

- `stack_init(stack, item_size)`    initialize the stack with a given item size, must call before use.
- `stack_push(stack, item)`         Pushes an item to the stack, returns STACK_MEMORY_ERROR on failed allocation.
- `stack_pop(stack, pop)`           Pops into pop, returns STACK_EMPTY if empty.
- `stack_drop(stack)`               Pops without storing the value.
- `stack_peek(stack, peek)`         Copies top into peek, returns STACK_EMPTY if empty.
- `stack_size(stack)`               Returns the number of items in the stack.
- `stack_empty(stack)`              Returns 1 if the stack is empty, 0 otherwise.
- `stack_clear(stack)`              Clears the stack and keeps the allocated capacity.
- `stack_destroy(stack)`            Destroys the stack and frees all allocated memory.

### Example

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

## UTest

### API

- `TEST(name)`         Define a test function. Body implement with braces.
- `TEST_SUITE(name)`   Define a test suite. Body implement with braces.
- `ASSERT(condition)`  Fail the current test if condition is false and return.
- `ASSERT_EQ(a, b)`        Fail the current test if `a != b` and report both values.
- `RUN_TEST(name)`     Run a test function, counting the number of tests passed and failed.
- `RUN_SUITE(name)`    Run a test suite, printing its results.
- `TEST_MAIN`          Generate main(). Body lists RUN_SUITE calls.

### Running

```Bash
gcc -o test test_*.c
./test
```

> Use `-q` for quiet mode or `-s` for silent (exit code only).

### Example

```c
#define SH25_UTEST_IMPLEMENTATION
#include "sh25_utest.h"
#include "stack.h"

TEST(test_stack_size) {
    ASSERT(1);
}

TEST_SUITE(test_stack) {
    RUN_TEST(test_stack_size);
}

TEST_MAIN {
    RUN_SUITE(test_stack);
}
```

> With multiple tests files, define `SH25_UTEST_IMPLEMENTATION` and use `TEST_MAIN`
> in exactly one of them.

## Examples

See [examples](examples).

## Tests

Run this repo's tests:

```Bash
make -C tests
```

## License

MIT, see [LICENSE](LICENSE)
