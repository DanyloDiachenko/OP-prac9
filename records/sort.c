void sortRecords()
{
    char filename[MAX_FILE_NAME_LENGTH];
    FILE *file;

    printf("Enter the name of the file to sort: ");
    scanf("%s", filename);

    file = fopen(filename, "rb");
    if (!file)
    {
        printf("Error opening file '%s'.\n", filename);
        return;
    }

    int signatureLength = strlen(MY_SIGNATURE);
    char signature[signatureLength + 1];
    fread(signature, sizeof(char), signatureLength, file);
    signature[signatureLength] = '\0';
    if (strcmp(signature, MY_SIGNATURE) != 0)
    {
        printf("Invalid file signature.\n");
        fclose(file);
        return;
    }

    fseek(file, 0, SEEK_END);
    int count = (ftell(file) - signatureLength) / sizeof(Record);
    if (count <= 0)
    {
        printf("The file contains no records to sort.\n");
        fclose(file);
        return;
    }

    fseek(file, signatureLength, SEEK_SET);

    Record *records = malloc(count * sizeof(Record));
    if (!records)
    {
        printf("Memory allocation error.\n");
        fclose(file);
        return;
    }

    if (fread(records, sizeof(Record), count, file) != count)
    {
        printf("Error reading records from file.\n");
        free(records);
        fclose(file);
        return;
    }
    fclose(file);

    int sortField = getSortField();
    int isAscending = getSortOrder();

    sortRecordArray(records, count, sortField, isAscending);

    file = fopen(filename, "wb");
    if (!file)
    {
        printf("Error opening file for writing.\n");
        free(records);
        return;
    }

    fwrite(MY_SIGNATURE, sizeof(char), signatureLength, file);
    if (fwrite(records, sizeof(Record), count, file) != count)
    {
        printf("Error writing sorted records to file.\n");
    }
    else
    {
        printf("Records sorted successfully.\n");
    }

    fclose(file);
    free(records);
}