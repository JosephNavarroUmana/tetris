//#ifndef GENERADOR_H
//#define GENERADOR_H
//
//class Generador {
//public:
//	Generador();
//private:
//};
//
//#endif
//
#ifndef GENERADOR_H
#define GENERADOR_H
#include "Pieza.h"

struct NodoPieza{
	Pieza dato;
	NodoPieza* siguiente;
};

struct Generador{
	NodoPieza* cabeza;
	NodoPieza* cola;
};

void encolar(NodoPieza* &cabeza, NodoPieza* &cola, Pieza nodo);
void desencolar(NodoPieza* &cabeza, NodoPieza* &cola);
void ver3piezas(NodoPieza* cabeza);
Pieza verFrente(NodoPieza* cabeza);
bool vacia(NodoPieza* cabeza);
void llenar(NodoPieza* &cabeza, NodoPieza* &cola);

#endif
