#include "hash_list.h"

template <typename K,typename V> hash_list<K,V>::hash_list() {
    head = NULL;
    size = 0;
    iter_ptr = NULL;
}

/**-----------------------------------------------------------------------------------
 * START Part 1
 *------------------------------------------------------------------------------------*/

template <typename K,typename V> void hash_list<K,V>::insert(K key,V value) {
    node<K, V> *ptr = head;
    node<K, V> *oldnode = NULL;
    while (ptr != NULL) {
        if (ptr->key == key) 
        {
            ptr->value = value;
            return;
        }
        oldnode = ptr;
        ptr = ptr->next;
    }   
    node<K, V> *newnode = new node<K, V>();    
    size++;
    newnode->key = key;
    newnode->value = value;
    newnode->next = NULL;
    if(head == NULL)
        head = newnode;
    else
        oldnode->next = newnode;
    return;
}

template <typename K,typename V> std::optional<V> hash_list<K,V>::get_value(K key) const { 
    node<K, V> *ptr = head;
    while (ptr != NULL) {
        if (ptr->key == key){
            std::optional<float> val = ptr->value;
            return val;
        } 
        ptr = ptr->next;
    }
    return std::nullopt; 
}

template <typename K,typename V> bool hash_list<K,V>::remove(K key) { 
    node<K, V> *ptr = head;
    node<K, V> *oldnode = NULL;
    while (ptr != NULL) {
        if (ptr->key == key){
            if(head != ptr)
                oldnode->next = ptr->next;
            else
                head = ptr->next;
            delete ptr;
            size--;
            return true;
        }
        oldnode = ptr;
        ptr = ptr->next;
    }
    return false; 
}

template <typename K,typename V> size_t hash_list<K,V>::get_size() const { 
    return size; 
}

template <typename K,typename V> hash_list<K,V>::~hash_list() {
    node<K, V> *ptr = head;
    node<K, V> *old = NULL;
    while(ptr != NULL)
    {
        old = ptr;
        ptr = ptr->next;
        delete old;
    }    
}

/**-----------------------------------------------------------------------------------
 * END Part 1
 *------------------------------------------------------------------------------------*/


/**-----------------------------------------------------------------------------------
 * START Part 2
 *------------------------------------------------------------------------------------*/

template <typename K,typename V> hash_list<K,V>::hash_list(const hash_list &other) {
    size = 0;
    iter_ptr = NULL;
    head = NULL;
    node<K, V> *ptr = other.head;
    while (ptr != NULL) {
        insert(ptr->key,ptr->value);
        ptr = ptr->next;
    }
}

template <typename K,typename V> hash_list<K,V> &hash_list<K,V>::operator=(const hash_list &other) { 
    hash_list temp = other;
    std::swap(size,temp.size);
    std::swap(head,temp.head);
    std::swap(iter_ptr,temp.iter_ptr);
    return *this;
}

template <typename K,typename V> void hash_list<K,V>::reset_iter() {
    if (head == NULL) {
        iter_ptr = NULL;
    }
    else {
        iter_ptr = head;
    }
}


template <typename K,typename V> void hash_list<K,V>::increment_iter() {
    if (iter_ptr == NULL) {
        return;
    }
    else if (iter_ptr->next == NULL) {
        iter_ptr = NULL;
    }
    else {
        iter_ptr = iter_ptr->next;
    }
}


template <typename K,typename V> std::optional<std::pair<const K *, V *>> hash_list<K,V>::get_iter_value() { 
    if (iter_ptr == NULL) {
        return std::nullopt;
    }
    else {
        return std::optional<std::pair<const K *, V *>>({&iter_ptr->key, &iter_ptr->value});
    } 
}


template <typename K,typename V> bool hash_list<K,V>::iter_at_end() { 
    if (iter_ptr == NULL) {
        return true;
    }
    else {
        return false;
    }
}

template <typename K,typename V> node<K, V> * hash_list<K,V>::get_head()  {
    return head;
}
/**-----------------------------------------------------------------------------------
 * END Part 2
 *------------------------------------------------------------------------------------*/
