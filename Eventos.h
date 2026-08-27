#ifndef EVENTOS_H
#define EVENTOS_H

//class Eventos {
//public:
//	Eventos();
//private:
//};

struct NodoEventos{
	float tiempo;
	char tipoEvento;
	NodoEventos* siguiente;
};

//struct ColaEventos{
//	NodoEventos cabeza;
//	NodoEventos cola;
//	
//};

void insertar(NodoEventos* &cabeza, NodoEventos* &cola, float tiempo, char TE);
char sacarDelFrente(NodoEventos* &cabeza);
#endif

