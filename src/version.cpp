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
//pos-cond: convierte un string con caracteres "numeros" en un arreglo de enteros y lo guarda en "numero", eliminando los puntos. Si el string es 1.2.1 lo convierte a 1 2 1 
void convertirStringEnArrInt(char *string, int *&numero, int &tope){
    //contamos la cantidad de puntos
    int largo = strlen(string);
    int puntos=0;
    for(int i=0; i<largo; i++){
        if(string[i]=='.')
            puntos++;
    }

    tope = puntos;
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


//Pre-cond: no tiene
//pos-cond: copia el arreglo origen en el arreglo destino
void copiarArreglo(int *origen, int *destino, int n){
    for (int i=0; i<n; i++)
        destino[i] = origen[i];
}

//pre-cond: no tiene
//pos-cond: crea un nodo con numero de version "numeroVersion" y tope "tope"
AV crearNodo(int *numeroVersion, int tope){
    AV nuevo = new nodoAV;
    nuevo->numeroVersion = new int;
    copiarArreglo(numeroVersion, nuevo->numeroVersion, tope);
    nuevo->tope = tope;
    nuevo->linea = crearLineaVacia();
    nuevo->pH = NULL;
    nuevo->sH = NULL;
    return nuevo;
}


//Pre-cond: no tiene
//Pos-cond: inserta el nodo nuevo en el arbol actual
AV insertarSubVersion(AV actual, AV nuevo){
    if (actual == NULL)
        return NULL;
    
    //si ambos tienen el mismo padre
    if (sonIgualesArrInt(actual->numeroVersion, nuevo->numeroVersion, actual->tope, nuevo->tope)){
        nuevo->sH = actual;
        return nuevo;
    }

    //si el nodo actual esta en un nivel supererior, entonces nuevo va como hijo de actual
    if (esAnterior(actual->numeroVersion, nuevo->numeroVersion, actual->tope, nuevo->tope)){
        actual->pH = insertarSubVersion(actual->pH, nuevo);
        return actual;
    }
    //sino, insertamos nuevo como hermano de actual
    else {
        actual->sH = insertarSubVersion(actual->sH, nuevo);
        return actual;
    }
}


//pre-cond: no tiene
//pos-cond: reenumera los hijos y hermanos de la version t como t + 1 a partir de la posicion pos
void renumeracionAscendente(AV t, int pos){
    if(t != NULL){
        t->numeroVersion[pos]++;

        if (t->pH != NULL){
            renumeracionAscendente(t->pH, pos);
        }
        if (t->sH != NULL){
            renumeracionAscendente(t->sH, pos);
        }
    }
}

//pre-cond: no tiene
//pos-cond: reenumera los hijos y hermanos de la version t como t - 1 a partir de la posicion pos
void renumeracionDescendente(AV t, int pos){
    if(t != NULL){
        t->numeroVersion[pos]--;

        if (t->pH != NULL){
            renumeracionDescendente(t->pH, pos);
        }
        if (t->sH != NULL){
            renumeracionDescendente(t->sH, pos);
        }
    }  
}


//Pre-Cond: num_version tiene que estar en el rango de 1 o la ultima version + 1 de	la Version "version" 
//Pos-Cond: Crea una nueva version con el numero de verion "num_version. Las versiones iguales y mayores a num_version se les suma 1 al numero de version, lo mismo con
void crearVersion (Version &version, char *num_version){
    int *numVer = new int;
    int tope;
    convertirStringEnArrInt(num_version, numVer, tope);   //convertimos el char *num_ver a un arreglo de enteros int *numVer

    int aBuscar = numVer[0];            //obtenemos el primer numero de numVer, el cual usaremos para buscar en la lista de versiones
    Version aux = version;

    while ((aux != NULL) & (aux->versionRaiz->numeroVersion[0] != aBuscar))
        aux = aux->sig;
    
    
    //si el tope es 0 quiere decir que queremos ingresar una nueva version raiz
    if (tope == 0){           
        //si entramos aca quiere decir que la version que queremos ingresar ya existe
        if (aux->versionRaiz->numeroVersion[0] == aBuscar){  
            //insertamos al comienzo de la lista de versiones
            if (aBuscar == 1){                
                Version nueva = new _rep_version;
                nueva->versionRaiz = crearNodo(numVer, tope + 1);
                nueva->sig = version;
                version = nueva;

                while (aux != NULL){
                    renumeracionAscendente(aux->versionRaiz, 0);
                    aux = aux->sig;
                }
            }
            //insertamos en el "medio"
            else {              
                Version temp = version;
                int cont = 1;
                while (temp != NULL && cont < aBuscar - 1){
                    temp = temp->sig;
                    cont++;
                }
                Version nueva = new _rep_version;
                nueva->versionRaiz = crearNodo(numVer, tope + 1);
                nueva->sig = temp->sig;
                temp->sig = nueva;

                while (aux != NULL) {
                    renumeracionAscendente(aux->versionRaiz, 0);
                    aux = aux->sig;
                }
            }

        }
        //si entramos aca es porque esa version no existe y por lo tanto, vamos a insertar al final
        else {                      
            Version temp = version;
            while (temp->sig != NULL)
                temp = temp->sig;
            Version nueva = new _rep_version;
            nueva->versionRaiz = crearNodo(numVer, tope + 1);
            nueva->sig = NULL;
            temp->sig = NULL;
        }
    }
    //sino, vamos a insertar una subversion
    else {
        AV nuevaSubVer = crearNodo(numVer, tope + 1);
        aux->versionRaiz = insertarSubVersion(aux->versionRaiz, nuevaSubVer);
        nuevaSubVer->linea = copiarLineas(aux->versionRaiz->linea);
        if (nuevaSubVer->sH != NULL)
            renumeracionAscendente(nuevaSubVer->sH, nuevaSubVer->tope);
    }

}    




//************************ SELECTORAS ********************* */

//Pre-cond: no tiene
//Pos-cond: devuelve un puntero al nodo version, si no existe, devuelve NULL
AV buscar(AV t, int *numeroVersion){
    if (t == NULL)
        return NULL;
    else {
        if (!sonIgualesArrInt(t->numeroVersion, numeroVersion, t->tope)){
            AV esta_SH = buscar(t->sH, numeroVersion);
            if (esta_SH != NULL)
                return esta_SH;
            else
                return buscar(t->pH, numeroVersion);
        }
        else 
            return t;
    }
}


//Pre-Cond: la version numVersion existe en version
//Pos-Cond: Retorna un puntero a la version de nombre "numVersion"
AV obtenerVersion(Version &version, char *numVersion){
    int *arrInt;
    int tope;
    convertirStringEnArrInt(numVersion, arrInt, tope);

    //si la version buscada es la version padre
    if (tope == 1){
        Version aux = version;
        while (aux != NULL){
            if (aux->versionRaiz->numeroVersion[0] == arrInt[0])
                return aux->versionRaiz;
            aux = aux->sig;
        }
        
    }
    //sino, es una subversion
    else {
        return buscar(version->versionRaiz, arrInt);
    }
}


//Pre-cond: La version "version" tiene por lo menos "numLinea" de Lineas
//Pos-Cond: Agrega el string textoFila como la fila numLinea de la Version "version". Las filas debajo de num_lineas se renumeran como numLinea=numLinea+1
void agregarFilaVersion (Version &version, char* numeroVersion, char *textoFila,unsigned int numLinea){
    AV aux = obtenerVersion(version, numeroVersion);        //obtenemos la version donde vamos a insertar el string texto
    insertarLinea(aux->linea, textoFila, numLinea);         //insertamos la linea en la version y reenumeramos las siguientes por debajo (si hay)   
}



//Pre-Cond: existeVersion(version, numeroVersion) retorna true.
//Pos-Cond: Imprime la Version "numeroVersion" junto con sus lineas
void imprimirVersion(Version version, char* numeroVersion){
    printf("%d \n", numeroVersion);
    printf("\n");
    int cant = numeroUltimaLineaVersion(version, numeroVersion);       //cantidad de lineas en la version
    if (cant == 0)
        printf("No contiene lineas.\n");
    else {
        for (int i=1; i<=cant; i++){
            char* renglon = obtenerTextoLinea(version->versionRaiz->linea, i);
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
char* nombreVersion(char *numeroVersion){
    int len = strlen(numeroVersion);
    char *resultado = new char[len + 1];
    strcpy(resultado, numeroVersion);
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
int numeroUltimaLineaVersion(Version version, char *numeroVersion){
    AV ver = obtenerVersion(version, numeroVersion);
    if (ver != NULL)
        return cantidadLineas(ver->linea);
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
    int *numVer;
    int tope;
    convertirStringEnArrInt(numeroVersion, numVer, tope);

    int aBuscar = numVer[0];
    Version aux = version;
    while (aux != NULL && aux->versionRaiz->numeroVersion[0] != aBuscar){
        aux = aux->sig;
    }
    AV esta = buscar(aux->versionRaiz, numVer);
    if (esta != NULL)
        return true;
    else
        return false;
}

//pre-cond: a y b tienen el mismo tamanio
//pos-cond: devuelve true si a y b son iguales
bool sonIgualesArrInt(int *a, int *b, int sizeA, int sizeB){
    if (sizeA != sizeB)
        return false;

    for (int i=0; i<sizeA; i++){
        if (a[i] != b[i])
            return false;
    }
    return true;
}

bool esAnterior(int *a, int *b, int sizeA, int sizeB){
    if (sizeA > sizeB)
        return false;    //a es mas largo que b y por lo tanto esta en un nivel inferior
    
    for(int i = 0; i < sizeA; i++){
        if(a[i] != b[i])
            return false;       //a esta en algun nivel superior
    }
    return true;
    
}



//****************  DESTRUCTORAS ***********************

//Pre-Cond: la version "numeroVersion" existe y la Linea "numLinea" existe en la version "version"
//Pos-Cond: se elimina la Linea de la posicion "numLinea"
//          el resto de las Lineas debajo se renumeran como numLinea=numLinea-1
void eliminarLineaVersion (Version &version, char* numeroVersion, unsigned int numLinea){
    Version aux = obtenerVersion(version, numeroVersion);
    eliminarLinea(aux->linea, numLinea);
}

//Pos-cond: elimina los hijos de un arbol
void eliminarSoloHijos(AV &t){
    if (t != NULL){
        eliminarSoloHijos(t->pH);
        delete t;
        t = NULL;
    }
}

//pos-cond: elimina un arbol completamente
void eliminarAV(AV &t){
    if (t != NULL){
        eliminarAV(t->pH);
        eliminarAV(t->sH);
        delete t;
        t = NULL;
    }
}


//pre-cond: la subversion a eliminar existe
//pos-cond: elimina una subversion y sus hijas
void eliminarSubVersion(AV &nodoVer, AV subVersion){
    if (nodoVer != NULL && subVersion != NULL){
        //si estamos en el nodo que queremos borrar
        if (nodoVer == subVersion){
            eliminarSoloHijos(nodoVer);
            nodoVer = NULL;
        }
        //sino
        else {
            //si lo que queremos borrar esta como primer hijo
            if (nodoVer->pH == subVersion){
                AV temp = subVersion->sH;
                eliminarSoloHijos(nodoVer->pH);
                nodoVer->pH = temp;
            }
            //sino
            else
                eliminarSubVersion(nodoVer->pH, subVersion);

            //si lo que queremos borrar esta como siguiente hermano
            if (nodoVer->sH == subVersion){
                AV temp = subVersion->sH;
                eliminarSoloHijos(nodoVer->sH);
                nodoVer->sH = temp;
            }
            //sino
            else
                eliminarSubVersion(nodoVer->sH, subVersion);
        }
    }
}

//Pre-Cond: la version "numeroVersion" existe en version
//Pos-Cond: elimina toda la mermoria reservada por "numeroVersion". Reenumera las siguientes versiones
void destruirVersion (Version &version, char* numeroVersion){
    int *numVer;
    int tope;
    convertirStringEnArrInt(numeroVersion, numVer, tope);
    
    int aEliminar = numVer[0];            //obtenemos el primer numero de numVer, el cual usaremos para buscar en la lista de versiones
    Version aux = version;

    while ((aux != NULL) & (aux->versionRaiz->numeroVersion[0] != aEliminar))
        aux = aux->sig;
    
    //si tope = 0 queremos borrar una version raiz padre, y por lo tanto, todas sus subversiones
    if (tope == 0){
        //si queremos eliminar la version padre 1
        if (aEliminar == 1){
            version = version->sig;
            eliminarAV(aux->versionRaiz);
            aux->sig = version;
            while (aux != NULL){
                renumeracionDescendente(version->versionRaiz, 0);
                aux = aux->sig;
        }
        //sino
        else {
            //si lo que queremos borrar esta en el "medio"
            if (aux->sig != NULL){
                Version iter = version;
                
            }
        }

    }


}


/*AV nuevaSubVer = crearNodo(numVer, tope + 1);
        aux->versionRaiz = insertarSubVersion(aux->versionRaiz, nuevaSubVer);
        nuevaSubVer->linea = copiarLineas(aux->versionRaiz->linea);
        if (nuevaSubVer->sH != NULL)
            renumeracionAscendente(nuevaSubVer->sH, nuevaSubVer->tope);*/

////////////////////////////////// AGREGADA 05/09/2025  ////////////
//Pre-Cond: No tiene
//Pos-Cond: Elimina toda la memoria reservada por "version"
void destruirTodasLasVersiones(Version &version){    
    while (!esVaciaVersion(version))
        destruirVersion(version, version->nombreVersion);
}