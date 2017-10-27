#include "pruebas_alumno.h"
#include "abb.h"
#include "testing.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void pruebas_abb_vacio(){
  abb_t* abb=abb_crear(strcmp,NULL);

  print_test("Prueba agregar cantidad abb vacio", abb_cantidad(abb)==0);
  print_test("Prueba obtener devuelve NULL", abb_obtener(abb,"asd")==NULL);
  print_test("Prueba borrar devuelve NULL", abb_borrar(abb,"asd")==NULL);

  abb_destruir(abb);
}

void pruebas_abb_agregar_borrar(){
  int n1=1; int n2=2; int n3=3; int n4=4; int n5=5;
  abb_t* abb=abb_crear(strcmp,NULL);


  print_test("Prueba agregar un elemento", abb_guardar(abb,"1",&n1));
  print_test("Prueba cantidad despues de agregar", abb_cantidad(abb)==1);
  print_test("Prueba obtener un elemento", abb_obtener(abb,"1")==&n1);
  print_test("Prueba pertenece", abb_pertenece(abb,"1"));
  print_test("Prueba borrar un elmento", abb_borrar(abb, "1")==&n1);
  print_test("Prueba cantidad despues de borrar", abb_cantidad(abb)==0);

  print_test("Prueba cadena vacia como clave", abb_guardar(abb,"",&n2));
  print_test("Prueba cantidad de elementos", abb_cantidad(abb)==1);
  print_test("Prueba obtener de cadena vacia", abb_obtener(abb, "")==&n2);
  print_test("Prueba pertenece 2 elementos", abb_pertenece(abb, ""));
  print_test("Prueba borrar cadena vacia", abb_borrar(abb, "")==&n2);

  abb_guardar(abb,"1",&n1);
  abb_guardar(abb,"2",&n2);
  abb_guardar(abb,"4",&n4);
  abb_guardar(abb,"3",&n3);
  abb_guardar(abb,"5",&n5);

  print_test("Prueba borrado 1", abb_borrar(abb,"1")==&n1);
  print_test("Prueba borrado 2", abb_borrar(abb,"2")==&n2);
  print_test("Prueba borrado 3", abb_borrar(abb,"4")==&n4);
  print_test("Prueba borrado 4", abb_borrar(abb,"3")==&n3);
  print_test("Prueba borrado 5", abb_borrar(abb,"5")==&n5);
  print_test("Prueba abb cantidad" , abb_cantidad(abb)==0);

  abb_destruir(abb);
}


void pruebas_abb_alumno(){
  pruebas_abb_agregar_borrar();
}
