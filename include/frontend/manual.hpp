
#include <string>

inline const std::string BANNER_MESSAGE = R"(
                         ____                   ____
                        / __ \____ __________ _/ / /_
                       / __  / __ `/ ___/ __ `/ / __/
                      / /_/ / /_/ (__  ) /_/ / / /_
                     /_____/\__,_/____/\__,_/_/\__/
                       version: 0.0.0 pre-release

NAME
    basalt - compiler and JIT runner for the Basalt programming language

SYNOPSIS
    basalt version
    basalt help
    basalt check -s SOURCE...
    basalt exec  -s SOURCE... [-l LIBRARY...]
    basalt build -s SOURCE... -o OUTPUT... [-t TARGET]

DESCRIPTION
    basalt reads one or more Basalt source files, type-checks them, and then,
    depending on the subcommand, either stops, runs the program in-process
    through a JIT, or emits LLVM IR, assembly and/or object files.
    Flags can be given in any order; every flag consumes the arguments that
    follow it, up to the next flag.

SUBCOMMANDS
    version
        Print the version details and exit. Takes no flags.

    help
        Print this manual and exit. Takes no flags.

    check
        Parse and type-check the sources without generating any code.

    exec
        Compile the sources for the host machine and run their `main`
        function immediately. The value returned by `main` (if any) is
        printed as the execution output.

    build
        Compile the sources and write one file per requested output.

OPTIONS
    -s, --source SOURCE...
        Basalt source files (.basalt or .bt). Required by check, exec and
        build. Every file must exist and appear only once.

    -o, --output OUTPUT...
        Files to emit, accepted by build only (and required by it). The
        kind of output is chosen by the file extension:
            .ll  .llvm    LLVM IR
            .s   .asm     assembly
            .o   .obj     object file
        At most one output file per kind.

    -t, --target TARGET
        Target triple to compile for, accepted by build only, in the form
        <arch><sub>-<vendor>-<sys>-<env> (e.g. x86_64-pc-linux-gnu).
        At most one. Defaults to the host's triple.

    -l, --links LIBRARY...
        Libraries the program is linked against when run, accepted by exec
        only. Symbols the program doesn't define are looked up in them.
            .a   .lib     static library
            .so  .dylib   .dll    dynamic library

EXIT STATUS
    0   success
    1   a command line, compilation or execution error occurred

EXAMPLES
    basalt check -s main.bt utils.bt
    basalt exec  -s main.bt -l libmath.a
    basalt build -s main.bt -o main.o main.ll -t x86_64-pc-linux-gnu

)";
