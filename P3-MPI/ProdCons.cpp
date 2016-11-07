#include <iostream>
#include <cmath>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include "mpi.h"


#define Buffer 5
#define ITERS 20

//orden para compilar mpicxx -g -Wall ProdCons.cpp -o ProdCons

using namespace std;

void productor(int rank) {
	for(int i=0; i<4; i++) {
		cout << "El productor " << rank << " produce el valor " << i << endl << flush;
		MPI_Ssend(&i, 1, MPI_INT, Buffer, 0, MPI_COMM_WORLD);
		cout << "Numero iteraciones productor: " << i << endl << flush;
	}
}

void consumidor (int rank) {
	int value, peticion = 1;
	float raiz;
	MPI_Status status;
	for (int i=0; i<5; i++) {
		cout << "Consumidor " << rank << " a la espera de valor" << endl << flush;
		MPI_Ssend(&peticion, 1, MPI_INT, Buffer, 1, MPI_COMM_WORLD);
		MPI_Recv(&value, 1, MPI_INT, Buffer, 1, MPI_COMM_WORLD, &status);
		cout << "Consumidor " << rank << " recibe valor " << value << endl << flush;
		raiz = sqrt(value);
		cout << "Numero iteraciones consumidor: " << i << endl << flush;
	}
}

void buffer() {
	int value[5];
	int peticion = 1, pos = 0, rama, num_prod, num_cons;
	MPI_Status status;

	for (int i=0; i<40; i++) {
		if(pos == 0) {
			cout << "Buffer vacio" <<  endl << flush;
			rama = 0;	//no se puede consumir, hay que producir
			MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			num_prod = status.MPI_SOURCE;
		}
		else if(pos == 5) {
			cout << "Buffer lleno" << endl << flush;
			rama = 1;	//no se puede producir, hay que consumir
			MPI_Probe(MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
			num_cons = status.MPI_SOURCE;
		}
		else {			//se puede producir o consumir
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			if(status.MPI_SOURCE <=4 ) {
				rama = 0;
				num_prod = status.MPI_SOURCE;
			}
			else {
				rama = 1;
				num_cons = status.MPI_SOURCE;
			}
			
		}

		switch(rama) {
			case 0:
				MPI_Recv(&value[pos], 1, MPI_INT, num_prod, 0, MPI_COMM_WORLD, &status);
				cout << "Buffer recibe " << value[pos] << " de productor" << endl << flush;
				pos++;
				break;

			case 1:
				MPI_Recv(&peticion, 1, MPI_INT, num_cons, 1, MPI_COMM_WORLD, &status);
				MPI_Ssend(&value[pos-1],1,MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
				cout << "Buffer envia " << value[pos-1] << " a consumidor" << endl << flush;
				pos--;
				break;
		}

		cout << "Numero iteraciones buffer" << i << endl << flush;
	}
}

int main (int argc, char *argv[]) {
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(size != 10) {
		if(rank == 0)
			cout << "Uso: mpirun -np 10 " << argv[0] << endl;
		MPI_Finalize();
		return 0;
	} else {
		if(rank < Buffer) 
			productor(rank);
		else if(rank == Buffer) 
			buffer();
		else
			consumidor(rank);
	}

	MPI_Finalize();

	return 0;
}
