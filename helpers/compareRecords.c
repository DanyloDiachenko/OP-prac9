int compareRecords(void *a, void *b, int sortField, int isAscending)
{
    const Record *recA = (const Record *)a;
    const Record *recB = (const Record *)b;

    int result = 0;
    switch (sortField)
    {
    case SORT_NAME:
        result = strcmp(recA->name, recB->name);
        break;
    case SORT_AREA:
        result = (recA->area > recB->area) - (recA->area < recB->area);
        break;
    case SORT_POPULATION:
        result = (recA->population > recB->population) - (recA->population < recB->population);
        break;
    }

    return isAscending ? result : -result;
}