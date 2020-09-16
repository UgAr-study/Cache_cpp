#include "cache_header.h"

template<typename T>
size_t LRU_Cache<T>::LookUp_LRU_Hits(T *pages, size_t page_number) {
    size_t hits = 0;

    for (int i = 0; i < page_number; ++i) {
        T page = pages[i];

        if (hash_table.find(page) != hash_table.end()) {
            auto iter = hash_table[page];
            list.splice (list.begin(), list, iter);
            ++hits;
            continue;
        }

        PushFront(page);
    }

    return hits;
}

template<typename T>
void LRU_Cache<T>::MoveFromAnother(T page, LRU_Cache<T> &other) {

    if (&other != this && list.size() == size) {
        hash_table.erase(list.back());
        list.pop_back();
    }

    auto iter = (other.getMap())[page];
    list.splice(list.begin(), other.list, iter);
    other.hash_table.erase(page);
    hash_table[page] = list.begin();
}

template<typename T>
void LRU_Cache<T>::PushFront(T page) {
    if (list.size() == size) {
        hash_table.erase(list.back());
        list.pop_back();
    }
    list.push_front(page);
    hash_table[page] = list.begin();
}



template<typename T>
size_t Cache_2Q<T>::LookUp_2Q_Hits(T *pages, size_t page_number) {
    size_t hits = 0;


    for (int i = 0; i < page_number; ++i) {
        T page = pages[i];

        if (main.getMap().find(page) != main.getMap().end()) {
            main.MoveFromAnother(page, main);
            ++hits;
            continue;
        }

        if (in.getMap().find(page) != in.getMap().end()) {
            in.MoveFromAnother(page, in);
            ++hits;
            continue;
        }

        if (out.getMap().find(page) != out.getMap().end()) {
            main.MoveFromAnother(page, out);
            ++hits;
            continue;
        }

        AddNewPage(page);
    }

    return hits;
}

template<typename T>
void Cache_2Q<T>::AddNewPage(T page) {

    if (in.getList().size() == in.getSize())
        out.MoveFromAnother(in.getList().back(), in);

    in.getList().push_front(page);
    in.getMap()[page] = in.getList().begin();
}

