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
        nueva->texto = crearCadenaVacia();  // Asegurar que se crea una cadena vacía
        char* texto = convertirCadenaArregloChar(aux->texto);
        agregarCaracteresCadena(nueva->texto, texto);

        delete[] texto;

        nueva->numLinea = aux->numLinea;
        nueva->sig = NULL;
        
        if (inicio == NULL) {
            // Primera línea
            inicio = nueva;
            nueva->ant = NULL;
            actual = nueva;
        } else {
            // Líneas siguientes
            actual->sig = nueva;
            nueva->ant = actual;
            actual = nueva;
        }
        
        aux = aux->sig;
    }

    return inicio;
}

//pos-cond: imprime los cambios entre una linea de una version hija con respecto de su padre
/*void mostrarCambiosLineas(Linea padre, Linea hija){
    bool huboCambios = false;
    
    while (padre != NULL || hija != NULL) {
        if (hija == NULL && padre != NULL) {
            printf("BL\t%d\n", padre->numLinea);
            padre = padre->sig;
            huboCambios = true;
        } else if (padre == NULL && hija != NULL) {
            printf("IL\t%d\t%s\n", hija->numLinea, convertirCadenaArregloChar(hija->texto));
            hija = hija->sig;
            huboCambios = true;
        } else if (padre->numLinea < hija->numLinea) {
            printf("BL\t%d\n", padre->numLinea);
            padre = padre->sig;
            huboCambios = true;
        } else if (padre->numLinea > hija->numLinea) {
            printf("IL\t%d\t%s\n", hija->numLinea, convertirCadenaArregloChar(hija->texto));
            hija = hija->sig;
            huboCambios = true;
        } else { // mismos números
            if (!sonIgualesLineas(padre, hija)) {
                printf("IL\t%d\t%s\n", hija->numLinea, convertirCadenaArregloChar(hija->texto));
                huboCambios = true;
            }
            padre = padre->sig;
            hija = hija->sig;
        }
    }
    if (!huboCambios)
        printf("No se realizaron modificaciones\n");
}
*/

void mostrarCambiosLineas(Linea padre, Linea hija){
    bool huboCambios = false;

    while (padre != NULL || hija != NULL){
        if (padre == NULL && hija != NULL){
            printf("IL\t%d\t", hija->numLinea);
            printf("%s\n", convertirCadenaArregloChar(hija->texto));
            hija = hija->sig;
            huboCambios = true;
        }
        else if (hija == NULL && padre != NULL){
            printf("BL\t%d\n", padre->numLinea);
            padre = padre->sig;
            huboCambios = true;
        }
        else {
            if (!sonIgualesLineas(hija, padre)){
                printf("BL\t%d\n", padre->numLinea);
                printf("IL\t%d\t", hija->numLinea);
                printf("%s\n", convertirCadenaArregloChar(hija->texto));
                huboCambios = true;
            }
            hija = hija->sig;
            padre = padre->sig;
        }
    }

    if (!huboCambios)
        printf("No se realizaron modificaciones\n");
}


/*void mostrarCambiosLineas(Linea padre, Linea hija){
    bool huboCambios = false;
    while (padre != NULL || hija != NULL){
        //si hija es mas "larga" que el padre
        if (padre == NULL && hija != NULL){
            printf("IL\t%d\t%s\n", hija->numLinea, convertirCadenaArregloChar(hija->texto));
            hija = hija->sig;
            huboCambios = true;
        }

        //si hija es mas "chica" que el padre
        if (hija == NULL && padre != NULL){
            printf("BL\t%d\n", padre->numLinea);
            padre = padre->sig;
            huboCambios = true;
        }

        //si hija tiene el mismo largo que el padre y sus lineas son iguales
        if (padre != NULL && hija != NULL){
            if (sonIgualesLineas(padre, hija)){
                padre = padre->sig;
                hija = hija->sig;
            }
        //si tienen el mismo largo pero sus lineas son distintas
            else {
                printf("IL\t%d\t%s\n", hija->numLinea, convertirCadenaArregloChar(hija->texto));
                padre = padre->sig;
                hija = hija->sig;
                huboCambios = true;
            }
        }
    }

    if(!huboCambios)
        printf("No se realizaron modificaciones\n");
    
}
        */







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
