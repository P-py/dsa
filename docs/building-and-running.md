# Building and Running C Projects — Gold Standard Guide

This guide covers how to compile and run the `.c` and `.h` files in this repository using GCC, both manually and with a Makefile.

---

## Prerequisites

You need GCC installed on your machine.

**Linux (Debian/Ubuntu):**
```sh
sudo apt update && sudo apt install gcc make
```

**Windows (WSL):**
```sh
sudo apt update && sudo apt install gcc make
```

**macOS:**
```sh
xcode-select --install
```

Verify:
```sh
gcc --version
```

---

## How C Multi-File Projects Work

Each module in this repo follows the same pattern:

```
module/
├── module.h        # Type definitions and function declarations (interface)
├── module.c        # Function implementations
└── main.c          # Entry point — includes the .h, links against the .c
```

The `.h` file is the **interface** — it tells `main.c` what types and functions exist.
The `.c` file is the **implementation** — it defines how those functions work.
You must compile **both** `.c` files together so the linker can resolve all symbols.

---

## Manual Compilation (GCC)

### Sensor1D (`TAD/`)

```sh
gcc TAD/sensor1d.c TAD/main.c -o sensor_app
```

Run:
```sh
./sensor_app
```

Expected session:
```
Digite a leitura 1: 23.5
Digite a leitura 2: 18.0
...
Leituras registradas:
23.50 18.00 ...
Media = 20.75
Maximo = 23.50
```

---

### Bag (`bag/`)

```sh
gcc bag/bag.c bag/aplicacaoBag.c -o bag_app
```

Run:
```sh
./bag_app
```

Expected session:
```
Bag inicializada!
O tamanho atual da bag: 0

1. Exibir o tamanho da bag
2. Inserir valores aleatórios na bag
...
7. Sair
Digite uma opcao:
```

---

## Recommended GCC Flags

For development and learning, always compile with warnings and debug info enabled:

```sh
gcc -Wall -Wextra -g TAD/sensor1d.c TAD/main.c -o sensor_app
gcc -Wall -Wextra -g bag/bag.c bag/aplicacaoBag.c -o bag_app
```

| Flag | Purpose |
|---|---|
| `-Wall` | Enable all common warnings |
| `-Wextra` | Enable extra warnings |
| `-g` | Include debug symbols (needed for `gdb`) |
| `-o <name>` | Set the output binary name |

---

## Building with a Makefile

A Makefile automates compilation so you don't have to retype commands.

Create a file named `Makefile` at the root of the project:

```makefile
CC     = gcc
CFLAGS = -Wall -Wextra -g

all: sensor bag

sensor: TAD/sensor1d.c TAD/main.c
	$(CC) $(CFLAGS) TAD/sensor1d.c TAD/main.c -o sensor_app

bag: bag/bag.c bag/aplicacaoBag.c
	$(CC) $(CFLAGS) bag/bag.c bag/aplicacaoBag.c -o bag_app

clean:
	rm -f sensor_app bag_app
```

> **Important:** The indentation in a Makefile must use **tabs**, not spaces.

### Usage

```sh
make          # build all targets
make sensor   # build only sensor_app
make bag      # build only bag_app
make clean    # remove compiled binaries
```

---

## Understanding Compiler Errors

### `undefined reference to 'function_name'`
You forgot to include the implementation `.c` file in the compile command.

```sh
# Wrong — missing sensor1d.c
gcc TAD/main.c -o sensor_app

# Correct
gcc TAD/sensor1d.c TAD/main.c -o sensor_app
```

### `fatal error: 'module.h' file not found`
The compiler can't find the header. Make sure you're compiling from the **project root** and the `#include "module.h"` path matches the actual file location.

### Implicit function declaration warning
A function is called before it is declared. Ensure the `.h` file is included at the top of `main.c`:

```c
#include "sensor1d.h"
```

---

## Adding a New Module

Follow this pattern when creating a new data structure (e.g., `linkedlist`):

1. Create `linkedlist/linkedlist.h` — define the struct and declare functions.
2. Create `linkedlist/linkedlist.c` — implement the functions, include its own header:
   ```c
   #include "linkedlist.h"
   ```
3. Create `linkedlist/main.c` — write the entry point, include the header:
   ```c
   #include "linkedlist.h"
   ```
4. Compile:
   ```sh
   gcc -Wall -Wextra -g linkedlist/linkedlist.c linkedlist/main.c -o linkedlist_app
   ```
5. Add a target to the `Makefile`:
   ```makefile
   linkedlist: linkedlist/linkedlist.c linkedlist/main.c
   	$(CC) $(CFLAGS) linkedlist/linkedlist.c linkedlist/main.c -o linkedlist_app
   ```

---

## Quick Reference

```sh
# Compile a module
gcc <impl>.c <main>.c -o <output>

# Compile with all warnings + debug symbols (recommended)
gcc -Wall -Wextra -g <impl>.c <main>.c -o <output>

# Build everything via Makefile
make

# Remove binaries
make clean
```
