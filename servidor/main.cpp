#include "mainHeader.hpp"

 
int main()
{
    char read_buf[256]; //bytes
    boost::system::error_code error;
    size_t len;
    bool running=true; //condicion para que el bucle sea infinito o que se termine con shutdown
    Estados stateMachine;

    try
    {
        io_context io;
        serial_port serial(io, "COM3");
        serial.set_option(serial_port_base::baud_rate(9600));
        serial.set_option(serial_port_base::character_size(8));
        serial.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
        serial.set_option(serial_port_base::parity(serial_port_base::parity::none));
 
        std::cout << "Servidor iniciado, esperando comandos" << std::endl;
 
        while(running)
        {   
            len = serial.read_some(buffer(read_buf), error);
 
            if (error) 
                 std::cerr << "Error de lectura: " << error.message() << std::endl;
             else 
                std::cout << "Recibido: " << std::string(read_buf, len) << std::endl; //string(read_buf, len) devuelve el string recibido

            if (std::string(read_buf, len) == "status")
                 status(&stateMachine, &serial);
            else if (std::string(read_buf, len) == "start")
                start(&stateMachine, &serial);
            else if (std::string(read_buf, len) == "stop")
                 stop(&stateMachine, &serial);
            else if (std::string(read_buf, len) == "shutdown")
            {
                 write(serial, buffer("shutdown"));
                 running = shutdown(&stateMachine, &serial);
            }
        }
    }    
    catch (boost::system::system_error& e) //para capturar errores relacionados con la inicializacion
    {
        std::cerr << "Falla al Establecer Conexion: " << e.what() << std::endl; //
    } catch (...) {
        std::cerr << "Error desconocido" << std::endl;
    }

    return 0;
}