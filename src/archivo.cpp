/* 5253404 */ // sustituiir con los 7 dÃ­gitos de la cÃ©dula

#include "../include/archivo.h"
#include "../include/version.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>





struct _rep_archivo {
    char* nombre;
    Version version;
};


//***********************  CONSTRUCTORAS ***************** */

//Pos-Cond: Retorna un archivo vacio
Archivo crearArchivoVacio(char *nomArchivo){
    Archivo nuevo = new _rep_archivo;
    int len = strlen(nomArchivo);
    nuevo->nombre = new char[len + 1];
    strcpy(nuevo->nombre, nomArchivo);
    nuevo->version = crearVersionVacia();
    return nuevo;
}

//Pre-Cond: No existe otra version en la estructura con nombre "numVersion"
//Pos-Cond: Crea una nueva version del archivo "archivo"
void crearVersionArchivo (Archivo &archivo, char *numVersion){
    crearVersion(archivo->version, numVersion);
}

//Pre-Cond: No existe otra version en la estructura con nombre "numVersion"
//Pos-Cond: Crea una nueva version de
void versionIndependienteArchivo(Archivo &archivo, char *numVersion);


//************************ SELECTORAS ********************* */
//Pre-Cond: existe el archivo
//Pos-Cond: Retorna un puntero a un arreglo dinamico con el nombre del Archivo "archivo"
char* obtenerNombreArchivo(Archivo archivo){
    int len = strlen(archivo->nombre);
    char* resultado = new char[len + 1];
    strcpy(resultado, archivo->nombre);
    return resultado;
}

//Pre-Cond: hay versiones para mostrar
//Pos-Cond: imprime todas las versiones de un archivo
void mostrarVersionesArchivo(Archivo archivo){
    Version version = archivo->version;
    imprimirTodasLasversiones(version);
}

//Pre-cond: La version "version" tiene por lo menos "num_fila" de Filas
//Pos-Cond: Agrega el string "textoFila" como la fila "num_fila" de la Version "numeroVersion"
//          Las filas debajo de num_filas se renumeran como num_fila=num_fila+1
void insertarLineaVersionDeArchivo (Archivo &archivo, char* numeroVersion, char *textoFila,unsigned int numFila){
    agregarFilaVersion(archivo->version, numeroVersion, textoFila, numFila);
}


//Pre-Cond: la version numeroVersion existe en Version 
//Pos-Cond: Imprime la Version "numeroVersion"
/*void imprimirVersionArchivo (Archivo archivo, char* numeroVersion){
    Version version = archivo->version;

    int num = atoi(numeroVersion);
    for (int i = 1; i < num; i++) 
        version = siguienteVersion(version);
    
    char* nombre = nombreVersion(version);
    printf("%s\n", nombre);
}
    */

//Pre-Cond: Tiene version padre.
//Pos-Cond: Imprime los cambios realizados en la version numeroVersion con respecto al padre.
void mostrarCambiosArchivo (Archivo archivo, char* numeroVersion);


//Pre-Cond: La Version "numeroVersion" existe en el Archivo "archivo".
//Pos-Cond: Imprime el texto correspondiente a la version "numeroVersion"
void mostrarTextoArchivoVersion (Archivo archivo, char* numeroVersion){
    AV version = obtenerVersion(archivo->version, numeroVersion);      //obtenemos la version de la cual queremos imprimir el texto
    printf("%s - ", archivo->nombre);
    imprimirVersion(version, numeroVersion);
    printf("\n");
}


//Pre-Cond: No tiene.
//Pos-Cond: Retorna el numero de la ultima version del Archivo "archivo"
//			Si no tiene versiones retorna 0
unsigned int numeroUltimaVersionArchivo (Archivo archivo){
    if (archivo->version == NULL)
        return 0;
    else {
        return numeroUltimaVersion(archivo->version);
    }
}


//Pre-Cond: Existe la version "nombreVersion" en el Archivo "archivo".
//Pos-Cond: Retorna el numero de la ultima linea de la Version "nombreVersion"
unsigned int numeroUltimaLinea (Archivo archivo, char* nombreVersion){
    AV ver = obtenerVersion(archivo->version, nombreVersion);
    return numeroUltimaLineaVersion(ver, nombreVersion);
}



//********************* PREDICADOS ************************* */

//Pos-Cond: Retorna true si la Version "numeroVersion1" es igual a la version "numeroVersion2"
bool igualesVersionesArchivo (Archivo archivo, char* numeroVersion1, char* numeroVersion2);


//***********************************AGREGADA 11/09/2025 **********************
//Pre-Cond: No tiene
//Pos-Cond: Retorna true si la Version "numeroVersion" existe en el Archivo "archivo"
bool existeVersionEnArchivo(Archivo archivo, char* numeroVersion){
    return existeVersion(archivo->version, numeroVersion);
}

//pre-cond: no tiene
//pos-cond: devuelve TRUE si la version numeroVersion puede insertarse en el archivo
bool puedeInsertarVersionEnArchivo(Archivo archivo, char* numeroVersion){
    return puedeInsertarVersion(archivo->version, numeroVersion);
}

//****************  DESTRUCTORAS ***********************

//Pre-Cond: No tiene
//Pos-Cond: elimina toda la mermoria reservada por "archivo"
void borrarArchivoCompleto(Archivo &archivo){
    destruirTodasLasVersiones(archivo->version);
    delete[] archivo->nombre;
    delete archivo;
}


//Pre-Cond: la version "numeroVersion" existe en version
//Pos-Cond: elimina toda la mermoria reservada por "numeroVersion"
//          y sus sub-versiones del Archivo "archivo"
void borrarVersionDeArchivo (Archivo &archivo, char* numeroVersion){
    destruirVersion(archivo->version, numeroVersion);
}


//Pre-Cond: Existe la Version "numeroVersion" y existe la Linea "numFila" en la Version "numeroVersion"
//Pos-Cond: se elimina la Fila de la posicion "num_fila"
//          el resto de las Filas debajo se renumeran como num_fila=num_fila-1
void borrarLineaVersionArchivo (Archivo &archivo, char* numeroVersion, unsigned int numFila){
    eliminarLineaVersion(archivo->version, numeroVersion, numFila);
}
