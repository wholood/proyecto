#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
  // Abrir un archivo en modo de lectura
  ifstream archivo("archivosprueba/toto.in");

  // Verificar si el archivo se abrió correctamente
  if (!archivo.is_open()) {
    cout << "Error: no se pudo abrir el archivo." << endl;
    return 1;
  }

  // Leer datos del archivo
  int num,num2;
  archivo >> num;
  string nombres [20], ip [20];
  
  for (int i=0; i<num; i++){
    archivo >> nombres[i];
    archivo >> ip[i];
  }

  archivo >>num2;
  int salto[20], ping[20];
  string via[20];
  for (int i=0; i<num2; i++){
    string basura;
    archivo >> basura;
    archivo >> basura;
    archivo >> salto[i];
    archivo >> ping[i];
    archivo >> via[i];
  }
  

  // Cerrar el archivo
  archivo.close();

  // Crear o abrir un archivo en modo de escritura
  ofstream archivo2("archivosprueba/toto.out");

  // Verificar si el archivo se abrió correctamente
  if (!archivo2.is_open()) {
    cerr << "Error: no se pudo abrir el archivo." << endl;
    return 1;
  }

  // Escribir datos en el archivo
  archivo2<< num;
  for (int i=0; i<num; i++){
    archivo2 << nombres[i]<<" ";
    archivo2 << ip[i]<<endl;
  }

  archivo2<<num2;
  for (int i=0; i<num2; i++){
    archivo2 << "OJO" << endl;
    archivo2 << salto[i]<<" ";
    archivo2 << ping[i]<<" ";
    archivo2 << via[i]<<endl;
  }
  // Cerrar el archivo
  archivo2.close();

  return 0;
}