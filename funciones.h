// Definir límites máximos
#define MAX_CLIENTES 10
#define MAX_VEHICULOS 10
#define MAX_VENTAS 5

typedef struct{
    char nombre[50];
    int id;
    int edad;
}Cliente;

typedef struct{
    char marca[30];
    char modelo[30];
    char tipo[20]; // Camioneta, Sedan, SUV, etc.
    int anio;
    float precio;
    int usado; // 1 = usado, 0 = nuevo
    int disponible; // 1 = disponible, 0 = vendido
}Vehiculo;

typedef struct{
    Cliente cliente;
    Vehiculo vehiculo;
    int id;
}Venta;

// Funciones de entrada
void leerCadena(char *cadena, int n);
int leerEnteroConRango(int min, int max);
float leerFlotanteConRango(float min, float max); 
// Menú
int menu();
// Funciones de Cliente
void registrarCliente();
void listarClientes(Cliente *clientes);
void guardarCliente(Cliente *cliente);
int obtenerClientes(Cliente *clientes);
// Funciones de Vehículo
void registrarVehiculo();
void listarVehiculos(Vehiculo *vehiculos);
void guardarVehiculo(Vehiculo *vehiculo);
int obtenerVehiculos(Vehiculo *vehiculos);
void editarVehiculo();
void buscarVehiculosPorPreferencias();
// Funciones de Venta
void realizarVenta(); 
void listarVentas(Venta *ventas); 
void guardarVenta(Venta *venta); 
int obtenerVentas(Venta *ventas); 
