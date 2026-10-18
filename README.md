# CppKV

A high-performance, concurrent, and thread-safe in-memory Key-Value store written in modern C++ from scratch.

> [!WARNING]
> **Active Side Project & Learning Journey**
> This system is actively developed during my free time away from my main role as a Python Backend Systems Engineer. The primary objective of this repository is to transition into low-level systems programming, break free from high-level abstractions, and master exactly what happens under the hood. Expect ongoing refactors and experimental memory management techniques!

## 🚀 Key Features
* **Zero-Dependency:** Built entirely from scratch using POSIX sockets and native C++ tools.
* **Real Concurrency:** Multi-threaded architecture utilizing a custom Thread Pool (no GIL constraints).
* **Thread-Safe Storage:** In-memory data store protected via fine-grained locking (`std::shared_mutex`).
* **Memory Efficient:** Governed strictly by RAII for deterministic memory management.

## 🛠️ Quick Start

### Build
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --parallel $(nproc)
```

### Run
```bash
./CppKV --port 6379 --threads 4
```

## 📋 Protocol
```text
SET <key> <value>  -> Responses: +OK\n
GET <key>          -> Responses: VALUE <value>\n or -ERR NOT_FOUND\n
DEL <key>          -> Responses: +DELETED\n or -ERR NOT_FOUND\n
```
