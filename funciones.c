#include "funciones.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

/* =========================
   Utilidades de consola
   ========================= */

static void pausar(void) {
	printf("\nPresione ENTER para continuar...");
	fflush(stdout);
	(void)getchar();
}

static void limpiarLineaFinal(char *s) {
	size_t n = strlen(s);
	if (n > 0 && s[n - 1] == '\n') {
		s[n - 1] = '\0';
	}
}

static void leerTextoNoVacio(const char *prompt, char *dest, int tam) {
	for (;;) {
		printf("%s", prompt);
		if (fgets(dest, tam, stdin) == NULL) {
			dest[0] = '\0';
			continue;
		}
		limpiarLineaFinal(dest);

		/* Quitar espacios al inicio/fin (simple) */
		while (*dest && isspace((unsigned char)*dest)) {
			memmove(dest, dest + 1, strlen(dest));
		}
		while (strlen(dest) > 0 && isspace((unsigned char)dest[strlen(dest) - 1])) {
			dest[strlen(dest) - 1] = '\0';
		}

		if (strlen(dest) == 0) {
			printf("Entrada no valida: no puede estar vacia.\n");
			continue;
		}
		return;
	}
}

static int leerEnteroRango(const char *prompt, int min, int max) {
	char linea[128];
	long valor;
	char *fin;

	for (;;) {
		printf("%s", prompt);
		if (fgets(linea, sizeof(linea), stdin) == NULL) {
			continue;
		}
		limpiarLineaFinal(linea);
		if (strlen(linea) == 0) {
			printf("Entrada no valida.\n");
			continue;
		}
		valor = strtol(linea, &fin, 10);
		if (*fin != '\0') {
			printf("Entrada no valida: escriba un numero entero.\n");
			continue;
		}
		if (valor < min || valor > max) {
			printf("Fuera de rango (%d a %d).\n", min, max);
			continue;
		}
		return (int)valor;
	}
}

static float leerFlotanteRango(const char *prompt, float min, float max) {
	char linea[128];
	float valor;
	char *fin;

	for (;;) {
		printf("%s", prompt);
		if (fgets(linea, sizeof(linea), stdin) == NULL) {
			continue;
		}
		limpiarLineaFinal(linea);
		if (strlen(linea) == 0) {
			printf("Entrada no valida.\n");
			continue;
		}
		valor = strtof(linea, &fin);
		if (*fin != '\0') {
			printf("Entrada no valida: escriba un numero (use punto para decimales).\n");
			continue;
		}
		if (valor < min || valor > max) {
			printf("Fuera de rango (%.2f a %.2f).\n", min, max);
			continue;
		}
		return valor;
	}
}

static int equalsIgnoreCase(const char *a, const char *b) {
	while (*a && *b) {
		if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
			return 0;
		}
		a++;
		b++;
	}
	return *a == '\0' && *b == '\0';
}

static int contieneIgnoreCase(const char *texto, const char *patron) {
	/* Búsqueda simple por subcadena, sin librerías externas */
	char t[200];
	char p[60];
	size_t i;

	strncpy(t, texto, sizeof(t) - 1);
	t[sizeof(t) - 1] = '\0';
	strncpy(p, patron, sizeof(p) - 1);
	p[sizeof(p) - 1] = '\0';

	for (i = 0; t[i]; i++) t[i] = (char)tolower((unsigned char)t[i]);
	for (i = 0; p[i]; i++) p[i] = (char)tolower((unsigned char)p[i]);

	return strstr(t, p) != NULL;
}

static int obtenerAnioActual(void) {
	time_t ahora = time(NULL);
	struct tm *t = localtime(&ahora);
	if (t == NULL) return 2025;
	return t->tm_year + 1900;
}

static int buscarIndicePorId(const Vehiculo vehiculos[], int cantidad, int id) {
	int i;
	for (i = 0; i < cantidad; i++) {
		if (vehiculos[i].id == id) return i;
	}
	return -1;
}

static void imprimirCabeceraVehiculos(void) {
	printf("\n%-6s %-12s %-12s %-12s %-6s %-12s %-12s\n",
		   "ID", "Marca", "Modelo", "Tipo", "Anio", "Precio", "Estado");
	printf("----------------------------------------------------------------------\n");
}

static void imprimirVehiculo(const Vehiculo *v) {
	printf("%-6d %-12s %-12s %-12s %-6d %-12.2f %-12s\n",
		   v->id, v->marca, v->modelo, v->tipo, v->anio, v->precio, v->estado);
}

/* =========================
   Menú
   ========================= */

