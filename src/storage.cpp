#include "../include/storage.h"

template<typename V>
KeyValueStore<V>::KeyValueStore(size_t size) {
    bucket.resize(size, nullptr);
}

template<typename V>
size_t KeyValueStore<V>::getHashIndex(const std::string& key) {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % bucket.size();
}


template<typename V>
void KeyValueStore<V>::put(const std::string& key, V value) {
    size_t index = getHashIndex(key);
    KeyValueStoreItem<V>* current = bucket[index];

    while (current != nullptr) {
        if (current->key == key) {
            current->value = value; // Actualización
            return;
        }
        current = current->next;
    }

    KeyValueStoreItem<V>* newItem = new KeyValueStoreItem<V>(key, value);

    newItem->key = key;
    newItem->value = value;
    newItem->next = bucket[index];
    
    bucket[index] = newItem;
}

template<typename V>
KeyValueStoreItem<V>* KeyValueStore<V>::get(const std::string& key) {
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


template struct KeyValueStore<int>;
template struct KeyValueStore<std::string>;
template struct KeyValueStore<double>;
