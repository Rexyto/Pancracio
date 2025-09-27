# Pancracio Programming Language

[![Version](https://img.shields.io/badge/version-4.0-blue.svg)](https://github.com/Rexyto/pancracio)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Manual de usuario](https://img.shields.io/badge/Manual%20de%20usuario-📖-orange.svg)](https://github.com/Rexyto/Pancracio/blob/main/Manual%20de%20usuario.md)
[![Status](https://img.shields.io/badge/status-en%20desarrollo-yellow.svg)]()


<p align="center">
  <img src="logo/logo.ico" alt="Logo" width="200"/>
</p>




Pancracio es un lenguaje de programación que combina Python y JavaScript, con una sintaxis en español diseñada para ser intuitiva y fácil de aprender. Utiliza terminología culinaria en sus comandos principales, lo que hace que la programación sea más accesible y divertida.

## Características Principales

- **Persistencia de Datos**: Sistema nativo de archivos `.pansos` para almacenamiento de datos
- **Ventanas Gráficas**: Creación de interfaces de usuario con ventanas personalizables
- **Motor de Juegos 2D**: Sistema completo para crear juegos simples con detección de colisiones
- **Integración Discord**: Bot de Discord nativo con comandos slash y prefijo
- **Funciones Definidas por Usuario**: Sistema de funciones personalizables llamadas "recetas"

## Requisitos del Sistema

### Windows
- Windows 10 o superior
- MinGW-w64 con GCC 7.0+ (recomendado: MSYS2)
- Git (opcional, para clonar el repositorio)

### Dependencias de Compilación
- `libgdi32` - Para gráficos
- `libuser32` - Para ventanas
- `libkernel32` - Para sistema
- `libshell32` - Para shell
- `libcomctl32` - Para controles
- `libole32` - Para OLE
- `libwininet` - Para funcionalidad Discord

## Instalación

### Opción 1: Instalador Automático (Recomendado)
1. Descarga `pancracio.exe` desde las releases
2. Ejecuta como administrador
3. Sigue el asistente de instalación
4. El comando `pan` estará disponible en tu PATH

### Opción 2: Compilación Manual

#### 1. Clonar el Repositorio
```bash
git clone https://github.com/Rexyto/pancracio.git
cd pancracio
```

#### 2. Estructura Requerida
Asegúrate de tener esta estructura:
```
pancracio/
├── src/
│   ├── main.cpp
│   ├── interpreter.cpp
│   ├── command_executor.cpp
│   ├── book_manager.cpp
│   ├── dependency_manager.cpp
│   ├── expressions.cpp
│   ├── window_manager.cpp
│   ├── utils.cpp
│   ├── installer.cpp
│   ├── installer.rc
│   └── *.h (archivos de encabezado)
├── logo/
│   ├── logo.ico
│   └── logo2.ico
├── build.bat # si quieres poner todo el proceso de compilación de forma automática
└── documentation.md
```

#### 3. Compilar
```bash
# En Windows con MinGW/MSYS2
./build.bat

# O manualmente:
mkdir obj
g++ -std=c++17 -O2 -Wall -Isrc -c src/*.cpp -o obj/
g++ -std=c++17 -O2 -Wall -o pan_core.exe obj/*.o -lgdi32 -luser32 -lkernel32 -lshell32 -lcomctl32 -lole32 -lwininet
```

#### 4. Verificar Instalación
```bash
./pan_core.exe --version
```

## Uso Básico

### Ejecutar un Programa
```bash
pan archivo.pan
```

### Ejemplo Hola Mundo
```pancracio
# archivo: hola.pan
hornear "¡Hola, mundo desde Pancracio!"
```

### Ejecutar el Ejemplo
```bash
pan hola.pan
```

## Estructura del Proyecto

### Módulos Principales
- **`main.cpp`**: Punto de entrada del programa
- **`interpreter.cpp/h`**: Intérprete principal del lenguaje
- **`command_executor.cpp/h`**: Ejecutor de comandos y lógica principal
- **`book_manager.cpp/h`**: Manejo de archivos .pansos y persistencia
- **`dependency_manager.cpp/h`**: Sistema de dependencias y Discord
- **`expressions.cpp/h`**: Evaluador de expresiones y variables
- **`window_manager.cpp/h`**: Manejo de ventanas gráficas y juegos
- **`utils.cpp/h`**: Utilidades generales
- **`structures.h`**: Definición de estructuras de datos

### Archivos de Datos
- **`.pan`**: Archivos de código fuente Pancracio
- **`.pansos`**: Archivos de datos persistentes (Pancracio Storage)

### Librería Estándar: Libro_de_recetas
El `Libro_de_recetas` es el conjunto de dependencias y funciones nativas que amplían las capacidades de Pancracio:

- **Integración Discord** mediante la dependencia `la_receta_de_discord` *(aún en desarrollo)*

## Scripts de Compilación

### `build.bat` (Windows)
Script automatizado que:
1. Verifica la estructura de directorios
2. Compila todos los módulos
3. Enlaza las librerías necesarias
4. Genera el instalador si están presentes los recursos
5. Limpia archivos temporales
6. Verifica que el ejecutable funcione

### Compilación Manual por Módulos
```bash
# Compilar cada módulo
g++ -std=c++17 -O2 -Wall -Isrc -c src/utils.cpp -o obj/utils.o
g++ -std=c++17 -O2 -Wall -Isrc -c src/book_manager.cpp -o obj/book_manager.o
g++ -std=c++17 -O2 -Wall -Isrc -c src/dependency_manager.cpp -o obj/dependency_manager.o
g++ -std=c++17 -O2 -Wall -Isrc -c src/expressions.cpp -o obj/expressions.o
g++ -std=c++17 -O2 -Wall -Isrc -c src/window_manager.cpp -o obj/window_manager.o
g++ -std=c++17 -O2 -Wall -Isrc -c src/command_executor.cpp -o obj/command_executor.o
g++ -std=c++17 -O2 -Wall -Isrc -c src/interpreter.cpp -o obj/interpreter.o
g++ -std=c++17 -O2 -Wall -Isrc -c src/main.cpp -o obj/main.o

# Enlazar
g++ -o pan_core.exe obj/*.o -lgdi32 -luser32 -lkernel32 -lshell32 -lcomctl32 -lole32 -lwininet
```

## Solución de Problemas

### Errores Comunes

#### "Error: No se encontró la carpeta src/"
- Asegúrate de que todos los archivos fuente están en la carpeta `src/`
- Verifica que el archivo `build.bat` está en el directorio raíz

#### "Error compilando [archivo].cpp"
- Verifica que tienes MinGW-w64 instalado correctamente
- Asegúrate de que `g++` está en tu PATH
- Revisa que no falten archivos de encabezado

#### "Error enlazando el intérprete"
- Verifica que todas las librerías están disponibles
- En MSYS2: `pacman -S mingw-w64-x86_64-gcc`

#### "Se requieren permisos de administrador"
- Para instalar, ejecuta `pancracio.exe` como administrador
- Para desarrollo, puedes usar `pan_core.exe` sin permisos especiales

### Debugging
```bash
# Compilar con símbolos de debug
g++ -std=c++17 -g -Wall -Isrc -c src/*.cpp
g++ -g -o pan_core_debug.exe obj/*.o [librerías]

# Ejecutar con información de debug
gdb ./pan_core_debug.exe
```

## Configuración del Entorno

### MSYS2 (Recomendado para Windows)
```bash
# Instalar MSYS2
# Luego instalar herramientas necesarias:
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-make
pacman -S git

# Agregar al PATH:
# C:\msys64\mingw64\bin
```

### Variables de Entorno
El instalador automático configura:
- Agrega `C:\Program Files\Pancracio\bin` al PATH del sistema
- Asocia archivos `.pan` con el intérprete
- Asocia archivos `.pansos` con el editor de texto

## Desinstalación

### Con Instalador
1. Panel de Control > Programas y características
2. Seleccionar "Pancracio Programming Language"
3. Hacer clic en "Desinstalar"

### Manual
```bash
# Si usas el ejecutable directo
pan --uninstall

# O ejecutar el desinstalador
./uninstall.exe
```
## Licencia

Este proyecto está bajo la Licencia MIT. Ver el archivo `LICENSE` para más detalles.

## Soporte

- **Documentación**: Ver `documentation.md`
- **Issues**: [GitHub Issues](https://github.com/Rexyto/pancracio/issues)
- **Discusiones**: [GitHub Discussions](https://github.com/Rexyto/pancracio/discussions)

---

Muchas gracias por probar Pancracio!! ❤
