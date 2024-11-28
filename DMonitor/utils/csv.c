#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "csv.h"

int GetFieldCount(char* header)
{
    int size = 0;

    strtok(header, ",");
    size++;

    char* isDone;
    while(1)
    {
        if ((isDone = strtok(NULL, ",")) == NULL)
        {
            break;
        }
        else
        {
            size++;
        }
    }

    return size;
}

int GetElementCount(char* CSVData)
{
    int size = 0;
    strtok(CSVData, "\n");
    size++;

    char* isDone;
    while(1)
    {
        if ((isDone = strtok(NULL, "\n")) == NULL)
        {
            break;
        }
        else
        {
            size++;
        }
    }

    return size;
}

void** ParseCSV(char* fileName)
{
    FILE* csv = fopen(fileName, "r");
    if(csv == NULL)
    {
        perror("fopen error");
        return NULL;
    }

    fseek(csv, 0, SEEK_END);
    int size = ftell(csv);

    char* csvData = malloc(sizeof(char) * size + 1);
    memset(csvData, 0, size + 1);

    fseek(csv, 0, SEEK_SET);
    fread(csvData, sizeof(char), size + 1, csv);

    // printf("%s", csvData);

    char* data = strchr(csvData, '\n');
    char* header = strtok(csvData, "\n");
    int elementCount = GetFieldCount(header);
    printf("%d", GetElementCount(data + 1));

    void* elements;
}