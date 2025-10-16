/* 5253404 */ // sustituiir con los 7 dÃ­gitos de la cÃ©dula

#include "../include/version.h"
#include "../include/linea.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_VERSIONES 10




struct nodoAV {
    int* numeroVersion;
    int tope;
    Linea linea;
    nodoAV * pH;        //primer hijo
    nodoAV * sH;        //siguiente hermano    
};


struct _rep_version {
    AV versionRaiz;
    _rep_version *sig;
};

//***********************  CONSTRUCTORAS ***************** */

//Pos-Cond: Retorna una version vacia
Version crearVersionVacia(){
    return NULL;  // Lista vacía de versiones
}

//pre-cond: no tiene
//pos-cond: convierte un string con caracteres "numeros" en un arreglo de enteros y lo guarda en "numero", eliminando los puntos 
void parsear(char *string, int *&numero, int &tope){
    //contamos la cantidad de puntos
    int largo = strlen(string);
    int puntos=0;
    for(int i=0; i<largo; i++){
        if(string[i]=='.')
            puntos++;
    }

    //calculo del tope
    tope = puntos;

    //arreglo dinamico para almacenar los numeros
    numero = new int[tope+1];

    int nivel=0;        //indice dentro del arreglo, representa el nivel dentro del arbol
    int valor=0;

    for(int i=0; i <= largo; i++){
        if (string[i] == '.' || string[i]=='\0'){
            numero[nivel] = valor;
            nivel++;
            valor = 0;
        }
        else {
            valor = valor*10 + (string[i] - '0');
        }
    }
}


//pre-cond: no tiene
//pos-cond: copia el arreglo de enteros origen en el arreglo de enteros destino
void copiarArrInt(int *origen, int *destino, int tope){
    for(int i=0; i<=tope; i++)
        destino[i] = origen[i];
}

//pre-cond: la version con numVer existe
//pos-cond: reenumera los hijos de la version numVer como numVer + 1
void renumHijosAsc(AV t, int *numVer, int tope){
    if(t != NULL){
        copiarArrInt(numVer, t->numeroVersion, tope);   //copiamos el numero de version numVer en t->numVer
        t->tope = tope;

        if (t->pH != NULL)
            renumHijosAsc(t->pH, t->numeroVersion, t->tope + 1);

        if (t->sH != NULL)
            renumHijosAsc(t->sH, numVer, tope);
    }
}

//Pre-Cond: num_version tiene que estar en el rango de 1 o la ultima version + 1 de	la Version "version" 
//Pos-Cond: Crea una nueva version con el numero de verion "num_version
//			Las versiones iguales y mayores a num_version se les suma 1 al numero de version.
void crearVersion (Version &version, char *num_version){
    AV nodoNuevo = new nodoAV;
    nodoNuevo->linea = crearLineaVacia();
    nodoNuevo->tope = -1;

    int *arrInt;
    int lim;        //sera el tope del arreglo
    parsear(num_version, arrInt, lim);          //paso el string num_version y lo devuelvo como arreglo de enteros en arrInt con tope "lim"

    unsigned int nivel = lim + 1;       //marca la cantidad de niveles del arbol de versiones

    if (nivel == 1){    //si nivel = 1, quiere decir que vamos a insertar una version padre que sera la raiz del arbol finitario de versiones
        Version nueva = new _rep_version;
        nueva->sig = NULL;
        nueva->versionRaiz = nodoNuevo;
        nodoNuevo->pH = NULL;
        nodoNuevo->sH = NULL;
        nodoNuevo->numeroVersion = arrInt;
        nodoNuevo->tope = lim;
    }
    else {

    }



    


    
}


/*    raiz->nombreVersion = new char[strlen(num_version) + 1];
    strcpy(raiz->nombreVersion, num_version);


    nueva->versionRaiz = raiz;
*/

