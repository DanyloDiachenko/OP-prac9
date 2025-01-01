#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>

#include "./constants.h"
#include "./functions.h"
#include "./menu.c"

#include "./files/create.c"
#include "./files/read.c"
#include "./files/delete.c"

#include "./helpers/getFileName.c"
#include "./helpers/getRecordData.c"
#include "./helpers/getSortField.c"
#include "./helpers/getSortOrder.c"
#include "./helpers/openAndValidateFile.c"
#include "./helpers/validateCreatingFileName.c"
#include "./helpers/validateFileExisting.c"
#include "./helpers/validateFileName.c"
#include "./helpers/validateFileSignature.c"
#include "./helpers/getRecordIndex.c"

#include "./records/create.c"
#include "./records/insert.c"
#include "./records/readSingle.c"
#include "./records/sort.c"
#include "./records/update.c"
#include "./records/delete.c"

#endif // INCLUDES_H