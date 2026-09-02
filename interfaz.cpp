#include "raylib.h"
#include "interfaz.h"
#include "Formas.h"

// Colores de fondo - Más cálidos y acogedores
static const Color BG_TOP    = {252, 240, 245, 255};
static const Color BG_BOTTOM = {235, 220, 250, 255};
static const Color PANEL_BG  = {250, 245, 248, 255};
static const Color SHADOW    = {180, 150, 170, 255};
static const Color TEXT_PLUM = {90, 60, 100, 255};

// Colores de borde y rejilla
static const Color GRID_LINE   = {210, 180, 200, 255}; 
static const Color PANEL_BORDE = {200, 160, 185, 255};

// Colores de piezas - ¡Más vibrantes y claros!
static const Color COLOR_I = {100, 200, 255, 255};
static const Color COLOR_O = {255, 215, 0, 255};
static const Color COLOR_T = {180, 130, 255, 255};
static const Color COLOR_S = {80, 230, 180, 255};
static const Color COLOR_Z = {255, 100, 130, 255};
static const Color COLOR_J = {80, 140, 255, 255};
static const Color COLOR_L = {255, 170, 80, 255};

// Mapeo de tipos a índices para nombres
static const char* NOMBRES_PIEZAS[] = {"I", "O", "T", "S", "Z", "J", "L"};

static void dibujarCelda(int x, int y, Color color, bool vacia) {
	Rectangle rec = { (float)x + 2, (float)y + 2, (float)TAM_CELDA - 4, (float)TAM_CELDA - 4 };
	
	if (vacia) {
		DrawRectangleRounded(rec, 0.4f, 8, Fade(WHITE, 0.3f));
		DrawRectangleLinesEx(rec, 1.5f, Fade(GRID_LINE, 0.5f));
	} else {
		DrawRectangleRounded(rec, 0.3f, 8, color);
		DrawRectangleLinesEx(rec, 1.5f, Fade(BLACK, 0.1f));
	}
}

void iniciarVentana() {
	InitWindow(VENTANA_ANCHO, VENTANA_ALTO, "?? Tetris Cozy");
	SetTargetFPS(60);
}

void cerrarVentana() {
	CloseWindow();
}

Color colorDePieza(char tipo) {
	switch(tipo) {
	case 'I': return COLOR_I;
	case 'O': return COLOR_O;
	case 'T': return COLOR_T;
	case 'S': return COLOR_S;
	case 'Z': return COLOR_Z;
	case 'J': return COLOR_J;
	case 'L': return COLOR_L;
	}
	return LIGHTGRAY;
}

void dibujarTablero(NodoFila* cabeza) {
	Rectangle panel = { (float)(BOARD_X - 12), (float)(BOARD_Y - 12),
												(float)(BOARD_ANCHO + 24), (float)(BOARD_ALTO + 24) };
	
	Rectangle sombraPanel = { panel.x + 5, panel.y + 5, panel.width, panel.height };
	DrawRectangleRounded(sombraPanel, 0.08f, 10, Fade(SHADOW, 0.35f));
	DrawRectangleRounded(panel, 0.08f, 10, PANEL_BG);
	DrawRectangleLinesEx(panel, 2.5f, PANEL_BORDE);
	
	Rectangle fondoInterno = { (float)BOARD_X, (float)BOARD_Y, 
		(float)BOARD_ANCHO, (float)BOARD_ALTO };
	DrawRectangleRounded(fondoInterno, 0.02f, 4, Fade(WHITE, 0.15f));
	
	NodoFila* filaActual = cabeza;
	int fila = 0;
	while(filaActual != nullptr) {
		for(int col = 0; col < 10; col++) {
			int x = BOARD_X + col * TAM_CELDA;
			int y = BOARD_Y + fila * TAM_CELDA;
			
			if(filaActual->vector[col] == '-') {
				dibujarCelda(x, y, WHITE, true);
			} else {
				dibujarCelda(x, y, colorDePieza(filaActual->vector[col]), false);
			}
		}
		filaActual = filaActual->siguiente;
		fila++;
	}
}

void dibujarPiezaActual(Pieza p) {
	int idx = indiceTipo(p.tipo);
	Color c = colorDePieza(p.tipo);
	
	for(int i = 0; i < 4; i++) {
		int col = p.ubicacion[0] + FORMAS[idx][p.direccion][i][0];
		int fil = p.ubicacion[1] + FORMAS[idx][p.direccion][i][1];
		
		int x = BOARD_X + col * TAM_CELDA;
		int y = BOARD_Y + fil * TAM_CELDA;
		
		dibujarCelda(x, y, c, false);
	}
}

void moverPieza(Pieza &p, NodoFila* tablero, int deltaCol, int deltaFila) {
	Pieza copia = p;
	copia.ubicacion[0] += deltaCol;
	copia.ubicacion[1] += deltaFila;
	
	if(!hayColision(tablero, copia)) {
		p = copia;
	}
}

