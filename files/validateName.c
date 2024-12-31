bool validateFileName(char *filename)
{
    if (strlen(filename) == 0 || strlen(filename) > 255)
        return 0;

    for (int i = 0; filename[i] != '\0'; i++)
    {
        if (!isalnum(filename[i]) && filename[i] != '.' && filename[i] != '_' && filename[i] != '-')
        {
            return false;
        }
    }

    return true;
}