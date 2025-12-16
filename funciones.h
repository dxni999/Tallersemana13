#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>

#define MAX_VEHICULOS 1000
#define MAX_VENTAS 2000

#define ARCH_VEHICULOS "vehiculos.dat"
#define ARCH_VENTAS "ventas.txt"

typedef struct {
	int id;
	char marca[30];
	char modelo[30];
	char tipo[20];      /* Ej: Camioneta, Sedan */
	int anio;
	float precio;
	char estado[20];    /* "Disponible" / "Vendido" */
} Vehiculo;

typedef struct {
	char nombreCliente[50];
	int edad;
	int idVehiculo;
	float precioFinal;
} Venta;

/* Menú principal */
void mostrarMenu(void);

/* Vehículos */
int registrarVehiculo(Vehiculo vehiculos[], int *cantidad);
void mostrarTodosLosVehiculos(const Vehiculo vehiculos[], int cantidad);
void buscarVehiculo(const Vehiculo vehiculos[], int cantidad);
void mostrarVehiculosDisponibles(const Vehiculo vehiculos[], int cantidad);

/* Ventas */
int registrarVenta(Vehiculo vehiculos[], int cantidad, Venta ventas[], int *cantidadVentas);

/* Persistencia */
int guardarVehiculosEnArchivo(const Vehiculo vehiculos[], int cantidad);
int cargarVehiculosDesdeArchivo(Vehiculo vehiculos[], int *cantidad);

int guardarVentasEnArchivoTxt(const Venta ventas[], int cantidadVentas);
int cargarVentasDesdeArchivoTxt(Venta ventas[], int *cantidadVentas);

#endif

