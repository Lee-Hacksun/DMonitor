#ifndef CSV_H
#define CSV_H

int GetFieldCount(char* header);
int GetElementCount(char* CSVData);
void** ParseCSV(char* fileName);

#endif