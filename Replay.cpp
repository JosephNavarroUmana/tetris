#include "Replay.h"
#include <cstddef>

void iniciarReplay(Replay& replay) {
	replay.cabeza = NULL;
	replay.cola = NULL;
	replay.actual = NULL;
	replay.cursor = NULL;
}

void registrarEstado(Replay& replay, const EstadoJuego& estado) {

	if (replay.actual != NULL && replay.actual->siguiente != NULL) {
		NodoReplay* aBorrar = replay.actual->siguiente;
		while (aBorrar != NULL) {
			NodoReplay* siguienteABorrar = aBorrar->siguiente; 
			delete aBorrar;
			aBorrar = siguienteABorrar;
		}
		replay.actual->siguiente = NULL;
		replay.cola = replay.actual;
	}
	
	NodoReplay* nuevo = new NodoReplay;
	nuevo->estado = estado;
	nuevo->siguiente = NULL;
	nuevo->anterior = replay.cola;
	
	if (replay.cabeza == NULL) {
		replay.cabeza = nuevo;
	} else {
		replay.cola->siguiente = nuevo;
	}
	
	replay.cola = nuevo;
	replay.actual = nuevo;
}

bool deshacer(Replay& replay, EstadoJuego& resultado) {
	if (replay.actual == NULL || replay.actual->anterior == NULL) {
		return false; 
	}
	replay.actual = replay.actual->anterior;
	resultado = replay.actual->estado;
	return true;
}

bool rehacer(Replay& replay, EstadoJuego& resultado) {
	if (replay.actual == NULL || replay.actual->siguiente == NULL) {
		return false; 
	}
	replay.actual = replay.actual->siguiente;
	resultado = replay.actual->estado;
	return true;
}

void iniciarReproduccion(Replay& replay) {
	replay.cursor = replay.cabeza;
}

bool avanzarReproduccion(Replay& replay, EstadoJuego& resultado) {
	if (replay.cursor == NULL || replay.cursor->siguiente == NULL) {
		return false;
	}
	replay.cursor = replay.cursor->siguiente;
	resultado = replay.cursor->estado;
	return true;
}

bool retrocederReproduccion(Replay& replay, EstadoJuego& resultado) {
	if (replay.cursor == NULL || replay.cursor->anterior == NULL) {
		return false;
	}
	replay.cursor = replay.cursor->anterior;
	resultado = replay.cursor->estado;
	return true;
}

bool reproduccionTerminada(const Replay& replay) {
	return replay.cursor == NULL || replay.cursor->siguiente == NULL;
}

void liberarReplay(Replay& replay) {
	NodoReplay* nodo = replay.cabeza;
	while (nodo != NULL) {
		NodoReplay* siguiente = nodo->siguiente; 
		delete nodo;
		nodo = siguiente;
	}
	replay.cabeza = NULL;
	replay.cola = NULL;
	replay.actual = NULL;
	replay.cursor = NULL;
}

void capturarEstado(NodoFila* cabeza, Pieza piezaActual, Pieza piezaHold,
					bool haySpuestaHold, int puntaje, int nivel,
					EstadoJuego& destino) {
	NodoFila* aux = cabeza;
	int f = 0;
	while(aux != nullptr && f < FILAS_TABLERO){
		for(int c = 0; c < COLUMNAS_TABLERO; c++){
			destino.filas[f][c] = aux->vector[c];
		}
		aux = aux->siguiente;
		f++;
	}
	destino.piezaActual = piezaActual;
	destino.piezaHold = piezaHold;
	destino.haySpuestaHold = haySpuestaHold;
	destino.puntaje = puntaje;
	destino.nivel = nivel;
}
					
void restaurarTablero(const EstadoJuego& estado, NodoFila* &cabeza) {
	NodoFila* aux = cabeza;
	int f = 0;
	while(aux != nullptr && f < FILAS_TABLERO){
		for(int c = 0; c < COLUMNAS_TABLERO; c++){
			aux->vector[c] = estado.filas[f][c];
		}
		aux = aux->siguiente;
		f++;
	}
}
