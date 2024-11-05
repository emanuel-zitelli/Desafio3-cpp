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

//desarrollo para la lectura con timer

//Esta función se activa cuando el deadline_timer alcanza el límite de tiempo sin que la lectura haya finalizado.
void onTimeout(const boost::system::error_code& error, serial_port& serial, bool& running) 
{
    if (!error) 
    {
        running = false;
        serial.cancel();  //cancela la lectura
    }
}

//Esta función se llama cuando la lectura termina ya sea con exito o error
void onRead(const boost::system::error_code& error, size_t len, boost::system::error_code& ec, size_t& length) 
{
    ec = error;
    length = len;
}

bool leerConTimeout(serial_port& serial, deadline_timer &timer, io_context &io, char* buffer, size_t buffer_size, boost::system::error_code& ec, int& timeout, bool& running, size_t& length) 
{
    ec = error::would_block;    //cada vez que se llama a la funcion reinicializo ec
    timer.expires_from_now(boost::posix_time::seconds(timeout)); //timer configurable

    timer.async_wait(boost::bind(onTimeout, boost::placeholders::_1, boost::ref(serial), boost::ref(running)));  //uso bind para enlazar onTimeout con el timer. así si se llega al timeout, la función cancela la lectura

    serial.async_read_some(boost::asio::buffer(buffer, buffer_size), boost::bind(onRead, boost::placeholders::_1, boost::placeholders::_2, boost::ref(ec), boost::ref(length)));  //la funcion onRead se llama cuando la lectura termine

    // Ejecuta la operación asíncrona en un bucle hasta que termine o se alcance el timeout
    do 
    {
        io.run_one();   //ejecutar solo una operación asíncrona a la vez
    }while (ec == boost::asio::error::would_block && running);  //ec cambia de valor cuando termina una lectura o finaliza el timeout

    timer.cancel();     //cancela el timeout
    return !ec;  //devuelve true si la lectura es existosa
}
