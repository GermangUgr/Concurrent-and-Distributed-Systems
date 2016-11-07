#include <iostream>
#include <cmath>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include "mpi.h"

#define soltar 0
#define coger 1

using namespace std;

void filosofo(int rank) {
	int izquierda = (rank + 1)%10;
	int derecha = (rank + 10 -1)%10;

	while (true) {
		if(rank == 0) {
			cout << "El filosofo " << rank << " coge el tenedor de su derecha " << derecha << endl << flush;
			MPI_Ssend(NULL, 0, MPI_INT, derecha, coger, MPI_COMM_WORLD);
			cout << "El filosofo " << rank << " coge el tenedor de su izquierda " << izquierda << endl << flush;
			MPI_Ssend(NULL, 0, MPI_INT, izquierda, coger, MPI_COMM_WORLD);
		} else {
			cout << "El filosofo " << rank << " coge el tenedor de su izquierda " << izquierda << endl << flush;
			MPI_Ssend(NULL, 0, MPI_INT, izquierda, coger, MPI_COMM_WORLD);
			cout << "El filosofo " << rank << " coge el tenedor de su derecha " << derecha << endl << flush;
			MPI_Ssend(NULL, 0, MPI_INT, derecha, coger, MPI_COMM_WORLD);
		}

		cout << "EL filosofo " << rank << " va a comer" << endl << flush;
		sleep((rand()%3))+1;

		cout << "El filosofo " << rank << " suelta el tenedor de su izquierda " << izquierda << endl << flush;
		MPI_Ssend(NULL, 0, MPI_INT, izquierda, soltar, MPI_COMM_WORLD);
		cout << "El filosofo " << rank << " suelta el tenedor de su izquierda " << izquierda << endl << flush;
		MPI_Ssend(NULL, 0, MPI_INT, derecha, soltar, MPI_COMM_WORLD);

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
		cout << "El Tenedor " << rank << " es liberador por el filosofo " << numFilo << endl << flush;

	}
}

int main (int argc, char *argv[]) {
	int rank, size;
	srand(time(0));
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(size != 10) {
		if(rank == 0)
			cout << "Uso: mpirun -np 10 ./" << argv[0] << endl << flush;
		MPI_Finalize();
		return 0;
	} else {
		if(rank % 2 == 0)
			filosofo(rank);
		else
			tenedor(rank);
	}

	MPI_Finalize();
	return 0;
}




















