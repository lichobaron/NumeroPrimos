//Librerias Utilizadas
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
//Definici�n de varibles globales
#define true 1
#define false 0
typedef char bool;
//Firmas de las funciones utilizadas.
bool esPrimo(int n);
void insercion(int *vec, int cont);
void unir(char*nameFile,int nprocesos);

//Programa Principal.
int main(int argc, char *argv[]){
  //Varibles Locales
  int status,nprocesos,N,cuantos,rango;
  pid_t childpid;
  char str[12];
  //Comando de ayuda
  if(argc==2 && strcmp(argv[1],"-h")==0){
    printf("El programa calcula los primos usando procesos concurrentes.\nModo de uso: L�mite_Superior_Primos N�mero_Procesos archivo_salida \n" );
    exit(0);
  }
  //Control de Errores de los argumentos de entrada.
  if(argc<4) {
    perror("Error");
    printf("Invocaci�n err�nea, faltan los argumentos. \n");
    exit(0);
  }
  if(argc>4) {
    perror("Error");
    printf("Invocaci�n err�nea, sobran argumentos. \n");
    exit(0);
  }
  N=atoi(argv[1]);
  nprocesos = atoi(argv[2]);
  cuantos= N/nprocesos;
  //Control de errores cuando cantidad de procesos sea mayor a cantidad de primos.
  if(cuantos==0){
    perror("Error");
    printf("El n�mero de procesos es mayor al n�mero de primos a calcular.\n");
    exit(0);
  }
  //Revisa que los archivos de salida para cada proceso no existen para evitar sobreescribir.
  for(int i=0;i<nprocesos;i++){
    char buffer[80]={0};
    sprintf(buffer, "p%02d.txt", i);
    FILE*file2=fopen(buffer,"r");
    if(file2!=NULL){
      remove(buffer);
    }
  }
  //Creaci�n de Procesos
  for (int i = 0; i < nprocesos; ++i) {
    //Calculo de rango para cada hijo.
    int min=i*cuantos+1;
    int max=min+cuantos;
    //Control de errores cuando se creen hijos.
    if ((childpid = fork()) < 0) {
      perror("fork:");
      exit(1);
    }
    if(childpid==0){
      //C�digo primos para cada hijo
      while(min>0 && min<max){
        if(esPrimo(min)){
          char buffer[80]={0};
          sprintf(buffer, "p%02d.txt", i);//Crea el nombre del archivo para el proceso.
          FILE*file2=fopen(buffer,"a+");//Guarda el primo en el archivo respectivo.
          fprintf(file2,"%d\n",min);
          fclose(file2);
        }
        min++;
      }
      exit(0);
    }
  }
  //El padre espera que sus hijos terminen de ejecutarse.
  while ((childpid = wait(&status)) != -1);
  //Llama la funci�n para unir los archivos en uno solo.
  unir(argv[3],nprocesos);
  printf ("El programa busca e imprime en el archivo %s los primos que se encuentran entre 1 y %s. Debe dividir el dominio de b�squeda entre %s procesos.\n", argv[3], argv[1], argv[2]);
}
//Une los archivos que cada proceso creo, en un solo archivo de salida.
//Recibe por par�metro el nombre del archivo final y la cantidad de procesos creados.
void unir(char*nameFile,int nprocesos){
  FILE*filefin=fopen(nameFile,"w");
  int j=0;
  for(int i=0;i<nprocesos;i++){
    char buffer[80]={0};
    sprintf(buffer, "p%02d.txt", i);
    FILE*file2=fopen(buffer,"r");
    if(file2!=NULL){
      while(!feof(file2)){
        char cadena[256];
        while(fgets(cadena,256,file2)!=NULL){
          fprintf(filefin, "%s",cadena);
          j++;
        }
      }
    }
    remove(buffer);
  }
  //Se cierra el archivo.
  fclose(filefin);
  printf("%d\n",j );
}
//M�todo para comprobar que un n�mero sea primo.
//Recibe un Entero como par�metro.
bool esPrimo(int n){
  int i;
  if(n == 2)
  return true;
  if(n%2 == 0 || n==1 )
  return false;
  for(i=2; i<=n/2; i++){
    if(n%i == 0)
    return false;
  }
  return true;
}
