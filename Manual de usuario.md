# Documentación Completa de Pancracio

Guía completa para programar en Pancracio, el lenguaje de programación con sintaxis culinaria en español y fácil.

## Tabla de Contenidos

1. [Introducción](#introducción)
2. [Conceptos Básicos](#conceptos-básicos)
3. [Variables (Ingredientes)](#variables-ingredientes)
4. [Entrada/Salida](#entradasalida)
5. [Operadores y Expresiones](#operadores-y-expresiones)
6. [Control de Flujo](#control-de-flujo)
7. [Funciones (Recetas)](#funciones-recetas)
8. [Ventanas Gráficas](#ventanas-gráficas)
9. [Sistema de Persistencia (Libro)](#sistema-de-persistencia-libro)
10. [Juegos 2D](#juegos-2d)
11. [Bot de Discord](#bot-de-discord)
12. [Ejemplos Completos](#ejemplos-completos)

## Introducción

Pancracio es un lenguaje de programación diseñado para ser intuitivo y fácil de aprender, utilizando terminología culinaria en español. Cada programa es como una "receta" que combina "ingredientes" (variables) para crear el resultado deseado.

### Filosofía del Lenguaje
- **Hornear**: Mostrar información en pantalla
- **Ingredientes**: Variables que almacenan datos
- **Recetas**: Funciones definidas por el usuario
- **Cocinar**: Ejecutar una función
- **Probar**: Estructuras condicionales
- **Libro**: Sistema de persistencia de datos

## Conceptos Básicos

### Estructura de un Programa
```pancracio
# Esto es un comentario
hornear "¡Hola, mundo!"

# Declarar variables
ingrediente nombre = "Juan"
ingrediente edad = 25

# Mostrar información
hornear "Mi nombre es " + nombre + " y tengo " + edad + " años"
```

### Comentarios
```pancracio
# Comentario de una línea
hornear "Esto se ejecuta"  # Comentario al final de línea
```

### Palabras Reservadas
- `hornear` - Mostrar en pantalla
- `ingrediente` - Declarar variable
- `probar` - Condicional if
- `sino` - Condicional else if
- `alternativa` - Condicional else
- `listo` - Cerrar bloque
- `receta` - Definir función
- `cocinar` - Llamar función
- `mostrador` - Crear ventana
- `escribir` - Escribir en ventana
- `libro` - Operaciones de persistencia
- `juego` - Crear ventana de juego
- `objeto` - Crear objeto de juego

## Variables (Ingredientes)

### Tipos de Datos
Pancracio maneja automáticamente diferentes tipos de datos:

```pancracio
# Números enteros
ingrediente contador = 10
ingrediente puntos = -5

# Números decimales
ingrediente precio = 29.99
ingrediente temperatura = -3.5

# Texto
ingrediente nombre = "María"
ingrediente mensaje = "¡Bienvenido!"

# Listas (arrays)
ingrediente colores = ["rojo", "verde", "azul"]
ingrediente numeros = [1, 2, 3, 4, 5]
```

### Entrada de Datos
```pancracio
# Pedir número al usuario
ingrediente edad = pedir_numero("¿Cuántos años tienes? ")

# Pedir texto al usuario  
ingrediente nombre = pedir_nombre("¿Cómo te llamas? ")

# Con mensaje personalizado
ingrediente puntos = pedir_numero()  # Mensaje por defecto
```

### Operaciones con Variables
```pancracio
ingrediente a = 10
ingrediente b = 5

ingrediente suma = a + b
ingrediente resta = a - b  
ingrediente multiplicacion = a * b
ingrediente division = a / b
ingrediente modulo = a % b
```

## Entrada/Salida

### Mostrar Información
```pancracio
# Texto simple
hornear "Hola mundo"

# Variables
ingrediente nombre = "Carlos"
hornear nombre

# Concatenación de texto
hornear "Mi nombre es " + nombre

# Múltiples variables
ingrediente edad = 30
hornear "Soy " + nombre + " y tengo " + edad + " años"
```

## Operadores y Expresiones

### Operadores Aritméticos
```pancracio
ingrediente resultado = 10 + 5    # Suma: 15
ingrediente resultado = 10 - 5    # Resta: 5
ingrediente resultado = 10 * 5    # Multiplicación: 50
ingrediente resultado = 10 / 5    # División: 2
ingrediente resultado = 10 % 3    # Módulo: 1
```

### Operadores de Comparación
```pancracio
probar 10 == 10:    # Igual a
    hornear "Son iguales"
listo

probar 10 != 5:     # Diferente de
    hornear "Son diferentes"
listo

probar 10 > 5:      # Mayor que
    hornear "10 es mayor que 5"
listo

probar 5 < 10:      # Menor que
    hornear "5 es menor que 10"
listo

probar 10 >= 10:    # Mayor o igual
    hornear "10 es mayor o igual a 10"
listo

probar 5 <= 10:     # Menor o igual
    hornear "5 es menor o igual a 10"
listo
```

### Números Aleatorios
```pancracio
# Número aleatorio entre 1 y 100
ingrediente numero = aleatorio(1, 100)
hornear "Número aleatorio: " + numero

# Usar en juegos
ingrediente posicion_x = aleatorio(0, 800)
ingrediente posicion_y = aleatorio(0, 600)
```

## Control de Flujo

### Condicionales
```pancracio
ingrediente edad = pedir_numero("¿Cuántos años tienes? ")

probar edad >= 18:
    hornear "Eres mayor de edad"
sino edad >= 13:
    hornear "Eres adolescente"
alternativa:
    hornear "Eres menor de edad"
listo
```

### Múltiples Condiciones
```pancracio
ingrediente nota = pedir_numero("Ingresa tu nota: ")

probar nota >= 90:
    hornear "Excelente"
sino nota >= 80:
    hornear "Muy bien"
sino nota >= 70:
    hornear "Bien"
sino nota >= 60:
    hornear "Suficiente"
alternativa:
    hornear "Necesitas mejorar"
listo
```

## Funciones (Recetas)

### Definir Funciones
```pancracio
# Función sin parámetros
receta saludar():
    hornear "¡Hola desde una receta!"
listo

# Función con parámetros
receta presentar(nombre, edad):
    hornear "Mi nombre es " + nombre
    hornear "Tengo " + edad + " años"
listo

# Función con cálculos
receta calcular_area(ancho, alto):
    ingrediente area = ancho * alto
    hornear "El área es: " + area
listo
```

### Llamar Funciones
```pancracio
# Llamar función simple
cocinar saludar()

# Llamar con parámetros
cocinar presentar("Ana", 25)

# Llamar con variables
ingrediente nombre = "Pedro"
ingrediente edad = 30
cocinar presentar(nombre, edad)

# Llamar función de cálculo
cocinar calcular_area(10, 5)
```

## Ventanas Gráficas

### Crear Ventanas
```pancracio
# Ventana básica
mostrador "Mi Primera Ventana"

# Ventana con color personalizado
mostrador "Ventana Azul", "azul"

# Colores disponibles: rojo, verde, azul, amarillo, negro, blanco, gris, naranja, rosa, morado
```

### Escribir en Ventanas
```pancracio
# Crear ventana
mostrador "Mi Aplicación", "verde"

# Escribir texto
escribir "Mi Aplicación", "Bienvenido a mi programa"
escribir "Mi Aplicación", "Este es un mensaje en la ventana"

# Escribir variables
ingrediente usuario = "María"
escribir "Mi Aplicación", "Hola " + usuario
```

### Ejemplo Completo de Ventana
```pancracio
# Crear ventana
mostrador "Calculadora Simple", "azul"

# Pedir datos al usuario
ingrediente num1 = pedir_numero("Primer número: ")
ingrediente num2 = pedir_numero("Segundo número: ")

# Calcular
ingrediente suma = num1 + num2
ingrediente producto = num1 * num2

# Mostrar resultados en la ventana
escribir "Calculadora Simple", "Primer número: " + num1
escribir "Calculadora Simple", "Segundo número: " + num2
escribir "Calculadora Simple", "Suma: " + suma
escribir "Calculadora Simple", "Producto: " + producto
```

## Sistema de Persistencia (Libro)

El sistema de "libro" permite guardar y cargar datos de forma persistente en archivos `.pansos`.

### Operaciones Básicas
```pancracio
# Crear un nuevo archivo de datos
libro "crear", "mi_datos.pansos"

# Abrir archivo existente
libro "abrir", "mi_datos.pansos"

# Escribir datos
libro "escribir", "nombre", "Juan Pérez"
libro "escribir", "edad", "30"
libro "escribir", "activo", "true"

# Guardar cambios
libro "guardar"
```

### Leer Datos
```pancracio
# Abrir archivo
libro "abrir", "mi_datos.pansos"

# Leer y mostrar en pantalla
libro "leer", "nombre"

# Obtener valor en variable
libro "obtener", "nombre", "usuario"
libro "obtener", "edad", "años"

hornear "Usuario: " + usuario + ", Edad: " + años
```

### Operaciones con Listas
```pancracio
# Crear lista y agregar elementos
libro "agregar", "colores_favoritos", "rojo"
libro "agregar", "colores_favoritos", "azul"
libro "agregar", "colores_favoritos", "verde"

# Quitar elemento por valor
libro "quitar", "colores_favoritos", "azul"

# Quitar elemento por índice (0 = primer elemento)
libro "quitar", "colores_favoritos", "0"
```

### Verificar Existencia
```pancracio
libro "abrir", "config.pansos"

probar libro "existe", "usuario":
    libro "leer", "usuario"
alternativa:
    hornear "Usuario no configurado"
    libro "escribir", "usuario", "Invitado"
listo
```

### Operaciones de Mantenimiento
```pancracio
# Ver todo el contenido
libro "listar"

# Borrar una clave específica
libro "borrar", "dato_viejo"

# Limpiar todo el archivo
libro "limpiar"
```

## Juegos 2D

### Crear Ventana de Juego
```pancracio
# Crear ventana de juego
juego "Mi Primer Juego"

# Ventana de juego con título personalizado
juego "Aventura Espacial"
```

### Crear Objetos
```pancracio
# Crear objeto: ventana, nombre, x, y, ancho, alto, color
objeto "Mi Primer Juego", "jugador", 100, 100, 50, 50, "verde"
objeto "Mi Primer Juego", "enemigo", 300, 200, 30, 30, "rojo"
objeto "Mi Primer Juego", "item", 500, 150, 20, 20, "amarillo"
```

### Movimiento de Objetos
```pancracio
# Establecer velocidad: ventana, objeto, velocidad_x, velocidad_y
mover "Mi Primer Juego", "jugador", 2, 0    # Mover hacia la derecha
mover "Mi Primer Juego", "enemigo", -1, 1   # Mover izquierda y abajo
```

### Posicionamiento Manual
```pancracio
# Posicionar objeto: ventana, objeto, nueva_x, nueva_y
posicionar "Mi Primer Juego", "jugador", 400, 300  # Centro de pantalla
```

### Control con Teclado
```pancracio
juego "Juego Controlado"
objeto "Juego Controlado", "nave", 400, 300, 40, 40, "azul"

# Control de movimiento
probar tecla("izquierda"):
    mover "Juego Controlado", "nave", -5, 0
listo

probar tecla("derecha"):
    mover "Juego Controlado", "nave", 5, 0
listo

probar tecla("arriba"):
    mover "Juego Controlado", "nave", 0, -5
listo

probar tecla("abajo"):
    mover "Juego Controlado", "nave", 0, 5
listo

# Teclas disponibles: arriba, abajo, izquierda, derecha, espacio, a, s, d, w
```

### Ocultar/Mostrar Objetos
```pancracio
# Ocultar objeto
ocultar "Mi Primer Juego", "item"

# Para mostrar de nuevo, usa posicionar
posicionar "Mi Primer Juego", "item", 100, 100
```

## Bot de Discord (Aún en desarollo)

### Configuración Inicial
Primero debes cargar la dependencia de Discord:

```pancracio
# Cargar la receta de Discord
receta "la_receta_de_discord"
```

### Conectar Bot
```pancracio
# Conectar con token y bot ID
discord "conectar", "TU_BOT_TOKEN", "TU_BOT_ID"

# Verificar conexión
discord "conectado"  # Muestra true/false
```

### Enviar Mensajes
```pancracio
# Enviar mensaje a un canal
discord "enviar", "ID_DEL_CANAL", "¡Hola desde Pancracio!"

# Usar variables
ingrediente canal = "123456789012345678"
ingrediente mensaje = "Bot funcionando correctamente"
discord "enviar", canal, mensaje
```

### Configurar Estado del Bot
```pancracio
# Establecer estado y actividad
discord "estado", "online", "Programando en Pancracio"
discord "estado", "idle", "Esperando comandos"
discord "estado", "dnd", "No molestar - Compilando"
```

### Comandos Slash
```pancracio
# Registrar comando slash
discord "registrar_slash", "ping", "Verificar si el bot está activo"
discord "registrar_slash", "saludar", "El bot saluda al usuario"
discord "registrar_slash", "info", "Mostrar información del servidor"
```

### Comandos con Prefijo
```pancracio
# Configurar prefijo personalizado
discord "configurar_prefijo", "!"

# Registrar manejador de comando
discord "registrar_manejador", "mensaje", "manejar_comando_texto"
```

### Mantener Bot Activo
```pancracio
# Iniciar bot en modo activo (escucha eventos reales)
discord "iniciar_bot_real", "ID_DEL_CANAL_PRINCIPAL"

# Mantener bot corriendo indefinidamente
discord "mantener_activo"
```

### Ejemplo Completo de Bot
```pancracio
# Bot completo de Discord
receta "la_receta_de_discord"

# Configuración desde archivo de datos
libro "abrir", "config.pansos"
libro "obtener", "discord_token", "token"
libro "obtener", "discord_bot_id", "bot_id"
libro "obtener", "canal_principal", "canal"

# Conectar bot
discord "conectar", token, bot_id

# Configurar estado
discord "estado", "online", "¡Programado en Pancracio!"

# Registrar comandos
discord "registrar_slash", "hola", "Saludar desde Pancracio"
discord "configurar_prefijo", "!"

# Enviar mensaje de bienvenida
discord "enviar", canal, "¡Bot de Pancracio iniciado correctamente!"

# Mantener activo
discord "mantener_activo"
```

## Ejemplos Completos

### Ejemplo 1: Calculadora con Ventana
```pancracio
# Calculadora con interfaz gráfica
mostrador "Calculadora Pancracio", "verde"

receta calcular():
    ingrediente num1 = pedir_numero("Primer número: ")
    ingrediente num2 = pedir_numero("Segundo número: ")
    
    ingrediente suma = num1 + num2
    ingrediente resta = num1 - num2
    ingrediente multiplicacion = num1 * num2
    ingrediente division = num1 / num2
    
    escribir "Calculadora Pancracio", "=== RESULTADOS ==="
    escribir "Calculadora Pancracio", num1 + " + " + num2 + " = " + suma
    escribir "Calculadora Pancracio", num1 + " - " + num2 + " = " + resta
    escribir "Calculadora Pancracio", num1 + " × " + num2 + " = " + multiplicacion
    escribir "Calculadora Pancracio", num1 + " ÷ " + num2 + " = " + division
listo

escribir "Calculadora Pancracio", "¡Bienvenido a la Calculadora Pancracio!"
cocinar calcular()
```

### Ejemplo 2: Juego Simple
```pancracio
# Juego de esquivar obstáculos
juego "Esquivar"

objeto "Esquivar", "jugador", 400, 500, 40, 40, "verde"
objeto "Esquivar", "obstaculo1", 200, 0, 30, 30, "rojo"
objeto "Esquivar", "obstaculo2", 600, -100, 30, 30, "rojo"

# Los obstáculos caen
mover "Esquivar", "obstaculo1", 0, 3
mover "Esquivar", "obstaculo2", 0, 2

# Control del jugador
probar tecla("izquierda"):
    mover "Esquivar", "jugador", -4, 0
listo

probar tecla("derecha"):
    mover "Esquivar", "jugador", 4, 0
listo

probar tecla("espacio"):
    # Reiniciar posición de obstáculos
    posicionar "Esquivar", "obstaculo1", aleatorio(50, 750), -50
    posicionar "Esquivar", "obstaculo2", aleatorio(50, 750), -150
listo
```

### Ejemplo 3: Sistema de Usuarios
```pancracio
# Sistema de registro y login
libro "crear", "usuarios.pansos"

receta registrar_usuario():
    ingrediente nombre = pedir_nombre("Nombre de usuario: ")
    ingrediente email = pedir_nombre("Email: ")
    ingrediente edad = pedir_numero("Edad: ")
    
    # Verificar si ya existe
    probar libro "existe", nombre:
        hornear "El usuario ya existe"
    alternativa:
        libro "escribir", nombre + "_email", email
        libro "escribir", nombre + "_edad", edad
        libro "agregar", "lista_usuarios", nombre
        libro "guardar"
        hornear "Usuario registrado exitosamente"
    listo
listo

receta mostrar_usuarios():
    libro "obtener", "lista_usuarios", "usuarios"
    hornear "Usuarios registrados: " + usuarios
listo

receta mostrar_perfil():
    ingrediente usuario = pedir_nombre("¿Qué usuario quieres ver? ")
    
    probar libro "existe", usuario + "_email":
        libro "obtener", usuario + "_email", "email"
        libro "obtener", usuario + "_edad", "edad"
        hornear "=== PERFIL DE USUARIO ==="
        hornear "Usuario: " + usuario
        hornear "Email: " + email
        hornear "Edad: " + edad + " años"
    alternativa:
        hornear "Usuario no encontrado"
    listo
listo

# Programa principal
hornear "=== SISTEMA DE USUARIOS ==="
hornear "1. Registrar usuario"
hornear "2. Mostrar usuarios"
hornear "3. Ver perfil"

ingrediente opcion = pedir_numero("Elige una opción: ")

probar opcion == 1:
    cocinar registrar_usuario()
sino opcion == 2:
    cocinar mostrar_usuarios()
sino opcion == 3:
    cocinar mostrar_perfil()
alternativa:
    hornear "Opción no válida"
listo
```

### Ejemplo 4: Bot Discord Completo
```pancracio
# Bot de Discord con múltiples funciones

# Cargar dependencia
receta "la_receta_de_discord"

# Configuración (guardar estos datos en config.pansos)
libro "crear", "config.pansos"
libro "escribir", "discord_token", "TU_TOKEN_AQUI"
libro "escribir", "discord_bot_id", "TU_BOT_ID_AQUI"
libro "escribir", "canal_general", "ID_DEL_CANAL"
libro "guardar"

# Cargar configuración
libro "abrir", "config.pansos"
libro "obtener", "discord_token", "token"
libro "obtener", "discord_bot_id", "bot_id"
libro "obtener", "canal_general", "canal"

# Conectar bot
discord "conectar", token, bot_id
discord "estado", "online", "¡Funcionando con Pancracio!"

# Registrar comandos slash
discord "registrar_slash", "ping", "Verificar latencia del bot"
discord "registrar_slash", "info", "Información del bot"
discord "registrar_slash", "saludo", "Recibir un saludo personalizado"

# Configurar prefijo para comandos de texto
discord "configurar_prefijo", "!"

# Mensaje de bienvenida
discord "enviar", canal, "🤖 **Bot Pancracio iniciado**"
discord "enviar", canal, "Comandos disponibles:"
discord "enviar", canal, "• `/ping` - Verificar latencia"
discord "enviar", canal, "• `/info` - Información del bot"
discord "enviar", canal, "• `/saludo` - Saludo personalizado"
discord "enviar", canal, "• `!help` - Ayuda adicional"

# Mantener bot activo y escuchando
discord "mantener_activo"
```

## Mejores Prácticas

### Estructura de Código
```pancracio
# 1. Comentarios al inicio explicando el propósito
# Sistema de gestión de inventario
# Autor: Tu Nombre
# Fecha: 2024

# 2. Definir funciones al principio
receta mostrar_menu():
    hornear "=== MENÚ PRINCIPAL ==="
    hornear "1. Agregar producto"
    hornear "2. Ver inventario"
    hornear "3. Salir"
listo

# 3. Lógica principal al final
libro "abrir", "inventario.pansos"
cocinar mostrar_menu()
# ... resto del código
```

### Manejo de Errores
```pancracio
# Verificar existencia de archivos
probar libro "existe", "configuracion":
    libro "obtener", "configuracion", "config"
alternativa:
    hornear "Archivo de configuración no encontrado"
    # Crear configuración por defecto
    libro "escribir", "configuracion", "valor_por_defecto"
listo
```

### Nombres Descriptivos
```pancracio
# Buenos nombres
ingrediente nombre_usuario = "Juan"
ingrediente puntos_jugador = 100
ingrediente lista_productos = ["manzana", "pera", "uva"]

receta calcular_precio_total(precio_unitario, cantidad):
    ingrediente total = precio_unitario * cantidad
    hornear "Total a pagar: $" + total
listo
```

## Consejos de Depuración

### Mostrar Variables para Debug
```pancracio
ingrediente contador = 0
hornear "DEBUG: contador = " + contador

ingrediente lista = ["a", "b", "c"]
hornear "DEBUG: lista = " + lista
```

### Verificar Estados del Juego
```pancracio
juego "Mi Juego"
objeto "Mi Juego", "jugador", 100, 100, 50, 50, "verde"

# Verificar posición
hornear "DEBUG: Jugador en posición inicial"
```

### Verificar Conexiones Discord
```pancracio
discord "conectado", "estado_conexion"
probar estado_conexion == 1:
    hornear "Bot conectado correctamente"
alternativa:
    hornear "ERROR: Bot no conectado"
listo
```

## Limitaciones Conocidas

1. **Persistencia**: Los archivos `.pansos` deben guardarse manualmente con `libro "guardar"`
2. **Juegos**: El motor 2D es básico, sin física avanzada
3. **Discord**: Requiere conexión a internet y tokens válidos
4. **Ventanas**: Solo soporta texto simple, sin elementos GUI complejos
5. **Arrays**: Las listas son de tipo string internamente

## Extensiones Futuras

El lenguaje está diseñado para crecer. Características planificadas:
- Más tipos de datos nativos
- Mejor manejo de archivos
- Sistema de módulos más robusto
- Soporte para bases de datos
- API REST básica

## Solución de Problemas Comunes

### Error: "Variable no encontrada"
```pancracio
# MAL: usar variable sin declarar
hornear nombre  # Error si 'nombre' no existe

# BIEN: declarar antes de usar
ingrediente nombre = "María"
hornear nombre
```

### Error: "Archivo no encontrado"
```pancracio
# Siempre verificar existencia
probar libro "existe", "mi_archivo.pansos":
    libro "abrir", "mi_archivo.pansos"
alternativa:
    libro "crear", "mi_archivo.pansos"
listo
```

### Error: "Ventana no encontrada"
```pancracio
# Crear ventana antes de usarla
mostrador "Mi App", "azul"
escribir "Mi App", "Este mensaje funcionará"
```

### Bot Discord no responde
```pancracio
# Verificar token y permisos
discord "conectado"  # Debe mostrar true
# Verificar que el bot tenga permisos en el servidor
# Verificar que el canal ID sea correcto
```
---

¡Felicidades! Ahora tienes todas las herramientas para programar en Pancracio. Comienza con ejemplos simples y ve aumentando la complejidad gradualmente.
