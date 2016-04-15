// This #include statement was automatically added by the Particle IDE.
#include "ThingSpeak/ThingSpeak.h"

// This #include statement was automatically added by the Particle IDE.
#include "spark-dallas-temperature/spark-dallas-temperature.h"

// This #include statement was automatically added by the Particle IDE.
#include "OneWire/OneWire.h"

#define ONE_WIRE_BUS D2  //data on pin d2
#define DAY_TIME_INTERVAL  5*60*1000    //5 minutes
#define NIGHT_TIME_INTERVAL 60*60*1000  //1 hour
#define DEBUG 1 //debug flag

OneWire ds = OneWire(ONE_WIRE_BUS);  // Setup a oneWire instance to communicate with any OneWire devices 
DallasTemperature sensors(&ds);  // Pass our oneWire reference to Dallas Temperature.

int led_red = B0;   //set up a red lied on Port B0
int led_green = B1; //set up a green lied on Port B1

double tempc1 = 0.0;    //a variable stores temperature
double tempc2 = 0.0;    //a variable stores temperature

DeviceAddress sensorAddr1 = {0x28, 0xFF, 0xBF, 0x92, 0x61, 0x15, 0x02, 0x2F};   //this is the probe sensor address
DeviceAddress sensorAddr2 = {0x28, 0x62, 0xC5, 0x5E, 0x07, 0x00, 0x00, 0xF4};   //this is chip2 sensor address

/* Thingspeak */
TCPClient client;
unsigned long myChannelNumber = 106736;
const char * myWriteAPIKey = "KCVFGJJX8JWA34YG";

// function to print a device address
void printAddress(DeviceAddress deviceAddress);
void printAddress(DeviceAddress deviceAddress)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        // zero pad the address if necessary
        if (deviceAddress[i] < 16) Serial.print("0");
        Serial.print(deviceAddress[i], HEX);
    }
}


void setup(void)
{
    pinMode(led_red, OUTPUT);
    pinMode(led_green, OUTPUT);
    Time.zone(-5);
    Serial.begin(9600);    
    sensors.begin();    // IC defaults to 9 bit. If you have trouble consider changing to 12. 
    //Spark.variable("tempc1", &tempc1, DOUBLE);   //expose vars to web interface  
    
    Particle.publish ("Report", "Electron is online!");
    ThingSpeak.begin(client);   //thingspeak setup online
    
    
    if(sensors.isConnected(sensorAddr1))
    {
        // blink the green LED
        digitalWrite(led_green, HIGH);
        delay(1000);
        digitalWrite(led_green, LOW);
        delay(1000);
        
        // show the addresses we found on the bus
        Serial.print("Device 0 Address: ");
        printAddress(sensorAddr1);
        Serial.println();
        
        // show the resolution of the DS18B20 sensor
        Serial.print("Device 0 Resolution: ");
        Serial.print(sensors.getResolution(sensorAddr1), DEC); 
        Serial.println();
    }
    else
    {
        Serial.println("Unable to find address for Device 0 - probe");
        
        // blink the red LED
        digitalWrite(led_red, HIGH);
        delay(1000);
        digitalWrite(led_red, LOW);
        delay(1000);
    }
    
    if(sensors.isConnected(sensorAddr2))
    {
        // blink the green LED
        digitalWrite(led_green, HIGH);
        delay(1000);
        digitalWrite(led_green, LOW);
        delay(1000);
        
        // show the addresses we found on the bus
        Serial.print("Device 1 Address: ");
        printAddress(sensorAddr2);
        Serial.println();
        
        // show the resolution of the DS18B20 sensor
        Serial.print("Device 1 Resolution: ");
        Serial.print(sensors.getResolution(sensorAddr2), DEC); 
        Serial.println();
    }
    else
    {
        Serial.println("Unable to find address for Device 1 - sensor chip");
        
        // blink the red LED
        digitalWrite(led_red, HIGH);
        delay(1000);
        digitalWrite(led_red, LOW);
        delay(1000);
    }

}   


void loop(void)
{
    Serial.println(Time.timeStr());
    Serial.print("Requesting temperatures...");
    sensors.requestTemperatures();
    Serial.println("DONE");
    tempc1 = sensors.getTempC(sensorAddr1);
    
    Serial.print("tempC from sensor 1: ");
    Serial.print(tempc1);
    Serial.print("C \r\n");
    
    tempc2 = sensors.getTempC(sensorAddr2);
    
    Serial.print("tempC from sensor 2: ");
    Serial.print(tempc2);
    Serial.print("C \r\n");
    
    ThingSpeak.setField(2, (float)tempc1);
    ThingSpeak.setField(1, (float)tempc2);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    
    if(Time.hour() <= 15 && Time.hour() >= 8)
    {
        Serial.println("delay 5 min");
        delay(DAY_TIME_INTERVAL);
    }
    else
    {
        Serial.println("delay 1 hr");
        delay(NIGHT_TIME_INTERVAL);
    }


}

