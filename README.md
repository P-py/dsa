# Data Structures and Algorithms in C

An educational repository implementing fundamental data structures as Abstract Data Types (ADTs) in C.

## Structure

```
dsa/
├── TAD/          # Sensor1D - final implementation
├── tad/          # Sensor1D - exercise/learning template
├── bag/          # Bag collection data structure
└── linkedlist/   # (planned)
```

## Implementations

### Sensor1D (`TAD/`)

A fixed-size array-based structure for storing and processing 1D sensor readings (capacity: 10).

| Function | Description |
|---|---|
| `s_init()` | Initialize sensor |
| `s_add(val)` | Add a reading |
| `s_media()` | Calculate average |
| `s_max()` | Find maximum reading |
| `s_print()` | Print all readings |

**Compile & run:**
```sh
gcc TAD/sensor1d.c TAD/main.c -o sensor_app
./sensor_app
```

---

### Bag (`bag/`)

An unordered collection that supports duplicate elements (capacity: 50 integers). Uses `-1` as a sentinel for empty slots.

| Function | Description |
|---|---|
| `init()` | Initialize empty bag |
| `insert(val)` | Add element |
| `delete(val)` | Remove first occurrence |
| `check(val)` | Count occurrences |
| `size()` | Get element count |
| `printall()` | Display all elements |

**Compile & run:**
```sh
gcc bag/bag.c bag/aplicacaoBag.c -o bag_app
./bag_app
```

---

### Linked List (`linkedlist/`)

Planned — directory reserved for future implementation.

## License

MIT License — Copyright 2026 Pedro
