#ifndef P2_MEMORIA_P2
#define P2_MEMORIA_P2

#include <bits/types/struct_tm.h>
#include "stdbool.h"
#include "stdlib.h"
#include <stdio.h>

#define MNULL NULL
typedef enum{
    Malloc_Memory,
    Shared_Memory,
    Mapped_Files
}typesAlloc;

typedef struct tItemM{
    void*  address;
    int  size;
    struct tm time;
    typesAlloc typesAllocation;
    int key;
    char nameFmapped[200];
    int descriptorFmapped;
}tItemM;

typedef struct tNode* tPosM;

typedef struct tNode{
    tItemM data;
    tPosM sig;
}tNode;

typedef tPosM tMem;

bool insertarDirMalloc(void* address, int tam, struct tm time,  typesAlloc type, tMem *m);
char *getItem(int n, tMem l);
char *getFichero(int desc, tMem l);
void verFicheros(tMem l);
void verComandos(int n, tMem l);
void createEmptyList(tMem* l);
bool insertItemFichero(char *nombre,int mode, int desc, tMem *l);
bool insertItemComando(char *nombre, tMem *l);
bool createNode(tPosM *p);
void deleteList(tMem *l);
void deleteFichero(int df, tMem *l);
bool ExisteFichero(int df, tMem l);
#endif