void rotarPieza(Pieza &p, NodoFila* tablero) {
	Pieza copia = p;
	copia.direccion = (copia.direccion + 1) % 4;
	
	if(!hayColision(tablero, copia)) {
		p = copia;
	}
}

void dibujarSiguientes(NodoPieza* cabeza) {
	int panelX = PANEL_DER_X + 10;
	int panelY = BOARD_Y + 200;
	
	// Panel de siguientes - más grande
	Rectangle panel = { (float)panelX - 15, (float)panelY - 20, 
		(float)PANEL_DER_ANCHO - 20, 200 };
	Rectangle sombra = { panel.x + 4, panel.y + 4, panel.width, panel.height };
	DrawRectangleRounded(sombra, 0.05f, 8, Fade(SHADOW, 0.25f));
	DrawRectangleRounded(panel, 0.05f, 8, Fade(WHITE, 0.4f));
	DrawRectangleLinesEx(panel, 1.5f, Fade(GRID_LINE, 0.6f));
	
	DrawText("? Siguientes", panelX + 10, panelY - 10, 18, TEXT_PLUM);
	
	NodoPieza* aux = cabeza;
	int contador = 0;
	int offsetY = panelY + 30;
	
	while(aux != nullptr && contador < 3) {
		char tipo = aux->dato.tipo;
		int idx = indiceTipo(tipo);
		Color c = colorDePieza(tipo);
		
		// Fondo del item más grande
		Rectangle item = { (float)panelX + 5, (float)offsetY, 140, 45 };
		DrawRectangleRounded(item, 0.3f, 6, Fade(c, 0.15f));
		DrawRectangleLinesEx(item, 1, Fade(c, 0.3f));
		
		// Dibujar la mini pieza más grande
		int miniCelda = 22;
		int offsetX = panelX + 20;
		int offsetY2 = offsetY + 12;
		
		for(int i = 0; i < 4; i++) {
			int col = FORMAS[idx][0][i][0];
			int fil = FORMAS[idx][0][i][1];
			int x = offsetX + col * miniCelda;
			int y = offsetY2 + fil * miniCelda;
			Rectangle rec = { (float)x, (float)y, (float)miniCelda - 2, (float)miniCelda - 2 };
			DrawRectangleRounded(rec, 0.2f, 4, c);
			DrawRectangleLinesEx(rec, 1, Fade(BLACK, 0.05f));
		}
		
		// Mostrar el nombre de la pieza
		DrawText(TextFormat("%s", NOMBRES_PIEZAS[idx]), panelX + 95, offsetY + 15, 16, Fade(TEXT_PLUM, 0.7f));
		
		offsetY += 58;
		aux = aux->siguiente;
		contador++;
	}
}

void dibujarHold(NodoHold* tope) {
	int cajaTam = 130; // Más grande
	int cajaX = PANEL_DER_X + (PANEL_DER_ANCHO - cajaTam) / 2;
	int cajaY = BOARD_Y + 30;
	
	// Panel hold más grande
	Rectangle panel = { (float)cajaX - 15, (float)cajaY - 20, 
		(float)cajaTam + 30, (float)cajaTam + 50 };
	Rectangle sombra = { panel.x + 4, panel.y + 4, panel.width, panel.height };
	DrawRectangleRounded(sombra, 0.05f, 8, Fade(SHADOW, 0.25f));
	DrawRectangleRounded(panel, 0.05f, 8, Fade(WHITE, 0.4f));
	DrawRectangleLinesEx(panel, 1.5f, Fade(GRID_LINE, 0.6f));
	
	DrawText("? Hold (Z)", cajaX + 25, cajaY - 10, 18, TEXT_PLUM);
	
	if(!vacia(tope)) {
		Pieza p = tope->dato;
		int idx = indiceTipo(p.tipo);
		Color c = colorDePieza(p.tipo);
		int miniCelda = 28;
		
		// Fondo suave para la pieza
		Rectangle bg = { (float)cajaX + 5, (float)cajaY + 15, (float)cajaTam - 10, (float)cajaTam - 10 };
		DrawRectangleRounded(bg, 0.1f, 8, Fade(c, 0.1f));
		
		// Centrar la pieza en el hold
		int offsetX = cajaX + (cajaTam - 4 * miniCelda) / 2 + 10;
		int offsetY = cajaY + (cajaTam - 4 * miniCelda) / 2 + 15;
		
		for(int i = 0; i < 4; i++) {
			int col = FORMAS[idx][0][i][0];
			int fil = FORMAS[idx][0][i][1];
			int x = offsetX + col * miniCelda;
			int y = offsetY + fil * miniCelda;
			Rectangle rec = { (float)x, (float)y, (float)miniCelda - 2, (float)miniCelda - 2 };
			DrawRectangleRounded(rec, 0.2f, 4, c);
			DrawRectangleLinesEx(rec, 1, Fade(BLACK, 0.05f));
		}
	}
}

