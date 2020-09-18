#include "cache_header.h"

void Generator (FILE *file, int page_number, int mean, int range) {
    unsigned seed = time(nullptr);
    std::default_random_engine generator(seed);
    std::normal_distribution<double> distribution ( static_cast<double>(mean),static_cast<double> (range));

    for (int i=0; i<page_number; ++i) {
        double number = distribution(generator);
        fprintf(file, "%d ", static_cast<int>(number));
    }
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

void FillTestFile (const char* filename, int page_namber, int mean1, int mean2) {
    FILE *test_file = fopen(filename, "w");
    Generator(test_file, page_namber, mean1, 30);
    Generator(test_file, page_namber, mean2, 40);
    fclose(test_file);
}