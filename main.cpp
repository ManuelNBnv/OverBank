
#include "include/users.h"
#include <iostream>
#include <unistd.h>

using namespace std;

int opcion;

int main() {

  cout << "========================\n";
  cout << "       OVERBANK\n";
  cout << "========================\n";
  cout << "1. Registrar usuario\n";
  cout << "2. Iniciar sesion\n";
  cout << "3. Salir\n\n";
  cout << "Selecciona una opcion: ";

  cin >> opcion;

  switch (opcion) {
  case 1:
    system("clear");
    registrarUsuarios();
    break;

  case 2:
    system("clear");
    iniciarSeccion();
    break;

  case 3:
    system("clear");
    cout << "Gracias por usar OverBank\n";
    sleep(3);
    return 0;
    break;

  default:
    cout << "Opcion no valida\n";
    break;
  }

  return 0;
}
