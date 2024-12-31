#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>

typedef struct
{
    char name[50];
    float area;
    float population;
} Record;

bool isValidFileName(const char *filename)
{
    if (strlen(filename) == 0 || strlen(filename) > 255)
        return 0;

    for (int i = 0; filename[i] != '\0'; i++)
    {
        if (!isalnum(filename[i]) && filename[i] != '.' && filename[i] != '_' && filename[i] != '-')
        {
            return false;
        }
    }

    return true;
}

void createFile()
{
    char filename[256];
    bool valid;
    FILE *file;

    do
    {
        printf("Enter the name of the file to create (letters, numbers, dots, underscores, and hyphens only) including"
               " .dat extension: ");
        if (fgets(filename, sizeof(filename), stdin) == NULL)
        {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        size_t len = strlen(filename);
        if (len > 0 && filename[len - 1] == '\n')
        {
            filename[len - 1] = '\0';
        }

        valid = isValidFileName(filename);
        if (!valid)
        {
            printf("Invalid file name. Only letters, numbers, dots, underscores, and hyphens are allowed.\n");
            continue;
        }

        if (access(filename, F_OK) == 0)
        {
            printf("File '%s' already exists. Please choose a different name.\n", filename);
            valid = false;
            continue;
        }

        file = fopen(filename, "wb");
        if (file == NULL)
        {
            printf("Error creating file: %s\n", strerror(errno));
            valid = false;
        }
    } while (!valid);

    const char *signature = "MY_SIGNATURE";
    if (fwrite(signature, sizeof(char), strlen(signature), file) != strlen(signature))
    {
        printf("Error writing to file.\n");
    }
    else
    {
        printf("File '%s' created successfully.\n", filename);
    }

    fclose(file);
}

void readFile() {
    char filename[256];
    FILE *file;
    bool valid;

    do {
        printf("Enter the name of the file to read: ");
        if (fgets(filename, sizeof(filename), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        size_t len = strlen(filename);
        if (len > 0 && filename[len - 1] == '\n') {
            filename[len - 1] = '\0';
        }

        valid = isValidFileName(filename);
        if (!valid) {
            printf("Invalid file name. Only letters, numbers, dots, underscores, and hyphens are allowed.\n");
            continue;
        }

        file = fopen(filename, "rb");
        if (file == NULL) {
            printf("Error opening file or file does not exist: %s\n", strerror(errno));
            valid = false;
        }
    } while (!valid);

    const char *expectedSignature = "MY_SIGNATURE";
    const int signatureLength = strlen(expectedSignature);
    char signature[signatureLength + 1];

    if (fread(signature, sizeof(char), signatureLength, file) != signatureLength) {
        printf("Error reading file signature.\n");
        fclose(file);
        return;
    }
    signature[signatureLength] = '\0';

    if (strcmp(signature, expectedSignature) != 0) {
        printf("Invalid file format.\n");
        fclose(file);
        return;
    }

    Record record;
    printf("\nRecords in file:\n");
    while (fread(&record, sizeof(Record), 1, file) == 1) {
        printf("Name: %s, Area: %.2f, Population: %.2f\n", record.name, record.area, record.population);
    }

    if (feof(file)) {
        printf("End of file reached.\n");
    } else {
        printf("Error reading file.\n");
    }

    fclose(file);
}

void addRecord() {
    char filename[256];
    bool valid;
    FILE *file;

    do {
        printf("Enter the name of the file to add record: ");
        if (fgets(filename, sizeof(filename), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        // Удаление символа новой строки
        size_t len = strlen(filename);
        if (len > 0 && filename[len - 1] == '\n') {
            filename[len - 1] = '\0';
        }

        // Проверка имени файла
        valid = isValidFileName(filename);
        if (!valid) {
            printf("Invalid file name. Only letters, numbers, dots, underscores, and hyphens are allowed.\n");
            continue;
        }

        // Открытие файла
        file = fopen(filename, "rb+");
        if (file == NULL) {
            printf("Error opening file: %s\n", strerror(errno));
            valid = false;
            continue;
        }

        // Проверка сигнатуры
        const char *expectedSignature = "MY_SIGNATURE";
        const int signatureLength = strlen(expectedSignature);
        char signature[signatureLength + 1]; // +1 для '\0'

        if (fread(signature, sizeof(char), signatureLength, file) != signatureLength) {
            printf("Error reading file signature.\n");
            fclose(file);
            continue;
        }
        signature[signatureLength] = '\0'; // Завершающий символ

        if (strcmp(signature, expectedSignature) != 0) {
            printf("Invalid file format.\n");
            fclose(file);
            continue;
        }

        // Перемещаем указатель в конец файла для добавления записи
        fseek(file, 0, SEEK_END);
    } while (!valid);

    // Заполнение новой записи
    Record record;
    bool inputValid = false;

    // Запрос имени региона (процесс повторяется, если ввод неверный)
    do {
        printf("Enter name of the region (max 50 characters): ");
        if (fgets(record.name, sizeof(record.name), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        size_t len = strlen(record.name);
        if (len > 0 && record.name[len - 1] == '\n') {
            record.name[len - 1] = '\0'; // Удаление символа новой строки
        }

        inputValid = true;
    } while (!inputValid); // Повторяется, если ввод имени невалиден

    // Запрос площади с проверкой на правильность
    do {
        printf("Enter area: ");
        if (scanf("%f", &record.area) != 1 || record.area <= 0) {
            printf("Invalid input. Area must be a positive number.\n");
            fflush(stdin);  // Очищаем буфер ввода
        } else {
            inputValid = true;  // Ввод корректен
        }
    } while (!inputValid); // Повторяется, если введено неверное значение

    // Запрос населения с проверкой на правильность
    do {
        printf("Enter population: ");
        if (scanf("%f", &record.population) != 1 || record.population <= 0) {
            printf("Invalid input. Population must be a positive number.\n");
            fflush(stdin);  // Очищаем буфер ввода
        } else {
            inputValid = true;  // Ввод корректен
        }
    } while (!inputValid); // Повторяется, если введено неверное значение

    // Запись записи в файл
    fwrite(&record, sizeof(Record), 1, file);
    printf("Record added successfully.\n");

    // Закрытие файла
    fclose(file);
}


void deleteFile()
{
    char filename[256];
    bool valid;

    do
    {
        printf("Enter the name of the file to delete: ");
        if (fgets(filename, sizeof(filename), stdin) == NULL)
        {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        size_t len = strlen(filename);
        if (len > 0 && filename[len - 1] == '\n')
        {
            filename[len - 1] = '\0';
        }

        valid = isValidFileName(filename);
        if (!valid)
        {
            printf("Invalid file name. Only letters, numbers, and dots are allowed.\n");
            continue;
        }

        if (access(filename, F_OK) != 0)
        {
            printf("File '%s' does not exist.\n", filename);
            continue;
        }

        FILE *file = fopen(filename, "rb");
        if (file == NULL)
        {
            printf("Error opening file: %s\n", strerror(errno));
            continue;
        }

        size_t signatureLength = strlen("MY_SIGNATURE");
        char fileSignature[signatureLength + 1];
        memset(fileSignature, 0, sizeof(fileSignature));

        fread(fileSignature, sizeof(char), signatureLength, file);
        fclose(file);

        if (strcmp(fileSignature, "MY_SIGNATURE") != 0)
        {
            printf("File '%s' is not recognized as a valid file. It will not be deleted.\n", filename);
            continue;
        }

        if (remove(filename) == 0)
        {
            printf("File '%s' deleted successfully.\n", filename);
        }
        else
        {
            printf("Error deleting file: %s\n", strerror(errno));
        }

        valid = true;
    } while (!valid);
}

void readSingleRecord() {
    char filename[256];
    bool valid;
    int index;

    do {
        printf("Enter the name of the file to read: ");
        if (fgets(filename, sizeof(filename), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        // Убираем символ новой строки, если он есть
        size_t len = strlen(filename);
        if (len > 0 && filename[len - 1] == '\n') {
            filename[len - 1] = '\0';
        }

        valid = isValidFileName(filename);
        if (!valid) {
            printf("Invalid file name. Only letters, numbers, dots, underscores, and hyphens are allowed.\n");
            continue;
        }

        FILE *file = fopen(filename, "rb");
        if (file == NULL) {
            printf("Error opening file: %s\n", strerror(errno));
            valid = false;
            continue;
        }

        const char *expectedSignature = "MY_SIGNATURE";
        const int signatureLength = strlen(expectedSignature);
        char signature[signatureLength + 1];  // +1 для null-терминатора

        // Считываем сигнатуру из файла
        if (fread(signature, sizeof(char), signatureLength, file) != signatureLength) {
            printf("Invalid file format or file is corrupted.\n");
            fclose(file);
            valid = false;
            continue;
        }

        signature[signatureLength] = '\0'; // Завершаем строку символом null

        if (strcmp(signature, expectedSignature) != 0) {
            printf("Invalid file format.\n");
            fclose(file);
            valid = false;
            continue;
        }

        fclose(file);  // Закрываем файл после проверки сигнатуры
    } while (!valid);

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file: %s\n", strerror(errno));
        return;
    }

    fseek(file, 12, SEEK_SET);  // Пропускаем 12 байт сигнатуры

    // Получаем количество записей
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    long recordCount = (fileSize - 12) / sizeof(Record); // Количество записей

    printf("The file contains %ld records.\n", recordCount);

    // Запрос индекса записи
    do {
        printf("Enter the index of the record to read (1-%ld): ", recordCount);
        if (scanf("%d", &index) != 1 || index < 1 || index > recordCount) {
            printf("Invalid input. Index must be a non-negative integer between 1 and %ld.\n", recordCount);
            fflush(stdin);
            valid = false;
            continue;
        }
        fflush(stdin);

        valid = true;
    } while (!valid);

    // Индексация в файле начинается с 0, поэтому уменьшаем индекс на 1
    index -= 1;

    // Чтение записи по индексу
    Record record;
    fseek(file, 12 + index * sizeof(Record), SEEK_SET); // Позиционируем указатель на нужную запись
    if (fread(&record, sizeof(Record), 1, file) == 1) {
        printf("\nRecord at index %d:\n", index + 1);  // Выводим индекс с учетом пользовательской индексации
        printf("Name: %s\nArea: %.2f\nPopulation: %.2f\n", record.name, record.area, record.population);
    } else {
        printf("Error reading record at index %d.\n", index);
    }

    fclose(file);
}

void editRecord() {
    char filename[256];
    bool valid;
    int index;

    do {
        printf("Enter the name of the file to edit: ");
        if (fgets(filename, sizeof(filename), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        // Убираем символ новой строки, если он есть
        size_t len = strlen(filename);
        if (len > 0 && filename[len - 1] == '\n') {
            filename[len - 1] = '\0';
        }

        valid = isValidFileName(filename);
        if (!valid) {
            printf("Invalid file name. Only letters, numbers, dots, underscores, and hyphens are allowed.\n");
            continue;
        }

        FILE *file = fopen(filename, "rb+");
        if (file == NULL) {
            printf("Error opening file: %s\n", strerror(errno));
            valid = false;
            continue;
        }

        const char *expectedSignature = "MY_SIGNATURE";
        const int signatureLength = strlen(expectedSignature);
        char signature[signatureLength + 1];  // +1 для null-терминатора

        // Считываем сигнатуру из файла
        if (fread(signature, sizeof(char), signatureLength, file) != signatureLength) {
            printf("Invalid file format or file is corrupted.\n");
            fclose(file);
            valid = false;
            continue;
        }

        signature[signatureLength] = '\0'; // Завершаем строку символом null

        if (strcmp(signature, expectedSignature) != 0) {
            printf("Invalid file format.\n");
            fclose(file);
            valid = false;
            continue;
        }

        fclose(file);  // Закрываем файл после проверки сигнатуры
    } while (!valid);

    FILE *file = fopen(filename, "rb+");
    if (file == NULL) {
        printf("Error opening file: %s\n", strerror(errno));
        return;
    }

    fseek(file, 12, SEEK_SET);  // Пропускаем 12 байт сигнатуры

    // Получаем количество записей
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    long recordCount = (fileSize - 12) / sizeof(Record); // Количество записей

    printf("The file contains %ld records.\n", recordCount);

    // Запрос индекса записи
    do {
        printf("Enter the index of the record to edit (1-%ld): ", recordCount);
        if (scanf("%d", &index) != 1 || index < 1 || index > recordCount) {
            printf("Invalid input. Index must be a non-negative integer between 1 and %ld.\n", recordCount);
            fflush(stdin);
            valid = false;
            continue;
        }
        fflush(stdin);

        valid = true;
    } while (!valid);

    // Индексация в файле начинается с 0, поэтому уменьшаем индекс на 1
    index -= 1;

    // Чтение записи по индексу
    Record record;
    fseek(file, 12 + index * sizeof(Record), SEEK_SET); // Позиционируем указатель на нужную запись
    if (fread(&record, sizeof(Record), 1, file) == 1) {
        printf("\nEditing record at index %d:\n", index + 1);  // Выводим индекс с учетом пользовательской индексации
        printf("Current Name: %s, Area: %.2f, Population: %.2f\n", record.name, record.area, record.population);

        // Ввод новых данных
        printf("\nEnter new name: ");
        do {
            if (fgets(record.name, sizeof(record.name), stdin) == NULL) {
                printf("Invalid input. Please try again.\n");
                continue;
            }

            size_t len = strlen(record.name);
            if (len > 0 && record.name[len - 1] == '\n') {
                record.name[len - 1] = '\0';
            }

            if (strlen(record.name) == 0 || strlen(record.name) > 49) {
                printf("Name is too long or empty. Please enter a valid name (1-49 characters).\n");
                continue;
            }

            break;
        } while (true);

        printf("Enter new area: ");
        do {
            if (scanf("%f", &record.area) == 1 && record.area > 0) {
                fflush(stdin);
                break;
            }
            printf("Invalid input. Area must be a positive number. Try again: ");
            fflush(stdin);
        } while (record.area <= 0);

        printf("Enter new population: ");
        do {
            if (scanf("%f", &record.population) == 1 && record.population > 0) {
                fflush(stdin);
                break;
            }
            printf("Invalid input. Population must be a positive number. Try again: ");
            fflush(stdin);
        } while (record.population <= 0);

        fseek(file, 12 + index * sizeof(Record), SEEK_SET);
        if (fwrite(&record, sizeof(Record), 1, file) == 1) {
            printf("Record updated successfully.\n");
        }
        else {
            printf("Error updating record.\n");
        }
    }
    else {
        printf("No record found at index %d.\n", index);
    }

    fclose(file);
}

void sortRecords()
{
    char filename[256];
    bool valid;

    do
    {
        printf("Enter the name of the file to sort: ");
        if (fgets(filename, sizeof(filename), stdin) == NULL)
        {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        size_t len = strlen(filename);
        if (len > 0 && filename[len - 1] == '\n')
        {
            filename[len - 1] = '\0';
        }

        valid = isValidFileName(filename);
        if (!valid)
        {
            printf("Invalid file name. Only letters, numbers, dots, underscores, and hyphens are allowed.\n");
            continue;
        }

        FILE *file = fopen(filename, "rb+");
        if (file == NULL)
        {
            printf("Error opening file: %s\n", strerror(errno));
            valid = false;
            continue;
        }

        const int signatureLength = 12;
        char signature[signatureLength];
        if (fread(signature, sizeof(char), signatureLength - 1, file) != signatureLength - 1)
        {
            printf("Invalid file format or file is corrupted.\n");
            fclose(file);
            valid = false;
            continue;
        }
        signature[signatureLength - 1] = '\0';

        if (strcmp(signature, "MY_SIGNATURE") != 0)
        {
            printf("Invalid file format.\n");
            fclose(file);
            valid = false;
            continue;
        }

        fclose(file);
    } while (!valid);

    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file or file does not exist.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    int count = ftell(file) / sizeof(Record);
    fseek(file, 0, SEEK_SET);

    Record *records = (Record *)malloc(count * sizeof(Record));
    if (records == NULL)
    {
        printf("Memory allocation error.\n");
        fclose(file);
        return;
    }

    fread(records, sizeof(Record), count, file);
    fclose(file);

    int sortField;
    printf("Choose the field to sort by:\n");
    printf("1 - Name\n");
    printf("2 - Area\n");
    printf("3 - Population\n");
    printf("Enter your choice: ");
    while (scanf("%d", &sortField) != 1 || sortField < 1 || sortField > 3)
    {
        printf("Invalid choice. Please enter 1, 2, or 3: ");
        fflush(stdin);
    }
    fflush(stdin);

    int ascending;
    printf("Choose the sort order:\n");
    printf("1 - Ascending\n");
    printf("2 - Descending\n");
    printf("Enter your choice: ");
    while (scanf("%d", &ascending) != 1 || (ascending != 1 && ascending != 2))
    {
        printf("Invalid choice. Please enter 1 or 2: ");
        fflush(stdin);
    }
    fflush(stdin);

    int isAscending = (ascending == 1);

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            int condition = 0;
            if (sortField == 1)
            {
                condition = isAscending ? (strcmp(records[j].name, records[j + 1].name) > 0) : (strcmp(records[j].name, records[j + 1].name) < 0);
            }
            else if (sortField == 2)
            {
                condition = isAscending ? (records[j].area > records[j + 1].area) : (records[j].area < records[j + 1].area);
            }
            else if (sortField == 3)
            {
                condition = isAscending ? (records[j].population > records[j + 1].population) : (records[j].population < records[j + 1].population);
            }

            if (condition)
            {
                Record temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }

    file = fopen(filename, "wb");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        free(records);
        return;
    }

    fwrite(records, sizeof(Record), count, file);
    fclose(file);
    free(records);

    printf("Records sorted by %s in %s order successfully.\n",
           (sortField == 1 ? "Name" : (sortField == 2 ? "Area" : "Population")),
           (isAscending ? "ascending" : "descending"));
}

void insertRecord()
{
    char filename[256];
    Record newRecord;
    FILE *file;

    printf("Enter the name of the file to insert the record into: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL)
    {
        printf("Error reading input. Operation aborted.\n");
        return;
    }

    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n')
    {
        filename[len - 1] = '\0';
    }

    if (!isValidFileName(filename))
    {
        printf("Invalid file name. Only letters, numbers, dots, underscores, and hyphens are allowed.\n");
        return;
    }

    file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file or file does not exist.\n");
        return;
    }

    const char *expectedSignature = "MY_SIGNATURE";
    size_t signatureLength = strlen(expectedSignature);
    char fileSignature[signatureLength + 1];
    memset(fileSignature, 0, sizeof(fileSignature));

    fread(fileSignature, sizeof(char), signatureLength, file);
    if (strcmp(fileSignature, expectedSignature) != 0)
    {
        printf("Invalid file format. Operation aborted.\n");
        fclose(file);
        return;
    }

    fseek(file, 0, SEEK_END);
    int count = (ftell(file) - signatureLength) / sizeof(Record);
    fseek(file, signatureLength, SEEK_SET);

    Record *records = (Record *)malloc((count + 1) * sizeof(Record));
    if (records == NULL)
    {
        printf("Memory allocation error.\n");
        fclose(file);
        return;
    }

    fread(records, sizeof(Record), count, file);
    fclose(file);

    printf("Enter name of the region (max 50 characters): ");
    while (scanf("%49s", newRecord.name) != 1 || strlen(newRecord.name) > 50)
    {
        printf("Invalid input. Name should be at most 50 characters. Try again: ");
        fflush(stdin);
    }
    fflush(stdin);

    printf("Enter area (positive number): ");
    while (scanf("%f", &newRecord.area) != 1 || newRecord.area <= 0)
    {
        printf("Invalid input. Area must be a positive number. Try again: ");
        fflush(stdin);
    }
    fflush(stdin);

    printf("Enter population (positive number): ");
    while (scanf("%f", &newRecord.population) != 1 || newRecord.population <= 0)
    {
        printf("Invalid input. Population must be a positive number. Try again: ");
        fflush(stdin);
    }
    fflush(stdin);

    int i;
    for (i = count - 1; i >= 0 && strcmp(records[i].name, newRecord.name) > 0; i--)
    {
        records[i + 1] = records[i];
    }
    records[i + 1] = newRecord;

    file = fopen(filename, "wb");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        free(records);
        return;
    }

    fwrite(expectedSignature, sizeof(char), signatureLength, file);
    fwrite(records, sizeof(Record), count + 1, file);
    fclose(file);
    free(records);

    printf("Record inserted successfully into file '%s'.\n", filename);
}

void deleteRecord()
{
    char filename[256];
    int index;
    FILE *file;

    printf("Enter the name of the file to delete a record from: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL)
    {
        printf("Error reading input. Operation aborted.\n");
        return;
    }

    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n')
    {
        filename[len - 1] = '\0';
    }

    if (!isValidFileName(filename))
    {
        printf("Invalid file name. Only letters, numbers, dots, underscores, and hyphens are allowed.\n");
        return;
    }

    file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file or file does not exist.\n");
        return;
    }

    const char *expectedSignature = "MY_SIGNATURE";
    size_t signatureLength = strlen(expectedSignature);
    char fileSignature[signatureLength + 1];
    memset(fileSignature, 0, sizeof(fileSignature));

    fread(fileSignature, sizeof(char), signatureLength, file);
    if (strcmp(fileSignature, expectedSignature) != 0)
    {
        printf("Invalid file format. Operation aborted.\n");
        fclose(file);
        return;
    }

    fseek(file, 0, SEEK_END);
    int count = (ftell(file) - signatureLength) / sizeof(Record);
    if (count == 0)
    {
        printf("The file contains no records. Nothing to delete.\n");
        fclose(file);
        return;
    }
    fseek(file, signatureLength, SEEK_SET);

    Record *records = (Record *)malloc(count * sizeof(Record));
    if (records == NULL)
    {
        printf("Memory allocation error.\n");
        fclose(file);
        return;
    }

    fread(records, sizeof(Record), count, file);
    fclose(file);

    printf("Enter the index of the record to delete (0 to %d): ", count - 1);
    while (scanf("%d", &index) != 1 || index < 0 || index >= count)
    {
        printf("Invalid input. Index must be between 0 and %d. Try again: ", count - 1);
        fflush(stdin);
    }
    fflush(stdin);

    for (int i = index; i < count - 1; i++)
    {
        records[i] = records[i + 1];
    }

    file = fopen(filename, "wb");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        free(records);
        return;
    }

    fwrite(expectedSignature, sizeof(char), signatureLength, file);
    fwrite(records, sizeof(Record), count - 1, file);
    fclose(file);
    free(records);

    printf("Record at index %d deleted successfully from file '%s'.\n", index, filename);
}

void menu()
{
    int choice;

    do
    {
        printf("\nMenu:\n");
        printf("1. Create file\n");
        printf("2. Read file\n");
        printf("3. Delete file\n");
        printf("4. Add record\n");
        printf("5. Read single record\n");
        printf("6. Edit record\n");
        printf("7. Sort records\n");
        printf("8. Insert record\n");
        printf("9. Delete record\n");
        printf("0. Exit\n");

        int validChoice;
        do {
            printf("Enter your choice: ");
            validChoice = scanf("%d", &choice);
            if (validChoice != 1)
            {
                printf("Invalid input. Please enter a number.\n");
                fflush(stdin);
                continue;
            }
            fflush(stdin);
        } while (validChoice != 1);

        switch (choice)
        {
        case 1:
            createFile();
            break;
        case 2:
            readFile();
            break;
        case 3:
            deleteFile();
            break;
        case 4:
            addRecord();
            break;
        case 5:
        {
            readSingleRecord();
            break;
        }
        case 6:
        {
            editRecord();
            break;
        }
        case 7:
        {
            sortRecords();
            break;
        }
        case 8:
        {
            insertRecord();
            break;
        }
        case 9:
        {
            deleteRecord();
            break;
        }
        case 0:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}

int main()
{
    menu();

    return 0;
}
