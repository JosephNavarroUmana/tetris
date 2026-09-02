#include <cstdlib>
#include <ctime>
#include "raylib.h"
#include "Interfaz.h"
#include "Tablero.h"
#include "Generador.h"

int main(){
	
	srand(time(0));
	
	NodoFila* tablero = nullptr;
	crear(tablero);
	
	NodoPieza* genCabeza = nullptr;
	NodoPieza* genCola = nullptr;
	NodoHold* holdTope = nullptr;
	int puntaje = 0;
	llenar(genCabeza, genCola);
	
	Pieza piezaActual = verFrente(genCabeza);
	desencolar(genCabeza, genCola);
	
	bool juegoTerminado = false;
	
	iniciarVentana();
	
	float tiempoAcumulado = 0;
	float tiempoCaida = 0.5f;
	
	while(!WindowShouldClose()){
		
		if(!juegoTerminado){
			
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
					limpiarFilas(tablero);
					
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
		
		BeginDrawing();
		dibujarFondo();
		dibujarTablero(tablero);
		
		if(!juegoTerminado){
			dibujarPiezaActual(piezaActual);
		} else {
			dibujarGameOver();
		}
		
		dibujarSiguientes(genCabeza);
		dibujarPuntaje(puntaje);
		dibujarHold(holdTope);
		dibujarSiguientes(genCabeza);
		dibujarControles();
		
		EndDrawing();
	}
	
	cerrarVentana();
	
	return 0;
}
