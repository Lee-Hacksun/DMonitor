#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "csv.h"
#include "DebugUtil.h"
#include "map.h"

HashMap* map;

void CSVWrite(FILE* file, CSV* csv, char* header)
{
    fprintf(file, "%s\n", header);
    for (int i = 0; i < csv->csvSize; i++)
    {
        for (int j = 0; j < csv->fieldSize; j++)
        {   
            if (csv->fieldSize - 1 == j)
            {
                fprintf(file, "%s", csv->data[i][j]);
            }
            else
            {
                fprintf(file, "%s,", csv->data[i][j]);
            }            
        }
        fprintf(file, "\n");
    }
}

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

    // ASSERT(*field_count == -1 || *field_count == count, "All fields in the csv file must be the same size.");
    *field_count = count;
    return fields;
}

char** Select(char** fields, int* index, char* value)
{
    if (strcmp(fields[*index], value) == 0)
    {
        return fields;
    }
    return NULL;
}

char** Distinct(char** fields, int* index, char* value)
{
    if (fields == NULL)
    {
        return NULL;
    }

    if (Get(map, fields[*index]) == -1)
    {
        Put(map, fields[*index], 1);
        return fields;
    }
    else 
    {
        return NULL;
    }
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

CSV* _ParseCSV(FILE* file, Option option, int* index, char* value)
{
    map = CreateHashMap();

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

        char** fields;
        if (option == NULL)
        {
            fields = ParseCSVLine(line, &csv->fieldSize);
        }   
        else 
        {
            fields =  option(ParseCSVLine(line, &csv->fieldSize), index, value);
        }

        if (fields != NULL)
        {
            csv->data = realloc(csv->data, (rowCount + 1) * sizeof(char**));
            csv->data[rowCount] = fields;
            rowCount++;
        }
    }
    csv->csvSize = rowCount;

    free(line);
    DestroyHashMap(map);
    return csv;
}

CSV* ParseCSV(FILE* file, int* column, int* ascending) 
{
    CSV* csv = _ParseCSV(file, NULL, NULL, NULL);
    if (csv == NULL) {
        return NULL;
    }

    // column 값이 유효한지 확인
    if (column == NULL || *column < 0 || *column >= csv->fieldSize) {
        return csv;
    }

    // ascending이 NULL인 경우 기본값(오름차순) 설정
    int sortOrder = (ascending == NULL) ? 1 : *ascending;

    // MergeSort 실행
    MergeSortCSV(csv->data, 0, csv->csvSize - 1, *column, sortOrder);
    return csv;
}

CSV* ParseCSVOption(FILE* file, Option option, int* index, char* value, int* column, int* ascending) 
{
    CSV* csv = _ParseCSV(file, option, index, value);
    if (csv == NULL) {
        return NULL;
    }

    // column 값이 유효한지 확인
    if (column == NULL || *column < 0 || *column >= csv->fieldSize) {
        return csv;
    }

    // ascending이 NULL인 경우 기본값(오름차순) 설정
    int sortOrder = (ascending == NULL) ? 1 : *ascending;

    // MergeSort 실행
    MergeSortCSV(csv->data, 0, csv->csvSize - 1, *column, sortOrder);

    return csv;
}

void MergeCSV(char*** data, int left, int mid, int right, int column, int ascending) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // 임시 배열 생성
    char*** leftArr = malloc(n1 * sizeof(char**));
    char*** rightArr = malloc(n2 * sizeof(char**));

    for (i = 0; i < n1; i++) {
        leftArr[i] = data[left + i];
    }
    for (j = 0; j < n2; j++) {
        rightArr[j] = data[mid + 1 + j];
    }

    i = 0; j = 0; k = left;

    // 정렬 및 병합
    while (i < n1 && j < n2) {
        int comparison = strcmp(leftArr[i][column], rightArr[j][column]);
        if ((ascending && comparison <= 0) || (!ascending && comparison > 0)) {
            data[k] = leftArr[i];
            i++;
        } else {
            data[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // 남은 부분 복사
    while (i < n1) {
        data[k] = leftArr[i];
        i++;
        k++;
    }
    while (j < n2) {
        data[k] = rightArr[j];
        j++;
        k++;
    }

    free(leftArr);
    free(rightArr);
}

void MergeSortCSV(char*** data, int left, int right, int column, int ascending) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // 재귀적으로 분할
        MergeSortCSV(data, left, mid, column, ascending);
        MergeSortCSV(data, mid + 1, right, column, ascending);

        // 병합
        MergeCSV(data, left, mid, right, column, ascending);
    }
}