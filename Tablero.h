#ifndef TABLERO_H
#define TABLERO_H

//class Tablero {
//public:
//	Tablero();
//private:
//};

struct NodoFila{
	NodoFila* siguiente;
	char vector[10];
	NodoFila(){
		siguiente = nullptr;
		for(int i = 0; i < 10; i++){
			vector[i] = '-';
		}
	}
};

void llena();
void imprimir();
void crear(NodoFila* &cabeza);
void insertarAlInicio(NodoFila* &cabeza);

#endif

