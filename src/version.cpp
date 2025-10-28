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

            nuevaSubVer->linea = copiarLineas(padre->linea);
        }

    }

}    






//************************ SELECTORAS ********************* */

//Pre-cond: no tiene
//Pos-cond: devuelve un puntero al nodo version, si no existe, devuelve NULL
AV buscar(AV t, int *numeroVersion, int tope){
    if (t == NULL)
        return NULL;
    else {
        if (!sonIgualesArrInt(t->numeroVersion, numeroVersion, t->tope + 1, tope + 1)){
            AV esta_SH = buscar(t->sH, numeroVersion, tope);
            if (esta_SH != NULL)
                return esta_SH;
            else
                return buscar(t->pH, numeroVersion, tope);
        }
        else 
            return t;
    }
}


//Pos-Cond: Retorna un puntero a la version de nombre "numVersion". Retora NULL sino existe
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
        return NULL;
        
    }
    //sino, es una subversion
    else {
        return buscar(version->versionRaiz, arrInt, tope);
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
void imprimirVersion(AV version, char* numeroVersion){
    printf("%s \n", numeroVersion);
    printf("\n");
    int cant = numeroUltimaLineaVersion(version, numeroVersion);       //cantidad de lineas en la version
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
    AV esta = buscar(aux->versionRaiz, numVer, tope);
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


/*TipoRet crearVersion(Archivo &a, char * version){
    bool esRaiz = true;
    for (int i = 0; version[i] != '\0' && esRaiz; i++){
        if (version[i] == '.')
            esRaiz = false;
    }

    //si es raiz
    if (esRaiz){
        unsigned int ult = numeroUltimaVersionArchivo(a) + 1;    //obtenemos el numero de ultima version del archivo y le sumamos 1
        int versionNum = atoi(version);
        if (versionNum >= 1 && versionNum <= (int)ult){
            crearVersionArchivo(a, version);
            return OK;
        }
        else
            return ERROR;
    }
    else {
        if (existeVersionEnArchivo(a, version)){
            crearVersionArchivo(a, version);
            return OK;
        }
        else if (!existePadreEnArchivo(a, version) || !existeSubversionHermanaAnteriorEnArchivo(a, version))
            return ERROR;
        else {
            crearVersionArchivo(a, version);
            return OK;
        }
    }
}*/


//****************  DESTRUCTORAS ***********************

//Pre-Cond: la version "numeroVersion" existe y la Linea "numLinea" existe en la version "version"
//Pos-Cond: se elimina la Linea de la posicion "numLinea" el resto de las Lineas debajo se renumeran como numLinea=numLinea-1
void eliminarLineaVersion (Version &version, char* numeroVersion, unsigned int numLinea){
    AV aux = obtenerVersion(version, numeroVersion);
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
            delete aux;
            aux->sig = version;
            while (aux != NULL){
                renumeracionDescendente(version->versionRaiz, 0);
                aux = aux->sig;
            }
        }
        //sino
        else {
            //si lo que queremos borrar esta en el "medio"
            if (aux->sig != NULL){
                Version iter = version;
                while (iter->sig != aux)
                    iter = iter->sig;
                eliminarAV(aux->versionRaiz);
                iter->sig = aux->sig;
                delete aux;
                aux = iter->sig;
                while(aux != NULL){
                    renumeracionDescendente(aux->versionRaiz, 0);
                    aux = aux->sig;
                }
            }
            //sino, esta al final
            if (aux == NULL){
                Version iter = version;
                while (iter->sig->sig != NULL)
                    iter = iter->sig;
                aux = iter->sig;
                eliminarAV(aux->versionRaiz);
                iter->sig = NULL;
                delete aux;
            }
        }

    }
    //sino, vamos a eliminar una subversion
    else {
        //buscamos la subversion a eliminar
        AV aBorrar = buscar(aux->versionRaiz, numVer, tope);
        eliminarSubVersion(aux->versionRaiz, aBorrar);
        if (aBorrar->sH != NULL)
            renumeracionDescendente(aBorrar->sH, aBorrar->tope);
    }
    delete[] numVer;
}


//Pre-Cond: No tiene
//Pos-Cond: Elimina toda la memoria reservada por "version"
void destruirTodasLasVersiones(Version &version){  
    if (version != NULL) {
        eliminarAV(version->versionRaiz);
        destruirTodasLasVersiones(version->sig);
        delete version;
        version = NULL;
    }
}