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

    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file or file does not exist.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    int count = (ftell(file) - strlen(MY_SIGNATURE)) / sizeof(Record);
    fseek(file, strlen(MY_SIGNATURE), SEEK_SET);

    Record *records = (Record *)malloc(count * sizeof(Record));
    if (records == NULL)
    {
        printf("Memory allocation error.\n");
        fclose(file);
        return;
    }

    fread(records, sizeof(Record), count, file);
    fclose(file);

    int sortField = 0;
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

    printf("Choose the sort order:\n");
    printf("1 - Ascending\n");
    printf("2 - Descending\n");
    printf("Enter your choice: ");
    unsigned short int ascending = 0;
    do {
        if (scanf("%hu", &ascending) != 1 || (ascending != 1 && ascending != 2))
        {
            printf("Invalid choice. Please enter 1 or 2: ");
            fflush(stdin);
        }
    } while(ascending != 1 && ascending != 2);

    int isAscending = (ascending == 1);

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            int condition = 0;

            switch (sortField)
            {
            case 1:
                condition = isAscending ? (strcmp(records[j].name, records[j + 1].name) > 0) : (strcmp(records[j].name, records[j + 1].name) < 0);
                break;
            case 2:
                condition = isAscending ? (records[j].area > records[j + 1].area) : (records[j].area < records[j + 1].area);
                break;
            case 3:
                condition = isAscending ? (records[j].population > records[j + 1].population) : (records[j].population < records[j + 1].population);
                break;
            default:
                printf("Invalid sort field.\n");
                return;
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

    fwrite(MY_SIGNATURE, sizeof(char), strlen(MY_SIGNATURE), file);
    fwrite(records, sizeof(Record), count, file);
    fclose(file);
    free(records);

    printf("Records sorted by %s in %s order successfully.\n",
           (sortField == 1 ? "Name" : (sortField == 2 ? "Area" : "Population")),
           (isAscending ? "ascending" : "descending"));
}
