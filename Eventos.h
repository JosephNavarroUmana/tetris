#ifndef EVENTOS_H
#define EVENTOS_H

const char EVENTO_INVERTIR = 'I';
const char EVENTO_BLOQUEAR_ROTACION = 'B';
const char EVENTO_MOVIMIENTO_DOBLE = 'P'; 

struct NodoEventos{
	float tiempo;
	char tipoEvento;
	NodoEventos* siguiente;
};

void insertar(NodoEventos* &cabeza, NodoEventos* &cola, float tiempo, char TE);
char sacarDelFrente(NodoEventos* &cabeza);
bool vaciaEventos(NodoEventos* cabeza);
char elegirEventoRandom();
void barajarEventos(char bolsa[3]);
char siguienteEvento(char bolsa[3], int &indiceEvento);

#endif
