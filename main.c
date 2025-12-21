#include <stdio.h>
#include "funciones.h"

int main (int argc, char *argv[]) {

    int opc1, opc2;
    
    do{
        opc1 = menu();

        switch (opc1)
        {
        case 1:
            registrarCliente();
            break;
        case 2:{
            Cliente clientes[MAX_CLIENTES]; 
            listarClientes(clientes);
            break;
        }
        case 3:
            registrarVehiculo();
            break;
        case 4:{
            Vehiculo vehiculos[MAX_VEHICULOS];  
            listarVehiculos(vehiculos);
            break;
        }
        case 5: 
            editarVehiculo();
            break;
        case 6:
            buscarVehiculosPorPreferencias();
            break;
        case 7: 
            realizarVenta();
            break;
        case 8:{ 
            Venta ventas[MAX_VENTAS];
            listarVentas(ventas);
            break;
        }
        case 9: 
            printf("\nGracias por usar el sistema. Hasta pronto!\n");
            return 0;
        default:
            printf("Opcion no valida\n"); 
            break;
        }
        
        printf("\nDesea seleccionar otra opcion? (1.-Si / 2.-No): ");
        opc2 = leerEnteroConRango(1,2);
        
    }while(opc2 == 1);
    
    printf("\nGracias por usar el sistema. Hasta pronto!\n"); 

    return 0;
}
