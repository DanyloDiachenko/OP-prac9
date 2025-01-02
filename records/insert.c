void insertRecord()
{
    char filename[MAX_FILE_NAME_LENGTH];
    Record newRecord;
    FILE *file;

    if (!getFileName(filename, sizeof(filename), "Enter the name of the file to insert the record into: "))
    {
        return;
    }

    if (!openAndValidateFile(filename, &file))
    {
        return;
    }

    int signatureLength = strlen(MY_SIGNATURE);
    fseek(file, 0, SEEK_END);
    int count = (ftell(file) - signatureLength) / sizeof(Record);
    if (count <= 0)
    {
        printf("The file contains no records.\n");
        fclose(file);
        return;
    }

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

    int sortField = getSortField();
    int isAscending = getSortOrder();

    getRecordDetails(&newRecord);

    int i;
    for (i = count - 1; i >= 0; i--)
    {
        if (compareRecords(&records[i], &newRecord, sortField, isAscending) <= 0)
        {
            break;
        }
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
