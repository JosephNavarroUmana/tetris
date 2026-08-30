#include "Eventos.h"

//Eventos::Eventos() {
//	
//}

#include <iostream>
using namespace std;

void insertar(NodoEventos* &cabeza, NodoEventos* &cola, float tiempo, char TE){
	
	NodoEventos* nuevo = new NodoEventos();
	nuevo->tiempo = tiempo;
	nuevo->siguiente = nullptr;
	nuevo->tipoEvento = TE;
	
	if(cabeza == nullptr){
		cabeza = nuevo;
		cola = nuevo;
		return;
	}
	
	NodoEventos* anterior = nullptr;
	NodoEventos* actual = cabeza;
	
	while(actual != nullptr && actual->tiempo < nuevo->tiempo){
		anterior = actual;
		actual = actual->siguiente;
	}
	
	if(anterior == nullptr){

		nuevo->siguiente = cabeza;
		cabeza = nuevo;
	} else {
		anterior->siguiente = nuevo;
		nuevo->siguiente = actual;
	}
	
	if(actual == nullptr){
		cola = nuevo;
	}
}
	
char sacarDelFrente(NodoEventos* &cabeza){
	
	if(cabeza==nullptr){
		cout<<"No se puede sacar un evento por que la lista de eventos esta vacia"<<endl;
		return 'E';
	}
	
	NodoEventos* aux = cabeza;
	char d = aux->tipoEvento;
	cabeza= aux->siguiente;
	delete aux;
	return d;
}
