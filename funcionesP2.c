#include "funcionesP2.h"

void mallocFunction(char* comando[], tMem *m){
    int tam;
    void* direcion;
    time_t t = time(NULL);
    struct tm tiempo = *localtime(&t);
    if(comando[1]!=NULL){
        if(strcmp(comando[1], "-free")!=0){
            if((tam=atoi(comando[1])>0)) {
                direcion = malloc(tam);
                if (direcion != NULL) {
                    if(!insertarDirMalloc(direcion, tam, tiempo, Malloc_Memory, m))
                        perror("malloc: ");
                }else perror("malloc: ");
            }
        }else if(comando[2]!=NULL){
            
        }
    }
}