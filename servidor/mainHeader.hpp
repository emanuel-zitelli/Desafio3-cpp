#ifndef MAINHEADER_HPP
#define MAINHEADER_HPP

#include <iostream>
#include <boost/asio.hpp>

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
#endif //MAINHEADER_HPP