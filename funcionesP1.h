//
// Created by sergio on 5/10/23.
//

#ifndef P1_FUNCIONESP1_H
#define P1_FUNCIONESP1_H
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "limits.h"
#include "string.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "dirent.h"
#include "time.h"
#include <pwd.h>
#include <ftw.h>
#define MAXBUFF 1000
void statFunction(char *comando[]);
void createFunction(char *nombre[]);
void listFunction(char *comando[]);
void deleteFunction(char *comando[]);
void deltreeFunction(char *comando[]);
#endif //P1_FUNCIONESP1_H
