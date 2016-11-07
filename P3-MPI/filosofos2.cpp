#include <iostream>
#include <cmath>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include "mpi.h"

#define soltar 0
#define coger 1
#define levantarse 2
#define sentarse 3

#define camarero 10

using namespace std;

void filosofo(int rank) {
	int izquierda = (rank + 1)%10;
	int derecha = (rank + 11 -2)%10;

	while (true) {

		//El filosofo pide sentarse
		cout << "El filosofo " << rank << " pide sentarse " << endl << flush;
		MPI_Ssend(NULL, 0, MPI_INT, camarero, sentarse, MPI_COMM_WORLD);
		//cout << "El filosofo " << rank << " se sienta " << endl << flush;

		//El filosofo coge los tenedores
		cout << "El filosofo " << rank << " coge el tenedor de su izquierda " << izquierda << endl << flush;
		MPI_Ssend(NULL, 0, MPI_INT, izquierda, coger, MPI_COMM_WORLD);
		cout << "El filosofo " << rank << " coge el tenedor de su derecha " << derecha << endl << flush;
		MPI_Ssend(NULL, 0, MPI_INT, derecha, coger, MPI_COMM_WORLD);

		//El filosofo come
		cout << "EL filosofo " << rank << " va a comer" << endl << flush;
		sleep((rand()%3))+1;

		//El filosofo suelta los tenedores
		cout << "El filosofo " << rank << " suelta el tenedor de su izquierda " << izquierda << endl << flush;
		MPI_Ssend(NULL, 0, MPI_INT, izquierda, soltar, MPI_COMM_WORLD);
		cout << "El filosofo " << rank << " suelta el tenedor de su derecha " << izquierda << endl << flush;
		MPI_Ssend(NULL, 0, MPI_INT, derecha, soltar, MPI_COMM_WORLD);

		//El filosofo se levanta
		MPI_Ssend(NULL,0,MPI_INT,camarero,levantarse,MPI_COMM_WORLD);


		//El filosofo piensa
		cout << "EL filosofo " << rank << " va a pensar" << endl << flush;
		sleep((rand()%3))+1;

	}
}

void tenedor (int rank) {
	int aux, numFilo;
	MPI_Status status;

	while(true) {
		MPI_Recv(&aux,1,MPI_INT, MPI_ANY_SOURCE, coger, MPI_COMM_WORLD,&status);
		numFilo = status.MPI_SOURCE;
		cout << "El Tenedor " << rank << " recibe peticion de " << numFilo << endl << flush;

		MPI_Recv(&aux,1,MPI_INT, numFilo, soltar, MPI_COMM_WORLD, &status);
		cout << "El Tenedor " << rank << " es liberado por el filosofo " << numFilo << endl << flush;

	}
}

void Camarero(int rank) {
	int aux, numSentados = 0;
	MPI_Status status;

	while (true) {
		if(numSentados < 4) 
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		else
			MPI_Probe(MPI_ANY_SOURCE,levantarse,MPI_COMM_WORLD,&status);

		if(status.MPI_TAG == sentarse) {
			aux = status.MPI_SOURCE;
			MPI_Recv(NULL,0,MPI_INT,aux,sentarse,MPI_COMM_WORLD,&status);
			numSentados++;

			cout << "El filosofo " << aux << " se sienta hay " << numSentados << " filosofos sentados " << endl << flush;
		} else {
			aux = status.MPI_SOURCE;
			MPI_Recv(NULL,0,MPI_INT,aux,levantarse, MPI_COMM_WORLD, &status);
			numSentados--;
			cout << "El filosofo " << aux << " se levanta, hay " << numSentados << " filosofos sentados " << endl << flush;
		}
	}
}

int main (int argc, char *argv[]) {
	int rank, size;
	srand(time(0));
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(size != 11) {
		if(rank == 0)
			cout << "Uso: mpirun -np 11 ./" << argv[0] << endl << flush;
		MPI_Finalize();
		return 0;
	} else {
		if(rank == 10)
			Camarero(rank);
		else if(rank % 2 == 0)
			filosofo(rank);
		else
			tenedor(rank);
	}

	MPI_Finalize();
	return 0;
}




















