//#include "Puntajes.h"
//
//Puntajes::Puntajes() {
//	
//}
//
#include "Puntajes.h"
#include <iostream>
#include <fstream>
using namespace std;

void agregarPuntaje(NodoPuntaje* &cabeza, string nombre, int puntaje){
	NodoPuntaje* nuevo = new NodoPuntaje{nombre, puntaje, nullptr};
	nuevo->siguiente = cabeza;
	cabeza = nuevo; // simplemente lo metemos al inicio, el orden real lo da ordenarQuicksort despues
}
	
	// Convierte la lista enlazada a un arreglo, para poder aplicar Quicksort clasico
	int contar(NodoPuntaje* cabeza){
		int c = 0;
		NodoPuntaje* aux = cabeza;
		while(aux != nullptr){
			c++;
			aux = aux->siguiente;
		}
		return c;
	}
		
		void quicksort(NodoPuntaje* arreglo[], int inicio, int fin){
			if(inicio >= fin) return;
			
			int pivote = arreglo[fin]->puntaje;
			int i = inicio - 1;
			
			for(int j = inicio; j < fin; j++){
				if(arreglo[j]->puntaje > pivote){ // > para orden DESCENDENTE (mayor a menor)
					i++;
					NodoPuntaje* temp = arreglo[i];
					arreglo[i] = arreglo[j];
					arreglo[j] = temp;
				}
			}
			
			NodoPuntaje* temp = arreglo[i+1];
			arreglo[i+1] = arreglo[fin];
			arreglo[fin] = temp;
			
			int posPivote = i + 1;
			
			quicksort(arreglo, inicio, posPivote - 1);
			quicksort(arreglo, posPivote + 1, fin);
		}
			
			void ordenarQuicksort(NodoPuntaje* &cabeza){
				int n = contar(cabeza);
				if(n <= 1) return;
				
				// pasamos los nodos a un arreglo de punteros (no copiamos los datos, solo las direcciones)
				NodoPuntaje** arreglo = new NodoPuntaje*[n];
				NodoPuntaje* aux = cabeza;
				for(int i = 0; i < n; i++){
					arreglo[i] = aux;
					aux = aux->siguiente;
				}
				
				quicksort(arreglo, 0, n - 1);
				
				// reconstruimos la lista enlazada en el nuevo orden
				cabeza = arreglo[0];
				for(int i = 0; i < n - 1; i++){
					arreglo[i]->siguiente = arreglo[i+1];
				}
				arreglo[n-1]->siguiente = nullptr;
				
				delete[] arreglo;
			}
				
				void limitarTop10(NodoPuntaje* &cabeza){
					// asume que la lista YA esta ordenada de mayor a menor
					if(cabeza == nullptr) return;
					
					NodoPuntaje* aux = cabeza;
					int contador = 1;
					
					while(aux != nullptr && contador < 10){
						aux = aux->siguiente;
						contador++;
					}
					
					if(aux != nullptr){
						// borramos todo lo que sigue despues del top 10
						NodoPuntaje* sobrante = aux->siguiente;
						aux->siguiente = nullptr;
						while(sobrante != nullptr){
							NodoPuntaje* borrar = sobrante;
							sobrante = sobrante->siguiente;
							delete borrar;
						}
					}
				}
					
					void imprimirTabla(NodoPuntaje* cabeza){
						NodoPuntaje* aux = cabeza;
						int pos = 1;
						while(aux != nullptr){
							cout << pos << ". " << aux->nombre << " - " << aux->puntaje << endl;
							aux = aux->siguiente;
							pos++;
						}
					}
						
						void guardarArchivo(NodoPuntaje* cabeza, string ruta){
							ofstream archivo(ruta);
							NodoPuntaje* aux = cabeza;
							while(aux != nullptr){
								archivo << aux->nombre << "," << aux->puntaje << endl;
								aux = aux->siguiente;
							}
							archivo.close();
						}
							
							void cargarArchivo(NodoPuntaje* &cabeza, string ruta){
								ifstream archivo(ruta);
								if(!archivo.is_open()) return; // primera vez que se corre, no existe el archivo aun
								
								string linea;
								while(getline(archivo, linea)){
									int coma = linea.find(',');
									string nombre = linea.substr(0, coma);
									int puntaje = stoi(linea.substr(coma + 1));
									agregarPuntaje(cabeza, nombre, puntaje);
								}
								archivo.close();
							}
								
							
