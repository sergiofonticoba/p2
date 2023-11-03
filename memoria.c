
#include "memoria.h"

bool insertarDirMalloc(void* address, int tam, struct tm time,  typesAlloc type, tMem *m){
    tPosM p, q;
    if(!createNode(&p)){
        return false;
    }else{
        p->data.address=address;
        p->data.size=tam;
        p->data.typesAllocation=type;
        p->data.time=time;
        p->sig=MNULL;
        if((*m)==MNULL){
            (*m)=p;
        }else{
            for(q=(*m); q->sig!=MNULL; q=q->sig);
            q->sig=p;
        }
        return true;
    }
}

bool createNode(tPosM *p){
    *p=malloc(sizeof(**p));
    return *p!=MNULL;
}