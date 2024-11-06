#include "mainHeader.hpp"

//desarrollo para la lectura con timer

//si se llega al tiempo maximo se cancela la lectura
void onTimeout(const boost::system::error_code& error, serial_port& serial, bool& running) 
{
    if (!error) 
    {
        running = false;
        serial.cancel();  //cancela la lectura
    }
}

//cuando se termina la lectura
void onRead(const boost::system::error_code& error, size_t len, boost::system::error_code& ec, size_t& length) 
{
    ec = error;
    length = len;
}

bool leerConTimeout(serial_port& serial, deadline_timer &timer, io_context &io, char* buffer, size_t buffer_size, boost::system::error_code& ec, int& timeout, bool& running, size_t& length) 
{
    ec = error::would_block;    //cada vez que se llama a la funcion se reinicia ec
    timer.expires_from_now(boost::posix_time::seconds(timeout)); //timer 

    timer.async_wait(boost::bind(onTimeout, boost::placeholders::_1, boost::ref(serial), boost::ref(running)));  // bind para enlazar onTimeout con el timer. la función cancela la lectura si se cumple el tiempo

    serial.async_read_some(boost::asio::buffer(buffer, buffer_size), boost::bind(onRead, boost::placeholders::_1, boost::placeholders::_2, boost::ref(ec), boost::ref(length)));

    // Ejecuta la operación  en un bucle hasta que se termine el tiempo
    do 
    {
        io.run_one(); 
    }while (ec == boost::asio::error::would_block);  //ec cambia de valor cuando termina una lectura o finaliza el timeout

    timer.cancel();     //cancela el timeout
    return !ec;  //devuelve true si la lectura es existosa
}