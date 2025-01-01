void deleteFile()
{
    char filename[256];

    while (true)
    {
        printf("Enter the name of the file to delete: ");
        if (!validateAndSanitizeFileName(filename, sizeof(filename)))
            continue;

        if (!validateFileExisting(filename))
        {
            printf("File '%s' does not exist.\n", filename);
            continue;
        }

        FILE *file = fopen(filename, "rb");
        if (file == NULL)
        {
            printf("Error opening file: %s\n", strerror(errno));
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
        }
        else
        {
            printf("Error deleting file: %s\n", strerror(errno));
        }

        break;
    }
}