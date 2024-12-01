#ifndef CSV_H
#define CSV_H

typedef struct _CSV 
{
    char*** data;
    int fieldSize;
    int csvSize;
} CSV;

char** ParseCSVLine(const char* line, int* field_count);
void DestroyCSV(CSV* csv);
CSV* ParseCSV(FILE* fileName);

#endif