void mostrarMenu(void) {
	printf("\n===== SGIC - Ruedas de Oro =====\n");
	printf("1) Registrar vehiculo\n");
	printf("2) Mostrar todos los vehiculos\n");
	printf("3) Buscar vehiculos\n");
	printf("4) Mostrar vehiculos disponibles\n");
	printf("5) Registrar venta\n");
	printf("6) Guardar datos en archivos\n");
	printf("7) Cargar datos desde archivos\n");
	printf("0) Salir\n");
}

/* =========================
   Vehículos
   ========================= */

int registrarVehiculo(Vehiculo vehiculos[], int *cantidad) {
	Vehiculo v;
	int anioActual;

	if (*cantidad >= MAX_VEHICULOS) {
		printf("No se pueden registrar mas vehiculos (max %d).\n", MAX_VEHICULOS);
		return 0;
	}

	anioActual = obtenerAnioActual();

	/* ID único */
	for (;;) {
		v.id = leerEnteroRango("Ingrese ID (entero positivo): ", 1, 1000000000);
		if (buscarIndicePorId(vehiculos, *cantidad, v.id) != -1) {
			printf("Ese ID ya existe. Debe ser unico.\n");
			continue;
		}
		break;
	}

	leerTextoNoVacio("Ingrese marca: ", v.marca, (int)sizeof(v.marca));
	leerTextoNoVacio("Ingrese modelo: ", v.modelo, (int)sizeof(v.modelo));
	leerTextoNoVacio("Ingrese tipo (Ej: Camioneta, Sedan): ", v.tipo, (int)sizeof(v.tipo));
	v.anio = leerEnteroRango("Ingrese anio: ", 1950, anioActual);
	v.precio = leerFlotanteRango("Ingrese precio: ", 0.01f, 1000000000.0f);
	strcpy(v.estado, "Disponible");

	vehiculos[*cantidad] = v;
	(*cantidad)++;

	printf("Vehiculo registrado correctamente.\n");
	return 1;
}

void mostrarTodosLosVehiculos(const Vehiculo vehiculos[], int cantidad) {
	int i;
	if (cantidad == 0) {
		printf("No hay vehiculos registrados.\n");
		return;
	}
	imprimirCabeceraVehiculos();
	for (i = 0; i < cantidad; i++) {
		imprimirVehiculo(&vehiculos[i]);
	}
}

void mostrarVehiculosDisponibles(const Vehiculo vehiculos[], int cantidad) {
	int i;
	int encontrados = 0;
	if (cantidad == 0) {
		printf("No hay vehiculos registrados.\n");
		return;
	}

	imprimirCabeceraVehiculos();
	for (i = 0; i < cantidad; i++) {
		if (equalsIgnoreCase(vehiculos[i].estado, "Disponible")) {
			imprimirVehiculo(&vehiculos[i]);
			encontrados++;
		}
	}

	if (encontrados == 0) {
		printf("(No hay vehiculos disponibles)\n");
	}
}

static void buscarPorMarca(const Vehiculo vehiculos[], int cantidad) {
	char marca[30];
	int i, encontrados = 0;

	leerTextoNoVacio("Ingrese marca a buscar: ", marca, (int)sizeof(marca));

	imprimirCabeceraVehiculos();
	for (i = 0; i < cantidad; i++) {
		if (contieneIgnoreCase(vehiculos[i].marca, marca)) {
			imprimirVehiculo(&vehiculos[i]);
			encontrados++;
		}
	}
	if (encontrados == 0) printf("(Sin resultados)\n");
}

static void buscarPorTipo(const Vehiculo vehiculos[], int cantidad) {
	char tipo[20];
	int i, encontrados = 0;

	leerTextoNoVacio("Ingrese tipo a buscar (Ej: Camioneta): ", tipo, (int)sizeof(tipo));

	imprimirCabeceraVehiculos();
	for (i = 0; i < cantidad; i++) {
		if (contieneIgnoreCase(vehiculos[i].tipo, tipo)) {
			imprimirVehiculo(&vehiculos[i]);
			encontrados++;
		}
	}
	if (encontrados == 0) printf("(Sin resultados)\n");
}

static void buscarPorPrecioMax(const Vehiculo vehiculos[], int cantidad) {
	float maximo;
	int i, encontrados = 0;

	maximo = leerFlotanteRango("Ingrese precio maximo: ", 0.0f, 1000000000.0f);

	imprimirCabeceraVehiculos();
	for (i = 0; i < cantidad; i++) {
		if (vehiculos[i].precio <= maximo) {
			imprimirVehiculo(&vehiculos[i]);
			encontrados++;
		}
	}
	if (encontrados == 0) printf("(Sin resultados)\n");
}

