# Typing Test
Test how fast you can type.
## Compilation
Replace gcc with clang to use clang instead.

```sh
gcc -O3 -c load.c main.c
gcc -o typetest load.o main.o
```

msvc

```sh
cl /O2 /Fetypetest load.c main.c
```

## Assets
This program reads from a file called ttdict.txt in the working directory.
It is recommended to have a file called ttdict.txt in the same folder as the executable.
Then run the executable from its parent directory.
The file ttdict.txt shall contain the list of possible words.
Each word must be separated by an LF, make sure it is only LF and not CRLF.
