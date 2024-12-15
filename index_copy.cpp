#include <iostream>
#include <vector>
#include <utility> // pair<int, int>
#include <cctype> // islower(), isupper()
#include <unordered_map> // unordered_map<char, int> pieces_values = {{'P', 1}, {'N', 3}, {'B', 3}, {'R', 5}, {'Q', 9}, {'K', 100},{'p', -1}, {'n', -3}, {'b', -3}, {'r', -5}, {'q', -9}, {'k', -100}};
#include <algorithm> // max(), min()
#include <climits> // INT_MIN, INT_MAX

using namespace std;

vector<pair<pair<int, int>, pair<int, int>>> pawn_movements(char board[8][8], int x, int y) {
	vector<pair<pair<int, int>, pair<int, int>>> movements;

	char piece = board[x][y];

	if (piece == 'P') {
		if (x > 0 && board[x - 1][y] == '.') {
      movements.push_back({{x, y}, {x - 1, y}});
    }
		if (x == 6 && board[x - 2][y] == '.' && board[x - 1][y] == '.') {
			movements.push_back({{x, y}, {x - 2, y}});
		}
		if (x > 0 && y > 0 && islower(board[x - 1][y - 1])) {
			movements.push_back({{x, y}, {x - 1, y - 1}});
		}
		if (x > 0 && y < 7 && islower(board[x - 1][y + 1])) {
			movements.push_back({{x, y}, {x - 1, y + 1}});
		}
	} else if (piece == 'p') {
		if (x < 7 && board[x + 1][y] == '.') {
			movements.push_back({{x, y}, {x + 1, y}});
		}
		if (x == 1 && board[x + 2][y] == '.' && board[x + 1][y] == '.') {
			movements.push_back({{x, y}, {x + 2, y}});
		}
		if (x < 7 && y > 0 && isupper(board[x + 1][y - 1])) {
			movements.push_back({{x, y}, {x + 1, y - 1}});
		}
		if (x < 7 && y < 7 && isupper(board[x + 1][y + 1])) {
			movements.push_back({{x, y}, {x + 1, y + 1}});
		}
	}

		// cout << (elem.first.first == 0 && elem.first.second == 0 && elem.second.first == 0 && elem.second.first == 0) << endl;

	// cout << "Atenciooon!!!" << endl;
	// for (const auto& elem : movements) {
	// 	cout << "{{" << elem.first.first << ", " << elem.first.second << "}, {" << elem.second.first << ", " << elem.second.second << "}}\n";
	// }

	// cout << "Ey, mira esto" << (movements.empty()) << endl;

	// if (movements.empty()) {
	// 	movements.push_back({{-1, -1}, {-1, -1}});
	// 	cout << "Fiiiin" << endl;
	// }

	return movements;
}

vector<pair<pair<int, int>, pair<int, int>>> generate_movements(char board[8][8], bool white_turn) {
	vector<pair<pair<int, int>, pair<int, int>>> movements;

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (white_turn && isupper(board[x][y])) {
				if (board[x][y] == 'P') {
					auto pawn_moves = pawn_movements(board, x, y);
					if (!pawn_moves.empty()) {
						movements.insert(movements.end(), pawn_moves.begin(), pawn_moves.end());
					}
				}	else if (!white_turn && islower(board[x][y])) {
					if (board[x][y] == 'p') {
						auto pawn_moves = pawn_movements(board, x, y);
						if (!pawn_moves.empty()) {
							movements.insert(movements.end(), pawn_moves.begin(), pawn_moves.end());
						}
					}
				}
			}
		} 
	}

	cout << "Movimientos generados:" << endl;
	for (const auto& move : movements) {
		cout << "(" << move.first.first << ", " << move.first.second << ") -> " << "(" << move.second.first << ", " << move.second.second << ")" << endl;
	}

	return movements;
}

int evaluate_board(char board[8][8]) {
	int value = 0;
	unordered_map<char, int> pieces_values = {
		{'P', 1}, {'N', 3}, {'B', 3}, {'R', 5}, {'Q', 9}, {'K', 100},
		{'p', -1}, {'n', -3}, {'b', -3}, {'r', -5}, {'q', -9}, {'k', -100}
	};

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			char piece = board[i][j];
			if (pieces_values.count(piece)) {
				value += pieces_values[piece];
			}
		}
	}

	cout << value << endl;

	return value;
}

