#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "csv.h"
#include "DebugUtil.h"

char** parse_csv_line(const char *line, int *field_count) 
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
        fields[count][length] = '\0'; // Null-terminate the field
        count++;
        start = end + 1;
    }

    // 마지막 필드 처리
    if (*start != '\0') 
    {
        fields = realloc(fields, (count + 1) * sizeof(char *));
        fields[count] = strdup(start); // 마지막 필드 복사
        count++;
    }

    *field_count = count;
    return fields;
}

// 메모리 해제 함수
void free_csv_data(CSV* csv) 
{
    int i = 0;
    while (csv->data[i] != NULL)
    {
        for (int j = 0; j < csv->fieldSize[i]; j++) 
        {
            free(csv->data[i][j]);
        }
        free(csv->data[i]);
        i++;
    }
    free(csv->data);
    free(csv->fieldSize);
}

CSV* ParseCSV(char* fileName) 
{
    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        perror("파일 열기 실패");
        return NULL;
    }

    char *line = NULL;
    size_t line_size = 0;
    int is_header = 1; // 첫 번째 줄은 헤더로 가정

    CSV* csv = malloc(sizeof(CSV));

    csv->data = NULL;
    csv->fieldSize = NULL;
    int row_count = 0;

    while (getline(&line, &line_size, file) != -1) 
    {
        // 개행 문자 제거
        line[strcspn(line, "\r\n")] = '\0';

        if (is_header) 
        {
            // 헤더는 무시
            is_header = 0;
            continue;
        }

        int field_count;
        char** fields =  parse_csv_line(line, &field_count);

        csv->data = realloc(csv->data, (row_count + 1) * sizeof(char**));
        csv->fieldSize = realloc(csv->fieldSize, (row_count + 1) * sizeof(int));
        csv->data[row_count] = fields;
        csv->fieldSize[row_count] = field_count;
        row_count++;
    }

    // 동적으로 할당된 라인 메모리 해제
    free(line);
    fclose(file);
    return csv;
}