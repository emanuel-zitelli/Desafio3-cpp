#include "mainHeader.hpp"


void start(Estados* estado, serial_port* serial)
{
    if((*estado).esperando)
    {
        write(*serial, buffer("Cambiando de estado: Esperando -> Ejecucion\n"));
        std::cout << "Cambiando a estado Ejecucion\n";
        cambiarEstados(estado, 0, 1, 0);
    }
    else
        write(*serial, buffer("No se acepta el comando START en Estado Esperando\n"));
}

void status(Estados* estado, serial_port* serial)
{
    std::string mensaje = "Estado actual del servidor: ";

    if((*estado).esperando)
        mensaje += "Esperando";
    else if((*estado).ejecucion)
        mensaje += "Ejecucion";
    else
        mensaje += "Apagado";

    mensaje += "\n";

    write(*serial, buffer(mensaje));
}

void stop(Estados* estado, serial_port* serial)
{
    if((*estado).ejecucion)
    {
        write(*serial, buffer("Cambiando de estado: Ejecucion -> Esperando\n"));
        std::cout << "Cambiando a estado Esperando\n";
        cambiarEstados(estado, 1, 0, 0);
    }
    else
        write(*serial, buffer("No se acepta el comando STOP en Estado Esperando\n"));
        
}

bool shutdown(Estados* estado, serial_port* serial)
{
    std::string mensaje = "Cambiando de estado: ";

    if((*estado).esperando)
        mensaje += "Esperando ";
    else if((*estado).ejecucion)
        mensaje += "Ejecucion";

    mensaje += "-> Apagado";
    mensaje += "\n";

    write(*serial, buffer(mensaje));
    std::cout << "Cambiando a estado shutdown\n";

    cambiarEstados(estado, 0, 0, 1);
    return false;
}
    
void cambiarEstados(Estados* estado, bool esperando, bool ejecucion, bool apagado)
{
    (*estado).esperando=esperando;
    (*estado).ejecucion=ejecucion;
    (*estado).apagado=apagado;
}