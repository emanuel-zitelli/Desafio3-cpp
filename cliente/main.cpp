#include "mainheader.hpp"


 
int main()
{
    //para establecer la conexion
    std::string puerto;
    int baudrate, datos, parada, timeout=30, timeoutHandshake=10;
    short int intentos=0, maxIntentos=3;
    char paridad, read_buf[256];;

    //para el intercambio de mensajes
    
    std::string comando; 
    boost::system::error_code error;
    //condiciones de los bucles
    bool running=false, handshake=false, datosIncorrectos=true;

    //para establecer el leer con timer
    boost::system::error_code ec = error::would_block;  //errorcode se inicializa con would_block 
    size_t length = 0;
    io_context io;
    deadline_timer timer(io);
    
    serial_port serial(io);

    while (intentos < maxIntentos)
    {
        try
        {
            if (datosIncorrectos)
            {
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

                serial.open(puerto);

                serial.set_option(serial_port_base::baud_rate(baudrate));
                serial.set_option(serial_port_base::character_size(datos));

                switch (paridad)
                {
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

                switch (parada)
                {
                case 1:
                    serial.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
                    break;
                case 2:
                    serial.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::two));
                    break;
                }
                datosIncorrectos=false;
                std::cin.ignore();
            }

            // handshake para confirmar que se establecio la conexion

            std::cout << "Estableciendo conexion con el servidor. Espere...\n";
            write(serial, buffer("handshake"));

            if (leerConTimeout(serial, timer, io, read_buf, sizeof(read_buf), ec, timeoutHandshake, handshake, length))
            {
                std::cout << "Conexion establecida con el servidor\n";
                handshake = true;
                running = true;
            }
            else
            {
                intentos++;
                std::cout << "El servidor ha tardado mucho en responder. Intentando nuevamente. Intentos: " << intentos << "\n";
            }
            //se ejecuta el envio de comandos solo si hubo un handshake
            while ((running) && (intentos < maxIntentos))
            { 
                std::cout << "Ingrese comandos: ";
                getline(std::cin, comando);

                write(serial, buffer(comando));

                if (leerConTimeout(serial, timer, io, read_buf, sizeof(read_buf), ec, timeout, running, length))
                {
                    if (error)
                        std::cerr << "Error de lectura: " << error.message() << "\n";
                    else
                        std::cout << "Recibido: " << std::string(read_buf, length) << "\n";
                }
                else
                {   intentos++;
                    std::cout << "Sin respuesta del servidor. Hay que reestablecer la conexion. Intentos: " << intentos << "\n";
                    handshake=false;
                }
                    
            }
        }

        catch (boost::system::system_error &e)
        {
            std::cout << "Falla al Establecer Conexion: " << e.what() << std::endl; // falla al establecer la conexion
            datosIncorrectos=true;
        }
        catch (...)
        {
            std::cout << "Error en los datos de comunicacion" << std::endl; // falla al establecer la conexion
        }
    }

    return 0;
}