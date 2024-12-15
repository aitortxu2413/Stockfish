#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cctype>
using namespace std;

typedef pair<int, int> Movimiento;
typedef vector<vector<char>> Tablero;

// Representación del tablero con caracteres
Tablero tablero = {
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}};

vector<pair<Movimiento, Movimiento>> movimientos_peon(const Tablero &tablero, int x, int y)
{
  vector<pair<Movimiento, Movimiento>> movimientos;

  if (tablero[x][y] == 'P')
  { // Peón blanco
    if (x > 0 && tablero[x - 1][y] == '.')
    {
      movimientos.push_back({{x, y}, {x - 1, y}});
    }
    if (x == 6 && tablero[x - 2][y] == '.' && tablero[x - 1][y] == '.')
    {
      movimientos.push_back({{x, y}, {x - 2, y}});
    }
    if (x > 0 && y > 0 && islower(tablero[x - 1][y - 1]))
    {
      movimientos.push_back({{x, y}, {x - 1, y - 1}});
    }
    if (x > 0 && y < 7 && islower(tablero[x - 1][y + 1]))
    {
      movimientos.push_back({{x, y}, {x - 1, y + 1}});
    }
  }
  else if (tablero[x][y] == 'p')
  { // Peón negro
    if (x < 7 && tablero[x + 1][y] == '.')
    {
      movimientos.push_back({{x, y}, {x + 1, y}});
    }
    if (x == 1 && tablero[x + 2][y] == '.' && tablero[x + 1][y] == '.')
    {
      movimientos.push_back({{x, y}, {x + 2, y}});
    }
    if (x < 7 && y > 0 && isupper(tablero[x + 1][y - 1]))
    {
      movimientos.push_back({{x, y}, {x + 1, y - 1}});
    }
    if (x < 7 && y < 7 && isupper(tablero[x + 1][y + 1]))
    {
      movimientos.push_back({{x, y}, {x + 1, y + 1}});
    }
  }
  return movimientos;
}

vector<pair<Movimiento, Movimiento>> generar_movimientos(const Tablero &tablero, bool turno_blanco)
{
  vector<pair<Movimiento, Movimiento>> movimientos;

  for (int x = 0; x < 8; ++x)
  {
    for (int y = 0; y < 8; ++y)
    {
      if (turno_blanco && isupper(tablero[x][y]))
      {
        if (tablero[x][y] == 'P')
        {
          vector<pair<Movimiento, Movimiento>> movs = movimientos_peon(tablero, x, y);
          movimientos.insert(movimientos.end(), movs.begin(), movs.end());
        }
      }
      else if (!turno_blanco && islower(tablero[x][y]))
      {
        if (tablero[x][y] == 'p')
        {
          vector<pair<Movimiento, Movimiento>> movs = movimientos_peon(tablero, x, y);
          movimientos.insert(movimientos.end(), movs.begin(), movs.end());
        }
      }
    }
  }
  return movimientos;
}

int evaluar_tablero(const Tablero &tablero)
{
  int valor = 0;
  const int valores_piezas[128] = {0};
  const_cast<int *>(valores_piezas)['P'] = 1;
  const_cast<int *>(valores_piezas)['N'] = 3;
  const_cast<int *>(valores_piezas)['B'] = 3;
  const_cast<int *>(valores_piezas)['R'] = 5;
  const_cast<int *>(valores_piezas)['Q'] = 9;
  const_cast<int *>(valores_piezas)['K'] = 100;
  const_cast<int *>(valores_piezas)['p'] = -1;
  const_cast<int *>(valores_piezas)['n'] = -3;
  const_cast<int *>(valores_piezas)['b'] = -3;
  const_cast<int *>(valores_piezas)['r'] = -5;
  const_cast<int *>(valores_piezas)['q'] = -9;
  const_cast<int *>(valores_piezas)['k'] = -100;

  for (const auto &fila : tablero)
  {
    for (char pieza : fila)
    {
      if (pieza != '.')
      {
        valor += valores_piezas[pieza];
      }
    }
  }
  return valor;
}

pair<int, pair<Movimiento, Movimiento>> minimax(Tablero tablero, int profundidad, int alfa, int beta, bool turno_blanco)
{
  if (profundidad == 0)
  {
    return {evaluar_tablero(tablero), {}};
  }

  pair<Movimiento, Movimiento> mejor_movimiento;

  if (turno_blanco)
  {
    int max_eval = numeric_limits<int>::min();
    for (const auto &movimiento : generar_movimientos(tablero, true))
    {
      Tablero copia_tablero = tablero;
      auto [origen, destino] = movimiento;
      copia_tablero[destino.first][destino.second] = copia_tablero[origen.first][origen.second];
      copia_tablero[origen.first][origen.second] = '.';

      int evaluacion = minimax(copia_tablero, profundidad - 1, alfa, beta, false).first;
      if (evaluacion > max_eval)
      {
        max_eval = evaluacion;
        mejor_movimiento = movimiento;
      }
      alfa = max(alfa, evaluacion);
      if (beta <= alfa)
      {
        break;
      }
    }
    return {max_eval, mejor_movimiento};
  }
  else
  {
    int min_eval = numeric_limits<int>::max();
    for (const auto &movimiento : generar_movimientos(tablero, false))
    {
      Tablero copia_tablero = tablero;
      auto [origen, destino] = movimiento;
      copia_tablero[destino.first][destino.second] = copia_tablero[origen.first][origen.second];
      copia_tablero[origen.first][origen.second] = '.';

      int evaluacion = minimax(copia_tablero, profundidad - 1, alfa, beta, true).first;
      if (evaluacion < min_eval)
      {
        min_eval = evaluacion;
        mejor_movimiento = movimiento;
      }
      beta = min(beta, evaluacion);
      if (beta <= alfa)
      {
        break;
      }
    }
    return {min_eval, mejor_movimiento};
  }
}

void imprimir_tablero(const Tablero &tablero)
{
  for (const auto &fila : tablero)
  {
    for (char pieza : fila)
    {
      cout << pieza << ' ';
    }
    cout << endl;
  }
  cout << endl;
}

void aplicar_movimiento(Tablero &tablero, const pair<Movimiento, Movimiento> &movimiento)
{
  auto [origen, destino] = movimiento;
  tablero[destino.first][destino.second] = tablero[origen.first][origen.second];
  tablero[origen.first][origen.second] = '.';
}

int main()
{
  int profundidad = 1;
  bool turno_blanco = true;

  cout << "Tablero inicial:" << endl;
  imprimir_tablero(tablero);

  while (true)
  {
    auto [evaluacion, mejor_movimiento] = minimax(tablero, profundidad, numeric_limits<int>::min(), numeric_limits<int>::max(), turno_blanco);

    if (mejor_movimiento.first == make_pair(0, 0) && mejor_movimiento.second == make_pair(0, 0))
    {
      cout << "No hay movimientos posibles. Fin del juego." << endl;
      break;
    }

    aplicar_movimiento(tablero, mejor_movimiento);

    cout << "Tablero después del movimiento:" << endl;
    imprimir_tablero(tablero);

    turno_blanco = !turno_blanco;
  }

  return 0;
}
