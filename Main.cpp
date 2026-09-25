#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include "raylib.h"
#include "Interfaz.h"
#include "Tablero.h"
#include "Generador.h"
#include "Puntajes.h"
#include "Eventos.h"
#include "Replay.h"
using namespace std;

void sacarNuevaPieza(NodoPieza* &genCabeza, NodoPieza* &genCola, Pieza &piezaActual,
					 int &piezasRestantesInvertido, bool &controlesInvertidos,
					 int &piezasRestantesBloqueo, bool &rotacionBloqueada,
					 int &piezasRestantesDoble, bool &movimientoDoble){
	
	if(vacia(genCabeza)){
		llenar(genCabeza, genCola);
	}
	
	piezaActual = verFrente(genCabeza);
	desencolar(genCabeza, genCola);
	
	if(piezasRestantesInvertido > 0){
		piezasRestantesInvertido--;
		if(piezasRestantesInvertido == 0) controlesInvertidos = false;
	}
	if(piezasRestantesBloqueo > 0){
		piezasRestantesBloqueo--;
		if(piezasRestantesBloqueo == 0) rotacionBloqueada = false;
	}
	if(piezasRestantesDoble > 0){
		piezasRestantesDoble--;
		if(piezasRestantesDoble == 0) movimientoDoble = false;
	}
}
	
	Pieza piezaDelHold(NodoHold* holdTope){
		if(vacia(holdTope)){
			return Pieza();
		}
		return holdTope->dato;
	}
		
	int main(){
		
		srand(time(0));
		
		NodoPuntaje* tablaPuntajes = nullptr;
		cargarArchivo(tablaPuntajes, "puntajes.txt");
		
		NodoFila* tablero = nullptr;
		crear(tablero);
		
		TipoOrdenamiento algoritmoActual = ORDEN_QUICKSORT;
		double tiempoQuicksort = -1;
		double tiempoInsercion = -1;
		
		NodoPieza* genCabeza = nullptr;
		NodoPieza* genCola = nullptr;
		NodoHold* holdTope = nullptr;
		int puntaje = 0;
		int ultimoUmbralMostrado = 0;
		llenar(genCabeza, genCola);
		
		bool verReplay = false;
		bool reproduciendoAuto = false;
		float tiempoAcumuladoReplay = 0;
		float velocidadReplay = 0.3f;
		int pasoActualReplay = 1;
		int totalPasosReplay = 0;
		
		Pieza piezaActual = verFrente(genCabeza);
		desencolar(genCabeza, genCola);
		
		bool juegoTerminado = false;
		bool puntajeGuardado = false;
		
		bool animandoLimpieza = false;
		float tiempoAnimacion = 0;
		float duracionAnimacion = 0.35f;
		
		float tiempoImagenLogro = 0;
		float duracionImagenLogro = 2.0f;
		
		char bolsaEventos[3];
		int indiceEvento = 0;
		barajarEventos(bolsaEventos);
		
		iniciarVentana();
		cargarImagenLogro();
		
		string nombreJugador = pantallaNombre();
		
		float tiempoAcumulado = 0;
		float tiempoCaida = 0.5f;
		
		float tiempoAcumuladoSuave = 0;
		float tiempoCaidaSuave = 0.05f;
		
		bool movimientoDoble = false;
		int piezasRestantesDoble = 0;
		
		bool controlesInvertidos = false;
		bool rotacionBloqueada = false;
		int piezasRestantesInvertido = 0;
		int piezasRestantesBloqueo = 0;
		
		char eventoActivo = ' ';
		float tiempoAvisoEvento = 0;
		float duracionAvisoEvento = 1.5f;
		
		Replay replay;
		iniciarReplay(replay);
		
		{
			EstadoJuego estadoInicial;
			capturarEstado(tablero, piezaActual, piezaDelHold(holdTope),
						   !vacia(holdTope), puntaje, 0, estadoInicial);
			registrarEstado(replay, estadoInicial);
		}
		
		while(!WindowShouldClose()){
			
			if(!juegoTerminado){
				
				if(animandoLimpieza){
					
					tiempoAnimacion += GetFrameTime();
					if(tiempoAnimacion >= duracionAnimacion){
						
						int lineasLimpiadas = limpiarFilas(tablero);
						puntaje += lineasLimpiadas * 100;
						
						int umbralActual = (puntaje / 1000) * 1000;
						if(umbralActual > ultimoUmbralMostrado && umbralActual > 0){
							ultimoUmbralMostrado = umbralActual;
							tiempoImagenLogro = duracionImagenLogro;
							
							char tipoEvento = siguienteEvento(bolsaEventos, indiceEvento);
							eventoActivo = tipoEvento;
							tiempoAvisoEvento = duracionAvisoEvento;
							
							if(tipoEvento == EVENTO_INVERTIR){
								controlesInvertidos = true;
								piezasRestantesInvertido = 3;
							} else if(tipoEvento == EVENTO_BLOQUEAR_ROTACION){
								rotacionBloqueada = true;
								piezasRestantesBloqueo = 3;
							} else if(tipoEvento == EVENTO_MOVIMIENTO_DOBLE){
								movimientoDoble = true;
								piezasRestantesDoble = 3;
							}
						}
						
						sacarNuevaPieza(genCabeza, genCola, piezaActual,
										piezasRestantesInvertido, controlesInvertidos,
										piezasRestantesBloqueo, rotacionBloqueada,
										piezasRestantesDoble, movimientoDoble);
						
										{
											EstadoJuego estado;
											capturarEstado(tablero, piezaActual, piezaDelHold(holdTope),
														   !vacia(holdTope), puntaje, 0, estado);
											registrarEstado(replay, estado);
										}
						
						if(hayColision(tablero, piezaActual)){
							juegoTerminado = true;
						}
						
						animandoLimpieza = false;
						tiempoAnimacion = 0;
					}
				} else {
					
					bool huboMovimientoLateral = false;
					
					if(controlesInvertidos){
						if(IsKeyPressed(KEY_LEFT)){
							moverPieza(piezaActual, tablero, 1, 0);
							if(movimientoDoble) moverPieza(piezaActual, tablero, 1, 0);
							huboMovimientoLateral = true;
						}
						if(IsKeyPressed(KEY_RIGHT)){
							moverPieza(piezaActual, tablero, -1, 0);
							if(movimientoDoble) moverPieza(piezaActual, tablero, -1, 0);
							huboMovimientoLateral = true;
						}
					} else {
						if(IsKeyPressed(KEY_LEFT)){
							moverPieza(piezaActual, tablero, -1, 0);
							if(movimientoDoble) moverPieza(piezaActual, tablero, -1, 0);
							huboMovimientoLateral = true;
						}
						if(IsKeyPressed(KEY_RIGHT)){
							moverPieza(piezaActual, tablero, 1, 0);
							if(movimientoDoble) moverPieza(piezaActual, tablero, 1, 0);
							huboMovimientoLateral = true;
						}
					}
					
					if(huboMovimientoLateral){
						EstadoJuego estado;
						capturarEstado(tablero, piezaActual, piezaDelHold(holdTope),
									   !vacia(holdTope), puntaje, 0, estado);
						registrarEstado(replay, estado);
					}
					
					if(IsKeyDown(KEY_DOWN)){
						tiempoAcumuladoSuave += GetFrameTime();
						if(tiempoAcumuladoSuave >= tiempoCaidaSuave){
							tiempoAcumuladoSuave = 0;
							Pieza copia = piezaActual;
							copia.ubicacion[1] += 1;
							if(!hayColision(tablero, copia)){
								piezaActual.ubicacion[1] += 1;
								
								EstadoJuego estado;
								capturarEstado(tablero, piezaActual, piezaDelHold(holdTope),
											   !vacia(holdTope), puntaje, 0, estado);
								registrarEstado(replay, estado);
							}
						}
					} else {
						tiempoAcumuladoSuave = 0;
					}
					
					if(IsKeyPressed(KEY_X) && !rotacionBloqueada){
						rotarPieza(piezaActual, tablero);
						
						EstadoJuego estado;
						capturarEstado(tablero, piezaActual, piezaDelHold(holdTope),
									   !vacia(holdTope), puntaje, 0, estado);
						registrarEstado(replay, estado);
					}
					
					if(IsKeyPressed(KEY_Z) && !rotacionBloqueada){
						if(vacia(holdTope)){
							Pieza aGuardar = piezaActual;
							aGuardar.direccion = 0;
							aGuardar.ubicacion[0] = 3;
							aGuardar.ubicacion[1] = 0;
							push(holdTope, aGuardar);
							
							sacarNuevaPieza(genCabeza, genCola, piezaActual,
											piezasRestantesInvertido, controlesInvertidos,
											piezasRestantesBloqueo, rotacionBloqueada,
											piezasRestantesDoble, movimientoDoble);
						} else {
							Pieza intercambio = pop(holdTope);
							
							Pieza aGuardar = piezaActual;
							aGuardar.direccion = 0;
							aGuardar.ubicacion[0] = 3;
							aGuardar.ubicacion[1] = 0;
							push(holdTope, aGuardar);
							
							piezaActual = intercambio;
						}
						
						EstadoJuego estado;
						capturarEstado(tablero, piezaActual, piezaDelHold(holdTope),
									   !vacia(holdTope), puntaje, 0, estado);
						registrarEstado(replay, estado);
					}
					
					tiempoAcumulado += GetFrameTime();
					if(tiempoAcumulado >= tiempoCaida){
						tiempoAcumulado = 0;
						
						Pieza copia = piezaActual;
						copia.ubicacion[1] += 1;
						
						if(!hayColision(tablero, copia)){
							piezaActual.ubicacion[1] += 1;
							
							EstadoJuego estado;
							capturarEstado(tablero, piezaActual, piezaDelHold(holdTope),
										   !vacia(holdTope), puntaje, 0, estado);
							registrarEstado(replay, estado);
						} else {
							colocarPieza(tablero, piezaActual);
							
							if(hayFilaLlena(tablero)){
								animandoLimpieza = true;
								tiempoAnimacion = 0;
								
								EstadoJuego estado;
								capturarEstado(tablero, piezaActual, piezaDelHold(holdTope),
											   !vacia(holdTope), puntaje, 0, estado);
								registrarEstado(replay, estado);
							} else {
								sacarNuevaPieza(genCabeza, genCola, piezaActual,
												piezasRestantesInvertido, controlesInvertidos,
												piezasRestantesBloqueo, rotacionBloqueada,
												piezasRestantesDoble, movimientoDoble);
								
								if(hayColision(tablero, piezaActual)){
									juegoTerminado = true;
								}
								
								EstadoJuego estado;
								capturarEstado(tablero, piezaActual, piezaDelHold(holdTope),
											   !vacia(holdTope), puntaje, 0, estado);
								registrarEstado(replay, estado);
							}
						}
					}
				}
				
				if(tiempoImagenLogro > 0){
					tiempoImagenLogro -= GetFrameTime();
				}
				if(tiempoAvisoEvento > 0){
					tiempoAvisoEvento -= GetFrameTime();
				}
				
			} else {
				if(!puntajeGuardado){
					agregarPuntaje(tablaPuntajes, nombreJugador, puntaje);
					ordenarPuntajes(tablaPuntajes, algoritmoActual);
					guardarArchivo(tablaPuntajes, "puntajes.txt");
					puntajeGuardado = true;
					
					NodoReplay* aux = replay.cabeza;
					totalPasosReplay = 0;
					while(aux != nullptr){ totalPasosReplay++; aux = aux->siguiente; }
				}
			}
			
			BeginDrawing();
			dibujarFondo();
			
			if(juegoTerminado){
				if(!verReplay){
					dibujarTablero(tablero);
					int accionGameOver = dibujarGameOver(tablaPuntajes, puntaje, algoritmoActual, tiempoQuicksort, tiempoInsercion);
					
					if(accionGameOver == 1){ EndDrawing(); break; }
					if(accionGameOver == 2){
						verReplay = true;
						iniciarReproduccion(replay);
						pasoActualReplay = 1;
					}
				} else {
					if(reproduciendoAuto){
						tiempoAcumuladoReplay += GetFrameTime();
						if(tiempoAcumuladoReplay >= velocidadReplay){
							tiempoAcumuladoReplay = 0;
							EstadoJuego resultado;
							if(avanzarReproduccion(replay, resultado)){
								pasoActualReplay++;
							} else {
								reproduciendoAuto = false;
							}
						}
					}
					
					int accion = dibujarPantallaReplay(replay.cursor->estado.filas,
													   replay.cursor->estado.piezaActual,
													   replay.cursor->estado.puntaje,
													   pasoActualReplay, totalPasosReplay,
													   reproduciendoAuto);
					
					EstadoJuego resultado;
					if(accion == 1){ if(avanzarReproduccion(replay, resultado)) pasoActualReplay++; }
					else if(accion == -1){ if(retrocederReproduccion(replay, resultado)) pasoActualReplay--; }
					else if(accion == 2){ reproduciendoAuto = !reproduciendoAuto; }
					else if(accion == 3){ verReplay = false; }
				}
			} else {
				dibujarTablero(tablero);
				
				if(animandoLimpieza){
					float progreso = tiempoAnimacion / duracionAnimacion;
					dibujarParpadeoLineas(tablero, progreso);
					dibujarFlashBorde(progreso);
				}
				
				if(!animandoLimpieza){
					dibujarPiezaActual(piezaActual);
				}
				
				dibujarPuntaje(puntaje);
				dibujarHold(holdTope);
				dibujarSiguientes(genCabeza);
				dibujarControles();
				dibujarImagenLogro(tiempoImagenLogro);
				dibujarAvisoEvento(eventoActivo, tiempoAvisoEvento);
			}
			
			EndDrawing();
		}
		
		liberarReplay(replay);
		
		descargarImagenLogro();
		cerrarVentana();
		
		return 0;
	}
