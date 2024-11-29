#ifndef CSV_H
#define CSV_H

typedef struct _CSV 
{
    char*** data;
    int* fieldSize;
} CSV;

char** parse_csv_line(const char* line, int* field_count);
void free_csv_data(CSV* csv);
CSV* ParseCSV(char* fileName);

#endif