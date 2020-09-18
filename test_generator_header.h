#pragma once

#include <vector>
#include <iostream>

std::vector<int> GetIntPage (const char *file_name);
void Generator (FILE *file, int page_number, int mean, int range);
void FillTestFile (const char* filename, int page_namber, int mean1, int mean2);
