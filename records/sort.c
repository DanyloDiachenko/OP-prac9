int compareRecords(const void *a, const void *b, int sortField, int isAscending)
{
    const Record *recA = (const Record *)a;
    const Record *recB = (const Record *)b;

    int result = 0;
    switch (sortField)
    {
    case 1:
        result = strcmp(recA->name, recB->name);
        break;
    case 2:
        result = (recA->area > recB->area) - (recA->area < recB->area);
        break;
    case 3:
        result = (recA->population > recB->population) - (recA->population < recB->population);
        break;
    }

    return isAscending ? result : -result;
}

void sortRecords()
{
    char filename[MAX_FILE_NAME_LENGTH];
    FILE *file;

    if (!getFileName(filename, sizeof(filename), "Enter the name of the file to sort: "))
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
        printf("The file contains no records to sort.\n");
        fclose(file);
        return;
    }

    fseek(file, signatureLength, SEEK_SET);

    Record *records = (Record *)malloc(count * sizeof(Record));
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

    qsort(records, count, sizeof(Record),
          (int (*)(const void *, const void *))compareRecords);

    file = fopen(filename, "wb");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        free(records);
        return;
    }

    fwrite(MY_SIGNATURE, sizeof(char), signatureLength, file);
    fwrite(records, sizeof(Record), count, file);
    fclose(file);
    free(records);

    printf("Records sorted by %s in %s order successfully.\n",
           (sortField == 1 ? "Name" : (sortField == 2 ? "Area" : "Population")),
           (isAscending ? "ascending" : "descending"));
}
