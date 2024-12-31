#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MY_SIGNATURE "MY_SIGNATURE"

#define MAX_RECORD_NAME_SIZE 50
#define MIN_RECORD_NAME_SIZE 0

typedef struct
{
    char name[MAX_RECORD_NAME_SIZE];
    float area;
    float population;
} Record;

#endif // CONSTANTS_H