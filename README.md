
# Sh25-shl

A collection of single-header libraries for c.

## Libraries

- **sh25_utest**: Simple unit testing framework for C.
- **sh25_array**: Dynamic array supporting any type.
- **sh25_stack**: Generic stack library.

## Install

Download the headers you need into your project.

```Bash
BASE=shalom2552.github.io/sh25-shl/include
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

## Examples

See [examples](examples).

## Tests

Run this repo's tests:

```Bash
make -C tests
```

## License

MIT, see [LICENSE](LICENSE)
