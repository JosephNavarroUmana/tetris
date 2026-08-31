#include "interfaz.h"

//interfaz::interfaz() {
//	
//}

#include "raylib.h"
#include "Interfaz.h"
#include "Formas.h"

void iniciarVentana(){
	InitWindow(800, 600, "Tetris");
	SetTargetFPS(60);
}
	
void cerrarVentana(){
	CloseWindow();
}

Color colorDePieza(char tipo){
	switch(tipo){
	case 'I': return SKYBLUE;
	case 'O': return YELLOW;
	case 'T': return PURPLE;
	case 'S': return GREEN;
	case 'Z': return RED;
	case 'J': return BLUE;
	case 'L': return ORANGE;
	}
	return GRAY; // por si acaso
}
	
void dibujarTablero(NodoFila* cabeza){
	NodoFila* filaActual = cabeza;
		int fila = 0;
		
		while(filaActual != nullptr){
			for(int col = 0; col < 10; col++){
				int x = col * TAM_CELDA;
				int y = fila * TAM_CELDA;
				
			if(filaActual->vector[col] == '-'){
				DrawRectangleLines(x, y, TAM_CELDA, TAM_CELDA, LIGHTGRAY); // celda vacia, solo borde
			} else {
				Color c = colorDePieza(filaActual->vector[col]);
				DrawRectangle(x, y, TAM_CELDA, TAM_CELDA, c);
				DrawRectangleLines(x, y, TAM_CELDA, TAM_CELDA, BLACK);
			}
		}
	filaActual = filaActual->siguiente;
	fila++;
	}
}

void dibujarPiezaActual(Pieza p){
	int idx = indiceTipo(p.tipo);
	Color c = colorDePieza(p.tipo);
	
	for(int i = 0; i < 4; i++){
		int col = p.ubicacion[0] + FORMAS[idx][p.direccion][i][0];
		int fil = p.ubicacion[1] + FORMAS[idx][p.direccion][i][1];
		
		int x = col * TAM_CELDA;
		int y = fil * TAM_CELDA;
		
		DrawRectangle(x, y, TAM_CELDA, TAM_CELDA, c);
		DrawRectangleLines(x, y, TAM_CELDA, TAM_CELDA, BLACK);
	}
}
	
void moverPieza(Pieza &p, NodoFila* tablero, int deltaCol, int deltaFila){
		Pieza copia = p;
		copia.ubicacion[0] += deltaCol;
		copia.ubicacion[1] += deltaFila;
		
		if(!hayColision(tablero, copia)){
			p = copia; // el movimiento es valido, se aplica de verdad
		}
	}

void rotarPieza(Pieza &p, NodoFila* tablero){
		Pieza copia = p;
		copia.direccion = (copia.direccion + 1) % 4;
		
	if(!hayColision(tablero, copia)){
		p = copia;
	}
}
void dibujarSiguientes(NodoPieza* cabeza){
	
	int panelX = 320; // a la derecha del tablero (10*30=300 + margen)
	int panelY = 20;
	
	DrawText("Siguientes:", panelX, panelY, 18, BLACK);
	
	NodoPieza* aux = cabeza;
	int contador = 0;
	int offsetY = panelY + 30;
	
	while(aux != nullptr && contador < 3){
		Color c = colorDePieza(aux->dato.tipo);
		DrawRectangle(panelX, offsetY, 25, 25, c);
		DrawRectangleLines(panelX, offsetY, 25, 25, BLACK);
		DrawText(TextFormat("%c", aux->dato.tipo), panelX + 35, offsetY + 4, 18, BLACK);
		
		offsetY += 35;
		aux = aux->siguiente;
		contador++;
	}
}
