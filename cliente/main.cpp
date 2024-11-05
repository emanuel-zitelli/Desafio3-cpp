#include "mainheader.hpp"

using namespace std;
 
using namespace std;
using namespace boost::asio;
 
int main(){
    string puerto;
    int baudrate;
    int datos;
    char paridad;
    int parada;
 
    try{
        io_context io;
        cout << "Ingrese nombre del puerto: ";
        cin >> puerto;
 
        cout << "Ingrese baud rate: ";
        cin >> baudrate;
 
        cout << "Ingrese cantidad de bits de datos: ";
        cin >> datos;
 
        cout << "Ingrese paridad (par, impar o ninguno): ";
        cin >> paridad;
 
        cout << "Ingrese cantidad de bits de parada (1 o 2): ";
        cin >> parada;
 
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
 
        cin.ignore();
 
        cout << "Conexion establecida" << endl;
 
        string message = "Hola desde Boost.Asio!\r\n";
        write(serial, buffer(message));
 
        cout << "Mensaje enviado: " << message << endl;
 
        string comando;
 
        while(true)
        {
            char read_buf[256]; //bytes
            boost::system::error_code error;
            
            

            cout << "Ingrese comandos: ";
            getline(cin,comando);
 
            write(serial, buffer(comando));

            size_t len = serial.read_some(buffer(read_buf), error);
            if (error) 
                 std::cerr << "Error de lectura: " << error.message() << std::endl;
             else 
                std::cout << "Recibido: " << std::string(read_buf, len) << std::endl;

            
        }
    }    
 
    catch (boost::system::system_error& e) {
        cerr << "Falla al Establecer Conexion: " << e.what() << endl; //falla al establecer la conexion
    } catch (...) {
        cerr << "Error en los datos de comunicacion" << endl; //falla al establecer la conexion
    }
 
    return 0;
}