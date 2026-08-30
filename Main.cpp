#include <iostream>
#include "Generador.h"
#include "Hold.h"
#include "Eventos.h"
#include "Tablero.h"
#include "raylib.h"

using namespace std;

int main(){
	
	iniciarVentana();
	
	while(!WindowShouldClose()){
		// input, actualizar estado (mover pieza, revisar colisión, etc.)
		
		BeginDrawing();
		ClearBackground(RAYWHITE);
		dibujarTablero(tablero);
		dibujarPiezaActual(piezaActual);
		dibujarSiguientes(genCabeza);
		dibujarHold(holdTope);
		dibujarPuntaje(puntaje);
		EndDrawing();
	}
	
	cerrarVentana();
	
	return 0;
}
