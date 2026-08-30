#ifndef INTERFAZ_H
#define INTERFAZ_H

class interfaz {
public:
	interfaz();
private:
};


void iniciarVentana();                                  
void cerrarVentana();                                    

void dibujarTablero(NodoFila* cabeza);                     
void dibujarPiezaActual(Pieza p);                            
void dibujarSiguientes(NodoPieza* cabeza);                   
void dibujarHold(NodoHold* tope);                            
void dibujarPuntaje(int puntaje);                             

char colorDePieza(char tipo);                              
#endif

