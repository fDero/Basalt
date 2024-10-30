![license: MIT](https://img.shields.io/badge/license-MIT-blue)
![version: pre-release](https://img.shields.io/badge/version-pre--release-red)

# Basalt
This repository hosts an interpreter for the Basalt programming language. In the future, 
the goal is to evolve this project into an all-in-one bundle made of an interpreter, a compiler, and a debugger.
Basalt is designed to be compiled directly to machine code, and offers all the features a low level language would, 
while still being able to be interpreted like languages like Python or Ruby (enhancing portability, 
and reducing wasted time by removing the need to perform a full compilation of your code-base just to run unit-tests).

### Building from source
In order to build Basalt from source, `conan`, `cmake` and `g++` must be installed on your machine.
Assuming such tools are indeed installed on the host machine, running the following command will download 
dependencies, setup and start the build.
```bash
$ git clone https://www.github.com/fDero/Basalt
$ cd Basalt
$ conan install . --output-folder=dependencies --build=missing
$ cmake -S . -B build -DCMAKE_BUILD_TYPE=release
$ cmake --build build --target basalt
```
**notice:** __conan installations prior to the `2.8.1` version might not be able to correctly download and install the
necessary dependencies for the project. Try to upgrade it before proceeding.__

If you want to run the unit-tests, run the command `cmake --build build --target basalt_unit_tests`. This will create
a binary called `basalt_unit_tests` that will perform the tests when executed. Tests are written using the
gtest (googletest) unit-testing framework. Keep in mind that compilation will take much longer when compiling
the test-suite, since gtest is a framework that heavily relies on the use of macros.


### Hello World
To make your first hello-world program in Basalt, all it takes is to create a file with the `.bt` extension 
(in this case, it will be called `hello.bt`) containing the following code:
```go
package main;

func main() {
    console::println("Hello, World!");
}
```

Once you have the file set up, to run it just use the `basalt -r hello.bt` command in console.
```bash
$ basalt -r hello.bt
Hello, World!
```

### Types
Basalt is a strictly typed language, it has a C-style type system with 
`Int`, `Float`, `Bool`, `Char`, `String` as primitive types. 
```go
package main;

func main() {
    var x : Int = 6;
    var y : Float = 9.4;
    var f : Bool = false;
    var c : Char = 'z';
    var s : String = "hello world";
}
```

### Recursion
Basalt supports recursion just like any other language, and a good way to show it is with the classic 
fibonacci example, consider the following code:
```go
package main;

func fibo(n : Int) -> Int {
    if (n < 2) {
        return 1;
    }
    else {
        return fibo(n-1) + fibo(n-2);
    }
}

func main() {
    console::println(fibo(6));
}
```
assuming the file is called `fibo.bt`, then it can be executed just like we did earlier with the hello-world example, 
using the `basalt -r` command.
```bash
$ basalt -r fibo.bt
21
```
