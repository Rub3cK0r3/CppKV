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

    // Store constructor: Defines the array size (e.g., 16) and fills each bucket with 'nullptr'
    KeyValueStore(size_t size = 16) {
        bucket.resize(size, nullptr);
    }

    // Basic DJB2 Hash Function to map strings to bucket indices
    size_t getHashIndex(const std::string& key) {
        unsigned long hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c;
        }
        return hash % bucket.size();
    }

    // Inserts or updates a key-value pair in low-level memory
    void put(const std::string& key, V value) {
        size_t index = getHashIndex(key);
        KeyValueStoreItem<V>* current = bucket[index];

        while (current != nullptr) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }

        KeyValueStoreItem<V>* newNode = new KeyValueStoreItem<V>(key, value);
        newNode->next = bucket[index];
        bucket[index] = newNode;
    }

    // Retrieves a value by key. Returns nullptr if not found
    KeyValueStoreItem<V>* get(const std::string& key) {
        size_t index = getHashIndex(key);
        KeyValueStoreItem<V>* current = bucket[index];

        while (current != nullptr) {
            if (current->key == key) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }
};

#endif // STORAGE_H

