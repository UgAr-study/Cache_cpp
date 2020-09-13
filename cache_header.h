#pragma once

#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <string>
#include <random>
#include <time.h>

void Generator (FILE *file, int page_number, int mean, int range);
void FillTestFile (const char* filename, int page_namber, int mean1, int mean2);
std::vector<int> GetIntPage (const char *file_name);
