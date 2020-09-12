//
//  main.c
//  Actividad 4
//
//  Created by Christian Alberto Dalma Schults and Antonio Junco de Hass on 10/09/20.
//  Copyright © 2020 Christian Alberto Dalma Schultz. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int media(int a, int b);
int maximoArchivos(int *vector, int n);
void imprimirHistograma(int *vector, int *vector2, int n);

/*
 *************** Start of my Methods **************
 */
int media(int a, int b){
  return((a+b)/2);
}

int maximoArchivos(int *vector,int n){
  int max=0;
  int *aux=vector;
  int *final=vector +n;

  for (aux=vector; aux < final; ++aux) {
      if(max<*aux){
        max=*aux;
      }
    }
  return max;
}

void imprimirHistograma(int *vector, int *vector2, int n){
  int max=maximoArchivos(vector, n);
  double asteriscos=40.0;


  int * final = vector + n;
  int *aux2=vector2;


  printf("%8s %8s %10s\n","PID hijo","Promedio","Histograma");
  for (int *aux = vector; aux < final; ++aux) {

    printf("%8d: %8d ",*aux2,*aux);
    for (int i=0; i<(*aux*asteriscos)/max;++i){
      printf("*");
    }
    printf("\n");

    aux2++;
  }
}
/*
 ******************** End of Methods **********************
 */

int main(int argc, char * argv[]) {

  char *cvalue = NULL;
  int c;

  int n=0;

  while ((c = getopt (argc, argv, "n:")) != -1)
        switch (c)
    {
        case 'n':
            cvalue = optarg;
            break;

        case '?':
            if (optopt == 'n')
                fprintf (stderr, "Opción -%c requiere un argumento.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Opción desconocida '-%c'.\n", optopt);
            else
                fprintf (stderr,
                         "Opción desconocida '\\x%x'.\n",
                         optopt);
            return 1;
        default:
            abort ();
    }


  n=atoi(cvalue);

  int * vector = (int *) malloc(n * sizeof(int));
  int * vector2 = (int *) malloc(n * sizeof(int));
    
    /*
     *Start of the Loop
     */
    
    int * final = vector + n;


    int *aux2=vector2;
    for (int *aux = vector; aux < final; ++aux){
      pid_t pid;
      int estado;


      int pipe_1[2];
      int pipe_2[2];
      pipe(pipe_1);
      pipe(pipe_2);
      pid = fork();

        
      if (pid == -1)
      {
          printf("ERROR no se pudo crear el proceso hijo \n");
          return -1;
      }
      else if (pid == 0)
      {
            int Media=media(getpid(),getppid());
            printf("Soy el proceso hijo con PID = %d y mi promedio es = %d\n",getpid(),Media);

            close (pipe_1[0]);
            write(pipe_1[1],&Media,sizeof(int));

            int id=getpid();
            close (pipe_2[0]);
            write(pipe_2[1],&id,sizeof(int));

          exit(0);
      }
      else {
          if (waitpid(pid, &estado, 0) != -1)
          {
            int a =WEXITSTATUS(estado);
              if (WIFEXITED(estado)){

                close(pipe_1[1]);
                int Media_Exit=0;
                read(pipe_1[0],&Media_Exit,sizeof(int));
                //printf("Promedio segun padre: %d\n",Media_Exit);
                *aux=Media_Exit;
                close(pipe_2[1]);
                read (pipe_2[0],aux2,sizeof(int));
              }
          }
      }
    aux2++;
    }

    aux2=vector2;

    imprimirHistograma(vector, vector2,n);
    free(vector);
    free(vector2);
    return 0;
}








