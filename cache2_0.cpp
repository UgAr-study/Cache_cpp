#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>

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
    explicit Cache (size_t m_s, size_t i_s, size_t o_s) : main(m_s),
                                                 in(i_s),
                                                 out(o_s) {};

    unsigned int LookUpHits (std::vector<T> pages);
    bool Is_in_queue (T page, un_map hash) { return (hash.find(page) != hash.end()); };

    void MovePage_FromFirst_ToSecondContainer (T page, Container& container1, Container& container2);
    void AddNewPage(T page);
};

int main () {
    int page, m_size, in_size, out_size;
    scanf ("%d", &m_size);
    scanf ("%d", &in_size);
    scanf ("%d", &out_size);

    std::vector<int> pages;
    while ((scanf("%d", &page)) == 1)
        pages.push_back(page);

    Cache<int> c = Cache<int> (m_size, in_size, out_size);

    printf ("\nresult is %d\n", c.LookUpHits(pages));
    return 0;
}

template<typename T>
unsigned int Cache<T>::LookUpHits(std::vector<T> pages) {
    unsigned int hits = 0;
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
        container2.hash_table.erase(*(--container2.list.end()));
        container2.list.erase((--container2.list.end()));
    }

    auto iter = container1.hash_table[page];
    container2.list.splice(container2.list.begin(), container1.list, iter);
    container1.hash_table.erase(page);
    container2.hash_table[page] = container2.list.begin();

}

template<typename T>
void Cache<T>::AddNewPage(T page) {
    if (in.list.size() == in.size)
        MovePage_FromFirst_ToSecondContainer(*(--in.list.end()), in, out);

    in.list.push_front(page);
    in.hash_table[page] = in.list.begin();
}

//template<typename T>
//T getPage(std::string basicString) {
//    return nullptr;
//}

