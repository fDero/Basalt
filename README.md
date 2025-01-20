![license](https://img.shields.io/badge/license-MIT-blue)
![version](https://img.shields.io/badge/version-pre--release-purple)

# Basalt
Compiler for the *Basalt programming language*, a low level,
statically typed language with manual memory management which has been designed for system-programming.

## Language Overview
Basalt is a *C-like* language. Just as you would expect, the execution starts from a function called
`main`. A basic hello-world example program in Basalt would look like this:

```go
package main;

func main() {
    console::print("Hello World!")
}
```

The Basalt programming language offers the following features:

- function overloading
- native union support
- pseudo-polimorfism based on common APIs over every member of a given union
- generics (implemented the C++ way)
- native typesystem support for dynamic arrays (slices)
- manual memory management

## Compiler features
The Basalt compiler is itself powered by the *llvm compiler infrastructure*. It's capable of emitting
object files (`.o`), assembly (`.s`) files and llvm-ir files (`.ll`) files as the compilation output. It's
also capable of running code directly via just-in-time compilation (jit).

Here's an example of how you can check for errors in your code using the `basalt` command
```bash
basalt typecheck -i src/*.basalt
```

Here's an example of how you can compile code using the `basalt` command
```bash
basalt compile -i src/*.basalt -o myprogram.o
```

Here's an example of how you can run your code directly using the `basalt` command
```bash
basalt run -i src/*.basalt
```

## Install / Build
Regardless of your operating-system, builing from source will require `g++`, `cmake`, `conan` to be
installed on your machine. Such tools are needed to download and build dependencies (LLVM, libxml2, gtest) and
setup an incremental build enviroment.

**notice:** conan versions prior to the `2.8.1` might not be able to correctly download and install the
necessary dependencies for the project. Try to upgrade it before proceeding.

#### Build (Linux / MacOs)
```bash
$ git clone https://www.github.com/fDero/Basalt
$ cd Basalt
$ conan install . --output-folder=dependencies --build=missing
$ cmake -S . -B build -DCMAKE_BUILD_TYPE=release
$ cmake --build build --target basalt
```

#### Build (Windows)
```ps
> git clone https://www.github.com/fDero/Basalt
> cd Basalt
> conan install . --output-folder=dependencies --build=missing
> cmake -S . -B build -DCMAKE_BUILD_TYPE=release -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++
> cmake --build build --target basalt
```

#### Install with the snap package manager (Ubuntu/Debian-based Linux distros)
```
snap install basalt
```

#### Install with .msi installer (Windows)
Check the releases tab of the github page, you should find a .msi installer
to download. Such installer will correctly install a statically linked windows-x86
pre-built instance of basalt in `%ProgramFiles%\basalt\<version>`
