#include "pruebas_alumno.h"
#include <string.h>
#include "testing.h"
#include <stdio.h>

int main(){
  printf("Pruebas alumno\n");
  printf("%d\n",strcmp("1",""));
  pruebas_abb_alumno();
  return failure_count()>0;
}
