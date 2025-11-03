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
}; typedef nodoAV * AV;


struct _rep_version {
    AV versionRaiz;
    _rep_version *sig;
};


//*****************  FUNCIONES AUXILIARES **************************/
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

AV obtenerNodoVersion(Version &version, char *numVersion);

//Pre-Cond: No tiene
//Pos-Cond: retorna un entero con el numero de la ultima linea de la Verison de "version"
int numeroUltimaLineaVersion(AV ver, char *numeroVersion);

//Pre-cond: no tiene
//Pos-cond: devuelve un puntero al nodo version, si no existe, devuelve NULL
AV buscar(AV t, int *numeroVersion, int tope);

//devuelve el maximo entre 2 numeros enteros
int maximo(int a, int b);

//retorna la cantidad de niveles de un arbol
int altura(AV t);

//imprime tabuladores
void imprimirTabs(int n);

//pos-cond: imprime todas las subversiones de la version raiz
void imprimirAV(AV t, int nivel);

//pos-cond: devuelve el nodo mas a la derecha de un arbol finitario
AV ultimoNodoDerecha(AV t);

char *obtenerNumeroPadre(char* numHija);

//pre-cond: a y b tienen el mismo tamanio
//pos-cond: devuelve true si a y b son iguales
bool sonIgualesArrInt(int *a, int *b, int sizeA, int sizeB);

//devuelve true si el arreglo "a" es padre de "b"
bool esPadre(int *a, int *b, int sizeA, int sizeB);


//pos-cond: devuelve truE si la subversion anterior a numeroVersion existe
bool tieneHermanaAnterior(Version version, char* numeroVersion);

//pos-cond: devuelve true si existe la version padre de la subversion numeroVersion
bool tienePadre(Version version, char* numeroVersion);

//pre-cond: existeVersion(numeroVersion) = TRUE
//pos-cond: devuelve TRUE si la version numeroVersion tiene subversiones
bool tieneHijas(Version version, char* numeroVersion);

bool agregueLinea(Version version, char* numeroVersion);

bool borreLinea(Version version, char* numeroVersion);

//Pos-cond: elimina los hijos de un arbol
void eliminarSoloHijos(AV &t);

//pos-cond: elimina un arbol completamente
void eliminarAV(AV &t);

char* convertirArrIntEnString(int *arrInt, int tope);

//pre-cond: la subversion a eliminar existe
//pos-cond: elimina una subversion y sus hijas
void eliminarSubVersion(AV &nodoVer, AV subVersion);

//***************** FIN FUNCIONES AUXILIARES****** */






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
    for (int i=0; i<=n; i++)
        destino[i] = origen[i];
}




//pre-cond: no tiene
//pos-cond: crea un nodo con numero de version "numeroVersion" y tope "tope"
AV crearNodo(int *numeroVersion, int tope){
    AV nuevo = new nodoAV;
    nuevo->numeroVersion = new int[tope + 1];
    copiarArreglo(numeroVersion, nuevo->numeroVersion, tope);
    nuevo->tope = tope;
    nuevo->linea = crearLineaVacia();
    nuevo->pH = NULL;
    nuevo->sH = NULL;
    return nuevo;
}


