void deleteRecord()
{
    char filename[256];
    int index;
    FILE *file;

    printf("Enter the name of the file to delete a record from: ");
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
    if (count == 0)
    {
        printf("The file contains no records. Nothing to delete.\n");
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

    printf("Enter the index of the record to delete (1 to %d): ", count);
    while (scanf("%d", &index) != 1 || index < 1 || index > count)
    {
        printf("Invalid input. Index must be between 1 and %d. Try again: ", count);
        fflush(stdin);
    }
    fflush(stdin);

    index--;

    for (int i = index; i < count - 1; i++)
    {
        records[i] = records[i + 1];
    }

    file = fopen(filename, "wb");

    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        free(records);
        return;
    }

    fwrite(MY_SIGNATURE, sizeof(char), signatureLength, file);
    fwrite(records, sizeof(Record), count - 1, file);
    fclose(file);
    free(records);

    printf("Record at index %d deleted successfully from file '%s'.\n", index + 1, filename);
}