#include "Puntajes.h"
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

void agregarPuntaje(NodoPuntaje* &cabeza, string nombre, int puntaje){
	NodoPuntaje* nuevo = new NodoPuntaje{nombre, puntaje, nullptr};
	nuevo->siguiente = cabeza;
	cabeza = nuevo;
}
	
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
		if(arreglo[j]->puntaje > pivote){
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
	
	NodoPuntaje** arreglo = new NodoPuntaje*[n];
	NodoPuntaje* aux = cabeza;
	for(int i = 0; i < n; i++){
		arreglo[i] = aux;
		aux = aux->siguiente;
	}
	
	quicksort(arreglo, 0, n - 1);
	
	cabeza = arreglo[0];
	for(int i = 0; i < n - 1; i++){
		arreglo[i]->siguiente = arreglo[i+1];
	}
	arreglo[n-1]->siguiente = nullptr;
	
	delete[] arreglo;
}
void imprimirTabla(NodoPuntaje* cabeza){
	NodoPuntaje* aux = cabeza;
	int cantidad = 10;
	int pos = 1;
	while(aux != nullptr){
		if(cantidad > 0 && pos > cantidad) break;
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
	if(!archivo.is_open()) return;
	
	string linea;
	while(getline(archivo, linea)){
		if(!linea.empty() && linea.back() == '\r'){
			linea.pop_back();
		}
		if(linea.empty()) continue;
		
		int coma = linea.find(',');
		if(coma == (int)string::npos) continue;
		
		string nombre = linea.substr(0, coma);
		string textoPuntaje = linea.substr(coma + 1);
		if(textoPuntaje.empty()) continue;
		
		int puntaje = stoi(textoPuntaje);
		agregarPuntaje(cabeza, nombre, puntaje);
	}
	archivo.close();
}

void ordenarInsercion(NodoPuntaje* &cabeza){
	int n = contar(cabeza);
	if(n <= 1) return;
	
	NodoPuntaje** arreglo = new NodoPuntaje*[n];
	NodoPuntaje* aux = cabeza;
	for(int i = 0; i < n; i++){
		arreglo[i] = aux;
		aux = aux->siguiente;
	}
	
	for(int i = 1; i < n; i++){
		NodoPuntaje* actual = arreglo[i];
		int j = i - 1;
		
		while(j >= 0 && arreglo[j]->puntaje < actual->puntaje){
			arreglo[j+1] = arreglo[j];
			j--;
		}
		arreglo[j+1] = actual;
	}
	
	cabeza = arreglo[0];
	for(int i = 0; i < n - 1; i++){
		arreglo[i]->siguiente = arreglo[i+1];
	}
	arreglo[n-1]->siguiente = nullptr;
	
	delete[] arreglo;
}

void ordenarPuntajes(NodoPuntaje* &cabeza, TipoOrdenamiento tipo){
	if(tipo == ORDEN_QUICKSORT){
		ordenarQuicksort(cabeza);
	} else {
		ordenarInsercion(cabeza);
	}
}

double medirTiempoOrdenamiento(NodoPuntaje* cabeza, TipoOrdenamiento tipo){
	NodoPuntaje* copiaCabeza = nullptr;
	NodoPuntaje* copiaCola = nullptr;
	NodoPuntaje* aux = cabeza;
	
	while(aux != nullptr){
		NodoPuntaje* nuevo = new NodoPuntaje{aux->nombre, aux->puntaje, nullptr};
		if(copiaCabeza == nullptr){
			copiaCabeza = nuevo;
			copiaCola = nuevo;
		} else {
			copiaCola->siguiente = nuevo;
			copiaCola = nuevo;
		}
		aux = aux->siguiente;
	}
	
	auto inicio = chrono::high_resolution_clock::now();
	ordenarPuntajes(copiaCabeza, tipo);
	auto fin = chrono::high_resolution_clock::now();
	
	chrono::duration<double, milli> duracion = fin - inicio;
	
	
	aux = copiaCabeza;
	while(aux != nullptr){
		NodoPuntaje* borrar = aux;
		aux = aux->siguiente;
		delete borrar;
	}
	
	return duracion.count();
}
