
#include "funcionesP1.h"

//funciones auxiliares de todo el resto de funciones
char LetraTF(mode_t m) {
    switch (m & S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK:
            return 's'; /*socket */
        case S_IFLNK:
            return 'l'; /*symbolic link*/
        case S_IFREG:
            return '-'; /* fichero normal*/
        case S_IFBLK:
            return 'b'; /*block device*/
        case S_IFDIR:
            return 'd'; /*directorio */
        case S_IFCHR:
            return 'c'; /*char device*/
        case S_IFIFO:
            return 'p'; /*pipe*/
        default:
            return '?'; /*desconocido, no deberia aparecer*/
    }
}

char *ConvierteModo(mode_t m, char *permisos) {
    strcpy(permisos, "---------- ");

    permisos[0] = LetraTF(m);
    if (m & S_IRUSR) permisos[1] = 'r';    /*propietario*/
    if (m & S_IWUSR) permisos[2] = 'w';
    if (m & S_IXUSR) permisos[3] = 'x';
    if (m & S_IRGRP) permisos[4] = 'r';    /*grupo*/
    if (m & S_IWGRP) permisos[5] = 'w';
    if (m & S_IXGRP) permisos[6] = 'x';
    if (m & S_IROTH) permisos[7] = 'r';    /*resto*/
    if (m & S_IWOTH) permisos[8] = 'w';
    if (m & S_IXOTH) permisos[9] = 'x';
    if (m & S_ISUID) permisos[3] = 's';    /*setuid, setgid y stickybit*/
    if (m & S_ISGID) permisos[6] = 's';
    if (m & S_ISVTX) permisos[9] = 't';

    return permisos;
}

bool information(char *path, struct stat *info) {
    if (lstat(path, info) == -1) {
        perror("No se puede acceder al archivo: ");
        return false;
    }
    return true;
}

bool PathCorrecto(char *path, char *finalPath) {
    //se encarga de recibir el nombre de un archivo y devolver su path
    struct stat info;
    char directorioPath[250];
    if (information(path, &info) && LetraTF(info.st_mode) == 'l') {
        getcwd(directorioPath, sizeof(directorioPath));
        snprintf(finalPath, MAXBUFF, "%s/%s", directorioPath, path);
        return true;
    } else if (realpath(path, finalPath) != NULL) return true;
    else {
        //en caso de no llevarse a cabo correctamente la funcion imprime un error
        //y devuelve false
        perror("realpath: ");
        return false;
    }
}

//funciones auxiliares función stat y la función principal
void mostrarInfo(struct stat info, char *nombre) {
    //imprimer el tamaño y el nombre de un archivo
    printf("%ld %s\n", info.st_size, nombre);
}

void mostrarInfoLongLink(struct stat info, char *nombre, int ACC, int link, char *directorioPath) {
    char fecha[17], permisos[10], pathSimbolico[MAXBUFF]={0}, path[MAXBUFF];
    struct tm *timeF;
    struct passwd *prop;
    if (ACC != 1) {
        time_t time = info.st_mtim.tv_sec;
        if((timeF = localtime(&time))==NULL) perror("localtime: ");
    } else {
        time_t time = info.st_atim.tv_sec;
        if((timeF = localtime(&time))==NULL) perror("localtime: ");
    }
    strftime(fecha, sizeof(fecha), "%Y/%m/%d %H:%M", timeF);
    printf("%s ", fecha);
    ConvierteModo(info.st_mode, permisos);
    //se consigue el nombre del propietario del archivo a partir del id que aparece en el archivo
    if ((prop = getpwuid(info.st_uid)) == NULL) {
        perror("getpwuid: ");
    } else {
        printf("%lu(%lu) %s %s %ld %s", info.st_nlink, info.st_ino, prop->pw_name, permisos, info.st_size,
               nombre);
        if (LetraTF(info.st_mode) == 'l' && link == 1) {
            snprintf(path, MAXBUFF, "%s/%s", directorioPath, nombre);
            if ((readlink(path, pathSimbolico, MAXBUFF - 1)) != -1) {
                printf("-> %s", pathSimbolico);
            } else perror("readlink: ");
        }
        printf("\n");
    }
}

int decimal(int const f[], int n) {
    int b = 0;
    //se convierte un array de enteros a un simple entero
    for (int i = 0; i < n; i++) {
        b = b * 10 + f[i];
    }
    return b;
}

