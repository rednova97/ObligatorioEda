/*
  Módulo de definición de 'Version'.

  Los elementos de tipo 'Version' son punteros a una estructura (que debe
  definirse en version.cpp).

  Laboratorio de EDA Turno Nocturno curso 2025.
  UTU-LATU
 */

#ifndef _VERSION_H
#define _VERSION_H


typedef struct nodoAV * AV;

typedef struct _rep_version *Version;


//***********************  CONSTRUCTORAS ***************** */

//Pos-Cond: Retorna una version vacia
Version crearVersionVacia();


//pre-cond: no tiene
//pos-cond: convierte un string con caracteres "numeros" en un arreglo de enteros y lo guarda en "numero", eliminando los puntos. Si el string es 1.2.1 lo convierte a 1 2 1 
void convertirStringEnArrInt(char *string, int *&numero, int &tope);

//Pre-cond: no tiene
//pos-cond: copia el arreglo origen en el arreglo destino
void copiarArreglo(int *origen, int *destino, int n);

//pre-cond: no tiene
//pos-cond: crea un nodo con numero de version "numeroVersion" y tope "tope"
AV crearNodo(int *numeroVersion, int tope);


//Pre-cond: no tiene
//Pos-cond: inserta el nodo nuevo en el arbol actual
AV insertarSubVersion(AV actual, AV nuevo);


//pre-cond: no tiene
//pos-cond: reenumera los hijos y hermanos de la version t como t + 1 a partir de la posicion pos
void renumeracionAscendente(AV t, int pos);

//pre-cond: no tiene
//pos-cond: reenumera los hijos y hermanos de la version t como t - 1 a partir de la posicion pos
void renumeracionDescendente(AV t, int pos);


//Pre-Cond: num_version tiene que estar en el rango de 1 o la ultima version + 1 de	la Version "version" 
//Pos-Cond: Crea una nueva version con el numero de verion "num_version. Las versiones iguales y mayores a num_version se les suma 1 al numero de version, lo mismo con
void crearVersion (Version &version, char *num_version); 


//************************ SELECTORAS ********************* */

//Pre-cond: no tiene
//Pos-cond: devuelve un puntero al nodo version, si no existe, devuelve NULL
AV buscar(AV t, int *numeroVersion, int tope);

//Pre-Cond: la version numVersion existe en version
//Pos-Cond: Retorna un puntero a la version de nombre "numVersion"
AV obtenerVersion(Version &version, char *numVersion);


//Pre-cond: La version "version" tiene por lo menos "numLinea" de Lineas
//Pos-Cond: Agrega el string textoFila como la fila numLinea de la Version "version". Las filas debajo de num_lineas se renumeran como numLinea=numLinea+1
void agregarFilaVersion (Version &version, char* numeroVersion, char *textoFila,unsigned int numLinea);


//Pre-Cond: existeVersion(version, numeroVersion) retorna true.
//Pos-Cond: Imprime la Version "numeroVersion" junto con sus lineas
void imprimirVersion(AV version, char* numeroVersion);

//imprime un arreglo de enteros
void imprimirNumeroVersion(int *numero, int tope);

//retorna la cantidad de nuiveles
int altura(AV t);


//pos-cond: imprime todas las subversiones de la version raiz junto con la version raiz
void imprimirAV(AV t, int nivel);

//pos-cond: imprime todas las versiones con sus respectivas subversiones
void imprimirTodasLasversiones(Version version);

//Pre-Cond: version != NULL
//Pos-Cond: retorna un puntero a la siguiente Version de "version"
Version siguienteVersion(Version version);

//Pre-Cond: version !=NULL
//Pos-Cond: retorna un puntero a un arreglo dinamico con el numero de la Version "version"
int* nombreVersion(int *numeroVersion, int tope);

//pos-cond: devuelve el nodo mas a la derecha de un arbol finitario
AV ultimoNodoDerecha(AV t);


//Pre-Cond: (!esVaciaVersion(version)) retorna true
//Pos-Cond: retorna un entero con el numero de la ultima verison que hay en la Version "version"
int numeroUltimaVersion(Version version);


//Pre-Cond: No tiene
//Pos-Cond: retorna un entero con el numero de la ultima linea de la Verison de "version"
int numeroUltimaLineaVersion(AV ver, char *numeroVersion);

//********************* PREDICADOS ************************* */
//pre-cond:No tiene
//pos-cond:Retorna true si la Version "version" es vacia.
bool esVaciaVersion (Version version);

//Retorna true si la Version "numeroVersion" existe en "version"
bool existeVersion (Version version, char* numeroVersion);

//pre-cond: a y b tienen el mismo tamanio
//pos-cond: devuelve true si a y b son iguales
bool sonIgualesArrInt(int *a, int *b, int sizeA, int sizeB);

bool esAnterior(int *a, int *b, int sizeA, int sizeB);



//****************  DESTRUCTORAS ***********************

//Pre-Cond: la version "numeroVersion" existe y la Linea "numLinea" existe en la version "version"
//Pos-Cond: se elimina la Linea de la posicion "numLinea" el resto de las Lineas debajo se renumeran como numLinea=numLinea-1
void eliminarLineaVersion (Version &version, char* numeroVersion, unsigned int numLinea);
//Pos-cond: elimina los hijos de un arbol
void eliminarSoloHijos(AV &t);

//pos-cond: elimina un arbol completamente
void eliminarAV(AV &t);


//pre-cond: la subversion a eliminar existe
//pos-cond: elimina una subversion y sus hijas
void eliminarSubVersion(AV &nodoVer, AV subVersion);

//Pre-Cond: la version "numeroVersion" existe en version
//Pos-Cond: elimina toda la mermoria reservada por "numeroVersion". Reenumera las siguientes versiones
void destruirVersion (Version &version, char* numeroVersion);

//Pre-Cond: No tiene
//Pos-Cond: Elimina toda la memoria reservada por "version"
void destruirTodasLasVersiones(Version &version);

#endif