//Pre-cond: no tiene
//Pos-cond: inserta el nodo nuevo en el arbol actual
AV insertarSubVersion(AV actual, AV nuevo) {
    if (actual == NULL)
        return nuevo;

    // Si es exactamente el mismo número de versión → no duplicar
    if (sonIgualesArrInt(actual->numeroVersion, nuevo->numeroVersion, actual->tope + 1, nuevo->tope + 1)) {
        return actual;
    }

    // Si el nodo actual es prefijo de nuevo → nuevo es descendiente (va como hijo)
    if (esPadre(actual->numeroVersion, nuevo->numeroVersion, actual->tope + 1, nuevo->tope + 1)) {
        actual->pH = insertarSubVersion(actual->pH, nuevo);
        return actual;
    }

    // Si no es descendiente → va como hermano
    actual->sH = insertarSubVersion(actual->sH, nuevo);
    return actual;
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
    int *numVer = NULL;
    int tope;
    convertirStringEnArrInt(num_version, numVer, tope);   //convertimos el char *num_ver a un arreglo de enteros int *numVer y nos devuelve su tope

    int aBuscar = numVer[0];            //obtenemos el primer numero de numVer, el cual usaremos para buscar en la lista de versiones
    Version aux = version;

    while ((aux != NULL) && (aux->versionRaiz->numeroVersion[0] != aBuscar))
        aux = aux->sig;
    
    
    //si el tope es 0 quiere decir que queremos ingresar una nueva version raiz
    if (tope == 0){           
        //si entramos aca quiere decir que la version que queremos ingresar ya existe
        if ( aux != NULL && aux->versionRaiz->numeroVersion[0] == aBuscar){  
            //insertamos al comienzo de la lista de versiones
            if (aBuscar == 1){                
                Version nueva = new _rep_version;
                nueva->versionRaiz = crearNodo(numVer, tope);
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
                nueva->versionRaiz = crearNodo(numVer, tope);
                nueva->sig = temp->sig;
                temp->sig = nueva;

                while (aux != NULL) {
                    renumeracionAscendente(aux->versionRaiz, 0);
                    aux = aux->sig;
                }
            }

        }
        //sino 
        else { 
            //si la lista de versiones esta vacia                     
            if (version == NULL){
                Version nueva = new _rep_version;
                nueva->versionRaiz = crearNodo(numVer, tope);
                nueva->sig = NULL;
                version = nueva;
            }
            //sino, insertamos al final
            else {
                Version temp = version;
                while (temp->sig != NULL)
                    temp = temp->sig;
                Version nueva = new _rep_version;
                nueva->versionRaiz = crearNodo(numVer, tope);
                nueva->sig = NULL;
                temp->sig = nueva;
            }
        }
    }
    //sino, vamos a insertar una subversion
    else {
        AV nuevaSubVer = crearNodo(numVer, tope);
        
        //buscamos al padre
        int topePadre = tope - 1;
        AV padre = buscar(aux->versionRaiz, numVer, topePadre);

        //si el padre existe (por la precondicion, si vamos a insertar una subversion el padre debe existir)
        if (padre != NULL){
            nuevaSubVer->linea = copiarLineas(padre->linea);
            AV actual = padre->pH;
            AV anterior = NULL;

            //buscamos la posicion a insertar entre los hijos del padre
            while (actual != NULL && actual->numeroVersion[tope] < numVer[tope]){
                anterior = actual;
                actual = actual->sH;
            }

            //si hay un hermano con el mismo numero
            if (actual != NULL && actual->numeroVersion[tope] == numVer[tope]){
                if (anterior == NULL)
                    padre->pH = nuevaSubVer;
                else
                    anterior->sH = nuevaSubVer;
                nuevaSubVer->sH = actual;
                renumeracionAscendente(actual, tope);
            }
            //sino
            else {
                //insertamos al final o en el medio
                if (anterior == NULL)
                    padre->pH = nuevaSubVer;
                else
                    anterior->sH = nuevaSubVer;
                nuevaSubVer->sH = actual;
            }
        }

    }

}    



//************************ SELECTORAS ********************* */

//devuelve un puntero a la version donde se encuentra la version con numero numVersion
Version obtenerVersion(Version version, char *numVersion){
    int* arrBusqueda = NULL;
    int tope;
    convertirStringEnArrInt(numVersion, arrBusqueda, tope);
    int aBuscar = arrBusqueda[0];
    Version res = version;
    while (res != NULL && res->versionRaiz->numeroVersion[0] != aBuscar)
        res = res->sig;
    
    return res;
}




//Pre-cond: no tiene
//Pos-cond: devuelve un puntero al nodo version, si no existe, devuelve NULL
AV buscar(AV t, int *numeroVersion, int tope){
    if (t == NULL)
        return NULL;
    else {
        if (!sonIgualesArrInt(t->numeroVersion, numeroVersion, t->tope + 1, tope + 1)){
            AV esta_PH = buscar(t->pH, numeroVersion, tope);
            if (esta_PH != NULL)
                return esta_PH;
            else
                return buscar(t->sH, numeroVersion, tope);
        }
        else 
            return t;
    }
}


