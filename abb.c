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

/*Recibe un nodo padre, el nodo actual y el nodo nuevo. Reemplaza la posicion
del nodo actual con el nodo nuevo.
  Pre: la clave del nodo nuevo es mayor que la del nodo actual si la clave del nodo
actual es mayor a la del padre (analogamente para el caso en que la clave del nodo
nuevo es menor que la del padre)*/
/*void reemplazar_hijo(abb_t* abb,nodo_t* padre, nodo_t* nodo_actual, nodo_t* nodo_nuevo){
  if(!padre){
    abb->raiz=nodo_nuevo;
    if(nodo_actual->izq)
      nodo_nuevo->izq=nodo_actual->izq;
  }
  else if(abb->abb_comparar_clave(padre->clave,nodo_actual->clave)>0){
    padre->izq=nodo_nuevo;
  }
  else{
    padre->der=nodo_nuevo;
  }
  return;
}*/

/*Recibe un nodo y devuelve el minimo del arbol asociado a ese nodo*/
/*nodo_t* nodo_minimo(nodo_t* nodo){
  nodo_t* actual=nodo;
  while(actual->izq){
    actual=nodo->izq;
  }
  return actual;
}*/
void reemplazar_nodo(nodo_t **nodo, nodo_t **aux){
  if (!(*nodo)->der){
		(*aux)->clave = (*nodo)->clave;
		(*aux)->valor = (*nodo)->valor;
		*aux = *nodo;
		*nodo = (*nodo)->izq;
  }
  else
  	reemplazar_nodo(&(*nodo)->der, aux);
}
// Función recursiva que recorre devuelve el dato a borrar.
// Pre: El abb fue creado. Recibe puntero a nodo desde donde empezar a buscar,
// Recibe la clave y el abb.

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

/*Recibe un abb, un nodo padre, nodo y una clave. Borra el nodo asociado a esa clave
y devuelve el valor. En caso de no encontrarse el nodo devuelve NULL.*/
/*void* nodo_borrar(abb_t* abb,nodo_t* padre,nodo_t* nodo, const char* clave){
  if(!nodo)    return NULL;
  if(abb->abb_comparar_clave(clave, nodo->clave)<0){
    return nodo_borrar(abb,nodo,nodo->der,clave);
  }
  if(abb->abb_comparar_clave(clave, nodo->clave)>0){
    return nodo_borrar(abb,nodo,nodo->izq,clave);
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

	void* valor = nodo->valor;
  	if(!nodo->der && !nodo->izq ){
    	reemplazar_hijo(abb,padre,nodo,NULL);
  	}
  else if(!nodo->der){
    reemplazar_hijo(abb,padre,nodo,nodo->izq);
  }
  else if(!nodo->izq){
    reemplazar_hijo(abb,padre,nodo,nodo->der);
  }
  else{
    nodo_t* nodo_nuevo=nodo_minimo(nodo->der);
    reemplazar_hijo(abb,padre,nodo,nodo_nuevo);
  }
  free(nodo->clave);
  free(nodo);
  abb->cantidad--;
  return dato;
}
*/// Crea un nodo con el valor y la clave recibida. Y referencias a nodo izquierdo
// y derecho inicializados en NULL.
// Devuelve NULL en caso de error.
// Pre: recibe una clave y un valor.
// Post: devuelve un nuevo nodo que contiene clave y valor.
nodo_t* nodo_crear(void* valor, const char * clave){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (!nodo) return NULL;

	nodo->valor = valor;
	nodo->clave = strdup(clave);
  	nodo->izq = NULL;
  	nodo->der = NULL;
	return nodo;
}

bool nodo_guardar(nodo_t **nodo, const char *clave, void *dato, abb_t* abb){
	if (!*nodo){
		*nodo = nodo_crear(dato, clave);
		abb->cantidad++;
		return true;
	}
	//Caso en el que hay que reemplazar valor
	else if (abb->abb_comparar_clave((*nodo)->clave, clave)== 0){
		if (abb->destruir_dato) abb->destruir_dato((*nodo)->valor);  //Destruyo el valor.
		(*nodo)->valor = dato; //Actualizo el dato;
		return true;
	}
  	else if (abb->abb_comparar_clave((*nodo)->clave, clave) < 0){
		return nodo_guardar(&(*nodo)->der, clave, dato, abb);
  	}
  	else if (abb->abb_comparar_clave((*nodo)->clave, clave) > 0){
		return nodo_guardar(&(*nodo)->izq, clave, dato, abb);
	}
	return false;
}


/*Recibe un abb y un nodo actual y destruye todos los nodos del arbol*/
void destruir_nodos(abb_t* abb, nodo_t** nodo){
	if (!*nodo) return;
	if ((*nodo)->izq) destruir_nodos(abb, &(*nodo)->izq);
	if ((*nodo)->der) destruir_nodos(abb, &(*nodo)->der);

	if (abb->destruir_dato) abb->destruir_dato((*nodo)->valor);
	free((char*)(*nodo)->clave);
	free(*nodo);
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
	return nodo_guardar(&arbol->raiz,clave,dato, arbol);
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
	destruir_nodos(arbol,arbol->raiz);
	free(arbol);
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
void apilar_hijos_izq(pila_t* pila, nodo_t* nodo){
	if (!nodo) return;
 	pila_apilar(pila,nodo);
	apilar_hijos_izq(pila,nodo->izq);
}
abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if ((!iter) || (!arbol)) return NULL;
	iter->pila = pila_crear();
	if(!iter->pila){
		free(iter);
		return NULL;
	}
  	apilar_hijos_izq(iter->pila,arbol->raiz);
	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	if (pila_esta_vacia(iter->pila))   return false;
	nodo_t* nodo = pila_desapilar(iter->pila);
	if (nodo->der) apilar_hijos_izq(iter->pila,nodo->der);
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
