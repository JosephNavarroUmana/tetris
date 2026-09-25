#include "raylib.h"
#include "interfaz.h"
#include "Formas.h"
#include "Puntajes.h"
#include "Eventos.h"

static const Color BG_TOP    = {252, 240, 245, 255};
static const Color BG_BOTTOM = {235, 220, 250, 255};
static const Color PANEL_BG  = {250, 245, 248, 255};
static const Color SHADOW    = {180, 150, 170, 255};
static const Color TEXT_PLUM = {90, 60, 100, 255};

static const Color GRID_LINE   = {210, 180, 200, 255};
static const Color PANEL_BORDE = {200, 160, 185, 255};

static const Color COLOR_I = {100, 200, 255, 255};
static const Color COLOR_O = {255, 215, 0, 255};
static const Color COLOR_T = {180, 130, 255, 255};
static const Color COLOR_S = {80, 230, 180, 255};
static const Color COLOR_Z = {255, 100, 130, 255};
static const Color COLOR_J = {80, 140, 255, 255};
static const Color COLOR_L = {255, 170, 80, 255};

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
	InitWindow(VENTANA_ANCHO, VENTANA_ALTO, "Tetris Cozy");
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

void dibujarHold(NodoHold* tope) {
	int cajaTam = 130;
	int cajaX = PANEL_DER_X + (PANEL_DER_ANCHO - cajaTam) / 2;
	int cajaY = BOARD_Y + 30;
	int panelAlto = cajaTam + 50;
	
	Rectangle panel = { (float)cajaX - 15, (float)cajaY - 20, 
		(float)cajaTam + 30, (float)panelAlto };
	Rectangle sombra = { panel.x + 4, panel.y + 4, panel.width, panel.height };
	DrawRectangleRounded(sombra, 0.05f, 8, Fade(SHADOW, 0.25f));
	DrawRectangleRounded(panel, 0.05f, 8, Fade(WHITE, 0.4f));
	DrawRectangleLinesEx(panel, 1.5f, Fade(GRID_LINE, 0.6f));
	
	DrawText("Hold (Z)", cajaX + 25, cajaY - 10, 18, TEXT_PLUM);
	
	if(!vacia(tope)) {
		Pieza p = tope->dato;
		int idx = indiceTipo(p.tipo);
		Color c = colorDePieza(p.tipo);
		int miniCelda = 22;
		
		Rectangle bg = { (float)cajaX + 5, (float)cajaY + 15, (float)cajaTam - 10, (float)cajaTam - 10 };
		DrawRectangleRounded(bg, 0.1f, 8, Fade(c, 0.1f));
		
		int offsetX = cajaX + (cajaTam - 4 * miniCelda) / 2;
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

void dibujarSiguientes(NodoPieza* cabeza) {
	int panelX = PANEL_DER_X + 10;
	int panelY = BOARD_Y + 30 + 130 + 50 + 30;
	
	Rectangle panel = { (float)panelX - 15, (float)panelY - 20, 
		(float)PANEL_DER_ANCHO - 20, 235 };
	Rectangle sombra = { panel.x + 4, panel.y + 4, panel.width, panel.height };
	DrawRectangleRounded(sombra, 0.05f, 8, Fade(SHADOW, 0.25f));
	DrawRectangleRounded(panel, 0.05f, 8, Fade(WHITE, 0.4f));
	DrawRectangleLinesEx(panel, 1.5f, Fade(GRID_LINE, 0.6f));
	
	DrawText("Siguientes", panelX + 10, panelY - 10, 18, TEXT_PLUM);
	
	NodoPieza* aux = cabeza;
	int contador = 0;
	int offsetY = panelY + 30;
	int itemAncho = (int)panel.width - 20;
	
	while(aux != nullptr && contador < 3) {
		char tipo = aux->dato.tipo;
		int idx = indiceTipo(tipo);
		Color c = colorDePieza(tipo);
		
		Rectangle item = { (float)panelX + 5, (float)offsetY, (float)itemAncho, 45 };
		DrawRectangleRounded(item, 0.3f, 6, Fade(c, 0.15f));
		DrawRectangleLinesEx(item, 1, Fade(c, 0.3f));
		
		int miniCelda = 12;
		int piezaAlto = 4 * miniCelda;
		int offsetX2 = panelX + 15;
		int offsetY2 = offsetY + (45 - piezaAlto) / 2;
		
		for(int i = 0; i < 4; i++) {
			int col = FORMAS[idx][0][i][0];
			int fil = FORMAS[idx][0][i][1];
			int x = offsetX2 + col * miniCelda;
			int y = offsetY2 + fil * miniCelda;
			Rectangle rec = { (float)x, (float)y, (float)miniCelda - 2, (float)miniCelda - 2 };
			DrawRectangleRounded(rec, 0.2f, 4, c);
			DrawRectangleLinesEx(rec, 1, Fade(BLACK, 0.05f));
		}
		
		DrawText(TextFormat("%s", NOMBRES_PIEZAS[idx]), panelX + itemAncho - 25, offsetY + 15, 16, Fade(TEXT_PLUM, 0.7f));
		
		offsetY += 58;
		aux = aux->siguiente;
		contador++;
	}
}

void dibujarPuntaje(int puntaje) {
	int panelX = PANEL_IZQ_X + 10;
	int panelY = BOARD_Y;
	
	Rectangle panel = { (float)panelX - 5, (float)panelY, (float)PANEL_IZQ_ANCHO - 25, 140 };
	Rectangle sombra = { panel.x + 4, panel.y + 4, panel.width, panel.height };
	DrawRectangleRounded(sombra, 0.05f, 8, Fade(SHADOW, 0.25f));
	DrawRectangleRounded(panel, 0.05f, 8, Fade(WHITE, 0.4f));
	DrawRectangleLinesEx(panel, 1.5f, Fade(GRID_LINE, 0.6f));
	
	DrawText("Puntaje", panelX + 15, panelY + 15, 20, TEXT_PLUM);
	DrawText(TextFormat("%06d", puntaje), panelX + 15, panelY + 50, 36, TEXT_PLUM);
	
	int nivel = puntaje / 1000 + 1;
	DrawText(TextFormat("Nivel %d", nivel), panelX + 15, panelY + 95, 16, Fade(TEXT_PLUM, 0.6f));
}

void dibujarControles() {
	int centroX = BOARD_X + BOARD_ANCHO / 2;
	int y = BOARD_Y + BOARD_ALTO + 55;
	int r = 24;
	
	Rectangle panel = { (float)centroX - 130, (float)y - 28, 260, 60 };
	Rectangle sombra = { panel.x + 3, panel.y + 3, panel.width, panel.height };
	DrawRectangleRounded(sombra, 0.05f, 8, Fade(SHADOW, 0.15f));
	DrawRectangleRounded(panel, 0.05f, 8, Fade(WHITE, 0.3f));
	DrawRectangleLinesEx(panel, 1.5f, Fade(GRID_LINE, 0.3f));
	
	Color ctrlColor = Fade(TEXT_PLUM, 0.8f);
	
	DrawCircleLines(centroX - 85, y, r, ctrlColor);
	DrawText("<", centroX - 85 - 7, y - 10, 20, ctrlColor);
	
	DrawCircleLines(centroX, y, r, ctrlColor);
	DrawText("R", centroX - 6, y - 10, 20, ctrlColor);
	
	DrawCircleLines(centroX + 85, y, r, ctrlColor);
	DrawText(">", centroX + 85 - 7, y - 10, 20, ctrlColor);
}

void dibujarFondo() {
	DrawRectangleGradientV(0, 0, VENTANA_ANCHO, VENTANA_ALTO, BG_TOP, BG_BOTTOM);
	
	for(int i = 0; i < 8; i++) {
		int x = 30 + i * 80;
		int y = 20 + (i % 3) * 60;
		DrawCircle(x, y, 15 + (i % 5) * 5, Fade(WHITE, 0.03f));
	}
	
	Rectangle titleBg = { (float)BOARD_X + 20, 18, 260, 40 };
	DrawRectangleRounded(titleBg, 0.5f, 8, Fade(WHITE, 0.3f));
	DrawText("COUSYTRYX", BOARD_X + 40, 25, 26, Fade(TEXT_PLUM, 0.9f));
	
	DrawLine(BOARD_X, 65, BOARD_X + BOARD_ANCHO, 65, Fade(TEXT_PLUM, 0.1f));
}

int dibujarGameOver(NodoPuntaje* &tablaPuntajes, int puntajeFinal,
					TipoOrdenamiento &algoritmoActual,
					double &tiempoQuicksort, double &tiempoInsercion){
	
	DrawRectangle(0, 0, VENTANA_ANCHO, VENTANA_ALTO, Fade(BLACK, 0.4f));
	
	int panelAncho = 340;
	int panelAlto = 540;
	int panelX = (VENTANA_ANCHO - panelAncho) / 2;
	int panelY = (VENTANA_ALTO - panelAlto) / 2;
	
	Rectangle panel = { (float)panelX, (float)panelY, (float)panelAncho, (float)panelAlto };
	Rectangle sombra = { panel.x + 6, panel.y + 6, panel.width, panel.height };
	DrawRectangleRounded(sombra, 0.06f, 10, Fade(SHADOW, 0.4f));
	DrawRectangleRounded(panel, 0.06f, 10, PANEL_BG);
	DrawRectangleLinesEx(panel, 2.5f, PANEL_BORDE);
	
	DrawText("GAME OVER", panelX + 75, panelY + 20, 28, colorDePieza('Z'));
	DrawText(TextFormat("Puntaje: %d", puntajeFinal), panelX + 100, panelY + 55, 16, TEXT_PLUM);
	
	DrawText("Top Jugadores", panelX + 95, panelY + 85, 16, TEXT_PLUM);
	
	NodoPuntaje* aux = tablaPuntajes;
	int pos = 1;
	int offsetY = panelY + 115;
	
	while(aux != nullptr && pos <= 10){
		Color colorPos = TEXT_PLUM;
		if(pos == 1){
			colorPos = COLOR_O;
		}
		DrawText(TextFormat("%d.", pos), panelX + 30, offsetY, 14, colorPos);
		DrawText(aux->nombre.c_str(), panelX + 60, offsetY, 14, colorPos);
		DrawText(TextFormat("%d", aux->puntaje), panelX + panelAncho - 70, offsetY, 14, colorPos);
		offsetY += 20;
		aux = aux->siguiente;
		pos++;
	}
	
	int panelOrdenY = panelY + 355;
	DrawText("Algoritmo de orden:", panelX + 30, panelOrdenY, 15, TEXT_PLUM);
	
	Rectangle botonQuick = { (float)panelX + 30, (float)panelOrdenY + 22, 130, 32 };
	Rectangle botonInsercion = { (float)panelX + 175, (float)panelOrdenY + 22, 130, 32 };
	
	Vector2 mouse = GetMousePosition();
	bool sobreQuick = CheckCollisionPointRec(mouse, botonQuick);
	bool sobreInsercion = CheckCollisionPointRec(mouse, botonInsercion);
	bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
	
	bool quickSeleccionado = (algoritmoActual == ORDEN_QUICKSORT);
	
	Color colorQuick = Fade(colorDePieza('I'), 0.4f);
	if(quickSeleccionado){
		colorQuick = colorDePieza('I');
	}
	DrawRectangleRounded(botonQuick, 0.3f, 8, colorQuick);
	DrawText("Quicksort", (int)botonQuick.x + 20, (int)botonQuick.y + 8, 14, WHITE);
	
	Color colorInsercion = Fade(colorDePieza('I'), 0.4f);
	if(!quickSeleccionado){
		colorInsercion = colorDePieza('I');
	}
	DrawRectangleRounded(botonInsercion, 0.3f, 8, colorInsercion);
	DrawText("Insercion", (int)botonInsercion.x + 20, (int)botonInsercion.y + 8, 14, WHITE);
	
	if(sobreQuick && click){
		algoritmoActual = ORDEN_QUICKSORT;
		tiempoQuicksort = medirTiempoOrdenamiento(tablaPuntajes, ORDEN_QUICKSORT);
		ordenarPuntajes(tablaPuntajes, ORDEN_QUICKSORT);
	}
	if(sobreInsercion && click){
		algoritmoActual = ORDEN_INSERCION;
		tiempoInsercion = medirTiempoOrdenamiento(tablaPuntajes, ORDEN_INSERCION);
		ordenarPuntajes(tablaPuntajes, ORDEN_INSERCION);
	}
	
	int offsetTiempos = panelOrdenY + 65;
	if(tiempoQuicksort >= 0){
		DrawText(TextFormat("Quicksort: %.4f ms", tiempoQuicksort), panelX + 30, offsetTiempos, 13, Fade(TEXT_PLUM, 0.8f));
	}
	if(tiempoInsercion >= 0){
		DrawText(TextFormat("Insercion: %.4f ms", tiempoInsercion), panelX + 30, offsetTiempos + 18, 13, Fade(TEXT_PLUM, 0.8f));
	}
	
	Rectangle botonReplay = { (float)panelX + 25, (float)panelY + panelAlto - 45, 130, 36 };
	Rectangle botonExit   = { (float)panelX + panelAncho - 155, (float)panelY + panelAlto - 45, 130, 36 };
	
	bool sobreReplay = CheckCollisionPointRec(mouse, botonReplay);
	bool sobreExit = CheckCollisionPointRec(mouse, botonExit);
	
	Color colorReplay = Fade(colorDePieza('O'), 0.7f);
	if(sobreReplay){
		colorReplay = colorDePieza('O');
	}
	DrawRectangleRounded(botonReplay, 0.3f, 8, colorReplay);
	DrawText("REPLAY", (int)botonReplay.x + 25, (int)botonReplay.y + 8, 16, WHITE);
	
	Color colorExit = Fade(colorDePieza('Z'), 0.7f);
	if(sobreExit){
		colorExit = colorDePieza('Z');
	}
	DrawRectangleRounded(botonExit, 0.3f, 8, colorExit);
	DrawText("SALIR", (int)botonExit.x + 35, (int)botonExit.y + 8, 16, WHITE);
	
	if(sobreReplay && click) return 2;
	if(sobreExit && click) return 1;
	return 0;
}
	
void dibujarTopJugadores(NodoPuntaje* cabeza){
	
	int panelAncho = 280;
	int panelAlto = 320;
	int panelX = (VENTANA_ANCHO - panelAncho) / 2;
	int panelY = (VENTANA_ALTO - panelAlto) / 2;
	
	Rectangle panel = { (float)panelX, (float)panelY, (float)panelAncho, (float)panelAlto };
	Rectangle sombra = { panel.x + 6, panel.y + 6, panel.width, panel.height };
	DrawRectangleRounded(sombra, 0.08f, 10, Fade(SHADOW, 0.35f));
	DrawRectangleRounded(panel, 0.08f, 10, PANEL_BG);
	DrawRectangleLinesEx(panel, 2.5f, PANEL_BORDE);
	
	DrawText("Top Jugadores", panelX + 55, panelY + 20, 22, TEXT_PLUM);
	
	NodoPuntaje* aux = cabeza;
	int pos = 1;
	int offsetY = panelY + 60;
	
	while(aux != nullptr && pos <= 10){
		Color colorPos = TEXT_PLUM;
		if(pos == 1){
			colorPos = COLOR_O;
		}
		
		DrawText(TextFormat("%d.", pos), panelX + 20, offsetY, 16, colorPos);
		DrawText(aux->nombre.c_str(), panelX + 50, offsetY, 16, colorPos);
		DrawText(TextFormat("%d", aux->puntaje), panelX + panelAncho - 80, offsetY, 16, colorPos);
		
		offsetY += 25;
		aux = aux->siguiente;
		pos++;
	}
	
	if(cabeza == nullptr){
		DrawText("Aun no hay puntajes", panelX + 40, offsetY, 16, Fade(TEXT_PLUM, 0.6f));
	}
}
	
string pantallaNombre(){
	string nombre = "";
	const int tam = 15;
	while(!WindowShouldClose()){
		
		bool shiftPresionado = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
		
		for(int k = KEY_A; k <= KEY_Z; k++){
			if(IsKeyPressed(k) && nombre.length() < tam){
				char c = (char)k;
				if(!shiftPresionado){
					c = c + 32;
				}
				nombre += c;
			}
		}
		
		for(int k = KEY_ZERO; k <= KEY_NINE; k++){
			if(IsKeyPressed(k) && nombre.length() < tam){
				nombre += (char)k;
			}
		}
		
		if(IsKeyPressed(KEY_SPACE) && nombre.length() <tam){
			nombre += ' ';
		}
		
		if(IsKeyPressed(KEY_BACKSPACE) && nombre.length() > 0){
			nombre.pop_back();
		}
		
		bool confirmado = IsKeyPressed(KEY_ENTER) && nombre.length() > 0;
		
		BeginDrawing();
		dibujarFondo();
		
		int panelAncho = 320;
		int panelAlto = 160;
		int panelX = (VENTANA_ANCHO - panelAncho) / 2;
		int panelY = (VENTANA_ALTO - panelAlto) / 2;
		
		Rectangle panel = { (float)panelX, (float)panelY, (float)panelAncho, (float)panelAlto };
		Rectangle sombra = { panel.x + 6, panel.y + 6, panel.width, panel.height };
		DrawRectangleRounded(sombra, 0.08f, 10, Fade(SHADOW, 0.35f));
		DrawRectangleRounded(panel, 0.08f, 10, PANEL_BG);
		DrawRectangleLinesEx(panel, 2.5f, PANEL_BORDE);
		
		DrawText("Ingresa tu nombre", panelX + 50, panelY + 20, 20, TEXT_PLUM);
		
		Rectangle caja = { (float)panelX + 30, (float)panelY + 60, (float)panelAncho - 60, 40 };
		DrawRectangleRounded(caja, 0.2f, 6, Fade(WHITE, 0.6f));
		DrawRectangleLinesEx(caja, 1.5f, GRID_LINE);
		DrawText(nombre.c_str(), (int)caja.x + 10, (int)caja.y + 10, 20, TEXT_PLUM);
		
		DrawText("Presiona ENTER para continuar", panelX + 40, panelY + 115, 14, Fade(TEXT_PLUM, 0.6f));
		
		EndDrawing();
		
		if(confirmado){
			return nombre;
		}
	}
	
	return "Jugador";
}
	
static Texture2D texturaLogro;

void cargarImagenLogro(){
	texturaLogro = LoadTexture("logro.png");
}
	
void descargarImagenLogro(){
	UnloadTexture(texturaLogro);
}
	
void dibujarParpadeoLineas(NodoFila* cabeza, float progreso){
	bool visible = ((int)(progreso * 12)) % 2 == 0;
	if(visible) return;
	
	NodoFila* aux = cabeza;
	int fila = 0;
	while(aux != nullptr){
		if(llena(aux)){
			int y = BOARD_Y + fila * TAM_CELDA;
			DrawRectangle(BOARD_X, y, BOARD_ANCHO, TAM_CELDA, WHITE);
		}
		aux = aux->siguiente;
		fila++;
	}
}
	
void dibujarFlashBorde(float progreso){
	float alpha = 1.0f - progreso;
	Color colorFlash = Fade(COLOR_O, alpha * 0.6f);
	
	int grosor = 6;
	DrawRectangle(BOARD_X - grosor, BOARD_Y - grosor, BOARD_ANCHO + grosor*2, grosor, colorFlash);
	DrawRectangle(BOARD_X - grosor, BOARD_Y + BOARD_ALTO, BOARD_ANCHO + grosor*2, grosor, colorFlash);
	DrawRectangle(BOARD_X - grosor, BOARD_Y, grosor, BOARD_ALTO, colorFlash);
	DrawRectangle(BOARD_X + BOARD_ANCHO, BOARD_Y, grosor, BOARD_ALTO, colorFlash);
}
	
void dibujarImagenLogro(float tiempoRestante){
	if(tiempoRestante <= 0) return;
	
	int anchoDeseado = 100;
	int altoDeseado = 100;
	
	int x = PANEL_IZQ_X + 10;
	int y = BOARD_Y + 160;
	
	Rectangle origen = { 0, 0, (float)texturaLogro.width, (float)texturaLogro.height };
	Rectangle destino = { (float)x, (float)y, (float)anchoDeseado, (float)altoDeseado };
	Vector2 origenRotacion = { 0, 0 };
	
	DrawTexturePro(texturaLogro, origen, destino, origenRotacion, 0.0f, WHITE);
}
	
void dibujarAvisoEvento(char tipo, float tiempoRestante){
	if(tiempoRestante <= 0) return;
	
	const char* texto = "";
	if(tipo == EVENTO_INVERTIR) texto = "¡CONTROLES INVERTIDOS!";
	else if(tipo == EVENTO_BLOQUEAR_ROTACION) texto = "¡ROTACION BLOQUEADA!";
	else if(tipo == EVENTO_MOVIMIENTO_DOBLE) texto = "¡Movimientos x2!";
	
	DrawText(texto, 200, 50, 20, RED);
}
	
void dibujarTableroDesdeEstado(const char filas[FILAS_TABLERO][COLUMNAS_TABLERO],
	int offsetX, int offsetY,
	Pieza piezaActual) {
	Rectangle panel = { (float)(offsetX - 12), (float)(offsetY - 12),
												(float)(BOARD_ANCHO + 24), (float)(BOARD_ALTO + 24) };
	
	Rectangle sombraPanel = { panel.x + 5, panel.y + 5, panel.width, panel.height };
	DrawRectangleRounded(sombraPanel, 0.08f, 10, Fade(SHADOW, 0.35f));
	DrawRectangleRounded(panel, 0.08f, 10, PANEL_BG);
	DrawRectangleLinesEx(panel, 2.5f, PANEL_BORDE);
	
	Rectangle fondoInterno = { (float)offsetX, (float)offsetY, (float)BOARD_ANCHO, (float)BOARD_ALTO };
	DrawRectangleRounded(fondoInterno, 0.02f, 4, Fade(WHITE, 0.15f));
	
	for(int fila = 0; fila < FILAS_TABLERO; fila++){
		for(int col = 0; col < COLUMNAS_TABLERO; col++){
			int x = offsetX + col * TAM_CELDA;
			int y = offsetY + fila * TAM_CELDA;
			
			if(filas[fila][col] == '-'){
				dibujarCelda(x, y, WHITE, true);
			} else {
				dibujarCelda(x, y, colorDePieza(filas[fila][col]), false);
			}
		}
	}
	
	if(piezaActual.tipo != '\0' && piezaActual.tipo != 0){
		int idx = indiceTipo(piezaActual.tipo);
		Color c = colorDePieza(piezaActual.tipo);
		
		for(int i = 0; i < 4; i++){
			int col = piezaActual.ubicacion[0] + FORMAS[idx][piezaActual.direccion][i][0];
			int fil = piezaActual.ubicacion[1] + FORMAS[idx][piezaActual.direccion][i][1];
			
			int x = offsetX + col * TAM_CELDA;
			int y = offsetY + fil * TAM_CELDA;
			
			dibujarCelda(x, y, c, false);
		}
	}
}
	
int dibujarPantallaReplay(const char filas[FILAS_TABLERO][COLUMNAS_TABLERO],
	Pieza piezaActual,
	int puntajeEnEsePaso, int pasoActual, int totalPasos,
	bool reproduciendo){
	
	DrawRectangle(0, 0, VENTANA_ANCHO, VENTANA_ALTO, Fade(BLACK, 0.5f));
	
	dibujarTableroDesdeEstado(filas, BOARD_X, BOARD_Y, piezaActual);
	
	DrawText("REPRODUCCION DE PARTIDA", BOARD_X + 20, 20, 24, WHITE);
	DrawText(TextFormat("Puntaje: %d", puntajeEnEsePaso), BOARD_X + 20, 50, 18, WHITE);
	DrawText(TextFormat("Paso %d / %d", pasoActual, totalPasos), BOARD_X + 20, 75, 16, Fade(WHITE, 0.8f));
	
	int centroX = BOARD_X + BOARD_ANCHO / 2;
	int y = BOARD_Y + BOARD_ALTO + 30;
	
	Rectangle botonAtras = { (float)centroX - 140, (float)y, 60, 40 };
	Rectangle botonPlay  = { (float)centroX - 30,  (float)y, 60, 40 };
	Rectangle botonAdelante = { (float)centroX + 80, (float)y, 60, 40 };
	Rectangle botonSalir = { (float)centroX - 50, (float)y + 55, 100, 36 };
	
	Vector2 mouse = GetMousePosition();
	Color activo = colorDePieza('O');
	Color inactivo = Fade(colorDePieza('O'), 0.7f);
	
	bool sobreAtras = CheckCollisionPointRec(mouse, botonAtras);
	Color colorAtras = inactivo;
	if(sobreAtras){
		colorAtras = activo;
	}
	DrawRectangleRounded(botonAtras, 0.3f, 8, colorAtras);
	DrawText("<<", (int)botonAtras.x + 15, (int)botonAtras.y + 10, 18, WHITE);
	
	bool sobrePlay = CheckCollisionPointRec(mouse, botonPlay);
	Color colorPlay = inactivo;
	if(sobrePlay){
		colorPlay = activo;
	}
	DrawRectangleRounded(botonPlay, 0.3f, 16, colorPlay);
	const char* textoPlay = "PLAY";
	if(reproduciendo){
		textoPlay = "PAUSA";
	}
	DrawText(textoPlay, (int)botonPlay.x + 22, (int)botonPlay.y + 10, 18, WHITE);
	
	bool sobreAdelante = CheckCollisionPointRec(mouse, botonAdelante);
	Color colorAdelante = inactivo;
	if(sobreAdelante){
		colorAdelante = activo;
	}
	DrawRectangleRounded(botonAdelante, 0.3f, 8, colorAdelante);
	DrawText(">>", (int)botonAdelante.x + 15, (int)botonAdelante.y + 10, 18, WHITE);
	
	bool sobreSalir = CheckCollisionPointRec(mouse, botonSalir);
	Color colorSalir = Fade(colorDePieza('Z'), 0.7f);
	if(sobreSalir){
		colorSalir = colorDePieza('Z');
	}
	DrawRectangleRounded(botonSalir, 0.3f, 8, colorSalir);
	DrawText("VOLVER", (int)botonSalir.x + 15, (int)botonSalir.y + 8, 16, WHITE);
	
	bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
	
	if((sobreAtras && click) || IsKeyPressed(KEY_LEFT))  return -1;
	if((sobreAdelante && click) || IsKeyPressed(KEY_RIGHT)) return 1;
	if((sobrePlay && click) || IsKeyPressed(KEY_SPACE)) return 2;
	if(sobreSalir && click) return 3;
	
	return 0;
}
