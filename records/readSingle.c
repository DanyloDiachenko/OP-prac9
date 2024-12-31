#include "../includes.h"

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

        const char *expectedSignature = "MY_SIGNATURE";
        const int signatureLength = strlen(expectedSignature);
        char signature[signatureLength + 1];

        if (fread(signature, sizeof(char), signatureLength, file) != signatureLength)
        {
            printf("Invalid file format or file is corrupted.\n");
            fclose(file);
            valid = false;
            continue;
        }

        signature[signatureLength] = '\0';

        if (strcmp(signature, expectedSignature) != 0)
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

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    long recordCount = (fileSize - 12) / sizeof(Record);

    printf("The file contains %ld records.\n", recordCount);

    do
    {
        printf("Enter the index of the record to read (1-%ld): ", recordCount);
        if (scanf("%d", &index) != 1 || index < 1 || index > recordCount)
        {
            printf("Invalid input. Index must be a non-negative integer between 1 and %ld.\n", recordCount);
            fflush(stdin);
            valid = false;
            continue;
        }
        fflush(stdin);

        valid = true;
    } while (!valid);

    index -= 1;

    Record record;
    fseek(file, 12 + index * sizeof(Record), SEEK_SET);
    if (fread(&record, sizeof(Record), 1, file) == 1)
    {
        printf("\nRecord at index %d:\n", index + 1);
        printf("Name: %s\nArea: %.2f\nPopulation: %.2f\n", record.name, record.area, record.population);
    }
    else
    {
        printf("Error reading record at index %d.\n", index);
    }

    fclose(file);
}
