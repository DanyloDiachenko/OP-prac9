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
void getRecordDetails(Record *record);
int getSortField();
int getSortOrder();
bool openAndValidateFile(const char *filename, FILE **file);
bool validateFileName(char *filename);
bool validateFileExisting(const char *filename);
bool validateFileName(char *filename, size_t size);
bool validateFileSignature(FILE *file);

#endif // FUNCTIONS_H