void statFunction(char *comando[]) {
    //se usa un array de enteros a modo de bits en los que se ponen a 1 en caso de tener esa
    //opcion de impresion el comando y a 0 en contrario
    //posicion 0: long posición 1: acc y posicion 2: link
    int f[3] = {0, 0, 0}, i;
    char finalPath[MAXBUFF], dirAct[MAXBUFF], directorioPath[MAXBUFF];
    struct stat info;
    //se ponen a 1 los valores del array en funcion de si aparece en el comando esa opcion
    for (i = 1; i < 4; i++) {
        if (comando[i] == NULL) break;
        else if (strcmp(comando[i], "-long") == 0) f[0] = 1;
        else if (strcmp(comando[i], "-acc") == 0) f[1] = 1;
        else if (strcmp(comando[i], "-link") == 0) f[2] = 1;
        else break;
    }
    if (comando[i] == NULL) {
        if(getcwd(dirAct, MAXBUFF)!=NULL){
            printf("%s\n", dirAct);
        }else perror("getcwd: ");
    }
    else {
        //dependediendo de los bits que esten activos se imprime las estadisticas de una manera o de otra
        switch (f[0]) {
            case 0:
                while (comando[i] != NULL) {
                    if (PathCorrecto(comando[i], finalPath)) {
                        if (information(finalPath, &info)) mostrarInfo(info, comando[i]);
                    }
                    i++;
                }
                break;
            case 1:
                while (comando[i] != NULL) {
                    if (PathCorrecto(comando[i], finalPath)) {
                        if(getcwd(directorioPath, MAXBUFF-1)){
                            if (information(finalPath, &info)) mostrarInfoLongLink(info, comando[i], f[1], f[2], directorioPath);
                        }
                    }
                    i++;
                }
                break;
            default:
                break;
        }
    }
}

//funciones auxiliares a la función list y la función principal
void mostrarInfoList(int n, char *nombre, struct stat info, char *directorioPath) {
    //esta funcion se encarga de lo mismo que el switch de la función stat
    //pero enfocado para la función list
    switch (n) {
        case 000:
        case 001:
        case 010:
            mostrarInfo(info, nombre);
            break;
        case 100:
            mostrarInfoLongLink(info, nombre, 0, 0, directorioPath);
            break;
        case 101:
            mostrarInfoLongLink(info, nombre, 0, 1, directorioPath);
            break;
        case 110:
            mostrarInfoLongLink(info, nombre, 1, 0, directorioPath);
            break;
        case 111:
            mostrarInfoLongLink(info, nombre, 1, 1, directorioPath);
            break;
        default:
            break;
    }
}

void recorrerReca(char *nombre, int h, int n) {
    char directorioPath[500], ficheroPath[MAXBUFF], finalFicheroPath[MAXBUFF];
    DIR *directorio;
    struct stat info;
    struct dirent *ficheros;
    //se consigye el path del directorio y se abre el directorio comenzando a leer todos los archivos que se encuentan
    //dentro de este hasta encontrarse con un NULL
    if (PathCorrecto(nombre, directorioPath)) {
        if ((directorio = opendir(directorioPath)) != NULL) {
            printf("*************%s\n", nombre);
            while ((ficheros = readdir(directorio)) != NULL) {
                //se compara el valor de h que es el bit que hace referencia a la opcion -hid para saber si imprimir los
                //ficheros ocultos o no
                if (h == 1 || ((strcmp(ficheros->d_name, ".") != 0 && strcmp(ficheros->d_name, "..") != 0))) {
                    //se concatena el nombre del directorio con el del fichero a modo de path para evitar errores
                    snprintf(ficheroPath, sizeof(ficheroPath), "%s/%s", nombre, ficheros->d_name);
                    if (PathCorrecto(ficheroPath, finalFicheroPath))
                        if (information(ficheroPath, &info)) {
                            //se envian a la funcion mostrar la informacion de un archivo de la lista n=bits{long, acc, link},
                            //nombre fichero e struct stat info, que contiene los datos del archivo
                            printf("\t");
                            mostrarInfoList(n, ficheros->d_name, info, directorioPath);
                        }
                }
            }
            //se cierra el directorio y se vuelve a abrir para comenzar ahora a leer el contenido de los subdirectorios
            closedir(directorio);
            if ((directorio = opendir(directorioPath)) != NULL) {
                while ((ficheros = readdir(directorio)) != NULL) {
                    //se eliminan de la comparacion los archivos ocultos ya que en realidad no son directorios
                    if (((strcmp(ficheros->d_name, ".") != 0 && strcmp(ficheros->d_name, "..") != 0))) {
                        snprintf(ficheroPath, sizeof(ficheroPath), "%s/%s", nombre, ficheros->d_name);
                        if (PathCorrecto(ficheroPath, finalFicheroPath))
                            if (information(ficheroPath, &info)) {
                                //se comprueba si el archivo es un directorio y en caso de que lo sea se recorre ese directorio con
                                //con la misma funcion recursiva
                                if (LetraTF(info.st_mode) == 'd') recorrerReca(ficheroPath, h, n);
                            }
                    }
                }
                closedir(directorio);
            } else perror("opendir: ");
        } else perror("opendir: ");
    }
}

