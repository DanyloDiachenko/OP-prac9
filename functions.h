#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void createFile();
void readFile();
void deleteFile();

void createRecord();
void readSingleRecord();
void updateRecord();
void sortRecords();
void insertRecord();
void deleteRecord();

bool getFileName(char *filename, size_t size, char message[]);
int compareRecords(const void *a, const void *b, int sortField, int isAscending);
void getRecordDetails(Record *record);
int getSortField();
int getSortOrder();
bool openAndValidateFile(const char *filename, FILE **file);
bool validateFileExisting(const char *filename);
bool validateFileName(char *filename, size_t size);
bool validateCreatingFileName(char *filename);
bool validateFileSignature(FILE *file);
void getRecordIndex(int *index, int recordCount, char message[]);

#endif // FUNCTIONS_H