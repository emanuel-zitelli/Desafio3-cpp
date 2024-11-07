#include "mainHeader.hpp"


int main()
{
    int timeout=30;
    char read_buf[256]; //bytes
    boost::system::error_code error; //manejo de errores
    bool running=true; //condicion para que el bucle sea infinito o que se termine con shutdown
    Estados stateMachine; //estados del servidor
    //para establecer el leer con timer
    boost::system::error_code ec = error::would_block;  //errorcode se inicializa con would_block 
    size_t length = 0;
    io_context io;
    deadline_timer timer(io);

    try
    {
        serial_port serial(io, "COM3");
        serial.set_option(serial_port_base::baud_rate(9600));
        serial.set_option(serial_port_base::character_size(8));
        serial.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
        serial.set_option(serial_port_base::parity(serial_port_base::parity::none));
 
        std::cout << "Servidor iniciado, esperando comandos. Shutdown establecido dentro de " 
        << timeout 
        << " segundos de inactividad" << std::endl;
        
        
        while (running)
        {
            if (leerConTimeout(serial, timer, io, read_buf, sizeof(read_buf), ec, timeout, running, length)) 
            {
                if (error)
                    std::cerr << "Error de lectura: " << error.message() << std::endl;
                else
                {
                    std::cout << "Recibido: " << std::string(read_buf, length) << std::endl; // string(read_buf, len) devuelve el string recibido
                    procesarComando(read_buf, length, stateMachine, serial, running); //
                }
            }
            else
                {
                    write(serial, buffer("Tiempo de espera alcanzado. Apagando servidor."));
                    std::cout << "\nTiempo de espera alcanzado. Cambiando a estado apagado";
                }
        }
    }   
    catch (boost::system::system_error& e) //para capturar errores relacionados con la inicializacion
    {
        std::cerr << "Falla al Establecer Conexion: " << e.what() << std::endl; 
    } catch (...) {
        std::cerr << "Error desconocido" << std::endl;
    }

    return 0;
}