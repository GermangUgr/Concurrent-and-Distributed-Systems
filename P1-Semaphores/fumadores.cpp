#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>      // incluye "time(....)"
#include <unistd.h>    // incluye "usleep(...)"
#include <stdlib.h>    // incluye "rand(...)" y "srand"

using namespace std;

const int NUMF = 3;

//Declaramos los semaforos que vamos a usar
sem_t fumadores[NUMF];
sem_t estanquero;
sem_t mutex;

// ----------------------------------------------------------------------------
// función que simula la acción de fumar  como un retardo aleatorio de la hebra
void fumar()
{
   //  inicializa la semilla aleatoria  (solo la primera vez)
   static bool primera_vez = true ;
   if ( primera_vez )
   {   primera_vez = false ;
      srand( time(NULL) );
   }

   // calcular un numero aleatorio de milisegundos (entre 1/10 y 2 segundos)
   const unsigned miliseg = 100U + (rand() % 1900U) ;

   // retraso bloqueado durante 'miliseg' milisegundos
   usleep( 1000U*miliseg );
}

void* Estanco (void *) {
    unsigned int ingrediente;

	while (true) {

    	sem_wait(&estanquero);
    	ingrediente = rand() % 3;

    	sem_wait(&mutex);
    	cout << "El estanquero pone en el mostrador el ingrediente " << ingrediente << "\n";
    	sem_post(&mutex);

    	sem_post(&fumadores[ingrediente]);

    }

    return NULL;
}

void* Fumadores(void* ingrediente_f) {

    while (true) {

    	sem_wait(&fumadores[(int)ingrediente_f]);

    	sem_wait(&mutex);
    	cout << "El fumador consume el ingrediente: " << (int)ingrediente_f << "\n";
    	sem_post(&mutex);

    	sem_post(&estanquero);
    	fumar();

    }
    return NULL;
}

int main()
{
  pthread_t hebras_f[NUMF];
  pthread_t estanquero_h;

//Incializamos los semanforos
  for (int i=0; i<NUMF; i++)
    sem_init(&fumadores[i],0,0);

  sem_init(&estanquero,0,1);
  sem_init(&mutex,0,1);

//Inicializamos las hebras
  for (unsigned int i = 0; i<NUMF; i++)
    pthread_create(&(hebras_f[i]),NULL,Fumadores,(void*)i);

  pthread_create(&estanquero_h,NULL,Estanco,NULL);

//Esperamos que terminen todas las hebras
  for (int i=0; i<NUMF; i++)
    pthread_join(hebras_f[i],NULL);

  pthread_join(estanquero_h,NULL);

//Destruimos los semaforos
  for (int i=0; i<NUMF; i++)
    sem_destroy(&fumadores[i]);

  sem_destroy(&estanquero);
  sem_destroy(&mutex);


  return 0 ;
}
