#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string.h>
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
        printf("Enter the name of the file to create (letters, numbers, dots, underscores, and hyphens only): ");
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

void readFile()
{
    char filename[256];
    bool valid;
    FILE *file;

    do
    {
        printf("Enter the name of the file to read: ");
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

        file = fopen(filename, "rb");
        if (file == NULL)
        {
            printf("Error opening file or file does not exist: %s\n", strerror(errno));
            valid = false;
        }
    } while (!valid);

    const int signatureLength = 12;
    char signature[signatureLength];
    if (fread(signature, sizeof(char), signatureLength - 1, file) != signatureLength - 1)
    {
        printf("Error reading file signature.\n");
        fclose(file);
        return;
    }
    signature[signatureLength - 1] = '\0';

    if (strcmp(signature, "MY_SIGNATURE") != 0)
    {
        printf("Invalid file format.\n");
        fclose(file);
        return;
    }

    Record record;
    printf("\nRecords in file:\n");
    while (fread(&record, sizeof(Record), 1, file) == 1)
    {
        printf("Name: %s, Area: %.2f, Population: %.2f\n", record.name, record.area, record.population);
    }

    if (feof(file))
    {
        printf("End of file reached.\n");
    }
    else
    {
        printf("Error reading file.\n");
    }

    fclose(file);
}

void addRecord()
{
    char filename[256];
    bool valid;
    FILE *file;

    do
    {
        printf("Enter the name of the file to add record: ");
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

        file = fopen(filename, "rb+");
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
            printf("Invalid file format.\n");
            fclose(file);
            return;
        }
        signature[signatureLength - 1] = '\0';

        if (strcmp(signature, "MY_SIGNATURE") != 0)
        {
            printf("Invalid file format.\n");
            fclose(file);
            return;
        }

        fseek(file, 0, SEEK_END);
    } while (!valid);

    Record record;

    do
    {
        printf("Enter name of the region (max 50 characters): ");
        if (fgets(record.name, sizeof(record.name), stdin) == NULL)
        {
            printf("Error reading input. Please try again.\n");
            valid = false;
            continue;
        }

        size_t len = strlen(record.name);
        if (len > 0 && record.name[len - 1] == '\n')
        {
            record.name[len - 1] = '\0';
        }

        printf("Enter area: ");
        if (scanf("%f", &record.area) != 1 || record.area < 0)
        {
            printf("Invalid input. Area must be a positive number.\n");
            valid = false;
            fflush(stdin);
            continue;
        }
        fflush(stdin);

        printf("Enter population: ");
        if (scanf("%f", &record.population) != 1 || record.population < 0)
        {
            printf("Invalid input. Population must be a positive number.\n");
            valid = false;
            fflush(stdin);
            continue;
        }
        fflush(stdin);

        valid = true;
    } while (!valid);

    fwrite(&record, sizeof(Record), 1, file);
    printf("Record added successfully.\n");
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
            printf("Invalid file name. Only letters, numbers, dots, underscores, and hyphens are allowed.\n");
            continue;
        }

        if (access(filename, F_OK) != 0)
        {
            printf("File '%s' does not exist.\n", filename);
            valid = false;
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

void readSingleRecord()
{
    char filename[256];
    bool valid;
    int index;

    do
    {
        printf("Enter the name of the file to read: ");
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

        FILE *file = fopen(filename, "rb");
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
        printf("Error opening file: %s\n", strerror(errno));
        return;
    }

    fseek(file, 12, SEEK_SET);

    do
    {
        printf("Enter the index of the record to read: ");
        if (scanf("%d", &index) != 1 || index < 0)
        {
            printf("Invalid input. Index must be a non-negative integer.\n");
            fflush(stdin);
            valid = false;
            continue;
        }
        fflush(stdin);

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        long recordCount = (fileSize - 12) / sizeof(Record);

        if (index >= recordCount)
        {
            printf("Index out of bounds. File contains %ld records.\n", recordCount);
            valid = false;
        }
        else
        {
            valid = true;
        }
    } while (!valid);

    Record record;
    fseek(file, 12 + index * sizeof(Record), SEEK_SET);
    if (fread(&record, sizeof(Record), 1, file) == 1)
    {
        printf("\nRecord at index %d:\n", index);
        printf("Name: %s\nArea: %.2f\nPopulation: %.2f\n", record.name, record.area, record.population);
    }
    else
    {
        printf("Error reading record at index %d.\n", index);
    }

    fclose(file);
}

void editRecord()
{
    char filename[256];
    bool valid;
    int index;

    do
    {
        printf("Enter the name of the file to edit: ");
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

    FILE *file = fopen(filename, "rb+");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", strerror(errno));
        return;
    }

    fseek(file, 12, SEEK_SET);

    do
    {
        printf("Enter the index of the record to edit: ");
        if (scanf("%d", &index) != 1 || index < 0)
        {
            printf("Invalid input. Index must be a non-negative integer.\n");
            clearInputBuffer();
            valid = false;
            continue;
        }
        clearInputBuffer();

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        long recordCount = (fileSize - 12) / sizeof(Record);

        if (index >= recordCount)
        {
            printf("Index out of bounds. File contains %ld records.\n", recordCount);
            valid = false;
        }
        else
        {
            valid = true;
        }
    } while (!valid);

    Record record;
    fseek(file, 12 + index * sizeof(Record), SEEK_SET);
    if (fread(&record, sizeof(Record), 1, file) == 1)
    {
        printf("\nEditing record at index %d:\n", index);
        printf("Current Name: %s, Area: %.2f, Population: %.2f\n", record.name, record.area, record.population);

        printf("\nEnter new name: ");
        do
        {
            if (fgets(record.name, sizeof(record.name), stdin) == NULL)
            {
                printf("Invalid input. Please try again.\n");
                continue;
            }

            size_t len = strlen(record.name);
            if (len > 0 && record.name[len - 1] == '\n')
            {
                record.name[len - 1] = '\0';
            }

            if (strlen(record.name) == 0 || strlen(record.name) > 49)
            {
                printf("Name is too long or empty. Please enter a valid name (1-49 characters).\n");
                continue;
            }

            break;
        } while (true);

        printf("Enter new area: ");
        do
        {
            if (scanf("%f", &record.area) == 1 && record.area > 0)
            {
                fflush(stdin);
                break;
            }
            printf("Invalid input. Area must be a positive number. Try again: ");
            fflush(stdin);
        } while (record.area <= 0);

        printf("Enter new population: ");
        do
        {
            if (scanf("%f", &record.population) == 1 && record.population > 0)
            {
                fflush(stdin);
                break;
            }
            printf("Invalid input. Population must be a positive number. Try again: ");
            fflush(stdin);
        } while (record.population <= 0);

        fseek(file, 12 + index * sizeof(Record), SEEK_SET);
        if (fwrite(&record, sizeof(Record), 1, file) == 1)
        {
            printf("Record updated successfully.\n");
        }
        else
        {
            printf("Error updating record.\n");
        }
    }
    else
    {
        printf("No record found at index %d.\n", index);
    }

    fclose(file);
}

// end

void sortRecords(const char *filename, int ascending)
{
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

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            int condition = ascending ? (strcmp(records[j].name, records[j + 1].name) > 0) : (strcmp(records[j].name, records[j + 1].name) < 0);
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

    printf("Records sorted %s successfully.\n", ascending ? "in ascending order" : "in descending order");
}

void insertRecord(const char *filename, Record newRecord)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file or file does not exist.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    int count = ftell(file) / sizeof(Record);
    fseek(file, 0, SEEK_SET);

    Record *records = (Record *)malloc((count + 1) * sizeof(Record));
    if (records == NULL)
    {
        printf("Memory allocation error.\n");
        fclose(file);
        return;
    }

    fread(records, sizeof(Record), count, file);
    fclose(file);

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

    fwrite(records, sizeof(Record), count + 1, file);
    fclose(file);
    free(records);

    printf("Record inserted successfully.\n");
}

