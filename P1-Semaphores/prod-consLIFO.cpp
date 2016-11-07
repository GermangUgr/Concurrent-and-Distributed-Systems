#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>

using namespace std ;

// ---------------------------------------------------------------------
// constantes 
const unsigned 
  num_items  = 40 ,
  tam_vector = 10 ;              
// variables
unsigned int sp = 0;
// vector tipo pila
	int vector_pila[tam_vector];
// ---------------------------------------------------------------------
// semanforos

sem_t	puede_consumir;
sem_t	puede_producir;
sem_t	mutex_pila;
sem_t	mutex_terminal;
//---------------------------------------------------------------------
unsigned producir_dato()
{
  static int contador = 0 ;
  cout << "producido: " << contador << endl << flush ;
  return contador++ ;
}
// ---------------------------------------------------------------------

void consumir_dato( int dato )
{
    cout << "dato recibido: " << dato << endl ;
}
// ---------------------------------------------------------------------

void * productor( void * )
{
  for( unsigned i = 0 ; i < num_items ; i++ ) { 
	sem_wait(&mutex_terminal); //bloqueamos el terminal
    int dato = producir_dato();
	sem_post(&mutex_terminal); //liberamos el terminal
	sem_wait(&puede_producir);
	sem_wait(&mutex_pila);
    vector_pila[sp] = dato;
	sp++;
	sem_post(&mutex_pila);
	sem_post(&puede_consumir);		 
  }
  return NULL ;
}
// ---------------------------------------------------------------------

void * consumidor( void * )
{  
  for( unsigned i = 0 ; i < num_items ; i++ ) {
  	   
	int dato;
	sem_wait(&puede_consumir);
	sem_wait(&mutex_pila); 
	sp--;
	dato = vector_pila[sp];
	sem_post(&mutex_pila);
    sem_wait(&mutex_terminal); //bloqueamos el terminal
    consumir_dato( dato ) ;
	sem_post(&mutex_terminal); //liberamos el terminal
	sem_post(&puede_producir);
  }
  return NULL ;
}
//----------------------------------------------------------------------

int main() {
	
	sem_init (&puede_producir,0,tam_vector);
	sem_init (&puede_consumir,0,0);
	sem_init (&mutex_pila,0,1);
	sem_init (&mutex_terminal,0,1);
   
	pthread_t hebra_productora, hebra_consumidora;
	
	pthread_create(&hebra_productora,NULL,productor,NULL);
	pthread_create(&hebra_consumidora,NULL,consumidor,NULL);

	pthread_join(hebra_productora,NULL);
	pthread_join(hebra_consumidora,NULL);

	sem_destroy(&puede_consumir);
	sem_destroy(&puede_producir);
	sem_destroy(&mutex_terminal);
	sem_destroy(&mutex_pila);

   return 0 ; 
}
