#ifndef CSV_H
#define CSV_H

#include <stdio.h>

typedef struct _CSV 
{
    char*** data;
    int fieldSize;
    int csvSize;
} CSV;

typedef char** (*Option)(char** fields, int* index, char* value);

void CSVWrite(FILE* file, CSV* csv, char* header);

char** ParseCSVLine(const char* line, int* field_count);
void DestroyCSV(CSV* csv);

char** Select(char** fields, int* index, char* value);
char** Distinct(char** fields, int* index, char* value);

CSV* _ParseCSV(FILE* file, Option option, int* index, char* value);
CSV* ParseCSV(FILE* file, int* column, int* ascending);
CSV* ParseCSVOption(FILE* file, Option option, int* index, char* value, int* column, int* ascending);

void MergeCSV(char*** arr, int left, int mid, int right, int column, int ascending);
void MergeSortCSV(char*** arr, int left, int right, int column, int ascending);

#endif