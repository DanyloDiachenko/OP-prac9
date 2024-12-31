void updateRecord()
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

        valid = validateFileName(filename);
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

        int signatureLength = strlen(MY_SIGNATURE);
        char signature[signatureLength + 1];

        if (fread(signature, sizeof(char), signatureLength, file) != signatureLength)
        {
            printf("Invalid file format or file is corrupted.\n");
            fclose(file);
            valid = false;
            continue;
        }

        signature[signatureLength] = '\0';

        if (strcmp(signature, MY_SIGNATURE) != 0)
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

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    long recordCount = (fileSize - 12) / sizeof(Record);

    printf("The file contains %ld records.\n", recordCount);

    do
    {
        printf("Enter the index of the record to edit (1-%ld): ", recordCount);
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
        printf("\nEditing record at index %d:\n", index + 1);
        printf("Current Name: %s, Area: %.2f, Population: %.2f\n", record.name, record.area, record.population);

        printf("\nEnter new name: ");
        do
        {
            printf("Enter name (1-49 characters): ");
            if (fgets(record.name, sizeof(record.name), stdin) == NULL)
            {
                printf("Invalid input. Please try again.\n");
            }
            else
            {
                size_t len = strlen(record.name);
                if (len > 0 && record.name[len - 1] == '\n')
                {
                    record.name[len - 1] = '\0';
                }

                if (strlen(record.name) > 0 && strlen(record.name) <= 49)
                {
                    break;
                }
                else
                {
                    printf("Name is too long or empty. Please enter a valid name (1-49 characters).\n");
                }
            }
        } while (strlen(record.name) == 0 || strlen(record.name) > 49);

        printf("Enter new area: ");
        do
        {
            printf("Enter a positive number for area (≤ %.2f): ", MAX_RECORD_AREA);
            if (scanf("%f", &record.area) != 1 || record.area <= 0 || record.area > MAX_RECORD_AREA)
            {
                printf("Invalid input. Area must be a positive number and ≤ %.2f.\n", MAX_RECORD_AREA);
                fflush(stdin);
            }
        } while (record.area <= 0 || record.area > MAX_RECORD_AREA);

        printf("Enter new population: ");
        do
        {
            printf("Enter a positive number for population (≤ %.2f): ", MAX_RECORD_POPULATION);
            if (scanf("%f", &record.population) != 1 || record.population <= 0 || record.population > MAX_RECORD_POPULATION)
            {
                printf("Invalid input. Population must be a positive number and ≤ %.2f.\n", MAX_RECORD_POPULATION);
                fflush(stdin);
            }
        } while (record.population <= 0 || record.population > MAX_RECORD_POPULATION);

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
