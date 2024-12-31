#include "../functions.h"

void readFile()
{
    char filename[256];
    FILE *file;
    bool valid;

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

        valid = validateFileName(filename);
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

    const int signatureLength = strlen(MY_SIGNATURE);
    char signature[signatureLength + 1];

    if (fread(signature, sizeof(char), signatureLength, file) != signatureLength)
    {
        printf("Error reading file signature.\n");
        fclose(file);
        return;
    }
    signature[signatureLength] = '\0';

    if (strcmp(signature, MY_SIGNATURE) != 0)
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
