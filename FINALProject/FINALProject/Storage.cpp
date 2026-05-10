#include "Storage.h"


template <class T>
T Storage<T>::data[100] = {};

template <class T>
Storage<T>::Storage()  { 
    count = 0;
}

template <class T>
bool Storage<T>::add(const T& item) {
    if (count < 100) {
        data[count++] = item;
        return true;
    }
    return false;
}

template <class T>
bool Storage<T>::removeByID(int id) {
    for (int i = 0; i < count; i++) {
        
        if (data[i].getId() == id) {
            
            for (int j = i; j < count - 1; j++) {
                data[j] = data[j + 1];
            }
            count--;
            return true;
        }
    }
    return false;
}

template <class T>
T* Storage<T>::findByID(int id) {
    for (int i = 0; i < count; i++) {
        if (data[i].getId() == id) {
            return &data[i];
        }
    }
    return nullptr;
}

template <class T>
T* Storage<T>::getAll() {
    return data;
}

template <class T>
int Storage<T>::size() const {
    return count;
}
