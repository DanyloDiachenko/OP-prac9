void deleteFile()
{
    char filename[MAX_FILE_NAME_LENGTH];
    bool validity;

    do
    {
        printf("Enter the name of the file to delete: ");
        validity = validateFileName(filename, sizeof(filename));

        if (!validity)
            continue;

        if (!validateFileExisting(filename))
        {
            printf("File '%s' does not exist.\n", filename);
            validity = false;
            continue;
        }

        FILE *file = fopen(filename, "rb");
        if (file == NULL)
        {
            printf("Error opening file: %s\n", strerror(errno));
            validity = false;
            continue;
        }

        if (!validateFileSignature(file))
        {
            fclose(file);
            return;
        }

        fclose(file);

        if (remove(filename) == 0)
        {
            printf("File '%s' deleted successfully.\n", filename);
            validity = true;
        }
        else
        {
            printf("Error deleting file: %s\n", strerror(errno));
            validity = false;
        }
    } while (!validity);
}