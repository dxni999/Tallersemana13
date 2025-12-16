# SGIC - Ruedas de Oro (Programación 1)

Sistema de Gestión de Inventario y Clientes (SGIC) para una concesionaria.

## Archivos

- `main.c`: menú principal.
- `funciones.h`: estructuras y prototipos.
- `funciones.c`: implementación (vehículos, ventas, persistencia).

## Persistencia

- Vehículos: `vehiculos.dat` (binario con `fwrite`/`fread`).
- Ventas: `ventas.txt` (texto con `fprintf`/`fscanf`).

## Compilar (Windows PowerShell)

Si tenés instalado `gcc` (MinGW / MSYS2):

```powershell
cd "c:\Users\Dxni999\Downloads\Tallersemana13"
gcc -std=c11 -Wall -Wextra -O2 main.c funciones.c -o sgic.exe
```

## Ejecutar

```powershell
cd "c:\Users\Dxni999\Downloads\Tallersemana13"
.\sgic.exe
```

Al salir, el programa guarda automáticamente los datos.
