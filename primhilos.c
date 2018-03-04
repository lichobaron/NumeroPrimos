#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <unistd.h>
#include  <pthread.h>

#define true 1
#define false 0
typedef char bool;

int *r; //Arreglo en el que todos los hilos guardan sus resultados

//Estructura para enviar parametros a cada hilo
struct parametros{
  int start;
  int end;
};
//Funcion que evalua si un numero n es primo
//Recibe un entero n
bool isprime(int n){
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

//Funcion que evalua los numeros primos dentro de un rango
//Recibe una estructura parametros en la que se encuentra el rango
void prim_aux(struct parametros *p){
  int pos=p->start/2;
  int start=p->start;
  int end=p->end;
  if(start%2==0){
    if(start==2){
      //fprintf(file,"%d\n",2);
      //printf("%d\n", 2);
      r[pos]=2;
      pos++;
    }
    start=start+1;
  }
  for(int i=start;i<=end;i=i+2){
    int cont=0;
    if(i%5!=0||i==5){
      if(isprime(i)){
        //fprintf(file,"%d\n",i);
        r[pos]=i;
        pos++;
      }
    }
  }
}

//Funcion a correr por cada hilo
//Recibe un apuntador a los parametros
void *prim(void *arg){
  struct  parametros  *p;
  p = (struct  parametros  *) arg;
  prim_aux(p);
  return NULL;
}

//Funcion principal
int  main(int  argc , char *argv []) {
  int N,h;
  if(argc==2 && strcmp(argv[1],"-h")==0){
    printf("El programa calcula los primos usando hilos.\nModo de uso: N numero_hilos archivo_salida\n" );
    exit(0);
  }

  if(argc<4) {
    perror("Error");
    printf("Invocacion erronea, faltan argumentos.\n");
    return -1;
  }
  if(argc>4) {
    perror("Error");
    printf("Invocacion erronea, sobran argumentos.\n");
    exit(-1);
  }
  else{
    N=atoi(argv[1]);//Numero hasta el que se calculan los primos
    h=atoi(argv[2]);//Numero de hilos a utilizar
  }

  int range=N/h;//rango en el que calculara cada hilo
  if(range==0){
    perror("Error");
    printf("El numero de hilos es mayor al numero de primos a calcular\n");
    exit(-1);
  }
  char str[12];
  int start=2, end=range;
  pthread_t  hi[h];//arreglo de hilos a crear
  struct parametros p[h];//creacion de parametros para los hilos
  r =(int *) calloc(N/2, sizeof(int));//Asignacion del tamaño de memoria de r

  for(int i=0;i<h;i++){
    p[i].start=start;
    p[i].end=end;
    if(pthread_create(&hi[i], NULL , prim, (void *) &p[i])!=0){//creacion de cada hilo
      perror("Error");
      printf("No se pudo crear el hilo\n");
      exit(-1);
    }
    start=end+1;
    if(i==h-2){
      end=N-1;
    }
    else{
      end=end+range;
    }
  }

  for(int i=0;i<h;i++){
    if(pthread_join(hi[i], NULL)!=0){//el padre espera que cada hilo acabe
      perror("Error");
      printf("No se pudo esperar por el hilo\n");
      exit(-1);
    }
  }

  FILE *file;
  file = fopen (argv[3],"w");//se abre el archivo de los resultados en modo de escritura
  if(file==NULL){
    perror("Error");
    printf("No se pudo abrir el archivo.\n");
    exit(-1);
  }

  for(int j=0;j<N/2;j++){
    if(r[j]!=0){
      fprintf(file,"%d\n",r[j]);//se escribe cada resultado en el archivo
    }
  }
  if(fclose (file)!=0){//se cierra el archivo
    perror("Error");
    printf("No se pudo cerrar el archivo.\n");
    exit(-1);
  }
  printf ("El programa busca e imprime en el archivo %s los primos que se encuentran entre 1 y %s. Debe dividir el dominio de búsqueda entre %s hilos.\n", argv[3], argv[1], argv[2]);
  return 0;
}
