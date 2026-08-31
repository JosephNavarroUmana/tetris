#include "Generador.h"
#include "Generador.h"
#include <cstdlib> 
#include <ctime>   
#include <iostream>
using namespace std;
//Generador::Generador() {
//	
//}

void encolar(NodoPieza* &cabeza, NodoPieza* &cola,Pieza nodo){
	
	NodoPieza* nuevo = new NodoPieza{nodo, nullptr};
	if(vacia(cabeza) == true){
		cabeza = nuevo;
		cola = nuevo;
		return;
	}
	
	cola->siguiente = nuevo;
	cola = nuevo;
	
}
	
void desencolar(NodoPieza* &cabeza, NodoPieza* &cola){
	
	if(vacia(cabeza) == true){
		return;//No se puede desencolar
	}
	
	NodoPieza* aux = cabeza;
	cabeza=cabeza->siguiente;
	
	if(cabeza == nullptr){
		cola = nullptr; 
	}
	
	delete aux;

}
	
void ver3piezas(NodoPieza* cabeza){
	
	if(vacia(cabeza) == true){
		cout<<"No se pueden ver las 3 fichas por que esta vacia"<<endl;
		return;
	}
	
	NodoPieza* aux = cabeza;
	cout<<aux->dato.tipo<<endl;
	if(aux->siguiente == nullptr){
		cout<<"No hay segunda pieza";
		return;
	}
	aux= aux->siguiente;
	cout<<aux->dato.tipo<<endl;
	if(aux->siguiente == nullptr){
		cout<<"No hay tercera pieza";
		return;
	}
	aux= aux->siguiente;
	cout<<aux->dato.tipo<<endl;
}
	
Pieza verFrente(NodoPieza* cabeza){
	return cabeza->dato;
}

bool vacia(NodoPieza* cabeza){
	
	if(cabeza == nullptr)return true;
	return false;
	
}
	
	
void llenar(NodoPieza* &cabeza, NodoPieza* &cola){
	
	char tipos[7] = {'I','O','T','S','Z','J','L'};
	

	for(int i = 6; i > 0; i--){
		int j = rand() % (i + 1);
		char temp = tipos[i];
		tipos[i] = tipos[j];
		tipos[j] = temp;
	}
	
	for(int i = 0; i < 7; i++){
		Pieza p;
		p.tipo = tipos[i];
		p.direccion = 0;
		p.ubicacion[0] = 3; 
		p.ubicacion[1] = 0; 
		
		encolar(cabeza, cola, p);
	}
}
