#pragma once

#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <cassert>

template <typename T>
class LRU_Cache {
    using un_map = std::unordered_map<T, typename std::list<T>::iterator>;

private:
    un_map hash_table;
    std::list<T> list;
    size_t size;
public:
    explicit LRU_Cache (size_t size_) : size (size_) {};
    explicit LRU_Cache () : size (0) {};

    size_t getSize() const { return size; }
    std::list<T> & getList() { return list; }
    un_map & getMap() { return hash_table; }

    void MoveFromAnother(T page, LRU_Cache<T> &other);
    void PushFront (T page);

    size_t LookUp_LRU_Hits (T *pages, size_t page_number);
};



template <typename T>
class Cache_2Q {
private:
    LRU_Cache<T> main, in, out;

public:
    explicit Cache_2Q(size_t cache_size) :
            main (cache_size / 10 * 2),
            in (cache_size / 10 * 2),
            out(cache_size - main.getSize() - in.getSize()) {};

    size_t LookUp_2Q_Hits (T *pages, size_t page_number);

    void AddNewPage(T page);
};



std::vector<int> GetIntPage (const char *file_name);
void Generator (FILE *file, int page_number, int mean, int range);
void FillTestFile (const char* filename, int page_namber, int mean1, int mean2);
std::vector<int> GetIntPage (const char *file_name);

size_t LRU (size_t cache_size, int *pages, size_t page_number);
size_t Q2 (size_t cache_size, int *pages, size_t page_number);

#include "cache_templates.tpp"