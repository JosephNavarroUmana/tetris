#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include "raylib.h"
#include "Interfaz.h"
#include "Tablero.h"
#include "Generador.h"
#include "Puntajes.h"
using namespace std;

int main(){
	
	srand(time(0));
	
	NodoPuntaje* tablaPuntajes = nullptr;
	cargarArchivo(tablaPuntajes, "puntajes.txt");
	
	NodoFila* tablero = nullptr;
	crear(tablero);
	
	NodoPieza* genCabeza = nullptr;
	NodoPieza* genCola = nullptr;
	NodoHold* holdTope = nullptr;
	int puntaje = 0;
	int ultimoUmbralMostrado = 0; // para saber cuando mostrar la imagen de logro otra vez
	llenar(genCabeza, genCola);
	
	Pieza piezaActual = verFrente(genCabeza);
	desencolar(genCabeza, genCola);
	
	bool juegoTerminado = false;
	bool puntajeGuardado = false;
	
	// ---- Estado de animacion de limpiar lineas ----
	bool animandoLimpieza = false;
	float tiempoAnimacion = 0;
	float duracionAnimacion = 0.35f;
	
	// ---- Estado de imagen de logro ----
	float tiempoImagenLogro = 0;
	float duracionImagenLogro = 2.0f;
	
	iniciarVentana();
	cargarImagenLogro();
	
	string nombreJugador = pantallaNombre();
	
	float tiempoAcumulado = 0;
	float tiempoCaida = 0.5f;
	
	while(!WindowShouldClose()){
		
		if(!juegoTerminado){
			
			if(animandoLimpieza){
				// mientras se anima, no se procesa input ni caida
				tiempoAnimacion += GetFrameTime();
				if(tiempoAnimacion >= duracionAnimacion){
					// termino la animacion: ahora si borramos de verdad
					int lineasLimpiadas = limpiarFilas(tablero);
					puntaje += lineasLimpiadas * 100;
					
					// chequeo de logro por puntaje (cada 1000 puntos)
					int umbralActual = (puntaje / 1000) * 1000;
					if(umbralActual > ultimoUmbralMostrado && umbralActual > 0){
						ultimoUmbralMostrado = umbralActual;
						tiempoImagenLogro = duracionImagenLogro;
					}
					
					if(vacia(genCabeza)){
						llenar(genCabeza, genCola);
					}
					
					piezaActual = verFrente(genCabeza);
					desencolar(genCabeza, genCola);
					
					if(hayColision(tablero, piezaActual)){
						juegoTerminado = true;
					}
					
					animandoLimpieza = false;
					tiempoAnimacion = 0;
				}
			} else {
				
				if(IsKeyPressed(KEY_LEFT))  moverPieza(piezaActual, tablero, -1, 0);
				if(IsKeyPressed(KEY_RIGHT)) moverPieza(piezaActual, tablero, 1, 0);
				if(IsKeyPressed(KEY_DOWN))  moverPieza(piezaActual, tablero, 0, 1);
				if(IsKeyPressed(KEY_X))     rotarPieza(piezaActual, tablero);
				if(IsKeyPressed(KEY_Z)){
					if(vacia(holdTope)){
						Pieza aGuardar = piezaActual;
						aGuardar.direccion = 0;
						aGuardar.ubicacion[0] = 3;
						aGuardar.ubicacion[1] = 0;
						push(holdTope, aGuardar);
						
						piezaActual = verFrente(genCabeza);
						desencolar(genCabeza, genCola);
					} else {
						Pieza intercambio = pop(holdTope);
						
						Pieza aGuardar = piezaActual;
						aGuardar.direccion = 0;
						aGuardar.ubicacion[0] = 3;
						aGuardar.ubicacion[1] = 0;
						push(holdTope, aGuardar);
						
						piezaActual = intercambio;
					}
				}
				
				tiempoAcumulado += GetFrameTime();
				if(tiempoAcumulado >= tiempoCaida){
					tiempoAcumulado = 0;
					
					Pieza copia = piezaActual;
					copia.ubicacion[1] += 1;
					
					if(!hayColision(tablero, copia)){
						piezaActual.ubicacion[1] += 1;
					} else {
						colocarPieza(tablero, piezaActual);
						
						if(hayFilaLlena(tablero)){
							animandoLimpieza = true;
							tiempoAnimacion = 0;
						} else {
							if(vacia(genCabeza)){
								llenar(genCabeza, genCola);
							}
							
							piezaActual = verFrente(genCabeza);
							desencolar(genCabeza, genCola);
							
							if(hayColision(tablero, piezaActual)){
								juegoTerminado = true;
							}
						}
					}
				}
			}
			
			if(tiempoImagenLogro > 0){
				tiempoImagenLogro -= GetFrameTime();
			}
			
		} else {
			if(!puntajeGuardado){
				agregarPuntaje(tablaPuntajes, nombreJugador, puntaje);
				ordenarQuicksort(tablaPuntajes);
				limitarTop10(tablaPuntajes);
				guardarArchivo(tablaPuntajes, "puntajes.txt");
				puntajeGuardado = true;
			}
		}
		
		BeginDrawing();
		dibujarFondo();
		dibujarTablero(tablero);
		
		if(animandoLimpieza){
			float progreso = tiempoAnimacion / duracionAnimacion;
			dibujarParpadeoLineas(tablero, progreso);
			dibujarFlashBorde(progreso);
		}
		
		if(!juegoTerminado){
			if(!animandoLimpieza){
				dibujarPiezaActual(piezaActual);
			}
		} else {
			bool quiereSalir = dibujarGameOver(tablaPuntajes, puntaje);
			if(quiereSalir){
				break;
			}
		}
		
		dibujarPuntaje(puntaje);
		dibujarHold(holdTope);
		dibujarSiguientes(genCabeza);
		dibujarControles();
		dibujarImagenLogro(tiempoImagenLogro);
		
		EndDrawing();
	}
	
	descargarImagenLogro();
	cerrarVentana();
	
	return 0;
}
