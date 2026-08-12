Proyecto IoT
El nodo sensor es formado con por un sensor DHT22 y una placa ESP8266.
El nodo servidor donde se aloja la base de datos y un dashboard Grafana se encuentra en un Raspberry PI 3 model B.
Comunicación entre nodo sensor y base de datos por medio del protocolo MQTT. Los datos en el dashboard son obtenidos utilizando consultas SQL.

Las Notificaciones son recibidas en discord por medio de un bot el cual es el punto de contacto configurado desde Grafana.
