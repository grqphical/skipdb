# skipdb
A lightweight, embedded key-value database based on a skip list index.

I built this mainly as a proof of concept after doing a research project on skip lists for a university class. Will this work in production systems? Yes, but I wouldn't
trust it to run nuclear reactors or anything important (yet).

## Features
- Key-Value Store
- Key expiry
- Lock-free concurrent reading and writing
- File persistence

## Pre-Requisites
The library itself has no external dependencies.

However the unit tests use Google Test and the project itself uses CMake for it's build system.

## Installation
1. Clone this repository and `cd` into it
2. Generate build files with CMake
```bash
cmake -B build -S .
```
3. Compile the library and unit tests
```bash
cmake --build build
```
4. The library will be output to `build/libskipdb.a` (At least for Linux/Unix)

**NOTE: If you want to build a shared library instead, add the `-DBUILD_SHARED_LIBS=ON` flag to step 2. The output file will be in the same place**

## License
`skipdb` is licensed under the MIT License. `skipdb` uses [uthash](https://github.com/troydhanson/uthash) which is licensed under the [revised BSD license](https://troydhanson.github.io/uthash/license.html)
