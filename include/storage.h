// Este módulo aloja el motor de almacenamiento interno. Diseña las estructuras de datos concurrentes 
// que organizan los bytes en el Heap de memoria RAM. Además implementa los mecanismos de exclusión mutua 
// para garantizar la seguridad de hilos cuando múltiples subprocesos acceden a datos en paralelo.

#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <vector>

// 1. [KEY]     --> Passed through the hash function to generate a bucket index.
// 2. [BUCKET]  --> Fast indexed array that only stores node memory addresses.
// 3. [NODE]    --> Structure that physically holds your <Key, Value> pair in memory.
// 4. [NEXT]    --> Pointer that chains nodes together only if a collision occurs in the bucket.

template<typename V>
struct KeyValueStoreItem {
    std::string key;
    V value;
    KeyValueStoreItem* next;

    // Node constructor: Initializes data and ensures 'next' starts as null
    KeyValueStoreItem(std::string k, V v) : key(k), value(v), next(nullptr) {}
};

template<typename V>
struct KeyValueStore {
    std::vector<KeyValueStoreItem<V>*> bucket;

    KeyValueStore(size_t size);

    // Basic DJB2 Hash Function to map strings to bucket indices
    size_t getHashIndex(const std::string& key);

    // Inserts or updates a key-value pair in low-level memory
    void put(const std::string& key, V value);

    // Retrieves a value by key. Returns nullptr if not found
    KeyValueStoreItem<V>* get(const std::string& key);
};

#endif // STORAGE_H

