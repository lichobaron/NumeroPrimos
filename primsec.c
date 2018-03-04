//Librerias Utilizadas
#include  <stdio.h>
#include  <stdlib.h>
#include <string.h>
//Programa Principal
int  main(int  argc , char *argv []) {
  //Declaraci�n de varibles
  FILE *file;
  int N;
  //Comando de ayuda
  if(argc==2 && strcmp(argv[1],"-h")==0){
    printf("El programa realiza b�squeda de primos existentes en un rango de n�meros enteros comprendidos del 2 al N-1.\nModo de uso: L�mite_Superior_Primos archivo_salida \n" );
    exit(0);
  }
  //Control de errores en los argumentos de entrada.
  if(argc<3) {
    perror("Error");
    printf("Invocaci�n err�nea, faltan los argumentos. \n");
    exit(0);
  }
  if(argc>3) {
    perror("Error");
    printf("Invocaci�n err�nea, sobran argumentos. \n");
    exit(0);
  }
  N=atoi(argv[1]);
 	file = fopen (argv[2],"w");
  //Control de errores al abrir el archivo
  if(file==NULL){
    perror("Error");
    printf("No se pudo abrir el archivo.\n" );
  }
  //C�lculo de primos cuando N sea mayor oigual a 7.
  if(N>=7){
    fprintf(file,"%d\n%d\n%d\n",2,3,5);
    for(int i=7;i<=N-1;i=i+2){
      int cont=0;
      if(i%5!=0){
        for(int j=i;j>0;j=j-2){
          if(i%j==0){
            cont++;
          }
        }
      }
      if(cont==2){
        fprintf(file,"%d\n",i);
      }
    }
  }
  //C�lculo de primos cuando N sea menor a 7
  else{
    switch(N){
      case 2:
        fprintf(file,"%d\n",2);
        break;
      case 3:
        fprintf(file,"%d\n%d\n",2,3);
        break;
      case 4:
        fprintf(file,"%d\n%d\n",2,3);
        break;
      case 5:
        fprintf(file,"%d\n%d\n%d\n",2,3,5);
        break;
      case 6:
        fprintf(file,"%d\n%d\n%d\n",2,3,5);
        break;
    }
  }
  //Se cierra el archivo.
 	fclose (file);
  printf ("El  programa  busca  e  imprime  en  el  archivo %s los  primos que se encuentran entre 1 y %s \n", argv[2], argv[1]);
  return 0;
}