void recorrerNorecur(char *nombre, int h, int n) {
    char directorioPath[500], ficheroPath[MAXBUFF];
    DIR *directorio;
    struct stat info;
    struct dirent *ficheros;
    //en esta función se realizan las mismas acciones que en la función recorrerReca pero en este caso sin la segunda parte
    //de listar el contenido de los subdirectores
    if (PathCorrecto(nombre, directorioPath)) {
        if ((directorio = opendir(directorioPath)) != NULL) {
            printf("*************%s\n", nombre);
            while ((ficheros = readdir(directorio)) != NULL) {
                if (h == 1 || ((strcmp(ficheros->d_name, ".") != 0 && strcmp(ficheros->d_name, "..") != 0))) {
                    snprintf(ficheroPath, sizeof(ficheroPath), "%s/%s", directorioPath, ficheros->d_name);
                    if (information(ficheroPath, &info)) {
                        printf("\t");
                        mostrarInfoList(n, ficheros->d_name, info, directorioPath);
                    }
                }
            }
            closedir(directorio);
        } else perror("opendir: ");
    }
}

void recorrerRecb(char *nombre, int h, int n) {
    char directorioPath[500], ficheroPath[MAXBUFF], finalFicheroPath[MAXBUFF];
    DIR *directorio;
    struct stat info;
    struct dirent *ficheros;
    //en esta función se realizan las mismas acciones que en la función recorrerReca solo que en esta ocasión
    //primero si lista el contenido de los subdirectorios y despues se procede a listar el directorio principal
    if (PathCorrecto(nombre, directorioPath)) {
        if ((directorio = opendir(directorioPath)) != NULL) {
            while ((ficheros = readdir(directorio)) != NULL) {
                if (((strcmp(ficheros->d_name, ".") != 0 && strcmp(ficheros->d_name, "..") != 0))) {
                    snprintf(ficheroPath, sizeof(ficheroPath), "%s/%s", nombre, ficheros->d_name);
                    if (PathCorrecto(ficheroPath, finalFicheroPath))
                        if (information(ficheroPath, &info)) {
                            if (LetraTF(info.st_mode) == 'd') recorrerRecb(ficheroPath, h, n);
                        }
                }
            }
            closedir(directorio);
            printf("*************%s\n", nombre);
            if ((directorio = opendir(directorioPath)) != NULL) {
                while ((ficheros = readdir(directorio)) != NULL) {
                    if (h == 1 || ((strcmp(ficheros->d_name, ".") != 0 && strcmp(ficheros->d_name, "..") != 0))) {
                        snprintf(ficheroPath, sizeof(ficheroPath), "%s/%s", nombre, ficheros->d_name);
                        if (PathCorrecto(ficheroPath, finalFicheroPath))
                            if (information(ficheroPath, &info)) {
                                printf("\t");
                                mostrarInfoList(n, ficheros->d_name, info, directorioPath);
                            }
                    }
                }
                closedir(directorio);
            } else perror("opendir: ");
        } else perror("opendir: ");
    }
}

