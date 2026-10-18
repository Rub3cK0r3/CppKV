# CppKV

A high-performance, concurrent, and thread-safe in-memory Key-Value store written in modern C++ from scratch.

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
