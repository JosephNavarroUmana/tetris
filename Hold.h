#ifndef HOLD_H
#define HOLD_H
#include "Pieza.h"

//class Hold {
//public:
//	Hold();
//private:
//};

struct NodoHold{
	Pieza dato;
};

void push(NodoHold* &tope, Pieza pieza);
Pieza pop(NodoHold* &tope);
bool vacia(NodoHold* tope);

#endif

