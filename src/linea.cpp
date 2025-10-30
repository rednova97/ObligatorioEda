/* 5253404 */ // sustituiir con los 7 dÃ­gitos de la cÃ©dula
#include "../include/linea.h"
#include "../include/cadena.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



struct _rep_linea {
    unsigned int numLinea;
    Cadena texto;
    _rep_linea *sig;
    _rep_linea *ant;
};

//**************************** CONSTRUCTORAS ***************** */
//Crea una linea vacia
Linea crearLineaVacia(){
    return NULL;
}

//Pre-cond: existen por lo menos (num_fila - 1)
//Poa-Cond: inserta una nueva linea en la posicion "numLila"; 
//          Las Lineas debajo, si existen, se renumeran numLila=numLila+1
void insertarLinea(Linea &linea, char* texto, unsigned int numLinea) {
    Linea aux = linea;
    unsigned int iter = 1;

    while ((aux != NULL) && (iter != numLinea)) {  //buscamos la posicion
        aux = aux->sig;
        iter++;
    }

    Linea nuevo = new _rep_linea;
    nuevo->numLinea = numLinea;
    nuevo->texto = crearCadenaVacia();
    agregarCaracteresCadena(nuevo->texto, texto);

    if (aux == NULL) {
        //si la lista esta vacia, insertamos en la primera posicion
        if ((linea == NULL) && numLinea == 1){
            nuevo->ant = NULL;
            nuevo->sig = NULL;
            linea = nuevo;
        }
        else {          //caso insertar al final
            Linea ultimo = linea;
            while (ultimo->sig != NULL)
                ultimo = ultimo->sig;
            
            nuevo->ant = ultimo;
            nuevo->sig = NULL;
            ultimo->sig = nuevo;
        }
    } 
    else {          // caso insertar en el medio o puede ser insertar al principio       
        nuevo->sig = aux;
        nuevo->ant = aux->ant;

        if (aux->ant != NULL)
            aux->ant->sig = nuevo;

        aux->ant = nuevo;
        
        if (aux == linea && nuevo->ant == NULL)     // entramos aca si vamos a insertar al principio
            linea = nuevo;
        
        while (aux != NULL) {                   // renumeramos los siguientes numeros de linea luego del elemento insertado
            aux->numLinea = numLinea++;
            aux = aux->sig;
        }
    }
    renumerarLineas(linea);
}


//************************** SELECTORAS ********************** */

//Pre-Cond: esVaciaLinea(linea) retorna false; 
//Pos-Cond: Retorna un puntero de Linea a la siguiente linea de la
//          estructura "linea"
Linea siguienteLinea(Linea linea){
    return linea->sig;
}

//Pre-Cond: esVaciaLinea(linea) retorna false; 
//Pos-Cond: Retorna un puntero de Linea a la anterior a linea de la
//          estructura "linea"
Linea anteriorLinea(Linea linea){
    return linea->ant;
}

//Pos-Cond: Retorna la cantidad de lineas que tiene la estructura linea
unsigned int cantidadLineas(Linea linea){
    unsigned int cant = 0;
    while (linea != NULL){
        linea = linea->sig;
        cant++;
    }
    return cant;
}

//pre-cond: linea != NULL
//pos-cond: retorna el numero de la Linea linea
unsigned int getNumeroLinea(Linea linea){
    return linea->numLinea;       
}

//Pre-cond: La Linea numLineaa, existe en la estructura "linea"
//Pos-Cond: retorna un arreglo dinamico con todo el texto de la Linea "numLinea".
//          sin compartir memoria con "linea"
char* obtenerTextoLinea (Linea linea, unsigned int numLinea){
    for (unsigned int i=1; i<numLinea; i++)
        linea = linea->sig;
    return convertirCadenaArregloChar(linea->texto); 
}

//pos-cond: devuelve una copia de todas las lineas sin compartir memoria
Linea copiarLineas(Linea linea){
    if (linea == NULL)
        return NULL;
    
    Linea inicio = NULL;
    Linea actual = NULL;
    Linea aux = linea;

    while (aux != NULL){
        Linea nueva = new _rep_linea;
        nueva->texto = NULL;
        char* texto = convertirCadenaArregloChar(aux->texto);
        agregarCaracteresCadena(nueva->texto, texto);

        delete[] texto;

        nueva->numLinea = aux->numLinea;
        nueva->sig = NULL;

        if (inicio == NULL)
            inicio = nueva;
        else
            actual->sig = nueva;
        
        actual = nueva;
        aux = aux->sig;
    }

    return inicio;
}

