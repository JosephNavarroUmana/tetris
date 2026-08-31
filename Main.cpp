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
			if(IsKeyPressed(KEY_UP))    rotarPieza(piezaActual, tablero);
			
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
		ClearBackground(RAYWHITE);
		dibujarTablero(tablero);
		
		if(!juegoTerminado){
			dibujarPiezaActual(piezaActual);
		} else {
			DrawText("GAME OVER", 260, 280, 30, RED);
		}
		
		dibujarSiguientes(genCabeza);
		
		EndDrawing();
	}
	
	cerrarVentana();
	
	return 0;
}
