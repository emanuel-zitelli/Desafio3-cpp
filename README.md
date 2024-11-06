# Descripción general del proyecto.

Se desarrollan 2 programas en lenguaje C++. Uno que es el cliente y otro que hace de servidor. Se conectan mediante un emulador de hardware (VSPE) y utilizando una biblioteca llamada Boost. El servidor recibe comandos enviados por el cliente y según el comando se cambia el estado del servidor (esperando, ejecucion y apagado) y devuelve un mensaje al cliente para mostrar estos cambios.

# Proceso de desarrollo y las decisiones de diseño tomadas.

El proceso de desarrollo estuvo compuesta por varias partes. 

### Proceso principal: 
Establecer la conexión, utilizando Boost.Asio y el emulador del cable serial.
### Otros procesos: 
Recepción de comandos por parte del servidor. Manejo de errores de parte del servidor. Lectura Asincrónica en servidor y cliente.

Como adicional, y siendo parte de las decisiones tomadas, establecí:
-Un "handshake" entre el servidor y el cliente para confirmar que hubo una conexión exitosa. 
-Máquina de Estados: El servidor se maneja con una máquina de Estados que desde una variable se establece en qué estado se encuentra, y va cambiando según los pedidos del cliente.

# Cómo configurar y ejecutar las aplicaciones cliente y servidor.

1. Descargar este repositorio
2. Dentro de las carpetas Cliente y Servidor se encuentran los ejecutables de cada uno. Con tener el emulador para el puerto com3 ya es suficiente.
2.a El servidor se puede ejecutar y no requiere ninguna configuracion especial.

2.b El cliente pide por pantalla los parámetros de conexión, que el servidor tiene ajustados automáticamente. Son los siguientes:
Nombre del puerto: **COM3**
Baud Rate: **9600**
Cantidad de bits de datos: **8**
Paridad: **n**
Bits de parada: **1**

Para una comunicación correcta se deben de ingresar estos datos.


Si se quiere recompilar los programas, se debe descargar la biblioteca Boost y el emulador VSPE, estableciendo la comunicación serial de tipo com3. El código se compila con el siguiente comando: **g++ -std=c++17 -I "C:/boost_1_86_0" main.cpp functions.cpp  -o main -lws2_32**

# Descripción de las herramientas utilizadas (como el emulador de puertos serie).
Las herramientas para llevar a cabo este proyecto fueron:

**Lenguaje C++:** Se eligió este lenguaje para poder utilizar Boost, y aprovechar las ventajas de este lenguaje sobre su contraparte, el lenguaje C.

**Emulador de puertos serie Virtual Serial Port Emulator (VSPE):** permiten emular una conexión de puerto serie para pruebas de comunicación local, sin necesidad de hardware real.

**Boost:** Biblioteca utilizada para manejar el tiempo de espera y la comunicación asincrónica de forma eficiente.

# Cualquier desafío técnico enfrentado y cómo fue resuelto.
De los desafíos enfrentados en este proyecto puedo destacar los siguientes:

**Implementación de lectura asincrónica:** Era necesario que la lectura con read_some no frenara todo el programa, ya que se quedaba congelado hasta recibie algo, no pudiendo precedir desconexiones. Un primer desarrollo para el servidor no fue aplicable directamente en el cliente, por lo que tuvo que recibir modificaciones

**Entendimiento de puertos series y conexión de programas:** Fue la primera vez que conecté dos programas entre ellos, y no estaba familiarizado con emuladores de hardware y sus usos.

**Handshaking entre servidores y manejo de errores:** La implementación general para asegurar las conexiones llevó un tiempo especial de desarrollo, ya que involucró el análisis de respuestas requeridas en cada uno, y explorar conceptos de la comunicación y como funciona la comunicación entre programas, como si fuese un protocolo.

# Posibles mejoras o optimizaciones futuras.

Una posible mejora sería adaptar este programa para que utilice el protocolo TCP/IP y se pueda conectar a través de la red y no solo con un emulador. Pudiendo utilizar el servidor como uno real, si adaptamos los comandos y ampliamos sus funcionalidades. Se podría tener mas de un cliente y un servidor que los comunique a todos, para armar, por ejemplo, un chatroom. O ver si es factible implementar un intercambio de archivos.

Como optimizaciones, podríamos implementar:
**Autenticación:** utilizando cuentas y usuarios (se pueden usar clases para definir el comportamiento de usuarios comunes o privilegiados)
**Guardado de comandos en un archivo (logs):** De esta manera podríamos recuperar la conversación entre el cliente y el servidor, u otros datos de interés como podrían ser cambios de estado del servidor.