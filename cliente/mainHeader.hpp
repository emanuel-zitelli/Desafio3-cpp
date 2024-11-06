#ifndef MAINHEADER_HPP
#define MAINHEADER_HPP

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp> //para la lectura asincronica

using namespace boost::asio;

void onTimeout(const boost::system::error_code& error, serial_port& serial, bool& running);
void onRead(const boost::system::error_code& error, size_t len, boost::system::error_code& ec, size_t& length);
bool leerConTimeout(serial_port& serial, deadline_timer &timer, io_context &io, char* buffer, size_t buffer_size, boost::system::error_code& ec, int& timeout, bool& running, size_t& length);

#endif //MAIN_HEADER_HPP