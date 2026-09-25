#include "Tablero.h"
#include "Formas.h"
#include <iostream>
using namespace std;

void crear(NodoFila* &cabeza){
	for(int i = 0; i <= 19; i++){
		insertarAlInicio(cabeza);
	}
}

void insertarAlInicio(NodoFila* &cabeza){
	NodoFila* nuevo = new NodoFila();
	if(cabeza == nullptr){
		cabeza = nuevo;
		return;
	}
	nuevo->siguiente = cabeza;
	cabeza = nuevo;
}

bool llena(NodoFila* fila){
	for(int i = 0; i < 10; i++){
		if(fila->vector[i] == '-'){
			return false;
		}
	}
	return true;
}

void imprimir(NodoFila* cabeza){
	NodoFila* aux = cabeza;
	while(aux != nullptr){
		for(int i = 0; i < 10; i++){
			cout << aux->vector[i];
		}
		cout << endl;
		aux = aux->siguiente;
	}
}

int indiceTipo(char tipo){
	switch(tipo){
	case 'I': return 0;
	case 'O': return 1;
	case 'T': return 2;
	case 'S': return 3;
	case 'Z': return 4;
	case 'J': return 5;
	case 'L': return 6;
	}
return -1;
}

NodoFila* obtenerFila(NodoFila* cabeza, int fila){
	NodoFila* aux = cabeza;
	int contador = 0;
	while(aux != nullptr && contador < fila){
		aux = aux->siguiente;
		contador++;
	}
	return aux;
}

bool hayColision(NodoFila* cabeza, Pieza p){
	int idx = indiceTipo(p.tipo);
	for(int i = 0; i < 4; i++){
		int col = p.ubicacion[0] + FORMAS[idx][p.direccion][i][0];
		int fil = p.ubicacion[1] + FORMAS[idx][p.direccion][i][1];
		
		if(col < 0 || col >= 10 || fil >= 20){
			return true;
		}
		
		NodoFila* filaNodo = obtenerFila(cabeza, fil);
		if(filaNodo != nullptr && filaNodo->vector[col] != '-'){
			return true;
		}
	}
	return false;
}

void colocarPieza(NodoFila* cabeza, Pieza p){
	int idx = indiceTipo(p.tipo);
	for(int i = 0; i < 4; i++){
		int col = p.ubicacion[0] + FORMAS[idx][p.direccion][i][0];
		int fil = p.ubicacion[1] + FORMAS[idx][p.direccion][i][1];
		
		NodoFila* filaNodo = obtenerFila(cabeza, fil);
		if(filaNodo != nullptr){
			filaNodo->vector[col] = p.tipo;
		}
	}
}

int limpiarFilas(NodoFila* &cabeza){
	NodoFila* aux = cabeza;
	NodoFila* anterior = nullptr;
	int contadorLimpiadas = 0;
	
	while(aux != nullptr){
		NodoFila* siguienteGuardado = aux->siguiente;
		
		if(llena(aux)){
			if(anterior == nullptr){
				cabeza = aux->siguiente;
			} else {
				anterior->siguiente = aux->siguiente;
			}
			delete aux;
			
			insertarAlInicio(cabeza);
			contadorLimpiadas++;
			
			aux = siguienteGuardado;
		} else {
			anterior = aux;
			aux = aux->siguiente;
		}
	}
	
	return contadorLimpiadas;
}
	
	bool hayFilaLlena(NodoFila* cabeza){
		NodoFila* aux = cabeza;
		while(aux != nullptr){
			if(llena(aux)) return true;
			aux = aux->siguiente;
		}
		return false;
}
