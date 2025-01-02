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