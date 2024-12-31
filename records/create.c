#include "../includes.h"

void createRecord()
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

        const char *expectedSignature = "MY_SIGNATURE";
        const int signatureLength = strlen(expectedSignature);
        char signature[signatureLength + 1];

        if (fread(signature, sizeof(char), signatureLength, file) != signatureLength)
        {
            printf("Error reading file signature.\n");
            fclose(file);
            continue;
        }
        signature[signatureLength] = '\0';

        if (strcmp(signature, expectedSignature) != 0)
        {
            printf("Invalid file format.\n");
            fclose(file);
            continue;
        }

        fseek(file, 0, SEEK_END);
    } while (!valid);

    Record record;
    bool inputValid = false;

    do
    {
        printf("Enter name of the region (max 50 characters): ");
        if (fgets(record.name, sizeof(record.name), stdin) == NULL)
        {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        size_t len = strlen(record.name);
        if (len > 0 && record.name[len - 1] == '\n')
        {
            record.name[len - 1] = '\0';
        }

        inputValid = true;
    } while (!inputValid);

    do
    {
        printf("Enter area: ");
        if (scanf("%f", &record.area) != 1 || record.area <= 0)
        {
            printf("Invalid input. Area must be a positive number.\n");
            fflush(stdin);
        }
        else
        {
            inputValid = true;
        }
        fflush(stdin);
    } while (!inputValid);

    do
    {
        printf("Enter population: ");
        if (scanf("%f", &record.population) != 1 || record.population <= 0)
        {
            printf("Invalid input. Population must be a positive number.\n");
            fflush(stdin);
        }
        else
        {
            inputValid = true; // Ввод корректен
        }
        fflush(stdin);
    } while (!inputValid);

    fwrite(&record, sizeof(Record), 1, file);
    printf("Record added successfully.\n");

    fclose(file);
}
