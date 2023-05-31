#include <iostream>
#include <fstream>
#include <string>

using namespace std; 
int main() {
    fstream file;
    string filename = "example.txt";
    
    // Abre el archivo para lectura y escritura
    file.open(filename, std::ios::in | std::ios::out);
    
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo " << filename << std::endl;
        return 1;
    }
    
    // Escribe en el archivo
    file << "Hola mundo!" << std::endl;
    
    // Mueve el puntero de lectura/escritura al principio del archivo
    file.seekg(0);
    
    // Lee desde el archivo
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
    
    // Cierra el archivo
    file.close();
    
    return 0;
}