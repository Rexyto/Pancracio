#include "interpreter.h"
#include <iostream>
#include <fstream>

PancracioInterpreter::PancracioInterpreter() {
    executor = new CommandExecutor();
}

PancracioInterpreter::~PancracioInterpreter() {
    delete executor;
}

void PancracioInterpreter::executeFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return;
    }
    
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    
    for (int i = 0; i < static_cast<int>(lines.size()); i++) {
        executor->executeLine(lines, i);
    }
    
    if (executor->hasWindows()) {
        std::cout << "Ventanas activas. Cierre las ventanas para salir o presione Ctrl+C." << std::endl;
        executor->runMessageLoop();
    }
}

void PancracioInterpreter::showVersion() {
    std::cout << "Pancracio v4.0 - Lenguaje de programacion culinario con persistencia de datos" << std::endl;
    std::cout << "Nuevas caracteristicas:" << std::endl;
    std::cout << "- Comando 'libro' para manejo de datos persistentes" << std::endl;
    std::cout << "- Archivos .pansos (Pancracio Storage) con formato nativo" << std::endl;
    std::cout << "- Operaciones CRUD completas para datos" << std::endl;
    std::cout << "- Soporte para enteros, decimales, texto y listas" << std::endl;
    std::cout << "- Ventanas de juego con graficos 2D" << std::endl;
    std::cout << "- Objetos de juego con fisica basica" << std::endl;
    std::cout << "- Deteccion de colisiones" << std::endl;
    std::cout << "- Manejo de entrada de teclado" << std::endl;
    std::cout << "- Numeros aleatorios" << std::endl;
    std::cout << std::endl;
    std::cout << "Comandos de libro:" << std::endl;
    std::cout << "- libro \"crear\", \"archivo.pansos\" - Crear archivo de datos" << std::endl;
    std::cout << "- libro \"abrir\", \"archivo.pansos\" - Abrir archivo existente" << std::endl;
    std::cout << "- libro \"guardar\" - Guardar cambios" << std::endl;
    std::cout << "- libro \"escribir\", \"clave\", \"valor\" - Escribir dato" << std::endl;
    std::cout << "- libro \"leer\", \"clave\" - Leer y mostrar dato" << std::endl;
    std::cout << "- libro \"obtener\", \"clave\", \"variable\" - Asignar a variable" << std::endl;
    std::cout << "- libro \"existe\", \"clave\" - Verificar existencia" << std::endl;
    std::cout << "- libro \"borrar\", \"clave\" - Eliminar dato" << std::endl;
    std::cout << "- libro \"agregar\", \"lista\", \"valor\" - Agregar a lista" << std::endl;
    std::cout << "- libro \"quitar\", \"lista\", \"valor/indice\" - Quitar de lista" << std::endl;
    std::cout << "- libro \"listar\" - Mostrar todo el contenido" << std::endl;
    std::cout << "- libro \"limpiar\" - Limpiar archivo" << std::endl;
    std::cout << std::endl;
    std::cout << "Comandos de juego:" << std::endl;
    std::cout << "- juego \"titulo\" - Crear ventana de juego" << std::endl;
    std::cout << "- objeto ventana, nombre, x, y, ancho, alto, color - Crear objeto" << std::endl;
    std::cout << "- mover ventana, objeto, vx, vy - Establecer velocidad" << std::endl;
    std::cout << "- posicionar ventana, objeto, x, y - Posicionar objeto" << std::endl;
    std::cout << "- tecla(\"tecla\") - Detectar tecla presionada" << std::endl;
    std::cout << "- aleatorio(min, max) - Numero aleatorio" << std::endl;
    std::cout << std::endl;
    std::cout << "Teclas soportadas: arriba, abajo, izquierda, derecha, espacio, a, s, d, w" << std::endl;
    std::cout << std::endl;
    std::cout << "Uso: pan archivo.pan" << std::endl;
}