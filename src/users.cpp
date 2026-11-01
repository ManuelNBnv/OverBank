#include "../include/users.h"
#include "../include/storage.h"
#include <cctype>
#include <iostream>
#include <string>

using namespace std;

void guardadoDeUsuarios();

bool saldoValido = false;
bool esNumerica = true;
int ids = 1000;
Usuarios usuarios[100];
int cantidadUsuarios = 0;

void registrarUsuarios() {

  if (cantidadUsuarios >= 100) {
    cout << "Limite de usuarios alcanzado\n";
    return;
  }

  cout << "-----------------------------\n";
  cout << "BIENVENIDO OVERBANK\n";
  cout << "-----------------------------\n";

  do {

    cout << "Digita tu usuario\n";
    cout << "-----------------------------\n";

    getline(cin, usuarios[cantidadUsuarios].nombre);

  } while (usuarios[cantidadUsuarios].nombre.empty());

  do {
    esNumerica = true;

    cout << "-----------------------------\n";
    cout << "Digita una contraseña de 4 digitos\n";
    cout << "-----------------------------\n";

    cin >> usuarios[cantidadUsuarios].contraseña;

    for (int i = 0; i < usuarios[cantidadUsuarios].contraseña.length(); i++) {
      if (!isdigit(usuarios[cantidadUsuarios].contraseña[i])) {
        esNumerica = false;
      }
    }

  } while (usuarios[cantidadUsuarios].contraseña.length() != 4 ||
           esNumerica == false);

  usuarios[cantidadUsuarios].id = ids;
  ids++;

  do {
    cout << "-----------------------------\n";
    cout << "Digita un saldo inicial\n";
    cout << "-----------------------------\n";

    cin >> usuarios[cantidadUsuarios].saldo;

    if (cin.fail()) {
      cout << "-----------------------------\n";
      cout << "Digita solo numeros para el saldo iniciall\n";
      cout << "-----------------------------\n";

      saldoValido = false;

      cin.clear();
      cin.ignore(1000, '\n');
    } else {
      saldoValido = usuarios[cantidadUsuarios].saldo >= 0;
    }

  } while (!saldoValido);

  cout << "-----------------------------\n";
  cout << "Datos de cuenta\n";
  cout << "-----------------------------\n";

  cout << "Tu ID es: " << usuarios[cantidadUsuarios].id << "\n";
  cout << "TU nombre es: " << usuarios[cantidadUsuarios].nombre << "\n";
  cout << "TU contraseña es: " << usuarios[cantidadUsuarios].contraseña << "\n";
  cout << "TU saldo es: " << usuarios[cantidadUsuarios].saldo << "\n";

  guardarUsuario(usuarios[cantidadUsuarios]);
  cantidadUsuarios++;
}
