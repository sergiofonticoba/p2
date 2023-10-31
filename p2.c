//Sergio Fonticoba Sueiras - sergio.fonticoba@udc.es
//José Valdivieso Díaz - jose.valdiviesod@udc.es
#include <stdio.h>
#include "listas.h"
#include "funcionesP1.h"
#include "stdbool.h"
#include "string.h"
#include "time.h"
#include "unistd.h"
#include "sys/utsname.h"
#include "fcntl.h"

#define MAX 300

void imprimirPromt();

void leerEntrada(char comandoEntrada[MAX]);

int TrocearCadena(char *cadena, char *trozos[]);

void crearListaFicheros(tList *l);

void procesarEntrada(char *comando, bool *terminado, tList *lComandos, tList *lFicheros);

void tipoComando(char *tComando[], bool *terminado, tList *lComandos, tList *lFicheros);

void authorsFunction(char *tComando[]);

void pidFunction(char *tComando[]);

void chdirFunction(char *tComando[]);

void dateFunction();

void timeFunction();

void histFunction(char *tComando[], tList *lComando);

void comandFunction(char *tComando[], tList *lComandos, tList *lFicheros);

void openFunction(char *tComando[], tList *lFicheros);

void closeFunction(char *tComando[], tList *lFicheros);

void dupFunction(char *tComando[], tList *lFicheros);

void infosysFunction();

void listOpenFunction(tList lFicheros);

void helpFunction(char *tComando[]);


int main() {
    tList lComandos, lFicheros;
    char comando[MAX];
    bool terminado = false;
    createEmptyList(&lComandos);
    createEmptyList(&lFicheros);
    crearListaFicheros(&lFicheros);
    while (!terminado) {
        leerEntrada(comando);
        procesarEntrada(comando, &terminado, &lComandos, &lFicheros);
    }
    return 0;
}

void imprimirPromt() {
    printf(">>");
}

void leerEntrada(char comandoEntrada[MAX]) {
    imprimirPromt();
    fgets(comandoEntrada, MAX, stdin);
}

void crearListaFicheros(tList *l){
    insertItemFichero("stdin", fcntl(0, F_SETFD, 0), fileno(stdin), l);
    insertItemFichero("stdout", fcntl(1, F_SETFD, 0), fileno(stdout), l);
    insertItemFichero("stderr", fcntl(2, F_SETFD, 0), fileno(stderr), l);
}

int TrocearCadena(char *cadena, char *trozos[]) {
    int i = 1;
    if ((trozos[0] = strtok(cadena, " \n\t")) == NULL)
        return 0;
    while ((trozos[i] = strtok(NULL, " \n\t")) != NULL) {
        i++;
    }
    return i;
}

void procesarEntrada(char *comando, bool *terminado, tList *lComandos, tList *lFicheros) {
    char *tComando[10], iCo[MAX];
    int i;
    i=TrocearCadena(comando, tComando);
    if(i==0) printf("Error al trocear cadena\n");
    else{
        for (int j = 0; j < i; j++) {
            if (j == 0) strcpy(iCo, tComando[j]);
            else{
                strcat(iCo, " ");
                strcat(iCo, tComando[j]);
            }
        }
        if (!insertItemComando(iCo, lComandos)) {
            perror("No hay mas memoria:");
        }
        tipoComando(tComando, terminado, lComandos, lFicheros);
    }
}

