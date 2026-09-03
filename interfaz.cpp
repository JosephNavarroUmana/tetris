#include "raylib.h"
#include "interfaz.h"
#include "Formas.h"
#include "Puntajes.h"


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
	int panelY = BOARD_Y + 30 + 130 + 50 + 30; // justo debajo del Hold, con margen de 30
	
	Rectangle panel = { (float)panelX - 15, (float)panelY - 20, 
		(float)PANEL_DER_ANCHO - 20, 200 };
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
		
		int miniCelda = 18;
		int piezaAncho = 4 * miniCelda;
		int piezaAlto = 4 * miniCelda;
		int offsetX2 = panelX + 5 + (itemAncho - 60 - piezaAncho) / 2; // deja 60px para el texto a la derecha
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
	
	// Panel de puntaje más grande
	Rectangle panel = { (float)panelX - 5, (float)panelY, (float)PANEL_IZQ_ANCHO - 25, 140 };
	Rectangle sombra = { panel.x + 4, panel.y + 4, panel.width, panel.height };
	DrawRectangleRounded(sombra, 0.05f, 8, Fade(SHADOW, 0.25f));
	DrawRectangleRounded(panel, 0.05f, 8, Fade(WHITE, 0.4f));
	DrawRectangleLinesEx(panel, 1.5f, Fade(GRID_LINE, 0.6f));
	
	DrawText("Puntaje", panelX + 15, panelY + 15, 20, TEXT_PLUM);
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
	
//	// Texto de ayuda
//	DrawText("? Mover abajo", centroX - 50, y + 45, 13, Fade(TEXT_PLUM, 0.4f));
//	DrawText("ESC para salir", centroX - 45, y + 65, 12, Fade(TEXT_PLUM, 0.3f));
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

bool dibujarGameOver(NodoPuntaje* tablaPuntajes, int puntajeFinal){
	
	DrawRectangle(0, 0, VENTANA_ANCHO, VENTANA_ALTO, Fade(BLACK, 0.4f));
	
	int panelAncho = 340;
	int panelAlto = 480;
	int panelX = (VENTANA_ANCHO - panelAncho) / 2;
	int panelY = (VENTANA_ALTO - panelAlto) / 2;
	
	Rectangle panel = { (float)panelX, (float)panelY, (float)panelAncho, (float)panelAlto };
	Rectangle sombra = { panel.x + 6, panel.y + 6, panel.width, panel.height };
	DrawRectangleRounded(sombra, 0.06f, 10, Fade(SHADOW, 0.4f));
	DrawRectangleRounded(panel, 0.06f, 10, PANEL_BG);
	DrawRectangleLinesEx(panel, 2.5f, PANEL_BORDE);
	
	DrawText("GAME OVER", panelX + 75, panelY + 25, 30, colorDePieza('Z'));
	DrawText(TextFormat("Puntaje: %d", puntajeFinal), panelX + 100, panelY + 65, 18, TEXT_PLUM);
	
	DrawText("Top Jugadores", panelX + 95, panelY + 100, 18, TEXT_PLUM);
	
	NodoPuntaje* aux = tablaPuntajes;
	int pos = 1;
	int offsetY = panelY + 135;
	
	while(aux != nullptr && pos <= 10){
		Color colorPos = (pos == 1) ? COLOR_O : TEXT_PLUM;
		DrawText(TextFormat("%d.", pos), panelX + 30, offsetY, 15, colorPos);
		DrawText(aux->nombre.c_str(), panelX + 60, offsetY, 15, colorPos);
		DrawText(TextFormat("%d", aux->puntaje), panelX + panelAncho - 70, offsetY, 15, colorPos);
		offsetY += 24;
		aux = aux->siguiente;
		pos++;
	}
	
	// boton Exit
	Rectangle botonExit = { (float)panelX + panelAncho/2 - 60, (float)panelY + panelAlto - 55, 120, 40 };
	bool mouseEncima = CheckCollisionPointRec(GetMousePosition(), botonExit);
	
	DrawRectangleRounded(botonExit, 0.3f, 8, mouseEncima ? colorDePieza('Z') : Fade(colorDePieza('Z'), 0.7f));
	DrawText("SALIR", (int)botonExit.x + 30, (int)botonExit.y + 10, 18, WHITE);
	
	return mouseEncima && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
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
		Color colorPos = (pos == 1) ? COLOR_O : TEXT_PLUM; // el primero resaltado en dorado
		
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
						c = c + 32; // minuscula
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
			texturaLogro = LoadTexture("logro.png"); // poné tu imagen en la carpeta del ejecutable
		}
			
			void descargarImagenLogro(){
				UnloadTexture(texturaLogro);
			}
				
				void dibujarParpadeoLineas(NodoFila* cabeza, float progreso){
					// progreso va de 0.0 a 1.0 durante la animacion
					// parpadeo: alterna visible/invisible cada ~0.08s
					bool visible = ((int)(progreso * 12)) % 2 == 0;
					if(visible) return; // si es "visible", dejamos que se dibuje normal (no hacemos nada extra)
					
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
						// progreso 0.0 (recien empieza, mas fuerte) a 1.0 (terminando, mas debil)
						float alpha = 1.0f - progreso;
						Color colorFlash = Fade(COLOR_O, alpha * 0.6f);
						
						int grosor = 6;
						DrawRectangle(BOARD_X - grosor, BOARD_Y - grosor, BOARD_ANCHO + grosor*2, grosor, colorFlash); // arriba
						DrawRectangle(BOARD_X - grosor, BOARD_Y + BOARD_ALTO, BOARD_ANCHO + grosor*2, grosor, colorFlash); // abajo
						DrawRectangle(BOARD_X - grosor, BOARD_Y, grosor, BOARD_ALTO, colorFlash); // izquierda
						DrawRectangle(BOARD_X + BOARD_ANCHO, BOARD_Y, grosor, BOARD_ALTO, colorFlash); // derecha
					}
						
						void dibujarImagenLogro(float tiempoRestante){
							if(tiempoRestante <= 0) return;
							
							int anchoDeseado = 100;  // ajustá estos dos numeros al tamaño que quieras
							int altoDeseado = 100;
							
							int x = PANEL_IZQ_X + 10;
							int y = BOARD_Y + 160;
							
							Rectangle origen = { 0, 0, (float)texturaLogro.width, (float)texturaLogro.height };
							Rectangle destino = { (float)x, (float)y, (float)anchoDeseado, (float)altoDeseado };
							Vector2 origenRotacion = { 0, 0 };
							
							DrawTexturePro(texturaLogro, origen, destino, origenRotacion, 0.0f, WHITE);
						}
