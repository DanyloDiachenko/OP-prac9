void readSingleRecord()
{
    char filename[256];
    FILE *file;

    if (!getFileName(filename, sizeof(filename), "Enter the name of the file to read: "))
    {
        return;
    }

    if (!openAndValidateFile(filename, &file))
    {
        return;
    }

    int signatureLength = strlen(MY_SIGNATURE);
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    long recordCount = (fileSize - signatureLength) / sizeof(Record);
    if (recordCount <= 0)
    {
        printf("The file contains no records.\n");
        fclose(file);
        return;
    }

    printf("The file contains %ld records.\n", recordCount);

    int index;
    if (!getRecordIndex(&index, recordCount, "Enter the index of the record to read"))
    {
        fclose(file);
        return;
    }

    Record record;
    fseek(file, signatureLength + (index - 1) * sizeof(Record), SEEK_SET);
    if (fread(&record, sizeof(Record), 1, file) == 1)
    {
        printf("\nRecord at index %d:\n", index);
        printf("Name: %s\nArea: %.2f\nPopulation: %.2f\n", record.name, record.area, record.population);
    }
    else
    {
        printf("Error reading record at index %d.\n", index);
    }

    fclose(file);
}
