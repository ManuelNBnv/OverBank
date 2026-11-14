#include "include/ui.h"
#include "include/users.h"

int main() {
  const char *opciones[] = {"Registrar usuario", "Iniciar sesion", "Salir"};

  uiIniciar();

  while (true) {
    int seleccion = uiMenu("CAJERO AUTOMATICO", "Bienvenido a OverBank", opciones, 3);

    if (seleccion == 0) {
      registrarUsuarios();
    } else if (seleccion == 1) {
      iniciarSeccion();
    } else {
      break;
    }
  }

  uiCerrar();
  return 0;
}
