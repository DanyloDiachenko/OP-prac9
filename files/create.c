void createFile()
{
    char filename[256];
    FILE *file;

    while (true)
    {
        printf("Enter the name of the file to create (letters, numbers, dots, underscores, and hyphens only) including '.dat' extension: ");
        if (!validateAndSanitizeFileName(filename, sizeof(filename)))
            continue;

        if (validateFileExisting(filename))
        {
            printf("File '%s' already exists. Please choose a different name.\n", filename);
            continue;
        }

        file = fopen(filename, "wb");
        if (file == NULL)
        {
            printf("Error creating file: %s\n", strerror(errno));
            continue;
        }

        if (fwrite(MY_SIGNATURE, sizeof(char), strlen(MY_SIGNATURE), file) != strlen(MY_SIGNATURE))
        {
            printf("Error writing to file.\n");
        }
        else
        {
            printf("File '%s' created successfully.\n", filename);
        }

        fclose(file);
        break;
    }
}