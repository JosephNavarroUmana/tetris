#ifndef REPLAY_H
#define REPLAY_H
#include "Pieza.h"
#include "Tablero.h"

const int FILAS_TABLERO = 20;
const int COLUMNAS_TABLERO = 10;

struct EstadoJuego {
	char filas[FILAS_TABLERO][COLUMNAS_TABLERO];
	Pieza piezaActual;
	Pieza piezaHold;
	bool haySpuestaHold;
	int puntaje;
	int nivel;
};

struct NodoReplay {
	EstadoJuego estado;
	NodoReplay* anterior;
	NodoReplay* siguiente;
};


struct Replay {
	NodoReplay* cabeza;  
	NodoReplay* cola;    
	NodoReplay* actual; 
	NodoReplay* cursor;   
};

void iniciarReplay(Replay& replay);
void registrarEstado(Replay& replay, const EstadoJuego& estado);
bool deshacer(Replay& replay, EstadoJuego& resultado);
bool rehacer(Replay& replay, EstadoJuego& resultado);
void iniciarReproduccion(Replay& replay); 
bool avanzarReproduccion(Replay& replay, EstadoJuego& resultado);
bool retrocederReproduccion(Replay& replay, EstadoJuego& resultado);
bool reproduccionTerminada(const Replay& replay); 
void liberarReplay(Replay& replay);
void capturarEstado(NodoFila* cabeza, Pieza piezaActual, Pieza piezaHold,bool haySpuestaHold, int puntaje, int nivel,EstadoJuego& destino);
void restaurarTablero(const EstadoJuego& estado, NodoFila* &cabeza);
#endif