void tipoComando(char *tComando[], bool *terminado, tList *lComandos, tList *lFicheros) {
    if (strcmp(tComando[0], "authors") == 0) {
        authorsFunction(tComando);
    } else if (strcmp(tComando[0], "pid") == 0) {
        pidFunction(tComando);
    } else if (strcmp(tComando[0], "chdir") == 0) {
        chdirFunction(tComando);
    } else if (strcmp(tComando[0], "date") == 0) {
        dateFunction();
    } else if (strcmp(tComando[0], "time") == 0) {
        timeFunction();
    } else if (strcmp(tComando[0], "hist") == 0) {
        histFunction(tComando, lComandos);
    } else if (strcmp(tComando[0], "command") == 0) {
        comandFunction(tComando, lComandos, lFicheros);
    } else if (strcmp(tComando[0], "open") == 0) {
        openFunction(tComando, lFicheros);
    } else if (strcmp(tComando[0], "close") == 0) {
        closeFunction(tComando, lFicheros);
    } else if (strcmp(tComando[0], "dup") == 0) {
        dupFunction(tComando, lFicheros);
    } else if (strcmp(tComando[0], "infosys") == 0) {
        infosysFunction();
    } else if (strcmp(tComando[0], "listopen") == 0) {
        listOpenFunction(*lFicheros);
    } else if (strcmp(tComando[0], "help") == 0) {
        helpFunction(tComando);
    } else if (strcmp(tComando[0], "bye") == 0 || strcmp(tComando[0], "quit") == 0 ||
               strcmp(tComando[0], "exit") == 0) {
        deleteList(lComandos);
        deleteList(lFicheros);
        *terminado = true;
    } else if(strcmp(tComando[0], "create")==0){
        createFunction(tComando);
    }else if(strcmp(tComando[0], "stat")==0) {
        statFunction(tComando);
    }else if(strcmp(tComando[0], "list")==0) {
        listFunction(tComando);
    }else if(strcmp(tComando[0], "delete")==0) {
        deleteFunction(tComando);
    }else if(strcmp(tComando[0], "deltree")==0){
        deltreeFunction(tComando);
    }else perror("No ejecutado: ");
}

void authorsFunction(char *tComando[]) {
    if (tComando[1] != NULL) {
        if (strcmp(tComando[1], "-n") == 0) {
            printf("Sergio Fonticoba Sueiras\n");
            printf("José Valdivieso Díaz\n");
        } else if (strcmp(tComando[1], "-l") == 0) {
            printf("sergio.fonticoba@udc.es\n");
            printf("jose.valdiviesod@udc.es\n");
        }
    } else {
        printf("Sergio Fonticoba Sueiras: sergio.fonticoba@udc.es\n");
        printf("José Valdivieso Díaz: jose.valdiviesod@udc.es\n");
    }
}

void pidFunction(char *tComando[]) {
    if(tComando[1]==NULL)printf("Pid del shell: %d\n", getpid());
    else if(strcmp(tComando[1], "-p") == 0) printf("Pid del padre del shell: %d\n", getppid());
}

void chdirFunction(char *tComando[]) {
    int MAX_BUFF = 250;
    char dirAct[MAX_BUFF];
    if (tComando[1] != NULL) {
        if (chdir(tComando[1]) == -1) {
            perror("Imposible cambiar directorio");
        }
    } else printf("%s\n", getcwd(dirAct, MAX_BUFF));
}

void dateFunction() {
    time_t t = time(NULL);
    struct tm tiempo = *localtime(&t);
    printf("%02d/%02d/%d\n", tiempo.tm_mday, tiempo.tm_mon + 1, 1900 + tiempo.tm_year);
}

void timeFunction() {
    time_t t = time(NULL);
    struct tm tiempo = *localtime(&t);
    printf("%02d:%02d:%02d\n", tiempo.tm_hour, tiempo.tm_min, tiempo.tm_sec);
}

void histFunction(char *tComando[], tList *lComando) {
    int n;
    if(tComando[1]!=NULL){
        if(strcmp(tComando[1], "-c")==0){
            deleteList(lComando);
        }else if((n=atoi(tComando[1]))!=0){
            verComandos(-n, *lComando);
        }
    }else{
        verComandos(-1, *lComando);
    }
}

void comandFunction(char *tComando[], tList *lComandos, tList *lFicheros) {
    bool terminado=false;
    char nComando[100], *trozos[10];
    int i = atoi(tComando[1]);
    strcpy(nComando,getItem(i, *lComandos));
    if(strcmp(nComando, "\0")==0){
        perror("Demasiada recursion en hist: ");
    }else{
        TrocearCadena(nComando, trozos);
        tipoComando(trozos, &terminado, lComandos, lFicheros);
    }
}

