#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>


class Cache {
    std::list<int> main_queue, in_queue, out_queue;
    std::unordered_map<int, std::list<int>::iterator> main_hash, in_hash, out_hash;

    int main_size, in_size, out_size;
public:

    explicit Cache(int m_s, int i_s, int o_s) : main_size(m_s),
                                                in_size(i_s),
                                                out_size(o_s) {};

    bool in_main(int page);
    bool in_in(int page);
    bool in_out(int page);
    bool front_main(int page);
    bool front_in(int page);
    bool from_out_to_main(int page);
    bool from_in_to_out(int page);
    bool add_to_in(int page);
};

int lookup_pages (Cache cache, std::vector<int> pages);

int main() {
    int page, m_size, in_size, out_size;
    scanf("%d", &m_size);
    scanf("%d", &in_size);
    scanf("%d", &out_size);

    std::vector<int> pages;
    while ((scanf("%d", &page)) == 1)
        pages.push_back(page);

    Cache c = Cache(m_size, in_size, out_size);

    printf ("\nresult is %d\n", lookup_pages(c, pages));
    return 0;
}



bool Cache::in_main(int page) {
    if (main_hash.find(page) != main_hash.end())
        return true;
    return false;
}

bool Cache::in_in(int page) {
    if (in_hash.find(page) != in_hash.end())
        return true;
    return false;
}

bool Cache::in_out(int page) {
    if (out_hash.find(page) != out_hash.end())
        return true;
    return false;
}

bool Cache::front_main(int page) {
    auto iter = main_hash[page];
    main_queue.splice(main_queue.begin(), main_queue, iter);
    main_hash[page] = main_queue.begin();
    return false;
}

bool Cache::front_in(int page) {
    auto iter = in_hash[page];
    main_queue.splice(in_queue.begin(), in_queue, iter);
    in_hash[page] = in_queue.begin();
    return false;
}

bool Cache::from_out_to_main(int page) {
    if (main_queue.size() == main_size) {
        main_hash.erase(*(--main_queue.end()));
        main_queue.erase((--main_queue.end()));
    }

    auto iter = out_hash[page];
    main_queue.splice(main_queue.begin(), out_queue, iter);
    out_hash.erase(page);
    main_hash[page] = main_queue.begin();
    return false;
}

bool Cache::from_in_to_out(int page) {
    if (out_queue.size() == out_size) {
        out_hash.erase(*(--out_queue.end()));
        out_queue.erase((--out_queue.end()));
    }

    auto iter = in_hash[page];
    out_queue.splice(out_queue.begin(), in_queue, iter);
    in_hash.erase(page);
    out_hash[page] = out_queue.begin();
    return false;
}

bool Cache::add_to_in(int page) {
    if (in_queue.size() == in_size)
        from_in_to_out(*(--in_queue.end()));

    in_queue.push_front(page);
    in_hash[page] = in_queue.begin();
    return false;
}


int lookup_pages (Cache cache, std::vector<int> pages) {
    int hash_hint = 0;
    std::vector<int>::iterator page_iter = pages.begin();

    for (; page_iter != pages.end(); ++page_iter) {
        int page = *page_iter;

        if (cache.in_main(page)) {
            cache.front_main(page);
            ++hash_hint;
            continue;
        }

        if (cache.in_in(page)) {
            cache.front_in(page);
            continue;
        }

        if (cache.in_out(page)) {
            cache.from_out_to_main(page);
            continue;
        }

        cache.add_to_in(page);
    }

    return hash_hint;
}

