# OverBank

Cajero automatico en consola, escrito en C++ con ncurses.

## Requisitos

- g++ con C++17
- libncurses

En Debian/Ubuntu:

```bash
sudo apt install g++ libncurses-dev
```

## Compilar y ejecutar

```bash
make
./overbank
```

## Uso

Navega con flechas y confirma con Enter. Esc vuelve atras o sale del menu.

- **Registrar usuario:** crea cuenta, PIN de 4 digitos y saldo inicial.
- **Iniciar sesion:** entra con ID y PIN.
- Dentro de la cuenta puedes consultar saldo, depositar, retirar, transferir, ver historial, consultar datos y cambiar el PIN.

Los datos se guardan en `data/users.txt` y `data/movimientos.txt`.
