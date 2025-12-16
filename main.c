#include "funciones.h"
#include <stdlib.h> /* strtol */

int main(void) {
	Vehiculo vehiculos[MAX_VEHICULOS];
	Venta ventas[MAX_VENTAS];
	int cantidadVehiculos = 0;
	int cantidadVentas = 0;
	int opcion;

	/* Cargar datos al inicio (si existen) */
	(void)cargarVehiculosDesdeArchivo(vehiculos, &cantidadVehiculos);
	(void)cargarVentasDesdeArchivoTxt(ventas, &cantidadVentas);

	for (;;) {
		mostrarMenu();
		opcion = 0;

		/* Lectura simple de opción (evita scanf para no dejar basura en stdin) */
		{
			char linea[64];
			long val;
			char *fin;
			printf("Opcion: ");
			if (fgets(linea, sizeof(linea), stdin) == NULL) continue;
			val = strtol(linea, &fin, 10);
			if (fin == linea) {
				printf("Opcion invalida.\n");
				continue;
			}
			opcion = (int)val;
		}

		switch (opcion) {
			case 1:
				(void)registrarVehiculo(vehiculos, &cantidadVehiculos);
				break;

			case 2:
				mostrarTodosLosVehiculos(vehiculos, cantidadVehiculos);
				break;

			case 3:
				buscarVehiculo(vehiculos, cantidadVehiculos);
				break;

			case 4:
				mostrarVehiculosDisponibles(vehiculos, cantidadVehiculos);
				break;

			case 5:
				(void)registrarVenta(vehiculos, cantidadVehiculos, ventas, &cantidadVentas);
				break;

			case 6:
				(void)guardarVehiculosEnArchivo(vehiculos, cantidadVehiculos);
				(void)guardarVentasEnArchivoTxt(ventas, cantidadVentas);
				break;

			case 7:
				(void)cargarVehiculosDesdeArchivo(vehiculos, &cantidadVehiculos);
				(void)cargarVentasDesdeArchivoTxt(ventas, &cantidadVentas);
				printf("Datos cargados. Vehiculos: %d | Ventas: %d\n", cantidadVehiculos, cantidadVentas);
				break;

			case 0:
				/* Guardar antes de salir */
				(void)guardarVehiculosEnArchivo(vehiculos, cantidadVehiculos);
				(void)guardarVentasEnArchivoTxt(ventas, cantidadVentas);
				printf("Saliendo...\n");
				return 0;

			default:
				printf("Opcion invalida.\n");
				break;
		}
	}
}

