#ifndef PUNTAJES_H
#define PUNTAJES_H
#include <string>
using namespace std;

enum TipoOrdenamiento { ORDEN_QUICKSORT, ORDEN_INSERCION };

struct NodoPuntaje{
	string nombre;
	int puntaje;
	NodoPuntaje* siguiente;
};

void agregarPuntaje(NodoPuntaje* &cabeza, string nombre, int puntaje);
void ordenarQuicksort(NodoPuntaje* &cabeza);
void ordenarInsercion(NodoPuntaje* &cabeza);
void ordenarPuntajes(NodoPuntaje* &cabeza, TipoOrdenamiento tipo);
void imprimirTabla(NodoPuntaje* cabeza);
void guardarArchivo(NodoPuntaje* cabeza, string ruta);
void cargarArchivo(NodoPuntaje* &cabeza, string ruta);
void dibujarTopJugadores(NodoPuntaje* cabeza);
double medirTiempoOrdenamiento(NodoPuntaje* cabeza, TipoOrdenamiento tipo);
#endif