//Pre-Cond: num_version tiene que estar en el rango de 1 o la ultima version + 1 de	la Version "version" 
//Pos-Cond: Crea una nueva version con el numero de verion "num_version
//			Las versiones iguales y mayores a num_version se les suma 1 al numero de version.
/*void crearVersion (Version &version, char *num_version){
    int pos = atoi(num_version);
    
    Version nueva = new _rep_version;
    int len = strlen(num_version) + 1;
    nueva->nombreVersion = new char[len];
    strcpy(nueva->nombreVersion, num_version);
    nueva->linea = crearLineaVacia();
    
    // Insertar en la posición correcta
    Version aux = version;
    int contador = 1;

    if (version == NULL) {
        nueva->ant = NULL;
        nueva->sig = NULL;
        version = nueva;
        aux = nueva;  
    } else {
        while (aux != NULL && contador < pos) {
            aux = aux->sig;
            contador++;
        }

        if (aux == NULL) {
            // Insertar al final
            Version ultima = version;
            while (ultima->sig != NULL)
                ultima = ultima->sig;

            nueva->ant = ultima;
            nueva->sig = NULL;
            ultima->sig = nueva;

            aux = nueva;  // empezamos renumeración desde la nueva versión
        } else {
            // Insertar antes de aux, puede ser en el medio o al principio
            nueva->sig = aux;
            nueva->ant = aux->ant;

            if (aux->ant != NULL)
                aux->ant->sig = nueva;
            else
                version = nueva;  // insertando al principio

            aux->ant = nueva;

            aux = nueva;  // empezamos renumeración desde la nueva versión
        }
    }
    
    // Reenumerar desde la nueva versión hasta el final
    int numero = pos;
    while (aux != NULL) {
        delete [] aux->nombreVersion;
        aux->nombreVersion = new char[MAX_VERSIONES];
        aux->nombreVersion[0] = numero + '0';  
        aux->nombreVersion[1] = '\0';
        aux = aux->sig;
        numero++;
    }
}
    */


//************************ SELECTORAS ********************* */

//Pre-Cond: la version numVersion existe en version
//Pos-Cond: Retorna un puntero a la version de nombre "numVersion"
Version obtenerVersion(Version &version, char *numVersion){
    Version aux = version;
    while (strcmp(aux->nombreVersion, numVersion) != 0)
        aux = aux->sig;
    return aux;
}

//Pre-cond: La version "version" tiene por lo menos "numLinea" de Lineas
//Pos-Cond: Agrega el string texto como la fila num_fila de la Version "version"
//          Las filas debajo de num_lineas se renumeran como numLinea=numLinea+1
void agregarFilaVersion (Version &version, char* numeroVersion, char *textoFila,unsigned int numLinea){
    Version aux = obtenerVersion(version, numeroVersion);   //obtenemos la version donde vamos a insertar el string texto
    insertarLinea(aux->linea, textoFila, numLinea);     //insertamos la linea en la version y reenumeramos las siguientes por debajo (si hay)
}

//Pre-cond: no tiene
//Pos-cond: devuelve un puneto al numero de version, si no existe, devuelve NULL
AV buscar(AV t, char *nombreVersion){
    if (t==NULL)
        return NULL;
    else {
        if (strcmp(t->nombreVersion, nombreVersion)!=0){
            AV esta_sH = buscar(t->sH, nombreVersion);
            if (esta_sH != NULL)
                return esta_sH;
            else
                return buscar(t->pH, nombreVersion);
        }
        else
            return t;
    }
}

//Pre-Cond: existeVersion(version, numeroVersion) retorna true.
//Pos-Cond: Imprime la Version "numeroVersion" junto con sus lineas
void imprimirVersion(Version version, char* numeroVersion){
    printf("%s \n", version->nombreVersion);
    printf("\n");
    int cant = numeroUltimaLineaVersion(version);     //cantidad de lineas en la version
    if (cant == 0)
        printf("No contiene lineas.\n");
    else {
        for (int i=1; i<=cant; i++){
            char* renglon = obtenerTextoLinea(version->linea, i);
            printf("%d    %s\n", i, renglon);
            delete [] renglon;
        }
    }  
}