//pos-cond: imprime los cambios entre una linea de una version hija con respecto de su padre
void mostrarDiferenciasLineas(Linea padre, Linea hija){
    //Contamos el tamanio del padre y de la hija
    unsigned int tamanioPadre = 0;
    unsigned int tamanioHija = 0;

    Linea iterPadre = padre;
    while (iterPadre != NULL){
        tamanioPadre++;
        iterPadre = iterPadre->sig;
    }

    Linea iterHija = hija;
    while (iterHija != NULL){
        tamanioHija++;
        iterHija = iterHija->sig;
    }

    // Copiamos los punteros en arreglos para hacer la busqueda mas "facil"
    Linea *arrPadre = NULL;
    if (tamanioPadre > 0)
        arrPadre = new Linea[tamanioPadre];
    
    Linea *arrHija  = NULL;
    if (tamanioHija > 0)
        arrHija = new Linea[tamanioHija];

    unsigned int i = 0;

    iterPadre = padre;
    while (iterPadre != NULL){
        arrPadre[i] = iterPadre;
        i++;
        iterPadre = iterPadre->sig;
    }

    i = 0;

    iterHija = hija;
    while (iterHija != NULL){
        arrHija[i] = iterHija;
        i++;
        iterHija = iterHija->sig;
    }
    

    //creamos arreglos booleanos para marcar las coincidencias
    bool *coincidePadre = NULL;
    if (tamanioPadre > 0)
        coincidePadre = new bool[tamanioPadre];
    
    bool *coincideHija  = NULL;
    if (tamanioHija > 0)
        coincideHija = new bool[tamanioHija];
    
    for (i = 0; i < tamanioPadre; i++){
        if (coincidePadre)
            coincidePadre[i] = false;
    }
    for (i = 0; i < tamanioHija;  i++){
        if (coincideHija)
            coincideHija[i] = false;
    }

    //cada hija busca su primera coincidencia con el padre
    for (unsigned int ih = 0; ih < tamanioHija; ih++){
        for (unsigned int ip = 0; ip < tamanioPadre; ip++){
            bool hijaYaCoincide = (coincideHija && coincideHija[ih]);
            if (!hijaYaCoincide){
                bool padreNoCoincidente = !(coincidePadre && coincidePadre[ip]);
                if (padreNoCoincidente){
                    if (sonIgualesCadenas(arrHija[ih]->texto, arrPadre[ip]->texto)){
                        if (coincideHija)
                            coincideHija[ih] = true;
                        if (coincidePadre)
                            coincidePadre[ip] = true;
                    }
                }
            }
        }
    }

    bool huboCambios = false;       //hija con respecto al padre

    // hacemos las inserciones IL en orden de la hija
    for (unsigned int ih = 0; ih < tamanioHija; ih++){
        if (!(coincideHija && coincideHija[ih])){
            printf("IL\t%d\t", arrHija[ih]->numLinea);
            printf("%s\n", convertirCadenaArregloChar(arrHija[ih]->texto));
            huboCambios = true;
        }
    }

    //hacemos los borrados BL en orden del padre
    for (unsigned int ip = 0; ip < tamanioPadre; ip++){
        if (!(coincidePadre && coincidePadre[ip])){
            printf("BL\t%d\n", arrPadre[ip]->numLinea);
            huboCambios = true;
        }
    }

    if (!huboCambios)
        printf("No se realizaron modificaciones\n");

    if (arrPadre) 
        delete[] arrPadre;
    if (arrHija) 
        delete[] arrHija;
    if (coincidePadre) 
        delete[] coincidePadre;
    if (coincideHija) 
        delete[] coincideHija;
}


//*************************** PREDICADOS ********************** */

//Pos-Cond: retorna true si linea es vaci­a
bool esVaciaLinea(Linea linea){
    return linea == NULL;
}

//Pos-Cond: Retorna true si la Linea "numLinea" existe en la estructura "linea"
bool existeNumeroLinea(Linea linea, unsigned int numLinea){
    while (linea != NULL){
        if (linea->numLinea == numLinea)
            return true;
        linea = linea->sig;
    }
    return false;
}

//Pre-Cond: (esVaciaFila(fila1) && esVaciaFila(fila2)) retorna false
//Pos-Cond: retorna true si fila1 tiene los mismos caracteres 
//          y en el mismo orden que fila2
bool sonIgualesLineas(Linea linea1, Linea linea2){
    if (linea1 == NULL || linea2 == NULL)
        return false; 
    
    return sonIgualesCadenas(linea1->texto, linea2->texto);
}


//***************************** DESTRUCTORAS **************** */

//Pre-cond: la Linea "numLinea" existe en la Linea "linea"
//Pos-Cond: elimina la Linea de la estructura "linea" de la posicion "numLinea" y reenumera las siguientes lineas (si hay)
void eliminarLinea(Linea &linea, unsigned int numLinea){
    Linea anterior = NULL;
    Linea actual = linea;

    while (actual != NULL && actual->numLinea != numLinea){
        anterior = actual;
        actual = actual->sig;
    }
        
    if (anterior == NULL){
        //borrar la primer linea
        linea = actual->sig;
        if (linea != NULL){
            linea->ant = NULL;
        }
    }
    else {
        anterior->sig = actual->sig;
        if (actual->sig != NULL){
            actual->sig->ant = anterior;
        }
    }

    destruirCadena(actual->texto);
    delete actual;

    renumerarLineas(linea);
}

//Pos-Cond: elimina toda la memoria de la estructura Linea "linea"
void destruirLinea(Linea &linea){
    while (linea != NULL){
        Linea aux = linea;
        destruirCadena(aux->texto);     //liberamos la memoria ocupada por el texto
        linea = linea->sig;
        delete aux;
    }
}

// Renumera todas las líneas desde el principio, dejando secuencia 1,2,3...
void renumerarLineas(Linea linea) {
    unsigned int i = 1;
    while (linea != NULL) {
        linea->numLinea = i++;
        linea = linea->sig;
    }
}
