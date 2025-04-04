#include "mbed.h"
#include "arm_book_lib.h"

#define UPDATE_TIME 3600000     //Tiempo de muestreo del sistema
#define IRRIGATION_TIME 10000   //Tiempo de riego
#define ALARM_BUZZ_TIME 500     //Mitad del período de el buzzer
#define EMPTY_TANK_MESSAGE "El tanque está vacío, por favor rellenar\n" //Mensaje de error ante tanque vacío
#define STATUS_MESSAGE "La temperatura del recinto es de: "             //Mensaje que acompaña la temperatura del recinto

// main() runs in its own thread in the OS

//==========[Declaración de objetos públicos]======================================================
DigitalIn  tankWaterSensor(D2);    //Sensor de agua en tanque
AnalogIn   tempSensor(A0);         //Sensor de Temperatura (LM35)
DigitalIn  soilMoistureSensor(D3); //Sensor de Humedad del suelo
DigitalIn  buzzerReset(D2);        //Botón de Reset para apagar el buzzer
DigitalOut irrigationValve(D10);   //Electroválvula de irrigación
DigitalOut emptyWaterLED(D11);     //LED de alarma (Ausencia de agua)
DigitalOut emptyWaterBuzzer(D12);  //Buzzer de alarma (Ausencia de agua prolongada)
UnbufferedSerial    uartUSB(USBTX, USBRX, 115200); //Comunicación serie con la computadora
    
//==========[Declaración de variables públicas]====================================================
bool tankEmpty = false;             //Variable asociada a la ausencia de agua en el tanque
float temperature;                  //Variable donde se guardará el valore de temperatura
bool soilDried = false;             //Variable asociada a la ausencia de agua en el suelo
bool firstAlertDetected = false;    //Variable asociada al encendido de la válvula de riego
bool alarmLEDstatus = OFF;          //Variable asociada al LED de alarma
bool alarmBuzzerstatus = OFF;       //Variable asociada al Buzzer de alarma

//==========[Declaración de los prototipos de función]=============================================
void inputsInit();                  //Esta función inicializa el estado de todas las entradas
void outputsInit();                 //Esta función inicializa el estado de todas las salidas
void readInputs();                  //Esta función lee el estado de todas las entradas
void statusUpdate();                //Esta función determina el estado del sistema
void alarmActivationUpdate();       //Esta función determina qué alarma debe activarse
void alarmDeactivationUpdate();     //Esta función determina si las alarmas deben desactivarse
void uartTask();                    //Función que envía información del sistema vía UART
void uartAlert();                   //Función que envía un alerta de tanque vacío
void processEmptyWater();           //Función para procesar la falta de agua en el tanque

//==========[Función Main]===========
int main()
{   
    inputsInit();
    outputsInit();
    while (true) {
        readInputs();
        statusUpdate();
        alarmActivationUpdate();
        alarmDeactivationUpdate();
        uartTask();
        delay(UPDATE_TIME);
    }
}

//==========[Implementación de las funciones]============

void inputsInit(){
    tankWaterSensor.mode(PullDown);
    soilMoistureSensor.mode(PullDown);
    buzzerReset.mode(PullDown);
}

void outputsInit(){
    irrigationValve = OFF;
    emptyWaterLED = OFF;
    emptyWaterBuzzer = OFF;
}

void readInputs(){
    if(tankWaterSensor == ON)
        tankEmpty = true;
    else
        tankEmpty = false;

    if(soilMoistureSensor == ON)
        soilDried = true;
    else
        soilDried = false;

    temperature = (tempSensor * 148) + 2;
}

void statusUpdate(){
    if(tankEmpty == true){              //Si el tanque está vacío llama a la función para activar la alarma
        processEmptyWater();
    }
    else{
        firstAlertDetected = false;     //Si hay agua riega
        if(soilDried == true){
            irrigationValve = ON;
            delay(IRRIGATION_TIME);
            irrigationValve = OFF;
        }
        readInputs();
        if(tankEmpty == true){
            processEmptyWater();
        }
    }
}

void alarmActivationUpdate(){
    if(alarmLEDstatus == true){
        emptyWaterLED = ON;
    }
    if(alarmBuzzerstatus == true){
        emptyWaterBuzzer = ON;
        uartUSB.write(EMPTY_TANK_MESSAGE, sizeof(EMPTY_TANK_MESSAGE));
        while(buzzerReset != ON);
        emptyWaterBuzzer = OFF;
    }
}

void alarmDeactivationUpdate(){
    readInputs();
    if(tankWaterSensor == OFF){
        tankEmpty = false;
        emptyWaterLED = OFF;
    }
}

void uartTask(){
    uartUSB.write(STATUS_MESSAGE, sizeof(STATUS_MESSAGE));
    uartUSB.write(&temperature, sizeof(temperature));
}

void processEmptyWater(){
    emptyWaterLED = ON;
    if(firstAlertDetected == true)
        alarmBuzzerstatus = true;
    alarmLEDstatus = true;
    firstAlertDetected = true;
}