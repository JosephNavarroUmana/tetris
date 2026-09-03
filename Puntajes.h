//#ifndef PUNTAJES_H
//#define PUNTAJES_H
//
//class Puntajes {
//public:
//	Puntajes();
//private:
//};
//
//#endif

#ifndef PUNTAJES_H
#define PUNTAJES_H
#include <string>
using namespace std;

struct NodoPuntaje{
	string nombre;
	int puntaje;
	NodoPuntaje* siguiente;
};

void agregarPuntaje(NodoPuntaje* &cabeza, string nombre, int puntaje);
void ordenarQuicksort(NodoPuntaje* &cabeza);
void imprimirTabla(NodoPuntaje* cabeza);
void guardarArchivo(NodoPuntaje* cabeza, string ruta);
void cargarArchivo(NodoPuntaje* &cabeza, string ruta);
void limitarTop10(NodoPuntaje* &cabeza);
void dibujarTopJugadores(NodoPuntaje* cabeza);
#endif