void openFunction(char *tComando[], tList *lFicheros){
    int i,df, mode=0;

    if (tComando[1]==NULL) { /*no hay parametro*/
        verFicheros(*lFicheros);
        return;
    }
    for (i=2; tComando[i]!=NULL; i++)
        if (!strcmp(tComando[i],"cr")) mode|=O_CREAT;
        else if (!strcmp(tComando[i],"ex")) mode|=O_EXCL;
        else if (!strcmp(tComando[i],"ro")) mode|=O_RDONLY;
        else if (!strcmp(tComando[i],"wo")) mode|=O_WRONLY;
        else if (!strcmp(tComando[i],"rw")) mode|=O_RDWR;
        else if (!strcmp(tComando[i],"ap")) mode|=O_APPEND;
        else if (!strcmp(tComando[i],"tr")) mode|=O_TRUNC;
        else break;

    if ((df=open(tComando[1],mode,0777))==-1)
        perror ("Imposible abrir fichero");
    else{
        if(!insertItemFichero(tComando[1], mode,df, lFicheros)){
            perror("no hay mas memoria:");
        }
        printf ("Anadida a la %d entrada tabla ficheros abiertos\n", df);
    }
}

void closeFunction(char *tComando[], tList *lFicheros) {
    int df;
    if (tComando[1]==NULL || (df=atoi(tComando[1]))<=0) {
        verFicheros(*lFicheros);
        return;
    }
    if (!ExisteFichero(df, *lFicheros)) perror("El fichero no existe o no está abierto:");
    else {
        if (close(df) == -1)
            perror("Imposible cerrar descriptor");
        else {
            deleteFichero(df, lFicheros);
        }
    }
}

void dupFunction(char *tComando[], tList *lFicheros) {
    int df, duplicado;
    char aux[MAX],*p;
    if (tComando[1]==NULL || (df=atoi(tComando[1]))<=0) {
        verFicheros(*lFicheros);
        return;
    }
    if (!ExisteFichero(df, *lFicheros)) perror("El fichero no existe o no está abierto:");
    else {
        duplicado= dup(df);
        p= getFichero(df, *lFicheros);
        sprintf (aux,"dup %d (%s)",df, p);
        if(!insertItemFichero(aux, -10, duplicado, lFicheros))
            perror("no hay memoria:");
    }
}

void infosysFunction() {
    struct utsname u;
    if (uname(&u) == 0) {
        printf("%s (%s), OS: %s-%s-%s\n", u.nodename, u.machine, u.sysname, u.release, u.version);
    }
}

void listOpenFunction(tList lFicheros) {
    verFicheros(lFicheros);
}

