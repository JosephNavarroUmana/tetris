#include "Eventos.h"
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

	if(cabeza == nullptr){
		cout << "No se puede sacar un evento por que la lista de eventos esta vacia" << endl;
		return 'E';
	}
	
	NodoEventos* aux = cabeza;
	char d = aux->tipoEvento;
	cabeza = aux->siguiente;
	delete aux;
	return d;
}

bool vaciaEventos(NodoEventos* cabeza){
	return cabeza == nullptr;
}

char elegirEventoRandom(){
	char opciones[2] = {EVENTO_INVERTIR, EVENTO_BLOQUEAR_ROTACION};
	int i = rand() % 2;
	return opciones[i];
}

void barajarEventos(char bolsa[3]){
	bolsa[0] = EVENTO_INVERTIR;
	bolsa[1] = EVENTO_BLOQUEAR_ROTACION;
	bolsa[2] = EVENTO_MOVIMIENTO_DOBLE;
	
	for(int i = 2; i > 0; i--){
		int j = rand() % (i + 1);
		char temp = bolsa[i];
		bolsa[i] = bolsa[j];
		bolsa[j] = temp;
	}
}

char siguienteEvento(char bolsa[3], int &indiceEvento){
	char resultado = bolsa[indiceEvento];
	indiceEvento++;
	
	if(indiceEvento >= 3){
		indiceEvento = 0;
		barajarEventos(bolsa);
	}
	
	return resultado;
}