//Pos-Cond: Retorna un puntero a un nodo de la version de nombre "numVersion". Retora NULL sino existe
AV obtenerNodoVersion(Version &version, char *numVersion){
    int *arrInt;
    int tope;
    convertirStringEnArrInt(numVersion, arrInt, tope);

    Version aux = version;
    while (aux != NULL && aux->versionRaiz->numeroVersion[0] != arrInt[0])
        aux = aux->sig;

    if (aux == NULL){
        delete[] arrInt;
        return NULL;
    }
        
    
    if (tope == 0){
        delete[] arrInt;
        return aux->versionRaiz;
    }
        
    
    AV res = buscar(aux->versionRaiz, arrInt, tope);
    delete[] arrInt;
    return res;
}


//Pre-cond: La version "version" tiene por lo menos "numLinea" de Lineas
//Pos-Cond: Agrega el string textoFila como la fila numLinea de la Version "version". Las filas debajo de num_lineas se renumeran como numLinea=numLinea+1
void agregarFilaVersion (Version &version, char* numeroVersion, char *textoFila,unsigned int numLinea){
    AV aux = obtenerNodoVersion(version, numeroVersion);        //obtenemos la version donde vamos a insertar el string texto
    insertarLinea(aux->linea, textoFila, numLinea);         //insertamos la linea en la version y reenumeramos las siguientes por debajo (si hay)   
}

//Pre-Cond: existeVersion(version, numeroVersion) retorna true.
//Pos-Cond: Imprime la Version "numeroVersion" junto con sus lineas
void imprimirVersion(Version version, char* numeroVersion){
    Version temp = obtenerVersion(version, numeroVersion);
    AV nodoVer = obtenerNodoVersion(temp, numeroVersion);
    printf("%s \n\n", numeroVersion);
    int cant = numeroUltimaLineaVersion(nodoVer, numeroVersion);       //cantidad de lineas en la version
    if (cant == 0)
        printf("No contiene lineas.\n");
    else {
        for (int i=1; i<=cant; i++){
            char* renglon = obtenerTextoLinea(nodoVer->linea, i);
            printf("%d    %s\n", i, renglon);
            delete[] renglon;
        }
    }  

}

//Pre-Cond: La Version "numeroVersion" existe en el Archivo "archivo".
//Pos-Cond: Imprime el texto correspondiente a la version "numeroVersion"
void mostrarTextoVersion(Version version, char* numeroVersion){
    Version ver = obtenerVersion(version, numeroVersion);
    imprimirVersion(ver, numeroVersion);
    printf("\n");
}


//Pre-Cond: La Version "numeroVersion" existe en el Archivo "archivo".
//Pos-Cond: Imprime el texto correspondiente a la version "numeroVersion"
/*void mostrarTextoArchivoVersion (Archivo archivo, char* numeroVersion){
    AV version = obtenerVersion(archivo->version, numeroVersion);      //obtenemos la version de la cual queremos imprimir el texto
    printf("%s - ", archivo->nombre);
    imprimirVersion(version, numeroVersion);
    printf("\n");
}
*/


//imprime un arreglo de enteros
void imprimirNumeroVersion(int *numero, int tope){
    for (int i = 0; i <= tope; i++){
        printf("%d", numero[i]);
        if (i < tope)
            printf(".");
    }
    printf("\n");
}


//devuelve el maximo entre 2 numeros enteros
int maximo(int a, int b){
    if (a >= b)
        return a;
    else
        return b;
}

//retorna la cantidad de niveles de un arbol
int altura(AV t){
    if (t == NULL)
        return 0;
    else
        return maximo(1 + altura(t->pH), altura(t->sH));
}

//imprime tabuladores
void imprimirTabs(int n){
    while (n > 0){
        printf("\t");
        n--;
    }
}


//pos-cond: imprime todas las subversiones de la version raiz
void imprimirAV(AV t, int nivel){
    if (t != NULL){
        imprimirTabs(nivel);
        imprimirNumeroVersion(t->numeroVersion, t->tope);
        imprimirAV(t->pH, nivel + 1);
        imprimirAV(t->sH, nivel);
    }
}

//pos-cond: imprime todas las versiones con sus respectivas subversiones
void imprimirTodasLasversiones(Version version){
    Version aux = version;
    while (aux != NULL){
        imprimirAV(aux->versionRaiz, 0);
        aux = aux->sig;
    }
}


