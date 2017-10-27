//Alumno: Manuel del Carril
//Numero de Padron: 100772
//Corrector: Joaquin Torre

#include "pila.h"
#include <stdlib.h>
#define CAPACIDAD_INICIAL 10
#define REDIMENSION 2
#define ESPACIO_MIN_OCUPADO 4

struct pila {
    void** datos;
    size_t cantidad;
    size_t capacidad;
};

/*Recibe una pila  y redimensiona la capacidad de datos de acuerdo al tamanio
pasado por parametro. Devuelve falso en caso de error.*/
bool redimensionar(pila_t* pila,size_t tamanio){
  void** datos_nuevo=realloc(pila->datos, tamanio*(sizeof(void*)));
  if(tamanio>0 && !datos_nuevo)
    return false;
  pila->datos=datos_nuevo;
  pila->capacidad=tamanio;
  return true;

}

pila_t* pila_crear(void){
  pila_t* pila=malloc(sizeof(pila_t));
  if (pila==NULL)
    return NULL;
  pila->datos=malloc(sizeof(void*)*CAPACIDAD_INICIAL);
  if (pila->datos==NULL){
    free(pila);
    return NULL;
  }
  pila->capacidad=CAPACIDAD_INICIAL;
  pila->cantidad=0;
  return pila;
}

void pila_destruir(pila_t* pila){
  free(pila->datos);
  free(pila);
  return;
}

bool pila_esta_vacia(const pila_t* pila){
  return pila->cantidad==0;
}

bool pila_apilar(pila_t* pila, void* valor){
  if (pila->capacidad==pila->cantidad){
    if (!redimensionar(pila,pila->capacidad*REDIMENSION))
      return false;
  }
  pila->datos[pila->cantidad]=valor;
  pila->cantidad +=1;
  return true;
}

void* pila_ver_tope(const pila_t* pila){
  if ((pila->cantidad)==0)
    return NULL;
  return (pila->datos)[pila->cantidad-1];
}

void* pila_desapilar(pila_t* pila){
  if((pila->cantidad)==0)
    return NULL;
  void* elemento=(pila->datos)[pila->cantidad-1];
  pila->cantidad--;
  if (pila->cantidad>CAPACIDAD_INICIAL && pila->cantidad==pila->capacidad/ESPACIO_MIN_OCUPADO)
    redimensionar(pila,pila->capacidad/REDIMENSION);
  return elemento;
}