/* Caso de uso obligatorio:
   Buscar camionetas usadas marca Chevrolet con precio <= 14000
   Mostrar resultados y permitir registrar venta del vehículo elegido.
*/
static int casoDeUsoObligatorio(Vehiculo vehiculos[], int cantidad, Venta ventas[], int *cantidadVentas) {
	const char *marca = "Chevrolet";
	const char *tipo = "Camioneta";
	const float precioMax = 14000.0f;
	int anioActual = obtenerAnioActual();
	int i, encontrados = 0;
	int idElegido;

	printf("\n--- Caso de uso obligatorio ---\n");
	printf("Buscar: tipo=%s, marca=%s, usada (anio < %d), precio <= %.2f\n",
		   tipo, marca, anioActual, precioMax);

	if (cantidad == 0) {
		printf("No hay vehiculos registrados.\n");
		return 0;
	}

	imprimirCabeceraVehiculos();
	for (i = 0; i < cantidad; i++) {
		if (!equalsIgnoreCase(vehiculos[i].estado, "Disponible")) continue;
		if (!equalsIgnoreCase(vehiculos[i].marca, marca)) continue;
		if (!equalsIgnoreCase(vehiculos[i].tipo, tipo)) continue;
		if (!(vehiculos[i].anio < anioActual)) continue; /* usada */
		if (!(vehiculos[i].precio <= precioMax)) continue;
		imprimirVehiculo(&vehiculos[i]);
		encontrados++;
	}

	if (encontrados == 0) {
		printf("(Sin resultados para el caso de uso)\n");
		return 0;
	}

	idElegido = leerEnteroRango("\nIngrese el ID del vehiculo a vender (0 para cancelar): ", 0, 1000000000);
	if (idElegido == 0) {
		printf("Operacion cancelada.\n");
		return 0;
	}

	/* Reutilizamos la lógica de registrarVenta */
	return registrarVenta(vehiculos, cantidad, ventas, cantidadVentas);
}

void buscarVehiculo(const Vehiculo vehiculosConst[], int cantidad) {
	/* Para permitir el caso de uso, hacemos un pequeño "cast" seguro:
	   solo se usa en el caso de uso para registrar venta (cambia estado).
	   En el resto, solo se imprime. */
	Vehiculo *vehiculos = (Vehiculo *)vehiculosConst;
	static Venta ventasDummy[MAX_VENTAS];
	static int cantidadVentasDummy = 0;

	int opcion;
	if (cantidad == 0) {
		printf("No hay vehiculos registrados.\n");
		return;
	}

	for (;;) {
		printf("\n--- Buscar vehiculos ---\n");
		printf("1) Por marca\n");
		printf("2) Por tipo\n");
		printf("3) Por precio maximo\n");
		printf("4) Caso de uso: Camionetas usadas Chevrolet <= 14000\n");
		printf("0) Volver\n");

		opcion = leerEnteroRango("Opcion: ", 0, 4);
		switch (opcion) {
			case 1:
				buscarPorMarca(vehiculosConst, cantidad);
				pausar();
				break;
			case 2:
				buscarPorTipo(vehiculosConst, cantidad);
				pausar();
				break;
			case 3:
				buscarPorPrecioMax(vehiculosConst, cantidad);
				pausar();
				break;
			case 4:
				/* Nota: este llamado permite vender (cambia estado) */
				(void)casoDeUsoObligatorio(vehiculos, cantidad, ventasDummy, &cantidadVentasDummy);
				pausar();
				break;
			case 0:
				return;
		}
	}
}

/* =========================
   Ventas
   ========================= */

int registrarVenta(Vehiculo vehiculos[], int cantidad, Venta ventas[], int *cantidadVentas) {
	int id;
	int idx;
	Venta v;

	if (cantidad == 0) {
		printf("No hay vehiculos registrados.\n");
		return 0;
	}
	if (*cantidadVentas >= MAX_VENTAS) {
		printf("No se pueden registrar mas ventas (max %d).\n", MAX_VENTAS);
		return 0;
	}

	printf("\n--- Registrar venta ---\n");
	mostrarVehiculosDisponibles(vehiculos, cantidad);

	id = leerEnteroRango("\nIngrese el ID del vehiculo a vender (0 para cancelar): ", 0, 1000000000);
	if (id == 0) {
		printf("Operacion cancelada.\n");
		return 0;
	}

	idx = buscarIndicePorId(vehiculos, cantidad, id);
	if (idx == -1) {
		printf("No existe un vehiculo con ese ID.\n");
		return 0;
	}
	if (!equalsIgnoreCase(vehiculos[idx].estado, "Disponible")) {
		printf("Ese vehiculo no esta disponible (estado: %s).\n", vehiculos[idx].estado);
		return 0;
	}

	leerTextoNoVacio("Nombre del cliente: ", v.nombreCliente, (int)sizeof(v.nombreCliente));
	v.edad = leerEnteroRango("Edad del cliente: ", 0, 120);
	v.idVehiculo = vehiculos[idx].id;
	v.precioFinal = leerFlotanteRango("Precio final de venta: ", 0.01f, 1000000000.0f);

	/* Cambiar estado a Vendido */
	strcpy(vehiculos[idx].estado, "Vendido");

	ventas[*cantidadVentas] = v;
	(*cantidadVentas)++;

	/* Guardar la venta inmediatamente en el archivo txt (append) */
	{
		FILE *f = fopen(ARCH_VENTAS, "a");
		if (f == NULL) {
			printf("Aviso: no se pudo abrir %s para guardar la venta.\n", ARCH_VENTAS);
		} else {
			/* Formato: nombre;edad;idVehiculo;precioFinal */
			fprintf(f, "%s;%d;%d;%.2f\n", v.nombreCliente, v.edad, v.idVehiculo, v.precioFinal);
			fclose(f);
		}
	}

	printf("Venta registrada. Vehiculo %d marcado como Vendido.\n", v.idVehiculo);
	return 1;
}

