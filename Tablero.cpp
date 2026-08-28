#include "Tablero.h"

//Tablero::Tablero() {
//	
//}
//

void crear(NodoFila* &cabeza){
	
	for(int i=0;i<=19;i++){
		insertarAlInicio(cabeza);
	}
	
}

void insertarAlInicio(NodoFila* &cabeza){
	
	NodoFila* nuevo = new NodoFila();
	
	if(cabeza==nullptr){
		cabeza = nuevo;
		return;
	}
	
	nuevo->siguiente = cabeza;
	cabeza = nuevo;
	
}
