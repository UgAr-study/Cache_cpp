#include "cache_header.h"

int main (){
    int* pages;
    size_t page_number, cache_size;

    int res = scanf ("%zu %zu", &cache_size, &page_number);
    assert(res == 2);

    pages = new int[page_number];

    for (int i = 0; i < page_number; ++i)
        scanf ("%d", &pages[i]);

    size_t result = 0;

    switch (cache_size < 10) {
        case true:
            result = LRU(cache_size, pages, page_number);
            break;
        case false:
            result = Q2 (cache_size, pages, page_number);
            break;
        default:
            printf ("Something goes wrong\n");

    }

    printf ("%zu\n", result);

    delete [] pages;
    return 0;
}

size_t LRU (size_t cache_size, int *pages, size_t page_number) {
    LRU_Cache<int> cache {cache_size};
    return cache.LookUp_LRU_Hits(pages, page_number);
}

size_t Q2 (size_t cache_size, int *pages, size_t page_number) {
    Cache_2Q<int> cache {cache_size};
    return cache.LookUp_2Q_Hits(pages, page_number);
}