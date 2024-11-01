# Representación del tablero con caracteres
tablero = [
	["r", "n", "b", "q", "k", "b", "n", "r"],
	["p", "p", "p", "p", "p", "p", "p", "p"],
	[".", ".", ".", ".", ".", ".", ".", "."],
	[".", ".", ".", ".", ".", ".", ".", "."],
	[".", ".", ".", ".", ".", ".", ".", "."],
	[".", ".", ".", ".", ".", ".", ".", "."],
	["P", "P", "P", "P", "P", "P", "P", "P"],
	["R", "N", "B", "Q", "K", "B", "N", "R"]
]

def movimientos_peon(tablero, x, y):
	movimientos = []
	if tablero[x][y] == "P":  # Peón blanco
		# Movimiento hacia adelante
		if x > 0 and tablero[x - 1][y] == ".":
			movimientos.append(((x, y), (x - 1, y)))  # Avanza un espacio
		# Doble avance desde la posición inicial
		if x == 6 and tablero[x - 2][y] == "." and tablero[x - 1][y] == ".":
			movimientos.append(((x, y), (x - 2, y)))  # Avanza dos espacios
		# Capturas
		if x > 0 and y > 0 and tablero[x - 1][y - 1].islower():
			movimientos.append(((x, y), (x - 1, y - 1)))  # Captura diagonal izquierda
		if x > 0 and y < 7 and tablero[x - 1][y + 1].islower():
			movimientos.append(((x, y), (x - 1, y + 1)))  # Captura diagonal derecha
	elif tablero[x][y] == "p":  # Peón negro
		# Movimiento hacia adelante
		if x < 7 and tablero[x + 1][y] == ".":
			movimientos.append(((x, y), (x + 1, y)))  # Avanza un espacio
		# Doble avance desde la posición inicial
		if x == 1 and tablero[x + 2][y] == "." and tablero[x + 1][y] == ".":
			movimientos.append(((x, y), (x + 2, y)))  # Avanza dos espacios
		# Capturas
		if x < 7 and y > 0 and tablero[x + 1][y - 1].isupper():
			movimientos.append(((x, y), (x + 1, y - 1)))  # Captura diagonal izquierda
		if x < 7 and y < 7 and tablero[x + 1][y + 1].isupper():
			movimientos.append(((x, y), (x + 1, y + 1)))  # Captura diagonal derecha
	return movimientos

def generar_movimientos(tablero, turno_blanco=True):
	movimientos = []
	for x in range(8):
		for y in range(8):
			if turno_blanco and tablero[x][y].isupper():
				if tablero[x][y] == "P":
					movimientos.extend(movimientos_peon(tablero, x, y))
			elif not turno_blanco and tablero[x][y].islower():
				if tablero[x][y] == "p":
					movimientos.extend(movimientos_peon(tablero, x, y))

	print("Movimientos generados:", movimientos)  # Imprimir movimientos generados
	return movimientos

def evaluar_tablero(tablero):
	valor = 0
	valores_piezas = {"P": 1, "N": 3, "B": 3, "R": 5, "Q": 9, "K": 100,
										"p": -1, "n": -3, "b": -3, "r": -5, "q": -9, "k": -100}
	for fila in tablero:
		for pieza in fila:
			if pieza != ".":
				valor += valores_piezas.get(pieza, 0)
	return valor

def minimax(tablero, profundidad, alfa, beta, turno_blanco):
	if profundidad == 0:
		return evaluar_tablero(tablero), None

	mejor_movimiento = None
	if turno_blanco:
		max_eval = float('-inf')
		for movimiento in generar_movimientos(tablero, True):
			# Aplica movimiento
			evaluacion = minimax(tablero, profundidad - 1, alfa, beta, False)[0]
			if evaluacion > max_eval:
				max_eval = evaluacion
				mejor_movimiento = movimiento
			alfa = max(alfa, evaluacion)
			if beta <= alfa:
				break
		return max_eval, mejor_movimiento
	else:
		min_eval = float('inf')
		for movimiento in generar_movimientos(tablero, False):
			# Aplica movimiento
			evaluacion = minimax(tablero, profundidad - 1, alfa, beta, True)[0]
			if evaluacion < min_eval:
				min_eval = evaluacion
				mejor_movimiento = movimiento
			beta = min(beta, evaluacion)
			if beta <= alfa:
				break
		return min_eval, mejor_movimiento

def imprimir_tablero(tablero):
	for fila in tablero:
		print(" ".join(fila))
	print("\n")

def aplicar_movimiento(tablero, movimiento):
	origen, destino = movimiento
	x1, y1 = origen
	x2, y2 = destino
	pieza = tablero[x1][y1]
	tablero[x1][y1] = "."  # Vacía la casilla de origen
	tablero[x2][y2] = pieza  # Coloca la pieza en la casilla de destino

profundidad = 1  # Profundidad de búsqueda para el algoritmo minimax
turno_blanco = True

# Imprime el estado inicial del tablero
print("Tablero inicial:")
imprimir_tablero(tablero)

while True:
	_, mejor_movimiento = minimax(tablero, profundidad, float('-inf'), float('inf'), turno_blanco)

	if mejor_movimiento is None:
		print("No hay movimientos posibles. Fin del juego.")
		break

	# Aplica el mejor movimiento encontrado
	aplicar_movimiento(tablero, mejor_movimiento)

	# Muestra el tablero después del movimiento
	print("Tablero después del movimiento:")
	imprimir_tablero(tablero)

	# Cambia el turno
	turno_blanco = not turno_blanco