//Pre-Cond: version != NULL
//Pos-Cond: retorna un puntero a la siguiente Version de "version"
Version siguienteVersion(Version version){
    return version->sig;
}

//Pre-Cond: version !=NULL
//Pos-Cond: retorna un puntero a un arreglo dinamico con el numero de la Version "version"
int* nombreVersion(int *numeroVersion, int tope){
    int *resultado = new int[tope + 1];
    copiarArreglo(numeroVersion, resultado, tope + 1);
    return resultado;
}


//pos-cond: devuelve el nodo mas a la derecha de un arbol finitario
AV ultimoNodoDerecha(AV t){
    AV res = NULL;
    if (t != NULL) {
        if (t->sH != NULL)
            res = ultimoNodoDerecha(t->sH);
        else if (t->pH != NULL)
            res = ultimoNodoDerecha(t->pH);
        else
            res = t;
    }
    return res;
}

//Pre-Cond: (!esVaciaVersion(version)) retorna true
//Pos-Cond: retorna un entero con el numero de la ultima verison que hay en la Version "version"
int numeroUltimaVersion(Version version){
    while(version->sig != NULL)    
        version = siguienteVersion(version);
    return version->versionRaiz->numeroVersion[0];
}


//Pre-Cond: No tiene
//Pos-Cond: retorna un entero con el numero de la ultima linea de la Verison de "version"
int numeroUltimaLineaVersion(AV ver, char *numeroVersion){
    if (ver != NULL)
        return cantidadLineas(ver->linea);
    else
        return 0;
}

char* convertirArrIntEnString(int *arrInt, int tope){
    // 'tope' representa el índice del ÚLTIMO elemento (no la cantidad)
    // Por ejemplo: para version 1.2.3, tope = 2 y elementos en arrInt[0..2]
    if (tope < 0){
        char* vacio = new char[1];
        vacio[0] = '\0';
        return vacio;
    }
            
    int cantidad = tope + 1;
    int maxTam = cantidad * 4 + 1;              
    char* resultado = new char[maxTam];
    resultado[0] = '\0';

    char buffer[16];

    for (int i = 0; i <= tope; i++){
        sprintf(buffer, "%d", arrInt[i]);
        strcat(resultado, buffer);
        if (i < tope)
            strcat(resultado, ".");
    }
    return resultado;
}




//pos-cond: devuelve el numero de la version de padre de la version con numero numHija. Si no tiene padre, devuelve NULL
char *obtenerNumeroPadre(char* numHija){
    int* arrInt = NULL;
    int tope;
    convertirStringEnArrInt(numHija, arrInt, tope);

    int nuevoTope = tope - 1;
    if (nuevoTope < 0)          // si no hay padre (ej: versión "1")
        nuevoTope = -1;

    char* numPadre = convertirArrIntEnString(arrInt, nuevoTope);
    delete[] arrInt;
    return numPadre;
}

