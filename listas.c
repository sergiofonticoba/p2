#include "listas.h"
#include "string.h"
#include "stdio.h"
#include "fcntl.h"
void createEmptyList(tList *l){
    *l=LNULL;
}
void verComandos(int n, tList l){
    tPosL p;
    int i=1;
    if(n==-1){
        for(p=l;p!=LNULL;p=p->sig){
            printf("%d->%s\n", i, p->data.nombre);
            i++;
        }
    }else{
        for(p=l;i<=n && p!=LNULL;p=p->sig){
            printf("%d->%s\n", i, p->data.nombre);
            i++;
        }
    }
}
char *getFichero(int desc, tList l){
    tPosL p;
    for(p=l;p!=NULL && p->data.descriptor!=desc;p=p->sig){
    }
    return p->data.nombre;
}
char *getItem(int n, tList l){
    tPosL p;
    int i=1;
    for(p=l;p!=NULL && i<n;p=p->sig){
        i++;
    }
    if(p==NULL || n==0){
        return "\0";
    }else{
        return p->data.nombre;
    }
}
void verFicheros(tList l){
    tPosL p;
    for(p=l; p!=LNULL; p=p->sig){
        switch (p->data.mode) {
            case O_CREAT:
                printf("descriptor: %d -> %s O_CREAT\n", p->data.descriptor, p->data.nombre);
                break;
            case O_EXCL:
                printf("descriptor: %d -> %s O_EXCL\n", p->data.descriptor, p->data.nombre);
                break;
            case O_RDONLY:
                printf("descriptor: %d -> %s O_RDONLY\n", p->data.descriptor, p->data.nombre);
                break;
            case O_WRONLY:
                printf("descriptor: %d -> %s O_WRONLY\n", p->data.descriptor, p->data.nombre);
                break;
            case O_RDWR:
                printf("descriptor: %d -> %s O_RDWR\n", p->data.descriptor, p->data.nombre);
                break;
            case O_APPEND:
                printf("descriptor: %d -> %s O_APPEND\n", p->data.descriptor, p->data.nombre);
                break;
            case O_TRUNC:
                printf("descriptor: %d -> %s O_TRUNC\n", p->data.descriptor, p->data.nombre);
                break;
            default:
                printf("descriptor: %d -> %s \n", p->data.descriptor, p->data.nombre);
                break;
        }
    }
}
bool insertItemFichero(char *nombre,int mode, int desc,  tList *l){
    tPosL node1, cont;
    if(!createNode(&node1)){
        return false;
    }else{
        strcpy(node1->data.nombre, nombre);
        node1->data.mode=mode;
        node1->data.descriptor=desc;
        node1->sig=LNULL;
        if((*l)==LNULL){
            (*l)=node1;
        }else{
            for(cont=(*l);cont->sig!=LNULL;cont=cont->sig){
            }
            cont->sig=node1;
        }
        return true;
    }
}
bool insertItemComando(char *nombre, tList *l){
    tPosL node1, cont;
    if(!createNode(&node1)){
        return false;
    }else{
        strcpy(node1->data.nombre, nombre);
        node1->sig=LNULL;
        if((*l)==LNULL){
            (*l)=node1;
        }else{
            for(cont=(*l);cont->sig!=LNULL;cont=cont->sig){
            }
            cont->sig=node1;
        }
        return true;
    }
}
bool createNode(tPosL *p){
    //se reserva memoria para un objeto
    *p= malloc(sizeof(**p));
    return *p!=LNULL;
}
void deleteList(tList *l){
    tPosL p;
    while((*l)!=LNULL){
        p=(*l);
        (*l)=(*l)->sig;
        free(p);
    }
}
bool ExisteFichero(int df, tList l){
    tPosL  p;
    for(p=(l); p!=NULL && p->data.descriptor!=df;p=p->sig);
    return p!=NULL;
}
void deleteFichero(int df ,tList *l){
    tPosL p=(*l), q;
    if((*l)->data.descriptor==df){
        (*l)=(*l)->sig;
    }else{
        for(p=(*l); p->sig!=LNULL && p->sig->data.descriptor!=df;p=p->sig);
        if(p->sig==LNULL) printf("no se encuentra el fichero\n");
        else{
            q=p;
            p=p->sig;
            q->sig=p->sig;
        }
    }
    free(p);

}

