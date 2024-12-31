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
#include "./files/validateName.c"

#include "./records/create.c"
#include "./records/insert.c"
#include "./records/readSingle.c"
#include "./records/sort.c"
#include "./records/update.c"

#endif // INCLUDES_H