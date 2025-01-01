void createRecord()
{
    char filename[256];
    FILE *file;

    if (!getFileName(filename, sizeof(filename), "Enter the name of the file to add record: "))
    {
        return;
    }

    if (!openAndValidateFile(filename, &file))
    {
        return;
    }

    Record record;
    getRecordDetails(&record);

    fwrite(&record, sizeof(Record), 1, file);
    printf("Record added successfully.\n");

    fclose(file);
}
