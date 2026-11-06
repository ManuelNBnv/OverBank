#include "../include/users.h"
#include "../include/overbank.h"
#include "../include/storage.h"
#include <cctype>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

void guardadoDeUsuarios();
void iniciarSeccion();

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
  cout << "       LOGIN OVERBANK\n";
  cout << "-----------------------------\n";

  do {

    cout << "-----------------------------\n";
    cout << "Digita tu usuario\n";
    cout << "-----------------------------\n";

    getline(cin, usuarios[cantidadUsuarios].nombre);

  } while (usuarios[cantidadUsuarios].nombre.empty());

  cout << "-----------------------------\n";
  cout << "Digita una contraseña de 4 digitos\n";
  cout << "-----------------------------\n";

  cin >> usuarios[cantidadUsuarios].contraseña;

  usuarios[cantidadUsuarios].id = obtenerSiguienteID();

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

  system("clear");

  cout << "-----------------------------\n";
  cout << "Datos de cuenta\n";
  cout << "-----------------------------\n";

  cout << "Tu ID es: " << usuarios[cantidadUsuarios].id << "\n";
  cout << "TU nombre es: " << usuarios[cantidadUsuarios].nombre << "\n";
  cout << "TU contraseña es: " << usuarios[cantidadUsuarios].contraseña << "\n";
  cout << "TU saldo es: " << usuarios[cantidadUsuarios].saldo << "\n\n";

  guardarUsuario(usuarios[cantidadUsuarios]);
  cantidadUsuarios++;

  cout << "Recuerde su datos lo REDIRECCIONAREMOS al LOGIN en breve\n";

  sleep(10);
  system("clear");

  iniciarSeccion();
}

void iniciarSeccion() {

  int idBuscado;
  Usuarios usuarioEncontrado;
  string contraseñaIngresada;

  cout << "-----------------------------\n";
  cout << "       LOGIN OVERBANK\n";
  cout << "-----------------------------\n";

  cout << "Ingresa tu ID: ";
  cin >> idBuscado;

  if (cargarUsuario(idBuscado, usuarioEncontrado)) {
    cout << "Ingresa tu contraseña: \n";
    cin >> contraseñaIngresada;

    if (contraseñaIngresada == usuarioEncontrado.contraseña) {
      overbank();
    } else {
      cout << "Contraseña incorrecta\n";
    }

  } else {
    cout << "Usuario no encontrado\n";
  }
}
