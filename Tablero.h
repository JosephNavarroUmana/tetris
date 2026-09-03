#ifndef TABLERO_H
#define TABLERO_H
#include "Pieza.h"

struct NodoFila{
	NodoFila* siguiente;
	char vector[10];
	NodoFila(){
		siguiente = nullptr;
		for(int i = 0; i < 10; i++){
			vector[i] = '-';
		}
	}
};

void crear(NodoFila* &cabeza);
void insertarAlInicio(NodoFila* &cabeza);
bool llena(NodoFila* fila);
void imprimir(NodoFila* cabeza);
int indiceTipo(char tipo);
bool hayColision(NodoFila* cabeza, Pieza p);
void colocarPieza(NodoFila* cabeza, Pieza p);
int limpiarFilas(NodoFila* &cabeza);
bool hayFilaLlena(NodoFila* cabeza);

#endif