void dibujarPuntaje(int puntaje) {
	int panelX = PANEL_IZQ_X + 10;
	int panelY = BOARD_Y;
	
	// Panel de puntaje más grande
	Rectangle panel = { (float)panelX - 5, (float)panelY, (float)PANEL_IZQ_ANCHO - 25, 140 };
	Rectangle sombra = { panel.x + 4, panel.y + 4, panel.width, panel.height };
	DrawRectangleRounded(sombra, 0.05f, 8, Fade(SHADOW, 0.25f));
	DrawRectangleRounded(panel, 0.05f, 8, Fade(WHITE, 0.4f));
	DrawRectangleLinesEx(panel, 1.5f, Fade(GRID_LINE, 0.6f));
	
	DrawText("? Puntaje", panelX + 15, panelY + 15, 20, TEXT_PLUM);
	DrawText(TextFormat("%06d", puntaje), panelX + 15, panelY + 50, 36, TEXT_PLUM);
	
	// Nivel
	int nivel = puntaje / 1000 + 1;
	DrawText(TextFormat("Nivel %d", nivel), panelX + 15, panelY + 95, 16, Fade(TEXT_PLUM, 0.6f));
}

void dibujarControles() {
	int centroX = BOARD_X + BOARD_ANCHO / 2;
	int y = BOARD_Y + BOARD_ALTO + 30;
	int r = 28;
	
	// Panel de controles más grande y claro
	Rectangle panel = { (float)centroX - 130, (float)y - 40, 260, 70 };
	Rectangle sombra = { panel.x + 3, panel.y + 3, panel.width, panel.height };
	DrawRectangleRounded(sombra, 0.05f, 8, Fade(SHADOW, 0.15f));
	DrawRectangleRounded(panel, 0.05f, 8, Fade(WHITE, 0.3f));
	DrawRectangleLinesEx(panel, 1.5f, Fade(GRID_LINE, 0.3f));
	
	// Controles con estilo - usando caracteres ASCII estándar
	Color ctrlColor = Fade(TEXT_PLUM, 0.8f);
	
	// Flecha izquierda
	DrawCircleLines(centroX - 85, y + 5, r, ctrlColor);
	DrawText("<", centroX - 85 - 8, y - 8, 24, ctrlColor);
	
	// Rotación
	DrawCircleLines(centroX, y + 5, r, ctrlColor);
	DrawText("R", centroX - 7, y - 8, 24, ctrlColor);
	
	// Flecha derecha
	DrawCircleLines(centroX + 85, y + 5, r, ctrlColor);
	DrawText(">", centroX + 85 - 8, y - 8, 24, ctrlColor);
	
	// Texto de ayuda
	DrawText("? Mover abajo", centroX - 50, y + 45, 13, Fade(TEXT_PLUM, 0.4f));
	DrawText("ESC para salir", centroX - 45, y + 65, 12, Fade(TEXT_PLUM, 0.3f));
}

void dibujarFondo() {
	DrawRectangleGradientV(0, 0, VENTANA_ANCHO, VENTANA_ALTO, BG_TOP, BG_BOTTOM);
	
	// Decoraciones de fondo
	for(int i = 0; i < 8; i++) {
		int x = 30 + i * 80;
		int y = 20 + (i % 3) * 60;
		DrawCircle(x, y, 15 + (i % 5) * 5, Fade(WHITE, 0.03f));
	}
	
	// Título con estilo
	Rectangle titleBg = { (float)BOARD_X + 40, 18, 220, 40 };
	DrawRectangleRounded(titleBg, 0.5f, 8, Fade(WHITE, 0.3f));
	DrawText("?? TETRIS", BOARD_X + 60, 25, 28, Fade(TEXT_PLUM, 0.9f));
	
	// Línea decorativa
	DrawLine(BOARD_X, 65, BOARD_X + BOARD_ANCHO, 65, Fade(TEXT_PLUM, 0.1f));
}

void dibujarGameOver() {
	DrawRectangle(0, 0, VENTANA_ANCHO, VENTANA_ALTO, Fade(BLACK, 0.3f));
	
	Rectangle box = { (float)(BOARD_X + 20), (float)(BOARD_Y + 220), 260, 140 };
	Rectangle sombraBox = { box.x + 8, box.y + 8, box.width, box.height };
	DrawRectangleRounded(sombraBox, 0.15f, 10, Fade(SHADOW, 0.4f));
	DrawRectangleRounded(box, 0.15f, 10, PANEL_BG);
	DrawRectangleLinesEx(box, 2, PANEL_BORDE);
	
	DrawText("??", (int)box.x + 30, (int)box.y + 20, 40, TEXT_PLUM);
	DrawText("GAME OVER", (int)box.x + 80, (int)box.y + 30, 28, colorDePieza('Z'));
	DrawText("presiona ESC para salir", (int)box.x + 55, (int)box.y + 85, 13, Fade(TEXT_PLUM, 0.7f));
}
