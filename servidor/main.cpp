#include "mainHeader.hpp"


int main()
{
    int timeout=100;
    char read_buf[256]; //bytes
    boost::system::error_code error; //manejo de errores
    size_t len; //size de la lectura
    bool running=true; //condicion para que el bucle sea infinito o que se termine con shutdown
    Estados stateMachine; //estados del servidor
    //para establecer el leer con timer
    boost::system::error_code ec = error::would_block;  //errorcode se inicializa con would_block (la opercación no está terminada)
    size_t length = 0;

    try
    {
        io_context io;
        serial_port serial(io, "COM3");
        serial.set_option(serial_port_base::baud_rate(9600));
        serial.set_option(serial_port_base::character_size(8));
        serial.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
        serial.set_option(serial_port_base::parity(serial_port_base::parity::none));
 
        std::cout << "Servidor iniciado, esperando comandos. Shutdown establecido dentro de " 
        << timeout 
        << " segundos de inactividad" << std::endl;

        deadline_timer timer(io);

        while (running)
        {
            if (leerConTimeout(serial, timer, io, read_buf, sizeof(read_buf), ec, timeout, running, length)) 
            {
                if (error)
                    std::cerr << "Error de lectura: " << error.message() << std::endl;
                else
                    std::cout << "Recibido: " << std::string(read_buf, length) << std::endl; // string(read_buf, len) devuelve el string recibido

                if (std::string(read_buf, length) == "status")
                    status(&stateMachine, &serial);
                else if (std::string(read_buf, length) == "start")
                    start(&stateMachine, &serial);
                else if (std::string(read_buf, length) == "stop")
                    stop(&stateMachine, &serial);
                else if (std::string(read_buf, length) == "shutdown")
                {
                    write(serial, buffer("shutdown"));
                    running = shutdown(&stateMachine, &serial);
                }
                
            }
            else
                {
                    write(serial, buffer("Tiempo de espera alcanzado. Apagando servidor."));
                    std::cout << "Tiempo de espera alcanzado. Cambiando a estado apagado";
                }
        }
            }
            //len = serial.read_some(buffer(read_buf), error);
 
            
    catch (boost::system::system_error& e) //para capturar errores relacionados con la inicializacion
    {
        std::cerr << "Falla al Establecer Conexion: " << e.what() << std::endl; //
    } catch (...) {
        std::cerr << "Error desconocido" << std::endl;
    }

    return 0;
}