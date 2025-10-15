/* 5253404 */ // sustituiir con los 7 dÃ­gitos de la cÃ©dula

#include "../include/version.h"
#include "../include/linea.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_VERSIONES 10




struct _rep_version {
    char* nombreVersion;
    Linea linea;
    _rep_version *sig;
    _rep_version *ant;
};

//***********************  CONSTRUCTORAS ***************** */

//Pos-Cond: Retorna una version vacia
Version crearVersionVacia(){
    return NULL;  // Lista vacía de versiones
}

//Pre-Cond: num_version tiene que estar en el rango de 1 o la ultima version + 1 de	la Version "version" 
//Pos-Cond: Crea una nueva version con el numero de verion "num_version
//			Las versiones iguales y mayores a num_version se les suma 1 al numero de version.
void crearVersion (Version &version, char *num_version){
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