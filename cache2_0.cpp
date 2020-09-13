#include "cache_header.h"

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
    explicit Cache (size_t size_main, size_t size_in, size_t size_out) :
        main(size_main),
        in(size_in),
        out(size_out) {};

    size_t LookUpHits (std::vector<T> pages);

    static void MovePage_FromFirst_ToSecondContainer (T page, Container& container1,
                                                              Container& container2);
    void AddNewPage(T page);
};


int main (int argc, char* argv[]) {
    std::vector<int> pages;
    int page_number, mean1, mean2;
    const char* filename;

    if (argc == 5) {
        filename = argv[1];
        page_number = static_cast<int>(strtol(argv[2], nullptr, 0));
        mean1 = static_cast<int>(strtol(argv[3], nullptr, 0));
        mean2 = static_cast<int>(strtol(argv[4], nullptr, 0));
        FillTestFile(filename,page_number, mean1, mean2);

        pages = GetIntPage(filename);
        size_t main_size = page_number / 5 * 2,
            in_size = page_number / 5,
            out_size = page_number / 5 * 6;
        Cache<int> cache{main_size, in_size, out_size};
        size_t result = cache.LookUpHits(pages);
        printf ("result is %zu\n", result);
    } else {
        printf ("Not all parameters\n");
        exit(0);
    }

    return 0;
}

template<typename T>
size_t Cache<T>::LookUpHits(std::vector<T> pages) {
    size_t hits = 0;
    auto page_iter = pages.begin();

    for (; page_iter != pages.end(); ++page_iter) {
        //T page = getPage(file_name);
        T page = *page_iter;

        if (main.hash_table.find(page) != main.hash_table.end()) {
            MovePage_FromFirst_ToSecondContainer(page, main, main);
            ++hits;
            continue;
        }

        if (in.hash_table.find(page) != in.hash_table.end()) {
            MovePage_FromFirst_ToSecondContainer(page, in, in);
            continue;
        }

        if (out.hash_table.find(page) != out.hash_table.end()) {
            MovePage_FromFirst_ToSecondContainer(page, out, main);
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

std::vector<int> GetIntPage (const char *file_name) {
    FILE *file = fopen(file_name, "r");
    fseek(file, 0, SEEK_SET);
    int page = 0;
    std::vector<int> pages;
    while (fscanf(file, "%d", &page) == 1)
        pages.push_back(page);
    return pages;
}