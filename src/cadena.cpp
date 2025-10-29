/* 5253404 */ // sustituiir con los 7 dÃ­gitos de la cÃ©dula

#include "../include/cadena.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;


struct _rep_cadena {
    char car;
    _rep_cadena *sig;
};


/* Operaciones de Cadena */

//Pos-Cond: retorna una cadena de caracteres vacia
Cadena crearCadenaVacia(){
    return NULL;
}

//Pre-Cond: esVaciaCadenaCaracteres(cadena)retorna true.
//Pos-Cond: agrega los caracteres de cadena a la fila vaica "fila"
void agregarCaracteresCadena(Cadena &cadena, char* texto){
    Cadena p;
    int i = 0;
    
    while (texto[i] != '\0'){
        Cadena c = new _rep_cadena;
        c->car = texto[i];
        c->sig = NULL;
        if (cadena == NULL) {
            cadena = c;
            p = c;
        }
        else {
            p->sig = c;
            p = c;
        }
        i++;
    }
}


// retorna true si la cadena es vacia
bool esVaciaCadenaCaracteres (Cadena cadena){
    return cadena == NULL;
}

//retorna true si la cadena1 es igual a la cadena2, false en caso contrario
bool sonIgualesCadenas(Cadena cadena1, Cadena cadena2){
    while ((cadena1 != NULL) && (cadena2 != NULL)){  
        if (cadena1->car != cadena2->car)
            return false;
        cadena1 = cadena1->sig;
        cadena2 = cadena2->sig;
    }
    return cadena1 == NULL && cadena2 == NULL;      //solo da true si ambas cadenas dan null, es decir, si recorrimos ambas listas y son iguales
}

//retorna el dato char* de la tupla tup
//pre-cond retorna un puntero a un arreglo dinamico 
char* convertirCadenaArregloChar(Cadena cadena){ 
    int largo = 0;
    Cadena aux = cadena;
    while (aux != NULL){
        largo++;
        aux = aux->sig;
    }

    char* arreglo = new char[largo + 1];
    aux = cadena;

    int i = 0;
    while (aux != NULL){
        arreglo[i] = aux->car;
        aux = aux->sig;
        i++;
    }
    
    arreglo[i] = '\0';
    return arreglo;
}

//Pos-Cond: libera toda la memoria de cadena
void destruirCadena (Cadena &cadena){
    while (cadena != NULL){
        Cadena aux = cadena;
        cadena = cadena->sig;
        delete aux;
    }
}  
