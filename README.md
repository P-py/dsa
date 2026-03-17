# Data Structures and Algorithms in C

An educational repository implementing fundamental data structures as Abstract Data Types (ADTs) in C based on the Data Structure & Algorithms class @ [Faculdade de Engenharia de Sorocaba (FACENS)](https://facens.br/cursos/graduacao/engenharia-de-computacao/)

## Structure

Each data structure lives in its own folder following this pattern:

```
module/
├── module.h    # interface — type definitions and function declarations
├── module.c    # implementation
└── main.c      # entry point and usage example
```

## Building

Compile from the **project root**, passing both `.c` files:

```sh
gcc -Wall -Wextra -g <module>/<module>.c <module>/main.c -o <output>
```

Or use `make` if a `Makefile` is present:

```sh
make          # build all
make clean    # remove binaries
```

For the full guide — prerequisites, Makefile template, compiler error explanations, and instructions for adding new modules — see [`docs/building-and-running.md`](docs/building-and-running.md).

## License

MIT License — Copyright 2026 @ Pedro Salviano Santos