/* =========================
   Persistencia
   ========================= */

int guardarVehiculosEnArchivo(const Vehiculo vehiculos[], int cantidad) {
	FILE *f = fopen(ARCH_VEHICULOS, "wb");
	if (f == NULL) {
		printf("No se pudo abrir %s para escritura.\n", ARCH_VEHICULOS);
		return 0;
	}

	if (fwrite(&cantidad, sizeof(int), 1, f) != 1) {
		fclose(f);
		printf("Error al escribir cantidad de vehiculos.\n");
		return 0;
	}

	if (cantidad > 0) {
		if (fwrite(vehiculos, sizeof(Vehiculo), (size_t)cantidad, f) != (size_t)cantidad) {
			fclose(f);
			printf("Error al escribir registros de vehiculos.\n");
			return 0;
		}
	}

	fclose(f);
	printf("Vehiculos guardados en %s.\n", ARCH_VEHICULOS);
	return 1;
}

int cargarVehiculosDesdeArchivo(Vehiculo vehiculos[], int *cantidad) {
	FILE *f = fopen(ARCH_VEHICULOS, "rb");
	int cant;

	if (f == NULL) {
		/* No es error fatal: puede ser que no exista todavía */
		*cantidad = 0;
		return 0;
	}

	if (fread(&cant, sizeof(int), 1, f) != 1) {
		fclose(f);
		printf("Error al leer cantidad de vehiculos.\n");
		*cantidad = 0;
		return 0;
	}

	if (cant < 0 || cant > MAX_VEHICULOS) {
		fclose(f);
		printf("Archivo corrupto o cantidad invalida (%d).\n", cant);
		*cantidad = 0;
		return 0;
	}

	if (cant > 0) {
		if (fread(vehiculos, sizeof(Vehiculo), (size_t)cant, f) != (size_t)cant) {
			fclose(f);
			printf("Error al leer registros de vehiculos.\n");
			*cantidad = 0;
			return 0;
		}
	}

	fclose(f);
	*cantidad = cant;
	return 1;
}

int guardarVentasEnArchivoTxt(const Venta ventas[], int cantidadVentas) {
	int i;
	FILE *f = fopen(ARCH_VENTAS, "w");
	if (f == NULL) {
		printf("No se pudo abrir %s para escritura.\n", ARCH_VENTAS);
		return 0;
	}

	for (i = 0; i < cantidadVentas; i++) {
		fprintf(f, "%s;%d;%d;%.2f\n",
				ventas[i].nombreCliente,
				ventas[i].edad,
				ventas[i].idVehiculo,
				ventas[i].precioFinal);
	}

	fclose(f);
	printf("Ventas guardadas en %s.\n", ARCH_VENTAS);
	return 1;
}

int cargarVentasDesdeArchivoTxt(Venta ventas[], int *cantidadVentas) {
	FILE *f = fopen(ARCH_VENTAS, "r");
	int cont = 0;

	if (f == NULL) {
		*cantidadVentas = 0;
		return 0;
	}

	while (cont < MAX_VENTAS) {
		Venta v;
		int leidos;

		/* Lee: nombre;edad;id;precio */
		leidos = fscanf(f, " %49[^;];%d;%d;%f",
						v.nombreCliente, &v.edad, &v.idVehiculo, &v.precioFinal);
		if (leidos != 4) {
			break;
		}
		/* Consumir fin de línea si quedó */
		(void)fscanf(f, "%*[\n\r]");

		ventas[cont++] = v;
	}

	fclose(f);
	*cantidadVentas = cont;
	return 1;
}

