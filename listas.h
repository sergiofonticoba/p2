//
// Created by sergio on 21/09/23.
//

#ifndef P0_LISTAS_H
#define P0_LISTAS_H

#include "stdbool.h"
#include "stdlib.h"

#define LNULL NULL
typedef struct tItemL{
    char  nombre[200];
    int  descriptor;
    int  mode;
}tItemL;
typedef struct tNode* tPosL;
typedef struct tNode{
    tItemL data;
    tPosL sig;
}tNode;
typedef tPosL tList;
char *getItem(int n, tList l);
char *getFichero(int desc, tList l);
void verFicheros(tList l);
void verComandos(int n, tList l);
void createEmptyList(tList* l);
bool insertItemFichero(char *nombre,int mode, int desc, tList *l);
bool insertItemComando(char *nombre, tList *l);
bool createNode(tPosL *p);
void deleteList(tList *l);
void deleteFichero(int df, tList *l);
bool ExisteFichero(int df, tList l);
#endif