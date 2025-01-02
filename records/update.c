void updateRecord()
{
    char filename[MAX_FILE_NAME_LENGTH];
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

        filename[strcspn(filename, "\n")] = '\0';

        valid = validateCreatingFileName(filename);
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

        if (validateFileSignature(file))
        {
            valid = true;
        }
        else
        {
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

    if (recordCount == 0)
    {
        printf("The file contains no records. Nothing to update.\n");
        fclose(file);
        return;
    } else {
        printf("The file contains %ld records.\n", recordCount);
    }

    do
    {
        printf("Enter the index of the record to edit (1-%ld): ", recordCount);
        if (scanf("%d", &index) != 1 || index < 1 || index > recordCount)
        {
            printf("Invalid input. Index must be a non-negative integer between 1 and %ld.\n", recordCount);
            fflush(stdin);
            valid = false;
        }
        else
        {
            valid = true;
        }
    } while (!valid);

    index -= 1;

    Record record;
    fseek(file, 12 + index * sizeof(Record), SEEK_SET);
    if (fread(&record, sizeof(Record), 1, file) == 1)
    {
        printf("\nEditing record at index %d:\n", index + 1);
        printf("Current Name: %s, Area: %.2f, Population: %.2f\n", record.name, record.area, record.population);

        getRecordDetails(&record);

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