void helpFunction(char *tComando[]) {
    int comando;
    char hComando[][MAX]={"authors", "pid", "hist", "chdir", "date"
            ,"time", "command ", "help", "open", "close", "dup"
            ,"infosys", "listopen", "bye", "exit", "quit",
            "create", "stat", "list", "delete", "deltree"};
    if(tComando[1]!=NULL){
        for(int i=0; i<21 ;i++){
            if(strcmp(tComando[1],hComando[i])==0) comando=i;
        }
        switch (comando) {
            case -1:
                printf("%s no encontrado\n", tComando[1]);
                break;
            case 0:
                printf("%s [-n][-l] Muestra los nombres y/o logins de los autores\n", hComando[comando]);
                break;
            case 1:
                printf("%s  [-p] Muestra el pid del shell o de su proceso padre\n", hComando[comando]);
                break;
            case 2:
                printf("%s [-c][-N] Muestra (o borra)el historico de comandos\n"
                       "                                    \t-N: muestra los N primeros\n"
                       "                                    \t-c: borra el historico\n", hComando[comando]);
                break;
            case 3:
                printf("%s [dir] Cambia (o muestra) el directorio actual del shell\n", hComando[comando]);
                break;
            case 4:
                printf("%s Muestra la fecha acual\n", hComando[comando]);
                break;
            case 5:
                printf("%s Muestra la hora actual\n", hComando[comando]);
                break;
            case 6:
                printf("%s [-N] Repite el comando N (del historico)\n", hComando[comando]);
                break;
            case 7:
                printf("%s [cmd|-lt|-T] Muestra ayuda sobre los comandos\n"
                       "\t-lt: lista topics de ayuda\n"
                       "\t-T topic: lista comandos sobre ese topic\n"
                       "\tcmd: info sobre el comando cmd\n", hComando[comando]);
                break;
            case 8:
                printf("%s fich m1 m2...\tAbre el fichero fich. y lo anade a la lista de ficheros abiertos del shell\n"
                       "\t m1, m2..es el modo de apertura (or bit a bit de los siguientes).\n"
                       "\tcr: O_CREAT\\tap: O_APPEND\n"
                       "\tex: O_EXCL \tro: O_RDONLY\n"
                       "\trw: O_RDWR \two: O_WRONLY\n"
                       "\ttr: O_TRUNC\n", hComando[comando]);
                break;
            case 9:
                printf("%s df Cierra el descriptor df y elimina el correspondiente fichero de la lista de ficheros abiertos\n", hComando[comando]);
                break;
            case 10:
                printf("%s df Duplica el descriptor de fichero df y anade una nueva entrada a la lista ficheros abiertos\n", hComando[comando]);
                break;
            case 11:
                printf("%s Muestra informacion de la maquina donde corre el shell\n", hComando[comando]);
                break;
            case 12:
                printf("%s [n] Lista los ficheros abiertos (al menos n) del shell\n", hComando[comando]);
                break;
            case 13:
            case 14:
            case 15:
                printf("%s Termina la ejecucion del shell\n", hComando[comando]);
                break;
            case 16:
                printf("%s [-f] [name]\tCrea un directorio o un fichero (-f)\n", hComando[comando]);
                break;
            case 17:
                printf("%s [-long][-link][-acc] name1 name2 ..\tlista ficheros;\n"
                       "\t-long: listado largo\n"
                       "\t-acc: acesstime\n"
                       "\t-link: si es enlace simbolico, el path contenido\n", hComando[comando]);
                break;
            case 18:
                printf("%s [-reca] [-recb] [-hid][-long][-link][-acc] n1 n2 ..\tlista contenidos de directorios\n"
                       "\t-hid: incluye los ficheros ocultos\n"
                       "\t-recb: recursivo (antes)\n"
                       "\t-reca: recursivo (despues)\n"
                       "\tresto parametros como stat\n", hComando[comando]);
                break;
            case 19:
                printf("%s [name1 name2 ..]\tBorra ficheros o directorios vacios\n", hComando[comando]);
                break;
            case 20:
                printf("%s [name1 name2 ..]\tBorra ficheros o directorios no vacios recursivamente\n", hComando[comando]);
                break;
            default:
                break;
        }

    }else{
        printf("'help [cmd|-lt|-T topic]' ayuda sobre comandos\n"
               "\t\tComandos disponibles:\n"
               "exit bye fin quit salir fecha date time pid getpid "
               "authors autores hist command comando chdir carpeta "
               "infosys infosis pwd open abrir close cerrar listopen "
               "listarabiertos dup help ayuda create crear delete borrar "
               "deltree borrarrec stat listfich list listdir recurse "
               "recursiva comando-o e-s comando/o e/s read write memdump volcarmem "
               "memfill fillmem llenarmem deallocate dealloc desasignar"
               " allocate alloc asignar malloc mmap shared memcreateshared"
               " memshared memdeshared dememshared memmalloc memdemalloc "
               "dememmalloc memmmap memdemmap dememmmap free delkey mem "
               "memoria memory priority redin redentrada redout redsalida "
               "rederr rederror initredir habilitarredireccion clearredir deshabilitarredireccion "
               "getuid uid showenv entorno environ showvar mostrarvar changevar cambiarvar subsvar "
               "susvar substitutevar sustituirvar changevarWrong cambiarvarMal fork execute ejec exec "
               "executepri ejecpri executeas ejecas back backpri fgas backas listjobs jobs deljobs borrarjobs "
               "deletejobs job pipe backpipe sigdefault sigignore sigmask sigunmask sighandler sigaction signal "
               "senal sigprocmask limitresend siginfo sigcount sigaltstack bucle segmentation\n");
    }
}
