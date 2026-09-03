//#ifndef INTERFAZ_H
//#define INTERFAZ_H
//#include "raylib.h"
//#include "Tablero.h"
//#include "Generador.h"
//#include "Hold.h"
//
//const int TAM_CELDA = 30;
//const int MARGEN = 40;
//
//const int BOARD_ANCHO = 10 * TAM_CELDA; // 300
//const int BOARD_ALTO  = 20 * TAM_CELDA; // 600
//
//const int PANEL_IZQ_ANCHO = 180; // Aumentado
//const int PANEL_DER_ANCHO = 200; // Aumentado
//
//const int VENTANA_ANCHO = MARGEN + PANEL_IZQ_ANCHO + MARGEN + BOARD_ANCHO + MARGEN + PANEL_DER_ANCHO + MARGEN;
//const int VENTANA_ALTO  = 80 + BOARD_ALTO + 140;
//
//const int BOARD_X = MARGEN + PANEL_IZQ_ANCHO + MARGEN;
//const int BOARD_Y = 80;
//
//const int PANEL_IZQ_X = MARGEN;
//const int PANEL_DER_X = BOARD_X + BOARD_ANCHO + MARGEN;
//
//// Declaración de funciones
//void iniciarVentana();
//void cerrarVentana();
//Color colorDePieza(char tipo);
//void dibujarFondo();
//void dibujarTablero(NodoFila* cabeza);
//void dibujarPiezaActual(Pieza p);
//void rotarPieza(Pieza &p, NodoFila* tablero);
//void moverPieza(Pieza &p, NodoFila* tablero, int deltaCol, int deltaFila);
//void dibujarSiguientes(NodoPieza* cabeza);
//void dibujarGameOver();
//void dibujarHold(NodoHold* tope);
//void dibujarPuntaje(int puntaje);
//void dibujarPanelMenu();
//void dibujarControles();
//
//#endif
//

#ifndef INTERFAZ_H
#define INTERFAZ_H
#include "raylib.h"
#include "Tablero.h"
#include "Generador.h"
#include "Hold.h"
#include "Puntajes.h"
#include <string>
using namespace std;

const int TAM_CELDA = 30;
const int MARGEN = 40;
const int BOARD_ANCHO = 10 * TAM_CELDA; // 300
const int BOARD_ALTO  = 20 * TAM_CELDA; // 600
const int PANEL_IZQ_ANCHO = 180;
const int PANEL_DER_ANCHO = 200;
const int VENTANA_ANCHO = MARGEN + PANEL_IZQ_ANCHO + MARGEN + BOARD_ANCHO + MARGEN + PANEL_DER_ANCHO + MARGEN;
const int VENTANA_ALTO  = 80 + BOARD_ALTO + 140;
const int BOARD_X = MARGEN + PANEL_IZQ_ANCHO + MARGEN;
const int BOARD_Y = 80;
const int PANEL_IZQ_X = MARGEN;
const int PANEL_DER_X = BOARD_X + BOARD_ANCHO + MARGEN;

void iniciarVentana();
void cerrarVentana();
Color colorDePieza(char tipo);
void dibujarTablero(NodoFila* cabeza);
void dibujarPiezaActual(Pieza p);
void dibujarSiguientes(NodoPieza* cabeza);
void moverPieza(Pieza &p, NodoFila* tablero, int deltaCol, int deltaFila);
void rotarPieza(Pieza &p, NodoFila* tablero);
void dibujarHold(NodoHold* tope);
void dibujarPuntaje(int puntaje);
void dibujarControles();
void dibujarFondo();
bool dibujarGameOver(NodoPuntaje* tablaPuntajes, int puntajeFinal);
string pantallaNombre();

void dibujarParpadeoLineas(NodoFila* cabeza, float progreso);
void dibujarFlashBorde(float progreso);
void cargarImagenLogro(); // llamar una sola vez al inicio
void descargarImagenLogro(); // llamar una sola vez al final
void dibujarImagenLogro(float tiempoRestante);
#endif
