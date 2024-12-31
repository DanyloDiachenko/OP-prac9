void deleteFile()
{
    char filename[256];
    bool valid;

    do
    {
        printf("Enter the name of the file to delete: ");
        if (fgets(filename, sizeof(filename), stdin) == NULL)
        {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        size_t len = strlen(filename);
        if (len > 0 && filename[len - 1] == '\n')
        {
            filename[len - 1] = '\0';
        }

        valid = validateFileName(filename);
        if (!valid)
        {
            printf("Invalid file name. Only letters, numbers, and dots are allowed.\n");
            continue;
        }

        if (access(filename, F_OK) != 0)
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

        size_t signatureLength = strlen("MY_SIGNATURE");
        char fileSignature[signatureLength + 1];
        memset(fileSignature, 0, sizeof(fileSignature));

        fread(fileSignature, sizeof(char), signatureLength, file);
        fclose(file);

        if (strcmp(fileSignature, "MY_SIGNATURE") != 0)
        {
            printf("File '%s' is not recognized as a valid file. It will not be deleted.\n", filename);
            continue;
        }

        if (remove(filename) == 0)
        {
            printf("File '%s' deleted successfully.\n", filename);
        }
        else
        {
            printf("Error deleting file: %s\n", strerror(errno));
        }

        valid = true;
    } while (!valid);
}
