#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "abb.h"
#include "pila.h"

typedef struct nodo{
 	char* clave;
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

/*Recibe un arbol, un nodo actual y el nodo a insertar y lo inserta en el arbol.
En caso de ya encontrarse un nodo con dicha clave, lo libera y guarda el dato en el
nodo antiguo.*/
void nodo_insertar(abb_t* abb,nodo_t* actual,nodo_t* nodo){
	if (!abb->raiz){
		abb->raiz=nodo;
		abb->cantidad++;
		return;
	}
	int relacion=abb->abb_comparar_clave(actual->clave,nodo->clave);
	if(relacion>0){
		if(!actual->der){
			actual->der=nodo;
			abb->cantidad++;
			return;
		}
		nodo_insertar(abb,actual->der,nodo);
		return;
	}
	if(relacion<0){
		if(!actual->izq){
			actual->izq=nodo;
			abb->cantidad++;
			return;
		}
		nodo_insertar(abb,actual->izq,nodo);
		return;
	}
	void* dato=nodo->valor;
	actual->valor=nodo->valor;
	if(abb->destruir_dato)
		abb->destruir_dato(dato);
	free(nodo->clave);
	free(nodo);
	return;
}

/*Recibe un abb y un nodo actual y destruye todos los nodos del arbol*/
void destruir_nodos(abb_t* abb,nodo_t* actual){
	nodo_t* nodo_der=actual->der;
	nodo_t* nodo_izq=actual->izq;
	if(abb->destruir_dato)
		abb->destruir_dato(actual->valor);
	free(actual->clave);
	free(actual);
	destruir_nodos(abb,nodo_der);
	destruir_nodos(abb,nodo_izq);
	return;
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

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	nodo_t* nodo_nuevo=malloc(sizeof(nodo_t));
	if(!nodo_nuevo)
		return false;
	nodo_nuevo->clave=strdup(clave);
	if(!nodo_nuevo->clave){
		free(nodo_nuevo);
		return false;
	}
	nodo_nuevo->valor=dato;
	nodo_insertar(arbol,arbol->raiz,nodo_nuevo);
	return true;
}

void *abb_borrar(abb_t *arbol, const char *clave){
	if (!abb_pertenece(arbol, clave)) return NULL;
	arbol->cantidad--;
	return nodo_borrar(&arbol->raiz, clave, arbol);
}

void *abb_obtener(const abb_t *arbol, const char *clave){
	nodo_t* nodo=encontrar_nodo(arbol->raiz,clave,arbol->abb_comparar_clave);
	return nodo ? nodo->valor : NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
  nodo_t* nodo = encontrar_nodo(arbol->raiz, clave, arbol->abb_comparar_clave);
	if (!nodo) return false;
	return true;
}

size_t abb_cantidad(abb_t *arbol){
  return arbol->cantidad;
}

void abb_destruir(abb_t *arbol){
	if(arbol->raiz)
		destruir_nodos(arbol,arbol->raiz);
	free(arbol);
	return;
}

/***********************************************************************
*                     ESTRUCTURA DE  ITER ABB                          *
***********************************************************************/
struct abb_iter{
	pila_t* pila;
};

/*********************************************************************
*                  Funciones de iteradores del ABB                   *
*********************************************************************/

/*Recibe una pila y un nodo. Apila recursivamente todos los hijos izquierdos de dicho
nodo.*/
bool apilar_hijos_izq(pila_t* pila, nodo_t* actual){
	if(!actual)
		return false;
	if(!pila_apilar(pila, actual))
		return false;
	if(actual->izq)
		return apilar_hijos_izq(pila,actual->izq);
	return true;
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	abb_iter_t* iter=malloc(sizeof(abb_iter_t));
	if(!iter)
		return NULL;
	iter->pila=pila_crear();
	if(!iter->pila){
		free(iter);
		return NULL;
	}
	if(!apilar_hijos_izq(iter->pila,arbol->raiz)){
		pila_destruir(iter->pila);
		free(iter);
		return NULL;
	}
	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	if(pila_esta_vacia(iter->pila))
		return false;
	nodo_t* nodo=pila_desapilar(iter->pila);
	pila_apilar(iter->pila,nodo->der);
	apilar_hijos_izq(iter->pila,nodo->der);//Que hacemos si apilar falla??
	return true;

}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	nodo_t* nodo=pila_ver_tope(iter->pila);
	return nodo ? nodo->clave : NULL;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
	pila_destruir(iter->pila);
	free(iter);
	return;
}

/***********************************************************************
*                     ESTRUCTURA DE  ITER INTERNO ABB                  *
***********************************************************************/
/*Dado un nodo iterar de forma inorder todo el arbol asociado a dicho nodo */
void iterar_nodo(nodo_t* nodo,bool visitar(const char*, void* , void*),void* extra){
	if(!nodo)
		return;
	iterar_nodo(nodo->izq,visitar,extra);
	if(!visitar(nodo->clave,nodo->valor,extra))
		return;
	iterar_nodo(nodo->der,visitar,extra);
	return;
}

/* Iterador interno in order.*/
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	if(!arbol->raiz)
		return;
	if(!visitar)
		return;
	iterar_nodo(arbol->raiz,visitar,extra);
	return;
}
