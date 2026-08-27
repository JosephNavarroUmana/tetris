#include "Hold.h"

//Hold::Hold() {
//	
//}

using namespace std;
void push(NodoHold* &tope, Pieza* pieza){
	if(vacia(tope)){
		NodoHold* nuevo = NodoHold{pieza};
		tope = nuevo;
	}
	
}

Pieza pop(NodoHold* &tope){
	if(vacia(tope)){
		cout<<"No se puede popear el dato de la pila hold por que esta vacia"<<endl;
		Pieza vacio;
		return vacio;
	}
	Pieza aux = tope->dato;
	NodoHold* aux2 = tope;
	tope = nullptr;
	delete aux2;
	return aux;
}

void vacia(NodoHold* tope){
	if(tope== nullptr)return true;
	return false;
}
