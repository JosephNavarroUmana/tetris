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
		
		// Recorre la lista hasta llegar al nodo de la fila numero "fila" (0 = arriba)
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
						return true; // se sale del tablero
					}
					
					NodoFila* filaNodo = obtenerFila(cabeza, fil);
					if(filaNodo != nullptr && filaNodo->vector[col] != '-'){
						return true; // ya hay algo ocupando esa celda
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
					
					void limpiarFilas(NodoFila* &cabeza){
						NodoFila* aux = cabeza;
						NodoFila* anterior = nullptr;
						
						while(aux != nullptr){
							NodoFila* siguienteGuardado = aux->siguiente;
							
							if(llena(aux)){
								// desconectar el nodo lleno de la lista
								if(anterior == nullptr){
									cabeza = aux->siguiente;
								} else {
									anterior->siguiente = aux->siguiente;
								}
								delete aux;
								
								// insertar una fila vacia nueva arriba de todo
								insertarAlInicio(cabeza);
								
								aux = siguienteGuardado; // seguimos desde donde iba
							} else {
								anterior = aux;
								aux = aux->siguiente;
							}
						}
					}
