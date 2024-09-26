#include <iostream>

#include "hash_map.h"

int main() {
    hash_map<int, int> map(10, 0.7, 0.3);
    map.insert(1, 2);
    map.insert(2, 3);
    map.insert(3, 4);
    map.insert(4, 5);
    map.insert(5, 6);
    map.insert(6, 7);
    map.insert(7, 8);
    map.insert(8, 9);
    map.insert(9, 10);
    map.insert(10, 11);
    int *arr= new int[map.get_size()];
    map.get_all_sorted_keys(arr);
    // map.get_bucket_sizes();
    delete[] arr;
}
