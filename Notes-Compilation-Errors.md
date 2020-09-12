
# Notes on compiling

These are notes that I have found helpful as I move between
systems (OS and hardware) and compile a fresh copy of OBNC.
The focus is on recognizing quickly what errors I have encountered
and how to remedy them.  Some of them are obvious other less so
or idiosyncratic to a machine, OS or version.


## Compile Errors Triage

### Debian/Ubuntu/Raspberry Pi OS

## Missing lex

```shell
    ./build --prefix=$HOME
    cd /home/rsdoiel/obnc/src
    lex --header-file=lex.yy.h Oberon.l
    ./build: 1: eval: lex: not found
```

This error is sometimes encounter when lex is not
found. Solution is to make sure a version of lex
is available.

```shell
	sudo apt install flex
```

## Missing Yacc

```shell
    ./build --prefix=$HOME
    lex --header-file=lex.yy.h Oberon.l
    yacc -D parse.error=verbose -d -t Oberon.y
    ./build: 1: eval: yacc: not found
    yacc -d -t Oberon.y
    ./build: 1: eval: yacc: not found
```

This error is sometimes encounter when yacc is not
found. Solution is to make sure a version of yacc
is available

```shell
    sudo apt install bison
```

