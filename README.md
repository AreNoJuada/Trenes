# Trenes
Documentación del Juego de Trenes

Repositorio Git: https://github.com/AreNoJuada/Trenes.git

Hecho por:
👨‍💻 MATHYUS OROZCO 👨‍💻 JUAN DAVID ARENAS 👨‍💻 MIGUEL CORTES

🎲 Juego de Trenes

Este es un pequeño proyecto en C++ que implementa una versión simplificada del “Juego de Trenes”, un juego de mesa donde los jugadores compiten por completar trayectos entre ciudades utilizando cartas de colores y vagones.

🧩 Contenido del juego
	•	1 tablero (virtual o físico)
	•	72 cartas (12 de cada color: rojo, azul, verde, morado, café, naranja)
	•	72 vagones distribuidos en partes iguales entre 4 jugadores (18 cada uno)

🎮 Reglas del juego

En cada turno, un jugador puede realizar una de dos acciones:
	1.	Robar dos cartas de la zona de cartas (las cartas robadas se reponen desde la baraja).
	2.	Realizar un trayecto si posee las cartas del color necesario (por ejemplo, 4 cartas rojas para el trayecto A-B). Las cartas utilizadas se descartan.

El juego termina cuando un jugador tiene menos de 5 trenes disponibles. Todos los jugadores completan la ronda actual y se procede al conteo de puntos.

🏆 Puntuación
	•	Trayectos de 2 vagones otorgan 1 punto.
	•	Trayectos de 3 vagones otorgan 2 puntos.
	•	Trayectos de 4 vagones otorgan 4 puntos.
	•	Trayectos de 5 vagones otorgan 6 puntos.
	•	Trayectos de 6 vagones otorgan 9 puntos.

Gana el jugador con mayor cantidad de puntos.

⸻

🛠️ Cómo ejecutar

Requisitos
	•	Compilador C++ (g++)

📁 Estructura del código

El archivo principal del juego es:
	•	principal.cpp: contiene la lógica principal del juego, incluyendo la administración de cartas, turnos y trayectos.

⸻

📌 Notas

Este proyecto fue desarrollado como ejercicio de programación para implementar lógica de juegos por turnos, manejo de recursos y puntuación. Aún puede expandirse con funcionalidades adicionales como interfaz gráfica o juego en línea.