void listFunction(char *comando[]) {
    //en el array f se guardan los mismos datos que en el array f del stat
    //en el array r se guarada 1 en la posicion 0 si se recorre el directorio de manera reca
    //y 1 en la posicion 1 si se recorre de manera recb
    //el bit h se pondrá a 1 si se listan los archivos ocultos también
    int f[3] = {0, 0, 0}, r[2] = {0, 0}, i, bit, h = 0;
    char dirAct[MAXBUFF];
    //en este caso ademas de las 3 opciones de la funcion stat se encuentran reca recb y hid
    //y como maximo puede haber 5 opciones(por eso el for es del 1 al 6)
    for (i = 1; i < 6; i++) {
        if (comando[i] == NULL) break;
        if (strcmp(comando[i], "-reca") == 0) r[0] = 1;
        else if (strcmp(comando[i], "-recb") == 0) r[1] = 1;
        else if (strcmp(comando[i], "-hid") == 0) h = 1;
        else if (strcmp(comando[i], "-long") == 0) f[0] = 1;
        else if (strcmp(comando[i], "-acc") == 0) f[1] = 1;
        else if (strcmp(comando[i], "-link") == 0) f[2] = 1;
        else break;
    }
    //se convierte el array f a un entero
    bit = decimal(f, 3);
    if (comando[i] == NULL) {
        if(getcwd(dirAct, MAXBUFF)!=NULL){
            printf("%s\n", dirAct);
        }else perror("getcwd: ");
    }else if (r[0] == 1) {
        //se mira la forma de recorrer el directorio y dependiendo de ello se llama a una función de las tres anteriores
        //pasando como parametro el nombre del directorio, el bit de hid (h) y el entero que marca la forma de mostrar las estadisticas
        while (comando[i] != NULL) {
            recorrerReca(comando[i], h, bit);
            i++;
        }
    } else if (r[1] == 1) {
        while (comando[i] != NULL) {
            recorrerRecb(comando[i], h, bit);
            i++;
        }
    } else {
        while (comando[i] != NULL) {
            recorrerNorecur(comando[i], h, bit);
            i++;
        }
    }
}

//funcion crear
void createFunction(char *comando[]) {
    char dirAct[MAXBUFF], path[MAXBUFF];
    FILE *fd;
    if (comando[1] == NULL) {
        if(getcwd(dirAct, MAXBUFF)!=NULL){
            printf("%s\n", dirAct);
        }else perror("getcwd: ");
    } else if (strcmp(comando[1], "-f") != 0) {
        if (mkdir(comando[1], 0777) == -1) perror("No es posible crear el directorio: ");
    } else {
      if (comando[2] == NULL) {
        if(getcwd(dirAct, MAXBUFF)!=NULL){
            printf("%s\n", dirAct);
        }else perror("getcwd: ");
      }else if(realpath(comando[2], path)==NULL){
        if ((fd = fopen(comando[2], "a")) == NULL) perror("No es posible crear el archivo: ");
        else fclose(fd);
      }else printf("ya existe un archivo con ese nombre\n");
    }
}

//funciones destructoras
void deleteFunction(char *comando[]) {
    char finalPath[MAXBUFF], dirAct[MAXBUFF];
    struct stat info;
    int i = 1;
    if (comando[1] == NULL) {
        if(getcwd(dirAct, MAXBUFF)!=NULL){
            printf("%s\n", dirAct);
        }else perror("getcwd: ");
    }
    //un bucle que va eliminando todos los archivos que se le pasan en caso de existir y tener permiso para ello
    while (comando[i] != NULL) {
        if (PathCorrecto(comando[i], finalPath)) {
            if (information(finalPath, &info)) {
                if (LetraTF(info.st_mode) == 'd') {
                    if (rmdir(comando[i]) != 0) perror("rmdir: ");
                } else if (LetraTF(info.st_mode) == '-' || LetraTF(info.st_mode) == 'l') {
                    if (unlink(comando[i]) != 0) perror("unlink: ");
                }
            }
        }
        i++;
    }
}
void deltreeAux(char *Pdirectorio){
    DIR* directorio;
    char ficheroPath[MAXBUFF];
    struct dirent *ficheros;
    struct stat info;
        if ((directorio = opendir(Pdirectorio)) != NULL) {
            while ((ficheros = readdir(directorio)) != NULL) {
                snprintf(ficheroPath, sizeof(ficheroPath), "%s/%s", Pdirectorio, ficheros->d_name);
                if(information(ficheroPath, &info)){
                    if(strcmp(ficheros->d_name, ".")==0 || strcmp(ficheros->d_name, "..")==0){
                        remove(ficheros->d_name);
                    }else if(LetraTF(info.st_mode)=='d'){
                        deltreeAux(ficheroPath);
                        rmdir(ficheroPath);
                    }else unlink(ficheroPath);
                }
            }
            closedir(directorio);
        }
}
void deltreeFunction (char *comando[]){
    int i=1;
    char Path[MAXBUFF], dirAct[250];
    if (comando[1] == NULL) {
        if(getcwd(dirAct, MAXBUFF)!=NULL){
            printf("%s\n", dirAct);
        }else perror("getcwd: ");
    }
    while (comando[i]!= NULL){
        if(PathCorrecto(comando[i], Path)) {
          deltreeAux(Path);
          rmdir(Path);
        }
      i++;
    }
    
}
