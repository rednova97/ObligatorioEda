/*
  Módulo de definición de 'Version'.

  Los elementos de tipo 'Version' son punteros a una estructura (que debe
  definirse en version.cpp).

  Laboratorio de EDA Turno Nocturno curso 2025.
  UTU-LATU
 */

#ifndef _VERSION_H
#define _VERSION_H




typedef struct _rep_version *Version;


//***********************  CONSTRUCTORAS ***************** */

//Pos-Cond: Retorna una version vacia
Version crearVersionVacia();

//Pre-Cond: num_version tiene que estar en el rango de 1 o la ultima version + 1 de	la Version "version" 
//Pos-Cond: Crea una nueva version con el numero de verion "num_version. Las versiones iguales y mayores a num_version se les suma 1 al numero de version, lo mismo con
void crearVersion (Version &version, char *num_version); 


//************************ SELECTORAS ********************* */


//Pre-Cond: la version numVersion existe en version
//Pos-Cond: Retorna un puntero a la version de nombre "numVersion"
Version obtenerVersion(Version version, char *numVersion);


//Pre-cond: La version "version" tiene por lo menos "numLinea" de Lineas
//Pos-Cond: Agrega el string textoFila como la fila numLinea de la Version "version". Las filas debajo de num_lineas se renumeran como numLinea=numLinea+1
void agregarFilaVersion (Version &version, char* numeroVersion, char *textoFila,unsigned int numLinea);


//Pre-Cond: existeVersion(version, numeroVersion) retorna true.
//Pos-Cond: Imprime la Version "numeroVersion" junto con sus lineas
void imprimirVersion(Version version, char* numeroVersion);

//Pre-Cond: La Version "numeroVersion" existe en el Archivo "archivo".
//Pos-Cond: Imprime el texto correspondiente a la version "numeroVersion"
void mostrarTextoVersion(Version version, char* numeroVersion);

//imprime un arreglo de enteros
void imprimirNumeroVersion(int *numero, int tope);

//pos-cond: imprime todas las versiones con sus respectivas subversiones
void imprimirTodasLasversiones(Version version);

//Pre-Cond: version != NULL
//Pos-Cond: retorna un puntero a la siguiente Version de "version"
Version siguienteVersion(Version version);

//Pre-Cond: version !=NULL
//Pos-Cond: retorna un puntero a un arreglo dinamico con el numero de la Version "version"
int* nombreVersion(int *numeroVersion, int tope);


//Pre-Cond: (!esVaciaVersion(version)) retorna true
//Pos-Cond: retorna un entero con el numero de la ultima verison que hay en la Version "version"
int numeroUltimaVersion(Version version);




//pos-cond: muestra los cambios de la version hija numeroVersion con respecto a su padre
void mostrarCambiosVersion(Version version, char* numeroVersion);

//********************* PREDICADOS ************************* */
//pre-cond:No tiene
//pos-cond:Retorna true si la Version "version" es vacia.
bool esVaciaVersion (Version version);

//Retorna true si la Version "numeroVersion" existe en "version"
bool existeVersion (Version version, char* numeroVersion);


//pre-cond: no tiene
//pos-cond: devuelve TRUE si se puede insertar la version numeroVersion
bool puedeInsertarVersion(Version version, char* numeroVersion);

//pos-cond: devuelve TRUE si se puede insertar una linea. Solo se puede insertar una linea de una version si la version existe, si esta no tiene hijas y si numLinea <= cantLineas + 1
bool puedeInsertarLinea(Version version, char* numeroVersion, unsigned int numLinea);

//pos-cond: devuelve TRUE si se puede eliminar una linea. Solo se puede eliminar una linea de una version si esta NO TIENE SUBVERSIONES
bool puedeBorrarLinea(Version version, char* numeroVersion, unsigned int numLinea);

//pre-cond: version1 y version2 existen
//pos-cond: devuelve TRUE si version1 y version2 tienen exactamente el mismo texto
bool sonIgualesVersiones(Version version, char* numeroVersion1, char* numeroVersion2);



//****************  DESTRUCTORAS ***********************

//Pre-Cond: la version "numeroVersion" existe y la Linea "numLinea" existe en la version "version"
//Pos-Cond: se elimina la Linea de la posicion "numLinea" el resto de las Lineas debajo se renumeran como numLinea=numLinea-1
void eliminarLineaVersion (Version &version, char* numeroVersion, unsigned int numLinea);


//Pre-Cond: la version "numeroVersion" existe en version
//Pos-Cond: elimina toda la mermoria reservada por "numeroVersion". Reenumera las siguientes versiones
void destruirVersion (Version &version, char* numeroVersion);

//Pre-Cond: No tiene
//Pos-Cond: Elimina toda la memoria reservada por "version"
void destruirTodasLasVersiones(Version &version);

#endif
