#ifndef PATHPARSER_H
#define PATHPARSER_H

struct path_root
{
    int drive_no;
    struct path_part* first;
};

struct path_part
{
    const char* part;
    struct path_part* next;
};

#endif