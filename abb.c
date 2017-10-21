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

void *abb_borrar(abb_t *arbol, const char *clave);

void *abb_obtener(const abb_t *arbol, const char *clave);

bool abb_pertenece(const abb_t *arbol, const char *clave);

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
