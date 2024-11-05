#ifndef MAINHEADER_HPP
#define MAINHEADER_HPP

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp> //para la lectura asincronica

using namespace boost::asio;

struct Estados
{
    bool esperando=true;
    bool ejecucion=false;
    bool apagado=false;
};


void start(Estados* estado, serial_port* serial);
void status(Estados* estado, serial_port* serial);
void stop(Estados* estado, serial_port* serial);
bool shutdown(Estados* estado, serial_port* serial);
void cambiarEstados(Estados* estado, bool esperando, bool ejecucion, bool apagado);

void onTimeout(const boost::system::error_code& error, serial_port& serial, bool& running);
void onRead(const boost::system::error_code& error, size_t len, boost::system::error_code& ec, size_t& length);
bool leerConTimeout(serial_port& serial, deadline_timer &timer, io_context &io, char* buffer, size_t buffer_size, boost::system::error_code& ec, int& timeout, bool& running, size_t& length);


#endif //MAINHEADER_HPP