#ifndef INTERFAZ_H
#define INTERFAZ_H
#include "raylib.h"
#include "Tablero.h"
#include "Generador.h"
#include "Hold.h"

const int TAM_CELDA = 30; // tamaño en pixeles de cada celda del tablero

void iniciarVentana();
void cerrarVentana();
Color colorDePieza(char tipo);
void dibujarTablero(NodoFila* cabeza);
void dibujarPiezaActual(Pieza p);
void rotarPieza(Pieza &p, NodoFila* tablero);
void moverPieza(Pieza &p, NodoFila* tablero, int deltaCol, int deltaFila);
void dibujarSiguientes(NodoPieza* cabeza);
#endif
