#include "mainheader.hpp"


 
int main()
{
    std::string puerto;
    int baudrate;
    int datos;
    char paridad;
    int parada;
 
    try{
        io_context io;
        std::cout << "Ingrese nombre del puerto: ";
        std::cin >> puerto;
 
        std::cout << "Ingrese baud rate: ";
        std::cin >> baudrate;
 
        std::cout << "Ingrese cantidad de bits de datos: ";
        std::cin >> datos;
 
        std::cout << "Ingrese paridad (par, impar o ninguno): ";
        std::cin >> paridad;
 
        std::cout << "Ingrese cantidad de bits de parada (1 o 2): ";
        std::cin >> parada;
 
        serial_port serial(io, puerto);
 
        serial.set_option(serial_port_base::baud_rate(baudrate));
        serial.set_option(serial_port_base::character_size(datos));
 
        switch(paridad){
            case 'p':
                serial.set_option(serial_port_base::parity(serial_port_base::parity::even));
                break;
            case 'i':
                serial.set_option(serial_port_base::parity(serial_port_base::parity::odd));
                break;
            case 'n':
                serial.set_option(serial_port_base::parity(serial_port_base::parity::none));
                break;
        }   
 
        switch(parada){
            case 1:
                serial.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
                break;
            case 2:
                serial.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::two));
                break;
        }
 
        std::cin.ignore();
 
        std::cout << "Conexion establecida" << std::endl;
 
        std::string message = "Hola desde Boost.Asio!\r\n";
        write(serial, buffer(message));
 
        std::cout << "Mensaje enviado: " << message << std::endl;
 
        std::string comando;
 
        while(true)
        {
            char read_buf[256]; //bytes
            boost::system::error_code error;
            
            

            std::cout << "Ingrese comandos: ";
            getline(std::cin,comando);
 
            write(serial, buffer(comando));

            size_t len = serial.read_some(buffer(read_buf), error);
            if (error) 
                 std::cerr << "Error de lectura: " << error.message() << std::endl;
             else 
                std::cout << "Recibido: " << std::string(read_buf, len) << std::endl;

            
        }
    }    
 
    catch (boost::system::system_error& e) {
        std::cout << "Falla al Establecer Conexion: " << e.what() << std::endl; //falla al establecer la conexion
    } catch (...) {
        std::cout << "Error en los datos de comunicacion" << std::endl; //falla al establecer la conexion
    }
 
    return 0;
}