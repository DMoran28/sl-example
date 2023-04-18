# Caso de estudia sobre la seguridad en protocolos de comunicación
En este caso de estudio práctico se realiza una demonstración de ataque de denegación de servicio sobre un dispositivo ESP32 que presenta un servidor HTTP a modo de RESTful API sin sistemas de autenticación. Por otro lado, se realiza un ataque MitM entre este dispositivo y el *broker* MQTT a través de Wireshark para obtener las credenciales de acceso del mismo al no tener mecanismos de cifrado en la comunicación.

## Instalación de dependencias
A continuación se describen los pasos a seguir a la hora de instalar el entorno ESP-IDF para la configuración del dispositivo ESP32, además de describir el proceso de instalación del *broker* MQTT en local.

### Entorno ESP-IDF
Para el desarrollo de este caso de estudio práctico se ha utilizado el entorno [ESP-IDF](https://github.com/espressif/esp-idf) en su versión 5.0.1. Los pasos a seguir para realizar la instalación de ESP-IDF se puede encontrar en la siguiente [guía de Espressif](https://docs.espressif.com/projects/esp-idf/en/v5.0.1/esp32/get-started/index.html).

A continuación, se clona este repositorio a través del siguiente comando:

```sh
git clone https://github.com/dmoran28/sl-example
```

### *Broker* MQTT
Por otro lado, se instala Eclipse Mosquitto como *broker* MQTT local para la realización de este caso de estudio práctico. La documentación necesaria para instalar Eclipse Mosquitto se encuentra en la siguiente [guía](https://mosquitto.org/download/).

Para ejecutar Eclipse Mosquitto en segundo plano se utiliza el siguiente comando:

```sh
mosquitto -d
```

Para activar y/o modificar las credenciales de acceso al *broker* MQTT, se debe modificar el fichero de configuración `mosquitto.conf`, tal y como se explica en la [documentación oficial de Eclipse Mosquitto](https://mosquitto.org/man/mosquitto-conf-5.html).

### Wireshark
Se utiliza este programa para capturar los paquetes entre el dispositivo ESP32 y el *broker* MQTT, permitiendo obtener las credenciales de acceso del mismo. Para instalar Wireshark, se pueden seguir las instrucciones de su [página web](https://www.wireshark.org/download.html).

## Configuración del dispositivo
Una vez realizada la instalación de las dependencias y exportado las variables de desarrollo de ESP-IDF, se realiza la configuración del dispositivo ESP32. Este dispositivo deberá estar conectado a través del puerto serie por defecto.

Antes de compilar el proyecto, se debe de modificar las variables predeterminados del menú de configuración del proyecto. Para ello se ejecuta el siguiente comando, donde se puede modificar las credenciales de acceso del *broker* MQTT (URI, usuario y contraseña) y las credenciales del Wi-Fi en el que el dispositivo se va a conectar para realizar este caso de estudio (SSID y contraseña).

```sh
idf.py menuconfig
```

Para modificar las credenciales de acceso al *broker* MQTT se debe acceder al menú `MQTT configuration`, mientras que para modificar las credenciales del Wi-Fi se debe acceder al menú `Wi-Fi configuration`. La URI del *broker* es de la forma `mqtt://<host>`, donde `host` es la IP del sistema donde se está ejecutando Eclipse Mosquitto.

Una vez ajustado el menú de configuración, se compila el proyecto y se realiza un *flash* del contenido hacia el dispositivo ESP32 a través de los siguientes comandos. La variable `port` debe ser sustituida por el puerto serie al que está conectado el dispositivo ESP32.

```sh
idf.py build
idf.py -p <port> flash
```

## Ejecución del ataque
A continuación se detalla el proceso de ataque hacia el dispositivo ESP32 a través de la vulnerabilidad de denegación de servicio y la vulnerabilidad del ataque MitM en una comunicación sin cifrado. Para ello, tanto el dispositivo ESP32 como el *broker* Eclipse Mosquitto se deben de encontrar en la misma red, ya que si esto no se cumple el caso de estudio fallará. Se recomienda activar la monitorización del dispositivo ESP32 a través del siguiente comando:

```sh
idf.py -p <port> monitor
```

### Denegación de servicio
Para la denegación de servicio se ha creado un *script* en Python que realiza una petición GET al *endpoint* habilitado en el servidor HTTP `/data` a través de múltiples *threads*. El objetivo de este *script* es saturar a peticiones al dispositivo, denegando cualquier tipo de respuesta. Para ejecutar este script se debe de indicar la dirección IP del dispositivo ESP32 y, de forma opcional, el número de *threads* a realizar las diferentes peticiones.

```sh
python3 dos.py --host <IP> --threads 1000
```

Una vez ejecutado este *script* se observa que es imposible el acceso al recurso `/data` a través de un navegador web. Sin embargo, una vez es desactivado este *script*, se puede acceder al recurso sin ningún tipo de problema.

### Comunicación sin cifrado con *broker* MQTT
El dispositivo ESP32, a parte de abrir un servidor HTTP con el *endpoint* indicado, también crea un cliente MQTT que se conecta con el *broker* MQTT definido en el menú de configuración para enviar datos de forma periódica a un *topic* concreto. En este caso, al tener la comunicación sin cifrar se pueden obtener las claves de acceso al *broker* si se realiza un ataque MitM a través de Wireshark en el momento de la primera conexión.

En el archivo `capture.pcapng` se encuentra una captura realizada de los paquetes MQTT entre el dispositivo ESP32 y el *broker* MQTT, donde se observa el envío de credenciales de acceso en la petición de conexión.