//Pre-Cond: version != NULL
//Pos-Cond: retorna un puntero a la siguiente Version de "version"
Version siguienteVersion(Version version){
    return version->sig;
}

//Pre-Cond: version !=NULL
//Pos-Cond: retorna un puntero a un arreglo dinamico con el numero de la Version "version"
char* nombreVersion(Version version){
    int len = strlen(version->nombreVersion);
    char *resultado = new char[len + 1];
    strcpy(resultado, version->nombreVersion);
    return resultado;
}

//Pre-Cond: (!esVaciaVersion(version)) retorna true
//Pos-Cond: retorna un entero con el numero de la ultima verison que hay en la Version "version"
int numeroUltimaVersion(Version version){
    while(version->sig != NULL)    
        version = siguienteVersion(version);
    return atoi(nombreVersion(version));
}


//Pre-Cond: No tiene
//Pos-Cond: retorna un entero con el numero de la ultima linea de la Verison de "version"
int numeroUltimaLineaVersion(Version version){
    if (version != NULL)
        return cantidadLineas(version->linea);    
    else
        return 0;
}

//********************* PREDICADOS ************************* */
//pre-cond:No tiene
//pos-cond:Retorna true si la Version "version" es vacia.
bool esVaciaVersion (Version version){
    return version == NULL;
}

//Retorna true si la Version "numeroVersion" existe en "version"
bool existeVersion (Version version, char* numeroVersion){
    Version aux = version;
    while (aux != NULL){
        if (strcmp(aux->nombreVersion, numeroVersion) == 0)
            return true;
        aux = aux->sig;
    }
    return false;
}

//****************  DESTRUCTORAS ***********************

//Pre-Cond: la version "numeroVersion" existe y la Linea "numLinea" existe en la version "version"
//Pos-Cond: se elimina la Linea de la posicion "numLinea"
//          el resto de las Lineas debajo se renumeran como numLinea=numLinea-1
void eliminarLineaVersion (Version &version, char* numeroVersion, unsigned int numLinea){
    Version aux = obtenerVersion(version, numeroVersion);
    eliminarLinea(aux->linea, numLinea);
}

//Pre-Cond: la version "numeroVersion" existe en version
//Pos-Cond: elimina toda la mermoria reservada por "numeroVersion". Reenumera las siguientes versiones
void destruirVersion (Version &version, char* numeroVersion){
    Version temp = obtenerVersion(version, numeroVersion);
    Version aux = version;
    int num = atoi(numeroVersion) - 1;
    int i = 1;

    while (aux != NULL && i < num){
        aux = aux->sig;
        i++;
    }

    //si queremos eliminar la primera version
    if (temp == version){
        version = temp->sig;
        if (version != NULL)
            version->ant = NULL;
        aux = aux->sig;
    }
    else {
        aux->sig = temp->sig;
        if (temp->sig != NULL)          //si queremos eliminar la ultima version, no entramos
            temp->sig->ant = aux;
        aux = aux->sig;
    }

    destruirLinea(temp->linea);
    delete temp;

    while (aux != NULL){
        int x = aux->nombreVersion[0] - '0' - 1;
        
        delete [] aux->nombreVersion;
        aux->nombreVersion = new char[MAX_VERSIONES];
        aux->nombreVersion[0] = x + '0';
        aux->nombreVersion[1] = '\0';
        
        aux = aux->sig;
    }
}

////////////////////////////////// AGREGADA 05/09/2025  ////////////
//Pre-Cond: No tiene
//Pos-Cond: Elimina toda la memoria reservada por "version"
void destruirTodasLasVersiones(Version &version){    
    while (!esVaciaVersion(version))
        destruirVersion(version, version->nombreVersion);
}