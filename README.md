# TPN1_Monitoreo_Electr-nico_de_Invernadero
El presente repositorio contiene la idea de proyecto correspondiente a la materia Sistemas Embebidos (86.65) a desarrollar durante el primer cuatrimestre del 2025

Titulo: Monitoreo Electrónico de Invernadero

Alumno: Wawryczuk Laureano Agustin (102400)

Objetivo: Realizar el monitoreo de la , para activar el sistema de riego, las lámparas acorde al horario correspondiente y emita alertas ante tanque vacío o ausencia de nutrientes. Via Ethernet enviará los datos de los sensores cada un tiempo configurable para monitoreo y estadísticas.

Desarrollo
El sistema de monitoreo del invernadero deberá medir temperatura y humedad ambiente, nivel de iluminación y humedad del suelo para mantener estos parametros dentro de los requeridos por una planta específica. 
Apunta a plantas que requieren un cuidado especial y deberá ser autónomo, solo precisará la intervención del usuario ante agotamiento del agua dentro del reservorio. Ante esta eventualidad enviará un alerta via Ethernet y encenderá un LED indicando la ausencia de agua. Si el alerta es ignorado luego de una hora activará un Buzzer.
El sistema deberá activar un sistema de riego y un sistema de iluminación en caso de ser necesario, comparando contra los datos cargados para la planta específica en la base de datos.

Las medición de los sensores, además de utilizarse para corrección de los parametros correspondientes, serán enviados via Ethernet cada un tiempo configurado para poder realizar el monitoreo remoto y obtener estadísticas.

Periféricos a utilizar:
-LDR para monitoreo de luz
-Sensor de Temperatura y Humedar DHT11
-Sensor de humedad del suelo anticorrosión 
-Lámpara LED de iluminación
-Electroválvula
-Display LCD
-LED de alerta por falta de agua
-Buzzer
-Sensor de nivel de agua