pair<int, pair<pair<int, int>, pair<int, int>>> minimax(char board[8][8], int depth, int alpha, int beta, bool white_turn) {
  if (depth == 0) {
    return {evaluate_board(board), {{-1, -1}, {-1, -1}}};  // Retorna evaluación y un movimiento ficticio
  }

  pair<int, pair<pair<int, int>, pair<int, int>>> best_move;
  if (white_turn) {
    best_move.first = INT_MIN;
    for (const auto& move : generate_movements(board, true)) {
      // Aplica movimiento (esto es solo una demostración, tendrías que implementarlo)
      board[move.second.first][move.second.second] = board[move.first.first][move.first.second];
      board[move.first.first][move.first.second] = '.';

      // Llamada recursiva al minimax
      int evaluation = minimax(board, depth - 1, alpha, beta, false).first;

      if (evaluation > best_move.first) {
        best_move.first = evaluation;
        best_move.second = move;  // El mejor movimiento encontrado
      }

      alpha = max(alpha, best_move.first);
      if (beta <= alpha) {
        break;  // Poda alfa-beta
      }

      // Deshacer el movimiento
      board[move.first.first][move.first.second] = board[move.second.first][move.second.second];
      board[move.second.first][move.second.second] = '.';
    }
  } else {
    best_move.first = INT_MAX;
    for (const auto& move : generate_movements(board, false)) {
      // Aplica movimiento (esto es solo una demostración, tendrías que implementarlo)
      board[move.second.first][move.second.second] = board[move.first.first][move.first.second];
      board[move.first.first][move.first.second] = '.';

      // Llamada recursiva al minimax
      int evaluation = minimax(board, depth - 1, alpha, beta, true).first;

      if (evaluation < best_move.first) {
        best_move.first = evaluation;
        best_move.second = move;  // El mejor movimiento encontrado
      }

      beta = min(beta, best_move.first);
      if (beta <= alpha) {
        break;  // Poda alfa-beta
      }

      // Deshacer el movimiento
      board[move.first.first][move.first.second] = board[move.second.first][move.second.second];
      board[move.second.first][move.second.second] = '.';
    }
  }

  return best_move;
}

void print_board(char board[8][8]) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      cout << board[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
}

void apply_move(char board[8][8], pair<pair<int, int>, pair<int, int>> movement) {
  auto origin = movement.first;
  auto destination = movement.second;
  
  int x1 = origin.first, y1 = origin.second;
  int x2 = destination.first, y2 = destination.second;
  
  char piece = board[x1][y1];
  board[x1][y1] = '.';  // Vacía la casilla de origen
  board[x2][y2] = piece;  // Coloca la pieza en la casilla de destino
}

int main() {
	char board[8][8] = {
		{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
		{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
		{'.', '.', '.', '.', '.', '.', '.', '.'},
		{'.', '.', '.', '.', '.', '.', '.', '.'},
		{'.', '.', '.', '.', '.', '.', '.', '.'},
		{'.', '.', '.', '.', '.', '.', '.', '.'},
		{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
		{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
	};

	int profundidad = 1;  // Profundidad de búsqueda para el algoritmo minimax
	bool turno_blanco = true;

	// Imprime el estado inicial del tablero
	cout << "Tablero inicial:" << endl;
	print_board(board);

	while (true) {
		auto [eval, best_move] = minimax(board, profundidad, INT_MIN, INT_MAX, turno_blanco);

		// cout << (best_move.first.first == -1 && best_move.second.first == -1) << endl;

		cout << "(" << best_move.first.first << ", " << best_move.first.second << ")" << endl;
		cout << "(" << best_move.second.first << ", " << best_move.second.second << ")" << endl;

		// Verifica si no hay más movimientos posibles
		if (best_move.first.first == -1 && best_move.second.first == -1) {
			cout << "No hay movimientos posibles. Fin del juego." << endl;
			break;  // Termina el ciclo si no hay movimientos posibles
		}

		// Aplica el mejor movimiento encontrado
		apply_move(board, best_move);

		// Muestra el tablero después del movimiento
		// cout << "Tablero despues del movimiento:" << endl;
		print_board(board);

		// Cambia el turno
		turno_blanco = !turno_blanco;
	}

	return 0;
}