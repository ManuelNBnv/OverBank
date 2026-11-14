#include "../include/overbank.h"
#include "../include/funciones.h"
#include "../include/ui.h"
#include <string>

using namespace std;

void overbank(Usuarios &usuario) {
  const char *opciones[] = {"Consultar saldo",          "Depositar dinero",
                            "Retirar dinero",           "Transferir dinero",
                            "Historial de movimientos", "Datos de mi cuenta",
                            "Cambiar PIN",              "Cerrar sesion"};

  while (true) {
    string subtitulo = "ID " + to_string(usuario.id) + "  |  " + usuario.nombre;
    int seleccion = uiMenu("MENU DE CUENTA", subtitulo, opciones, 8);

    switch (seleccion) {
    case 0:
      consultarSaldo(usuario);
      break;
    case 1:
      depositarDinero(usuario);
      break;
    case 2:
      retirarDinero(usuario);
      break;
    case 3:
      transferirDinero(usuario);
      break;
    case 4:
      verHistorial(usuario);
      break;
    case 5:
      mostrarDatosCuenta(usuario);
      break;
    case 6:
      cambiarContraseña(usuario);
      break;
    case 7:
      return;
    }
  }
}
