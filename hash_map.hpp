#include "hash_map.h"
#include <algorithm>

template <typename K,typename V> hash_map<K,V>::hash_map(size_t capacity,float upper_load_factor,float lower_load_factor) {
    _size = 0;
    _capacity = capacity;
    _upper_load_factor = upper_load_factor;
    _lower_load_factor = lower_load_factor;
    _head = new hash_list<K, V>[int(_capacity)];
}

template <typename K,typename V> hash_map<K,V>::hash_map(const hash_map<K,V> &other) {
    _size = other._size;
    _capacity = other._capacity;
    _upper_load_factor = other._upper_load_factor;
    _lower_load_factor = other._lower_load_factor;
    _head = new hash_list<K, V>[(int)_capacity];

    for (int i = 0; i < int(_capacity);i++) {
        _head[i] = other._head[i];
    }
}

template <typename K,typename V> hash_map<K,V> &hash_map<K,V>::operator=(const hash_map<K,V> &other) {
    hash_map<K,V> temp = other;
    std::swap(_upper_load_factor,temp._upper_load_factor);
    std::swap(_lower_load_factor,temp._lower_load_factor);
    std::swap(_size,temp._size);
    std::swap(_head,temp._head);
    std::swap(_capacity,temp._capacity);
    return *this;
}

template <typename K,typename V> void hash_map<K,V>::insert(K key,V value) {
    size_t index = _hash(key) % _capacity;
    int oldsize = _head[index].get_size();
    _head[index].insert(key,value);
    if(oldsize != int(_head[index].get_size())) {
        _size++;
    }
    //rehash check
    this->need_to_rehash();
}

template <typename K,typename V> std::optional<V> hash_map<K,V>::get_value(K key) const {
    for (int i = 0;i < int(_capacity);i++) {
        node<K, V> *ptr = _head[i].get_head();
        while (ptr != NULL) {
            if (ptr->key == key){
                std::optional<V> val = ptr->value;
                return val;
            } 
            ptr = ptr->next;
        }
    }
    return std::nullopt;
}

template <typename K,typename V> bool hash_map<K,V>::remove(K key) {
    bool keyv = false;
    for (int i = 0;i < int(_capacity);i++) {
        keyv = _head[i].remove(key); 
        if (keyv) {
            _size--;
            return keyv;
        }
    }
    //rehash check
    this->need_to_rehash();
    return keyv;
}

template <typename K,typename V> size_t hash_map<K,V>::get_size() const {
    return _size;
}

template <typename K,typename V> size_t hash_map<K,V>::get_capacity() const {
    return _capacity;
}

template <typename K,typename V> void hash_map<K,V>::get_all_keys(K *keys) {
    int j = 0;
    for (int i = 0;i < int(_capacity);i++) {
        node<K, V> *ptr = _head[i].get_head();
        while (ptr != NULL) {
            keys[j] = ptr->key;
            ptr = ptr->next;
            j++;
        }
    }
}

template <typename K,typename V> void hash_map<K,V>::get_bucket_sizes(size_t *buckets) {
    for (int i = 0;i < int(_capacity);i++) {
        buckets[i] = _head[i].get_size();
    }
}

template <typename K,typename V> hash_map<K,V>::~hash_map() {
    delete[] _head;
}

template <typename K,typename V> void hash_map<K,V>::need_to_rehash() {
    //setting the next increase or decrease capacity
    size_t next_cap;
    size_t prev_cap;
    if (_capacity >= _capacities[0] && _capacity < _capacities[1]) {
        next_cap = _capacities[1];
        prev_cap = _capacities[0];
    }
    else if (_capacity > _capacities[1] && _capacity <= _capacities[2]) {
        next_cap = _capacities[2];
        prev_cap = _capacities[1];
    }
    else if (_capacity == _capacities[1]) {
        next_cap = _capacities[2];
        prev_cap = _capacities[0];
    }
    //setting capacity lower or higher and rehashing
    if (_size > _upper_load_factor * _capacity) {
        this->rehash(next_cap);
    }
    else if (_size < _lower_load_factor * _capacity) {
        this->rehash(prev_cap);
    }
}

//rehashes the list after determining if its necessary
template <typename K,typename V> void hash_map<K,V>::rehash(size_t new_cap) {
    K *keys = new K[_size];
    std::optional<V> values[_size] = {};
    //get all key/value pairs
    this->get_all_keys(keys);
    for (int i=0;i<int(_size);i++) {
        values[i] = get_value(keys[i]);
    }
    //resize/reset the array with new capacity
    _capacity = new_cap;
    hash_list<K,V> *temp = _head;
    _head = new hash_list<K,V>[int(_capacity)];
    //use rehash_insert to rehash
    for (int j=0;j<int(_size);j++) {
        this->rehash_insert(keys[j],*values[j]);
    }
    delete[] temp;
    delete[] keys;
}

//this is here to prevent infinite need_to_rehash looping from the normal insert
template <typename K,typename V> void hash_map<K,V>::rehash_insert(K key,V value) {
    size_t index = _hash(key) % _capacity;
    _head[index].insert(key,value);
}

template <typename K,typename V> void hash_map<K,V>::get_all_sorted_keys(K *keys) { 
    this->get_all_keys(keys);
    std::sort(keys, keys + _size);
}