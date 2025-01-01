bool validateFileExisting(const char *filename)
{
    return access(filename, F_OK) == 0;
}