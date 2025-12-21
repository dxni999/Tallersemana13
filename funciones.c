#include <stdio.h>
#include <string.h>
#include "funciones.h"

void leerCadena(char *cadena, int n){
    fgets(cadena,n,stdin);
    int len = strlen(cadena) - 1;
    if(len >= 0 && cadena[len] == '\n') 
        cadena[len]='\0';
}
//funcion para leer enteros con rango
int leerEnteroConRango(int min, int max){
    int num;
    int val;        
    do{
        val = scanf("%d",&num);
        if (val != 1 || num < min || num > max)
        {
            printf("El dato ingresado no es valido\n");
            while(getchar() != '\n');
        }
    }while(val != 1 || num < min || num > max);
    while(getchar() != '\n');
    return num;
}

float leerFlotanteConRango(float min, float max){ 
    float num;
    int val;
    do{
        val = scanf("%f",&num);
        if (val != 1 || num < min || num > max)
        {
            printf("El dato ingresado no es valido\n");
            while(getchar() != '\n');
        }
    }while(val != 1 || num < min || num > max);
    while(getchar() != '\n');
    return num;
}

int menu(){
    printf("\n=== SISTEMA DE GESTION CONCESIONARIA RUEDAS DE ORO ===\n");
    printf("Seleccione una opcion:\n");
    printf("1. Registrar Cliente\n");
    printf("2. Mostrar Clientes\n");
    printf("3. Registrar Vehiculo\n");
    printf("4. Mostrar Vehiculos\n");
    printf("5. Editar Vehiculo\n");
    printf("6. Buscar Vehiculos por Preferencias\n");
    printf("7. Realizar Venta\n");
    printf("8. Mostrar Ventas\n");
    printf("9. Salir\n");
    printf(">> ");
    int opc = leerEnteroConRango(1,9);
    return opc;
}
// Funciones de Cliente 
// Registrar un nuevo cliente y validacion de ID unico
void registrarCliente(){
    Cliente cliente;
    Cliente clientes[MAX_CLIENTES];
    int count = obtenerClientes(clientes);
    
    printf("Ingrese el ID del cliente (1-999): ");
    int idBuscado = leerEnteroConRango(1,999);
    
    for(int i = 0; i < count; i++){
        if(clientes[i].id == idBuscado){
            printf("ERROR: Ya existe un cliente con ese ID\n");
            return;
        }
    }
    // Asignar datos al nuevo cliente
    cliente.id = idBuscado;
    printf("Ingrese el nombre del cliente: ");
    leerCadena(cliente.nombre,50);
    printf("Ingrese la edad del cliente(18-120): ");
    cliente.edad = leerEnteroConRango(18,120);
    guardarCliente(&cliente);
    printf("Cliente registrado exitosamente\n"); 
}
// Listar todos los clientes registrados
void listarClientes(Cliente *clientes){
    int c = obtenerClientes(clientes);//c guarda clientes
    if(c == 0){ 
        printf("No hay clientes registrados\n");
        return;
    }
    printf("\n--- LISTA DE CLIENTES ---\n");
    printf("%-5s %-5s %-30s %-10s\n", "#", "ID", "Nombre", "Edad");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < c; i++)
    {
        printf("%-5d %-5d %-30s %-10d\n",i,
                                    clientes[i].id,
                                    clientes[i].nombre,
                                    clientes[i].edad);
    }
}
// Guardar un nuevo cliente en el archivo clientes.dat
void guardarCliente(Cliente *cliente){
    Cliente clientes[MAX_CLIENTES];
    int count = obtenerClientes(clientes);
    
    // Agregar el nuevo cliente al arreglo
    clientes[count] = *cliente;
    count++;
    
    // Sobreescribir el archivo completo
    FILE *f;
    f = fopen("clientes.dat","wb");//abre archivo en modo escritura binaria (wb)
    if (f==NULL)//verifica si el archivo se abrio correctamente
    {
        printf("No se pudo abrir el archivo\n");
        return;
    }
    fwrite(clientes,sizeof(Cliente),count,f);//escribe el arreglo completo en el archivo
    fclose(f);
}

int obtenerClientes(Cliente *clientes){
    FILE *f;
    f = fopen("clientes.dat","rb"); //Abre el archivo en modo lectura binaria (rb)
    if (f==NULL)
    {
        return 0; 
    }
    int count = fread(clientes,sizeof(Cliente),MAX_CLIENTES,f);//fileread
    fclose(f);
    return count;
}

