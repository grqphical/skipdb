# skipdb
A lightweight, embedded key-value database based on a skip list index.

I built this mainly as a proof of concept after doing a research project on skip lists for a university class. Will this work in production systems? Yes, but I wouldn't
trust it to run nuclear reactors or anything important (yet).

## Features
- Key-Value Store
- Key expiry
- Simple CLI for testing (code contained in `cli/`, seperate from library)

## Pre-Requisites
The library itself has no external dependencies.

However the unit tests use Google Test and the project itself uses CMake for it's build system.

Building the project requires CMake 3.28+ and a C11/C++17 compiler.

## Installation

Add this to your `CMakeLists.txt` file
```cmake
include(FetchContent)
FetchContent_Declare(
    skipdb
    GIT_REPOSITORY https://github.com/grqphical/skipdb.git
    GIT_TAG v0.1.0
)
FetchContent_MakeAvailable(skipdb)

target_link_libraries(your_target PRIVATE SkipDB::skipdb)
```

## Example Usage
```c
#include "skipdb/skipdb.h"

int main(void) {
    skipdb_t *db = skipdb_init("<memory>");

    const char *key = "hello";
    const char *value = "world";
    skipdb_insert(db, key, value, 0);

    const char* out = skipdb_lookup(db, key);

    skipdb_destroy(db);
    return 0;
}
```

## Building the Project

```bash
git clone https://github.com/grqphical/skipdb.git
cd skipdb
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

By default this also builds the test suite and CLI. To build just the library:

```bash
cmake -B build -DBUILD_TESTS=OFF -DBUILD_CLI=OFF
```

## License
`skipdb` is licensed under the MIT License. `skipdb` uses [uthash](https://github.com/troydhanson/uthash) which is licensed under the [revised BSD license](https://troydhanson.github.io/uthash/license.html)
