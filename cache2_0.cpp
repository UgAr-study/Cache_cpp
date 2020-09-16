#include <iostream>
#include <unordered_map>
#include <list>
#include <cassert>

template <typename T>
class Cache {
private:
    using un_map = std::unordered_map<T, typename std::list<T>::iterator>;
    class Container {
        friend class Cache<T>;

    private:
        un_map hash_table;
        std::list<T> list;
        size_t size;
    public:
        explicit Container(size_t size_) : size (size_) {};
    } main, in, out;

public:
    explicit Cache (size_t cache_size) :
            main(cache_size / 10 * 2),
            in(cache_size / 10 * 2),
            out(cache_size - main.size - in.size) {};

    size_t LookUpHits (T *pages, size_t page_number);

    static void MovePage_FromFirst_ToSecondContainer (T page, Container& container1,
                                                      Container& container2);
    void AddNewPage(T page);
};

int main (){
    int* pages;
    size_t page_number, cache_size;

    int res = scanf ("%zu %zu", &cache_size, &page_number);
    assert(res == 2);
    pages = new int[page_number];

    for (int i = 0; i < page_number; ++i)
        scanf ("%d", &pages[i]);


    Cache<int> cache{cache_size};
    size_t result = cache.LookUpHits(pages, page_number);
    printf ("%zu\n", result);

    delete [] pages;
    return 0;
}


template<typename T>
size_t Cache<T>::LookUpHits(T *pages, size_t page_number) {
    size_t hits = 0;


    for (int i = 0; i < page_number; ++i) {
        //T page = getPage(file_name);
        T page = pages[i];

        if (main.hash_table.find(page) != main.hash_table.end()) {
            MovePage_FromFirst_ToSecondContainer(page, main, main);
            ++hits;
            continue;
        }

        if (in.hash_table.find(page) != in.hash_table.end()) {
            MovePage_FromFirst_ToSecondContainer(page, in, in);
            ++hits;
            continue;
        }

        if (out.hash_table.find(page) != out.hash_table.end()) {
            MovePage_FromFirst_ToSecondContainer(page, out, main);
            ++hits;
            continue;
        }

        AddNewPage(page);
    }

    return hits;
}

template<typename T>
void Cache<T>::MovePage_FromFirst_ToSecondContainer(T page,
                                                    Cache::Container &container1,
                                                    Cache::Container &container2) {

    if ((&container1 != &container2) && (container2.list.size() == container2.size)) {
        container2.hash_table.erase(container2.list.back());
        container2.list.pop_back();
    }

    auto iter = container1.hash_table[page];
    container2.list.splice(container2.list.begin(), container1.list, iter);
    container1.hash_table.erase(page);
    container2.hash_table[page] = container2.list.begin();

}

template<typename T>
void Cache<T>::AddNewPage(T page) {
    if (in.list.size() == in.size)
        MovePage_FromFirst_ToSecondContainer(in.list.back(), in, out);

    in.list.push_front(page);
    in.hash_table[page] = in.list.begin();
}