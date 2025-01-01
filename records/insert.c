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

    if (!validateFileName(filename))
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

    int signatureLength = strlen(MY_SIGNATURE);
    char signature[signatureLength + 1];

    if (fread(signature, sizeof(char), signatureLength, file) != signatureLength)
    {
        printf("Invalid file format or file is corrupted.\n");
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

    printf("Enter name of the region (max %d characters): ", MAX_RECORD_NAME_SIZE);
    while (scanf("%49s", newRecord.name) != 1 || strlen(newRecord.name) > MAX_RECORD_NAME_SIZE)
    {
        printf("Invalid input. Name should be at most %d characters. Try again: ", MAX_RECORD_NAME_SIZE);
        fflush(stdin);
    }
    fflush(stdin);

    printf("Enter area (positive number): ");
    while (scanf("%f", &newRecord.area) != 1 || newRecord.area <= 0 || newRecord.area > MAX_RECORD_AREA)
    {
        printf("Invalid input. Area must be a positive number and less than or equal to %.2f. Try again: ", MAX_RECORD_AREA);
        fflush(stdin);
    }
    fflush(stdin);

    printf("Enter population (positive number): ");
    while (scanf("%f", &newRecord.population) != 1 || newRecord.population <= 0 || newRecord.population > MAX_RECORD_POPULATION)
    {
        printf("Invalid input. Population must be a positive number and less than or equal to %.2f. Try again: ", MAX_RECORD_POPULATION);
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

    fwrite(MY_SIGNATURE, sizeof(char), signatureLength, file);
    fwrite(records, sizeof(Record), count + 1, file);
    fclose(file);
    free(records);

    printf("Record inserted successfully into file '%s'.\n", filename);
}