//pre-cond: la version numeroVersion existe
//pos-cond: muestra los cambios de la version hija numeroVersion con respecto a su padre
void mostrarCambiosVersion(Version version, char* numeroVersion){
    int *numVer = NULL;
    int tope;
    convertirStringEnArrInt(numeroVersion, numVer, tope);

    AV hija = obtenerNodoVersion(version, numeroVersion);

    AV padre = NULL;
    if (tope > 0){
        int *padreVer = new int[tope];
        copiarArreglo(numVer, padreVer, tope - 1);
        char* padreStr = convertirArrIntEnString(padreVer, tope - 1);
        padre = obtenerNodoVersion(version, padreStr);

        delete[] padreVer;
        delete[] padreStr;
    }

    if (padre != NULL)
        mostrarCambiosLineas(padre->linea, hija->linea);
    else
        mostrarCambiosLineas(NULL, hija->linea);

    delete[] numVer;
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

    if (aux == NULL){
        delete[] numVer;
        return false;
    }

    AV esta = buscar(aux->versionRaiz, numVer, tope);
    if (esta != NULL){
        delete[] numVer;
        return true;
    }
    else {
        delete[] numVer;
        return false;
    }
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

//devuelve true si el arreglo "a" es padre de "b"
bool esPadre(int *a, int *b, int sizeA, int sizeB){
    if (sizeA > sizeB)
        return false;    //a es mas largo que b y por lo tanto esta en un nivel inferior
    
    for(int i = 0; i < sizeA; i++){
        if(a[i] != b[i])
            return false;       //a esta en algun nivel superior, pero no es padre
    }
    return true;
    
}


//pos-cond: devuelve truE si la subversion anterior a numeroVersion existe
bool tieneHermanaAnterior(Version version, char* numeroVersion){
    int *numVer = NULL;
    int tope;
    convertirStringEnArrInt(numeroVersion, numVer, tope);

    int *anterior = new int [tope + 1];
    copiarArreglo(numVer, anterior, tope);

    anterior[tope] = numVer[tope] - 1;                          //si numVer = [1 3 4] -> anterior = [1 3 3]

    Version aux = version;
    while (aux != NULL && aux->versionRaiz->numeroVersion[0] != numVer[0])
        aux = aux->sig;

    if (aux == NULL){
        delete[] anterior;
        delete[] numVer;
        return false;
    }
    
    AV ver = buscar(aux->versionRaiz, anterior, tope);

    bool existe = (ver != NULL);

    delete[] anterior;
    delete[] numVer;

    return existe;
}

//pos-cond: devuelve true si existe la version padre de la subversion numeroVersion
bool tienePadre(Version version, char* numeroVersion){
    int *numVer = NULL;
    int tope;
    convertirStringEnArrInt(numeroVersion, numVer, tope);

    int *padre = new int[tope];
    int topePadre = tope - 1;
    copiarArreglo(numVer, padre, topePadre);

    Version aux = version;
    while (aux != NULL && aux->versionRaiz->numeroVersion[0] != numVer[0])
        aux = aux->sig;

    if (aux == NULL){
        delete[] padre;
        delete[] numVer;
        return false;
    }

    AV ver = buscar(aux->versionRaiz, padre, topePadre);

    bool existe = (ver != NULL);

    delete[] padre;
    delete[] numVer;

    return existe;
}

//pre-cond: existeVersion(numeroVersion) = TRUE
//pos-cond: devuelve TRUE si la version numeroVersion tiene subversiones
bool tieneHijas(Version version, char* numeroVersion){
    int *numVer = NULL;
    int tope;
    convertirStringEnArrInt(numeroVersion, numVer, tope);
    int aBuscar = numVer[0];

    Version aux = version;
    while (aux != NULL && aux->versionRaiz->numeroVersion[0] != aBuscar)
        aux = aux->sig;
    
    AV nodo = buscar(aux->versionRaiz, numVer, tope);

    if (nodo->pH != NULL){
        delete[] numVer;
        return true;
    }
    else {
        delete[] numVer;
        return false;
    }
}

//pre-cond: no tiene
//pos-cond: devuelve TRUE si se puede insertar la version numeroVersion
bool puedeInsertarVersion(Version version, char* numeroVersion){
    bool esRaiz = true;

    for (int i = 0; numeroVersion[i] != '\0' && esRaiz; i++){
        if (numeroVersion[i] == '.')
            esRaiz = false;
    }

    //si es raiz
    if (esRaiz){
        if (version == NULL){
            return (atoi(numeroVersion) == 1);
        }
        unsigned int ult = numeroUltimaVersion(version) + 1;    //obtenemos el numero de ultima version del archivo y le sumamos 1
        int versionNum = atoi(numeroVersion);
        if (versionNum >= 1 && versionNum <= (int)ult)
            return true;
        else 
            return false;
    }
    else {
        if (existeVersion(version, numeroVersion))
            return true;
        if (!tienePadre(version, numeroVersion))
            return false;
        
        int *numVer = NULL;
        int tope;
        convertirStringEnArrInt(numeroVersion, numVer, tope);

        if (numVer[tope] >= 2 && !tieneHermanaAnterior(version, numeroVersion)){
            delete[] numVer;
            return false;
        }
        else {
            delete[] numVer;
            return true;
        }

        
    }
}

//pos-cond: devuelve TRUE si se puede insertar una linea. Solo se puede insertar una linea de una version si la version existe, si esta no tiene hijas y si numLinea <= cantLineas + 1
bool puedeInsertarLinea(Version version, char* numeroVersion, unsigned int numLinea){
    if (!existeVersion(version, numeroVersion))
        return false;

    AV ver = obtenerNodoVersion(version, numeroVersion);
    if (ver == NULL)
        return false;

    if ((int)numLinea > numeroUltimaLineaVersion(ver, numeroVersion) + 1)
        return false;

    if (tieneHijas(version, numeroVersion))
        return false;

    return true;
}


//pos-cond: devuelve TRUE si se puede eliminar una linea. Solo se puede eliminar una linea de una version si la version existe, si esta no tiene hijas y si la linea existe 
bool puedeBorrarLinea(Version version, char* numeroVersion, unsigned int numLinea){
    if (!existeVersion(version, numeroVersion))
        return false;

    AV ver = obtenerNodoVersion(version, numeroVersion);
    if (ver == NULL)
        return false;

    if (tieneHijas(version, numeroVersion))
        return false;    
    
    if (!existeNumeroLinea(ver->linea, numLinea))
        return false;
    
    return true;
}

//pre-cond: version1 y version2 existen
//pos-cond: devuelve TRUE si version1 y version2 tienen exactamente el mismo texto
bool sonIgualesVersiones(Version version, char* numeroVersion1, char* numeroVersion2){
    AV v1 = obtenerNodoVersion(version, numeroVersion1);
    AV v2 = obtenerNodoVersion(version, numeroVersion2);

    return sonIgualesTodasLasLineas(v1->linea, v2->linea);
}


//****************  DESTRUCTORAS ***********************

//Pre-Cond: la version "numeroVersion" existe y la Linea "numLinea" existe en la version "version"
//Pos-Cond: se elimina la Linea de la posicion "numLinea" el resto de las Lineas debajo se renumeran como numLinea=numLinea-1
void eliminarLineaVersion (Version &version, char* numeroVersion, unsigned int numLinea){
    AV aux = obtenerNodoVersion(version, numeroVersion);
    eliminarLinea(aux->linea, numLinea);
}

//Pos-cond: elimina los hijos de un arbol
void eliminarSoloHijos(AV &t){
    if (t != NULL){
        AV hijo = t->pH;
        AV sig;

        while (hijo != NULL){
            sig = hijo->sH;         //guardamos el siguiente hermano de "hijo"
            eliminarAV(hijo);
            hijo = sig;
        }
        t->pH = NULL;
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
            AV sig = nodoVer->sH;
            nodoVer->sH = NULL;         //desenganchamos los hermanos para no borrarlos
            eliminarAV(nodoVer);
            nodoVer = sig;              //reenganchamos con el siguiente hermano
        }
        //sino
        else {
            eliminarSubVersion(nodoVer->pH, subVersion);

            if (nodoVer != NULL)
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

    while ((aux != NULL) && (aux->versionRaiz->numeroVersion[0] != aEliminar))
        aux = aux->sig;
    
    //si tope = 0 queremos borrar una version raiz padre, y por lo tanto, todas sus subversiones
    if (tope == 0){
        Version iter = version;
        Version anterior = NULL;

        while (iter != NULL && iter->versionRaiz->numeroVersion[0] != aEliminar){
            anterior = iter;
            iter = iter->sig;
        }

        if (iter != NULL){
            if (anterior == NULL)
                version = iter->sig;
            else
                anterior->sig = iter->sig;

            eliminarAV(iter->versionRaiz);
            delete iter;

            Version temp;
            if (anterior == NULL)
                temp = version;
            else
                temp = anterior->sig;

            while (temp != NULL){
                renumeracionDescendente(temp->versionRaiz, 0);
                temp = temp->sig;
            }
        }
    }
    //sino, vamos a eliminar una subversion
    else {
        AV aBorrar = buscar(aux->versionRaiz, numVer, tope);
        
        if (aBorrar != NULL){
            AV hermana = aBorrar->sH;
            int nivel = aBorrar->tope;

            eliminarSubVersion(aux->versionRaiz, aBorrar);

            if (hermana != NULL)
                renumeracionDescendente(hermana, nivel);
        }
    }

    delete[] numVer;
}


//Pre-Cond: No tiene
//Pos-Cond: Elimina toda la memoria reservada por "version"
void destruirTodasLasVersiones(Version &version){  
    while (version != NULL){
        Version sig = version->sig;
        eliminarAV(version->versionRaiz);
        delete version;
        version = sig;                      //avanzamos en la lista de versiones
    }
}