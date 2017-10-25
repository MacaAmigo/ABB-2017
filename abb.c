#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "abb.h"
#include "pila.h"

typedef struct nodo{
	const char* clave;
	void* valor;
	struct nodo *izq;
  	struct nodo *der;
}nodo_t;

struct abb{
	nodo_t* raiz;
	abb_comparar_clave_t abb_comparar_clave;
	abb_destruir_dato_t destruir_dato;
	size_t cantidad;
};

/*******************************************************************************
*                           FUNCIONES AUXILIARES DE ABB                        *
*******************************************************************************/
// Función recursiva que recorre un arbol en busca de un nodo y lo devuelve.
// En caso de error, devuelve NULL.
// Pre: Recibe un nodo desde donde se comienza la búsqueda. Una clave y una
// función de comparación.
// Pos: Devuelve el nodo encontrado o NULL en caso contrario.
nodo_t* encontrar_nodo(nodo_t *nodo, const char *clave,  abb_comparar_clave_t abb_comparar_clave){
	if (!nodo) return NULL;
	if (abb_comparar_clave(nodo->clave, clave) == 0) return nodo;

  	else if (abb_comparar_clave(nodo->clave, clave) < 0){
    	if (nodo->der != NULL) return encontrar_nodo(nodo->der, clave, abb_comparar_clave);
	}
  	else if (abb_comparar_clave(nodo->clave, clave) > 0){
    	if (nodo->izq != NULL) return encontrar_nodo(nodo->izq, clave, abb_comparar_clave);
  	}
  	return NULL;
}
// Función recursiva que recorre devuelve el valor del elemento a borrar.
// En caso de error, devuelve NULL.
// Pre: El abb fue creado. Recibe puntero a nodo desde donde empezar a buscar
// lugar y recibe la clave.

void* nodo_borrar(nodo_t **nodo, const char *clave,  abb_t* abb){
	if (abb->abb_comparar_clave((*nodo)->clave, clave) < 0){
		return nodo_borrar(&(*nodo)->der, clave, abb);
	 }
	else if (abb->abb_comparar_clave((*nodo)->clave, clave) > 0){
		return nodo_borrar(&(*nodo)->izq, clave, abb);
	 }
	else if (abb->abb_comparar_clave((*nodo)->clave, clave) == 0)	{
		nodo_t* aux = *nodo;
		void* valor = aux->valor;
		free((char*)(*nodo)->clave);
		if (!(*nodo)->izq)
			*nodo = (*nodo)->der;
		else if (!(*nodo)->der)
			*nodo = (*nodo)->izq;
		else
			reemplazar_nodo(&(*nodo)->izq, &aux);

		free(aux);
		return valor;
	}
	return NULL;
}


/******************************************************************************
*                          Primitivas del ABB                                  *
******************************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	if (!cmp) return NULL;

	abb_t* abb = malloc(sizeof(abb_t));
	if (!abb) return NULL;

	abb->destruir_dato = destruir_dato;
	abb->abb_comparar_clave = cmp;
	abb->raiz = NULL;
	abb->cantidad = 0;
	return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato);


void *abb_borrar(abb_t *arbol, const char *clave){
	if (!abb_pertenece(arbol, clave)) return NULL;
	arbol->cantidad--;
	return nodo_borrar(&arbol->raiz, clave, arbol);
}

void *abb_obtener(const abb_t *arbol, const char *clave);

bool abb_pertenece(const abb_t *arbol, const char *clave){
  nodo_t* nodo = encontrar_nodo(arbol->raiz, clave, arbol->abb_comparar_clave);
	if (!nodo) return false;
	return true;
}

size_t abb_cantidad(abb_t *arbol){
  return arbol->cantidad;
}

void abb_destruir(abb_t *arbol);

/***********************************************************************
*                     ESTRUCTURA DE  ITER ABB                          *
***********************************************************************/
struct abb_iter{
	pila_t* pila;
};

/*********************************************************************
*                  Funciones de iteradores del ABB                   *
*********************************************************************/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

bool abb_iter_in_avanzar(abb_iter_t *iter);

const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

bool abb_iter_in_al_final(const abb_iter_t *iter);

void abb_iter_in_destruir(abb_iter_t* iter);

/***********************************************************************
*                     ESTRUCTURA DE  ITER INTERNO ABB                  *
***********************************************************************/


/* Iterador interno in order.*/
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);
