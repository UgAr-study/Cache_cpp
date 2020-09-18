#include "cache_header.h"

static size_t LRU (size_t cache_size, int *pages, size_t page_number);
static size_t Q2 (size_t cache_size, int *pages, size_t page_number);

int main (){
    //std::ios::sync_with_stdio(false);

    int* pages;
    size_t page_number, cache_size;

    std::cin >> cache_size >> page_number;


    pages = new int[page_number];

    for (int i = 0; i < page_number; ++i)
        std::cin >> pages[i];

    size_t result = 0;

    if (cache_size < 10)
        result = LRU(cache_size, pages, page_number);
    else
        result = Q2(cache_size, pages, page_number);


    std::cout << result;

    delete [] pages;
    return 0;
}

static inline size_t LRU (size_t cache_size, int *pages, size_t page_number) {
    LRU_Cache<int> cache {cache_size};
    return cache.LookUp_LRU_Hits(pages, page_number);
}

static inline size_t Q2 (size_t cache_size, int *pages, size_t page_number) {
    Cache_2Q<int> cache {cache_size};
    return cache.LookUp_2Q_Hits(pages, page_number);
}