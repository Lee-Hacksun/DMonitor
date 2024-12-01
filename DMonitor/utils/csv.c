#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "csv.h"
#include "DebugUtil.h"

char** ParseCSVLine(const char *line, int *field_count) 
{
    char** fields = NULL;
    int count = 0;
    const char* start = line;
    const char* end;

    while ((end = strchr(start, ',')) != NULL) 
    {
        size_t length = end - start;
        fields = realloc(fields, (count + 1) * sizeof(char *));
        fields[count] = malloc(length + 1);
        strncpy(fields[count], start, length);
        fields[count][length] = '\0'; 
        count++;
        start = end + 1;
    }

    if (*start != '\0') 
    {
        fields = realloc(fields, (count + 1) * sizeof(char *));
        fields[count] = strdup(start); 
        count++;
    }

    ASSERT(*field_count == -1 || *field_count == count, "All fields in the csv file must be the same size.");
    *field_count = count;
    return fields;
}

void DestroyCSV(CSV* csv) 
{
    if (csv == NULL)
    {
        return;
    }

    for (int i = 0; i < csv->csvSize; i++)
    {
        for (int j = 0; j < csv->fieldSize; j++) 
        {
            free(csv->data[i][j]);
        }
        free(csv->data[i]);
    }

    free(csv->data);
    free(csv);
}

CSV* ParseCSV(FILE* file) 
{
    char *line = NULL;
    size_t line_size = 0;
    int isHeader = 1; 

    CSV* csv = malloc(sizeof(CSV));

    csv->data = NULL;
    csv->fieldSize = -1;
    int rowCount = 0;

    while (getline(&line, &line_size, file) != -1) 
    {
        line[strcspn(line, "\r\n")] = '\0';

        if (isHeader) 
        {
            isHeader = 0;
            continue;
        }

        char** fields =  ParseCSVLine(line, &csv->fieldSize);

        csv->data = realloc(csv->data, (rowCount + 1) * sizeof(char**));
        csv->data[rowCount] = fields;
        rowCount++;
    }
    csv->csvSize = rowCount;

    free(line);
    return csv;
}