//Funciones de Vehiculo
void registrarVehiculo(){
    Vehiculo vehiculo;
    printf("Ingrese la marca del vehiculo: ");
    leerCadena(vehiculo.marca,30);
    printf("Ingrese el modelo del vehiculo: ");
    leerCadena(vehiculo.modelo,30);
    printf("Ingrese el tipo de vehiculo (Camioneta/Sedan/SUV/etc): ");
    leerCadena(vehiculo.tipo,20);
    printf("Ingrese el año del vehiculo (1900-2026): ");
    vehiculo.anio = leerEnteroConRango(1900,2026);
    printf("Ingrese el precio del vehiculo (1000-500000): ");
    vehiculo.precio = leerFlotanteConRango(1000,500000); 
    printf("Es vehiculo usado? (1.-Si / 0.-No): ");
    vehiculo.usado = leerEnteroConRango(0,1);
    vehiculo.disponible = 1;// Por defecto, el vehiculo se registra como disponible
    guardarVehiculo(&vehiculo); 
    printf("Vehiculo registrado exitosamente\n"); 
}
// Listar todos los vehiculos registrados
void listarVehiculos(Vehiculo *vehiculos){
    int c = obtenerVehiculos(vehiculos);// c guarda vehiculos
    if(c == 0){ 
        printf("No hay vehiculos registrados\n");
        return;
    }
    printf("\n--- INVENTARIO DE VEHICULOS ---\n");
    printf("%-5s %-15s %-15s %-15s %-10s %-12s %-10s %-12s\n", "#", "Marca", "Modelo", "Tipo", "Anio", "Precio", "Condicion", "Estado");
    printf("--------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < c; i++)
    {
        printf("%-5d %-15s %-15s %-15s %-10d $%-11.2f %-10s %-12s\n",i,
                                        vehiculos[i].marca,
                                        vehiculos[i].modelo,
                                        vehiculos[i].tipo,
                                        vehiculos[i].anio,
                                        vehiculos[i].precio,
                                        vehiculos[i].usado ? "Usado" : "Nuevo",
                                        vehiculos[i].disponible ? "Disponible" : "Vendido"); 
    }
}
// Guardar un nuevo vehiculo en el archivo vehiculos.dat
    void guardarVehiculo(Vehiculo *vehiculo){
        FILE *f;
        f = fopen("vehiculos.dat","ab");//abre archivo en modo agregar binario ab
        if (f==NULL)
        {
            printf("No se pudo abrir el archivo\n");
            return;
        }
        fwrite(vehiculo,sizeof(Vehiculo),1,f);
        fclose(f);
    }
// Obtener todos los vehiculos registrados desde el archivo
int obtenerVehiculos(Vehiculo *vehiculos){
    FILE *f;
    f = fopen("vehiculos.dat","rb");
    if (f==NULL)
    {
        return 0; 
    }
    int count = fread(vehiculos,sizeof(Vehiculo),MAX_VEHICULOS,f); 
    fclose(f);
    return count;
}
// Editar un vehiculo existente
void editarVehiculo(){
    Vehiculo vehiculos[MAX_VEHICULOS];
    int count = obtenerVehiculos(vehiculos);
    
    if(count == 0){
        printf("No hay vehiculos registrados para editar\n");
        return;
    }
    
    listarVehiculos(vehiculos);
    printf("\nSeleccione el numero de vehiculo a editar (0-%d): ", count-1);//elegir el vehiculo a editar rango permitido
    int indice = leerEnteroConRango(0, count-1);
    
    printf("\n--- Editando: %s %s ---\n", vehiculos[indice].marca, vehiculos[indice].modelo);
    printf("Nuevo precio (actual: %.2f): ", vehiculos[indice].precio);
    vehiculos[indice].precio = leerFlotanteConRango(1000, 500000);
    
    printf("Estado - 1=Disponible / 0=Vendido (actual: %s): ", 
           vehiculos[indice].disponible ? "Disponible" : "Vendido");
    vehiculos[indice].disponible = leerEnteroConRango(0, 1);
        
    // Reescribir todo el archivo con los cambios
    FILE *f = fopen("vehiculos.dat", "wb");//abre archivo en modo escritura binaria (wb)
    if(f != NULL){
        fwrite(vehiculos, sizeof(Vehiculo), count, f);//escribe el arreglo completo en el archivo
        fclose(f);
        printf("Vehiculo actualizado exitosamente\n");
    } else {
        printf("Error al guardar los cambios\n");
    }
}

// Buscar vehiculos por preferencias del cliente
void buscarVehiculosPorPreferencias(){
    Vehiculo vehiculos[MAX_VEHICULOS];
    int count = obtenerVehiculos(vehiculos);
    
    if(count == 0){
        printf("No hay vehiculos registrados\n");
        return;
    }
    
    printf("\n=== BUSQUEDA DE VEHICULOS POR PREFERENCIAS ===\n");
    
    // Pedir tipo de vehiculo
    printf("Ingrese el tipo de vehiculo (Camioneta/Sedan/SUV/etc - 0 para omitir): ");
    char tipoBuscado[20];
    leerCadena(tipoBuscado, 20);
    
    // Pedir marca
    printf("Ingrese la marca preferida (o 0 para omitir): ");
    char marcaBuscada[30];
    leerCadena(marcaBuscada, 30);
    
    // Pedir precio maximo
    printf("Ingrese el precio maximo disponible (0 para omitir): ");
    float precioMax = leerFlotanteConRango(0, 500000);
    
    // Pedir condicion
    printf("Prefiere vehiculo usado? (1.-Si / 0.-No / 2.-Cualquiera): ");
    int usadoPref = leerEnteroConRango(0, 2);
    
    // Buscar y mostrar vehiculos que coincidan
    printf("\n--- VEHICULOS QUE COINCIDEN CON SUS PREFERENCIAS ---\n");
    printf("%-5s %-15s %-15s %-15s %-10s %-12s %-10s\n", "#", "Marca", "Modelo", "Tipo", "Anio", "Precio", "Condicion");
    printf("------------------------------------------------------------------------------------------------\n");
    
    int encontrados = 0;
    for(int i = 0; i < count; i++){
        // Verificar si el vehiculo esta disponible
        if(vehiculos[i].disponible == 0) continue;
        
        // Filtrar por tipo
        if(strcmp(tipoBuscado, "0") != 0 && 
           strcasecmp(vehiculos[i].tipo, tipoBuscado) != 0) continue;
        
        // Filtrar por marca
        if(strcmp(marcaBuscada, "0") != 0 && 
           strcasecmp(vehiculos[i].marca, marcaBuscada) != 0) continue;
        
        // Filtrar por precio
        if(precioMax > 0 && vehiculos[i].precio > precioMax) continue;
        
        // Filtrar por condicion (usado/nuevo)
        if(usadoPref != 2 && vehiculos[i].usado != usadoPref) continue;
        
        // Si paso todos los filtros, mostrar el vehiculo
        printf("%-5d %-15s %-15s %-15s %-10d $%-11.2f %-10s\n",
               i,
               vehiculos[i].marca,
               vehiculos[i].modelo,
               vehiculos[i].tipo,
               vehiculos[i].anio,
               vehiculos[i].precio,
               vehiculos[i].usado ? "Usado" : "Nuevo");
        encontrados++;
    }
    
    if(encontrados == 0){
        printf("No se encontraron vehiculos que coincidan con sus preferencias\n");
    } else {
        printf("\nTotal de vehiculos encontrados: %d\n", encontrados);
    }
}

// Funciones de Venta

void realizarVenta(){
    Venta venta;
    Cliente clientes[MAX_CLIENTES];
    Vehiculo vehiculos[MAX_VEHICULOS];
    // Obtener clientes y vehiculos disponibles
    int numClientes = obtenerClientes(clientes);
    int numVehiculos = obtenerVehiculos(vehiculos);
    
    if(numClientes == 0){
        printf("No hay clientes registrados. Registre clientes primero\n");
        return;
    }
    
    if(numVehiculos == 0){
        printf("No hay vehiculos registrados. Registre vehiculos primero\n");
        return;
    }
    
    // Asignar ID de venta
    Venta ventas[MAX_VENTAS];
    int numVentas = obtenerVentas(ventas);
    venta.id = numVentas + 1;
    
    // Seleccionar cliente
    printf("\n--- REALIZAR NUEVA VENTA ---\n");
    listarClientes(clientes);
    printf("\nSeleccione el numero de cliente (0-%d): ", numClientes-1);//elegir cliente rango permitido
    int idxCliente = leerEnteroConRango(0, numClientes-1);
    venta.cliente = clientes[idxCliente];
    
    // Mostrar solo vehiculos disponibles
    printf("\n--- Vehiculos Disponibles ---\n");
    printf("%-5s %-15s %-15s %-15s %-10s %-12s %-10s\n", "#", "Marca", "Modelo", "Tipo", "Anio", "Precio", "Condicion");
    printf("------------------------------------------------------------------------------------------------\n");
    int disponibles = 0;
    for(int i = 0; i < numVehiculos; i++){
        if(vehiculos[i].disponible == 1){
            printf("%-5d %-15s %-15s %-15s %-10d $%-11.2f %-10s\n",
                   i,
                   vehiculos[i].marca,
                   vehiculos[i].modelo,
                   vehiculos[i].tipo,
                   vehiculos[i].anio,
                   vehiculos[i].precio,
                   vehiculos[i].usado ? "Usado" : "Nuevo");
            disponibles++;
        }
    }
    
    if(disponibles == 0){
        printf("ERROR: No hay vehiculos disponibles para vender\n");
        return;
    }
    
    printf("\nSeleccione vehiculo (0-%d): ", numVehiculos-1);
    int idxVehiculo = leerEnteroConRango(0, numVehiculos-1);
    
    if(vehiculos[idxVehiculo].disponible == 0){
        printf("ERROR: Este vehiculo ya ha sido vendido\n");
        return;
    }
    
    // Realizar la venta
    venta.vehiculo = vehiculos[idxVehiculo];
    
    // Marcar vehiculo como vendido
    vehiculos[idxVehiculo].disponible = 0;
    
    // Actualizar archivo de vehiculos
    FILE *f = fopen("vehiculos.dat", "wb");//abre archivo en modo escritura binaria (wb)
    if(f != NULL){//verifica si el archivo se abrio correctamente
        fwrite(vehiculos, sizeof(Vehiculo), numVehiculos, f);
        fclose(f);
    }
    
    // Guardar venta
    guardarVenta(&venta);
    
    // Mostrar resumen
    printf("\n========================================\n");
    printf("VENTA REALIZADA EXITOSAMENTE\n");
    printf("========================================\n");
    printf("Venta #%d\n", venta.id);
    printf("Cliente: %s (ID: %d)\n", venta.cliente.nombre, venta.cliente.id);
    printf("Edad: %d anios\n", venta.cliente.edad);
    printf("\n--- VEHICULO VENDIDO ---\n");
    printf("Marca: %s\n", venta.vehiculo.marca);
    printf("Modelo: %s\n", venta.vehiculo.modelo);
    printf("Tipo: %s\n", venta.vehiculo.tipo);
    printf("Anio: %d\n", venta.vehiculo.anio);
    printf("Condicion: %s\n", venta.vehiculo.usado ? "Usado" : "Nuevo");
    printf("PRECIO FINAL: $%.2f\n", venta.vehiculo.precio);
    printf("========================================\n");
}

// Listar todas las ventas registradas
void listarVentas(Venta *ventas){
    int c = obtenerVentas(ventas);// c guarda ventas
    if(c == 0){
        printf("No hay ventas registradas\n");
        return;
    }
    
    printf("\n--- HISTORIAL DE VENTAS ---\n");
    for(int i = 0; i < c; i++){
        printf("\n========================================\n");
        printf("VENTA #%d\n", ventas[i].id);
        printf("Cliente: %s (ID: %d) - %d anios\n", 
               ventas[i].cliente.nombre, 
               ventas[i].cliente.id,
               ventas[i].cliente.edad);
        printf("----------------------------------------\n");
        printf("Vehiculo: %s %s\n", ventas[i].vehiculo.marca, ventas[i].vehiculo.modelo);
        printf("Tipo: %s\n", ventas[i].vehiculo.tipo);
        printf("Anio: %d\n", ventas[i].vehiculo.anio);
        printf("Condicion: %s\n", ventas[i].vehiculo.usado ? "Usado" : "Nuevo");
        printf("PRECIO: $%.2f\n", ventas[i].vehiculo.precio);
    }
    printf("\n========================================\n");
}


void guardarVenta(Venta *venta){
    FILE *f;
    f = fopen("ventas.dat","ab");//abre archivo en modo agregar binario (ab)
    if (f==NULL)
    {
        printf("No se pudo abrir el archivo\n");
        return;
    }
    fwrite(venta,sizeof(Venta),1,f);
    fclose(f);
}


int obtenerVentas(Venta *ventas){
    FILE *f;
    f = fopen("ventas.dat","rb");//Abre el archivo en modo lectura binaria (rb)
    if (f==NULL)
    {
        return 0;
    }
    int count = fread(ventas,sizeof(Venta),MAX_VENTAS,f);
    fclose(f);
    return count;
}