void deleteRecord(const char *filename, int index)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file or file does not exist.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    int count = ftell(file) / sizeof(Record);
    fseek(file, 0, SEEK_SET);

    if (index < 0 || index >= count)
    {
        printf("Invalid index.\n");
        fclose(file);
        return;
    }

    Record *records = (Record *)malloc(count * sizeof(Record));
    if (records == NULL)
    {
        printf("Memory allocation error.\n");
        fclose(file);
        return;
    }

    fread(records, sizeof(Record), count, file);
    fclose(file);

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

    fwrite(records, sizeof(Record), count - 1, file);
    fclose(file);
    free(records);

    printf("Record deleted successfully.\n");
}

void menu()
{
    const char *filename = "regions.dat";
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
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            createFile();
            break;
        case 2:
            readFile(filename);
            break;
        case 3:
            deleteFile(filename);
            break;
        case 4:
            addRecord(filename);
            break;
        case 5:
        {
            readSingleRecord(filename, index);
            break;
        }
        case 6:
        {
            int index;

            printf("Enter record index to edit: ");
            scanf("%d", &index);
            editRecord(filename, index);

            break;
        }
        case 7:
        {
            int ascending;

            printf("Enter 1 for ascending or 0 for descending order: ");
            scanf("%d", &ascending);
            sortRecords(filename, ascending);

            break;
        }
        case 8:
        {
            Record newRecord;

            printf("Enter name of the region: ");
            scanf("%49s", newRecord.name);
            printf("Enter area: ");
            scanf("%f", &newRecord.area);
            printf("Enter population: ");
            scanf("%f", &newRecord.population);
            insertRecord(filename, newRecord);

            break;
        }
        case 9:
        {
            int index;

            printf("Enter record index to delete: ");
            scanf("%d", &index);
            deleteRecord(filename, index);

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
