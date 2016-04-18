// This #include statement was automatically added by the Particle IDE.
#include "spark-dallas-temperature/spark-dallas-temperature.h"

// This #include statement was automatically added by the Particle IDE.
#include "OneWire/OneWire.h"

// This #include statement was automatically added by the Particle IDE.
#include "ThingSpeak/ThingSpeak.h"

#define ONE_WIRE_BUS D2  //data on pin d2
#define DAY_TIME_INTERVAL  5*60*1000    //5 minutes
#define NIGHT_TIME_INTERVAL 60*60*1000  //1 hour
#define DEBUG 1 //debug flag

OneWire ds = OneWire(ONE_WIRE_BUS);  // Setup a oneWire instance to communicate with any OneWire devices 
DallasTemperature sensors(&ds);  // Pass our oneWire reference to Dallas Temperature.

int led_red = B0;   //set up a red lied on Port B0
int led_green = B1; //set up a green lied on Port B1

double tempC_DisplayPanelFront = 0.0;    
double tempC_FrontGlass = 0.0;    
double tempC_InteriorAmbient = 0.0;
double tempC_MediaPlayerBox = 0.0;
double tempC_ExteriorAmbient = 0.0;
double tempC_Blackbody = 0.0;
double tempC_Whitebody = 0.0;

DeviceAddress sensorAddr1 = {0x3B, 0X2B, 0X03, 0X1E, 0X03, 0X0C, 0X0C, 0X85};   //Display Panel Front-Type K thermocouple
DeviceAddress sensorAddr2 = {0x3B, 0x78, 0x09, 0x1E, 0x03, 0x0C, 0X0C, 0X4F};   //Front Glass-Type K thermocouple
DeviceAddress sensorAddr3 = {0x28, 0X83, 0X98, 0X5D, 0X07, 0X00, 0X00, 0X1B};   //Interior Ambient-DS18B20 TO-92 packet
DeviceAddress sensorAddr4 = {0x28, 0xC1, 0X9F, 0X5D, 0X07, 0X00, 0X00, 0X51};   //Media Player Box-DS18B20 TO-92 packet
DeviceAddress sensorAddr5 = {0x28, 0xBB, 0x19, 0x2F, 0x07, 0x00, 0x00, 0xFE};   //Exterior Ambient-DS18B20 High Temp
DeviceAddress sensorAddr6 = {0x28, 0xCC, 0xEB, 0x30, 0x07, 0x00, 0x00, 0x64};   //Blackbody-DS18B20 High Temp
//DeviceAddress sensorAddr7 = {0x28, 0xDD, 0xED, 0x30, 0x07, 0x00, 0x00, 0x94};   //Whitebody-DS18B20 High Temp (optional)

/* Thingspeak */
TCPClient client;
unsigned long myChannelNumber = 106677;
const char * myWriteAPIKey = "JPQ1HDJRBBE46717";

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
    sensors.begin();
    
    Particle.publish ("Report", "Electron is online!");
    ThingSpeak.begin(client);   //thingspeak setup online
    
    //Display Panel Front-Type K thermocouple
    if(sensors.isConnected(sensorAddr1))
    {
        // blink the green LED
        digitalWrite(led_green, HIGH);
        delay(1000);
        digitalWrite(led_green, LOW);
        delay(1000);
        
        // show the addresses we found on the bus
        Serial.print("Device 1 Address: ");
        printAddress(sensorAddr1);
        Serial.println();
        
        // show the resolution of the DS18B20 sensor
        Serial.print("Device 1 Resolution: ");
        Serial.print(sensors.getResolution(sensorAddr1), DEC); 
        Serial.println();
    }
    else
    {
        Serial.println("Unable to find address for Device 1 - Display Panel Front-Type K thermocouple");
        
        // blink the red LED
        digitalWrite(led_red, HIGH);
        delay(1000);
        digitalWrite(led_red, LOW);
        delay(1000);
    }
    
    //Front Glass-Type K thermocouple
    if(sensors.isConnected(sensorAddr2))
    {
        // blink the green LED
        digitalWrite(led_green, HIGH);
        delay(1000);
        digitalWrite(led_green, LOW);
        delay(1000);
        
        // show the addresses we found on the bus
        Serial.print("Device 2 Address: ");
        printAddress(sensorAddr2);
        Serial.println();
        
        // show the resolution of the DS18B20 sensor
        Serial.print("Device 2 Resolution: ");
        Serial.print(sensors.getResolution(sensorAddr2), DEC); 
        Serial.println();
    }
    else
    {
        Serial.println("Unable to find address for Device 2 - Front Glass-Type K thermocouple");
        
        // blink the red LED
        digitalWrite(led_red, HIGH);
        delay(1000);
        digitalWrite(led_red, LOW);
        delay(1000);
    }
    
    //Interior Ambient-DS18B20 TO-92 packet
    if(sensors.isConnected(sensorAddr3))
    {
        // blink the green LED
        digitalWrite(led_green, HIGH);
        delay(1000);
        digitalWrite(led_green, LOW);
        delay(1000);
        
        // set resolution to 10 (0.25C)
        sensors.setResolution(sensorAddr3, 10);
        
        // show the addresses we found on the bus
        Serial.print("Device 3 Address: ");
        printAddress(sensorAddr2);
        Serial.println();
        
        // show the resolution of the DS18B20 sensor
        Serial.print("Device 3 Resolution: ");
        Serial.print(sensors.getResolution(sensorAddr3), DEC); 
        Serial.println();
    }
    else
    {
        Serial.println("Unable to find address for Device 3 - Interior Ambient-DS18B20 TO-92 packet");
        
        // blink the red LED
        digitalWrite(led_red, HIGH);
        delay(1000);
        digitalWrite(led_red, LOW);
        delay(1000);
    }
    
    //Media Player Box-DS18B20 TO-92 packet
    if(sensors.isConnected(sensorAddr4))
    {
        // blink the green LED
        digitalWrite(led_green, HIGH);
        delay(1000);
        digitalWrite(led_green, LOW);
        delay(1000);
        
        // set resolution to 10 (0.25C)
        sensors.setResolution(sensorAddr4, 10);
        
        // show the addresses we found on the bus
        Serial.print("Device 4 Address: ");
        printAddress(sensorAddr2);
        Serial.println();
        
        // show the resolution of the DS18B20 sensor
        Serial.print("Device 4 Resolution: ");
        Serial.print(sensors.getResolution(sensorAddr4), DEC); 
        Serial.println();
    }
    else
    {
        Serial.println("Unable to find address for Device 4 - Media Player Box-DS18B20 TO-92 packet");
        
        // blink the red LED
        digitalWrite(led_red, HIGH);
        delay(1000);
        digitalWrite(led_red, LOW);
        delay(1000);
    }
    
    //Exterior Ambient-DS18B20 High Temp
    if(sensors.isConnected(sensorAddr5))
    {
        // blink the green LED
        digitalWrite(led_green, HIGH);
        delay(1000);
        digitalWrite(led_green, LOW);
        delay(1000);
        
        // set resolution to 10 (0.25C)
        sensors.setResolution(sensorAddr5, 10);
        
        // show the addresses we found on the bus
        Serial.print("Device 5 Address: ");
        printAddress(sensorAddr2);
        Serial.println();
        
        // show the resolution of the DS18B20 sensor
        Serial.print("Device 5 Resolution: ");
        Serial.print(sensors.getResolution(sensorAddr5), DEC); 
        Serial.println();
    }
    else
    {
        Serial.println("Unable to find address for Device 5 - Exterior Ambient-DS18B20 High Temp");
        
        // blink the red LED
        digitalWrite(led_red, HIGH);
        delay(1000);
        digitalWrite(led_red, LOW);
        delay(1000);
    }
    
    //Blackbody-DS18B20 High Temp
    if(sensors.isConnected(sensorAddr6))
    {
        // blink the green LED
        digitalWrite(led_green, HIGH);
        delay(1000);
        digitalWrite(led_green, LOW);
        delay(1000);
        
        // set resolution to 10 (0.25C)
        sensors.setResolution(sensorAddr6, 10);
        
        // show the addresses we found on the bus
        Serial.print("Device 6 Address: ");
        printAddress(sensorAddr2);
        Serial.println();
        
        // show the resolution of the DS18B20 sensor
        Serial.print("Device 6 Resolution: ");
        Serial.print(sensors.getResolution(sensorAddr6), DEC); 
        Serial.println();
    }
    else
    {
        Serial.println("Unable to find address for Device 6 - Blackbody-DS18B20 High Temp");
        
        // blink the red LED
        digitalWrite(led_red, HIGH);
        delay(1000);
        digitalWrite(led_red, LOW);
        delay(1000);
    }
    
    //show final status for all the sensors thru LEDs
    if(sensors.isConnected(sensorAddr1) && sensors.isConnected(sensorAddr2) && sensors.isConnected(sensorAddr3) && sensors.isConnected(sensorAddr4) && sensors.isConnected(sensorAddr5) && sensors.isConnected(sensorAddr6))
    {
         digitalWrite(led_green, HIGH);
    }
    else
    {
        digitalWrite(led_red, HIGH);
    }

}   

void loop(void)
{
    Serial.println(Time.timeStr());
    Serial.print("Requesting temperatures...");
    sensors.requestTemperatures();
    Serial.println("DONE");
    
    tempC_DisplayPanelFront = sensors.getTempC(sensorAddr1);
    Serial.print("tempC from sensor 1: ");
    Serial.print(tempC_DisplayPanelFront);
    Serial.print("C \r\n");
    
    tempC_FrontGlass = sensors.getTempC(sensorAddr2);
    Serial.print("tempC from sensor 2: ");
    Serial.print(tempC_FrontGlass);
    Serial.print("C \r\n");
    
    tempC_InteriorAmbient = sensors.getTempC(sensorAddr3);
    Serial.print("tempC from sensor 3: ");
    Serial.print(tempC_InteriorAmbient);
    Serial.print("C \r\n");
    
    tempC_MediaPlayerBox = sensors.getTempC(sensorAddr4);
    Serial.print("tempC from sensor 4: ");
    Serial.print(tempC_MediaPlayerBox);
    Serial.print("C \r\n");
    
    tempC_ExteriorAmbient = sensors.getTempC(sensorAddr5);
    Serial.print("tempC from sensor 5: ");
    Serial.print(tempC_ExteriorAmbient);
    Serial.print("C \r\n");
    
    tempC_Blackbody = sensors.getTempC(sensorAddr6);
    Serial.print("tempC from sensor 6: ");
    Serial.print(tempC_Blackbody);
    Serial.print("C \r\n");
    
    //tempC_Whitebody = sensors.getTempC(sensorAddr7);
    //Serial.print("tempC from sensor 7: ");
    //Serial.print(tempC_Whitebody);
    //Serial.print("C \r\n");
    
    ThingSpeak.setField(1, (float)tempC_DisplayPanelFront);
    ThingSpeak.setField(2, (float)tempC_FrontGlass);
    ThingSpeak.setField(2, (float)tempC_InteriorAmbient);
    ThingSpeak.setField(2, (float)tempC_MediaPlayerBox);
    ThingSpeak.setField(2, (float)tempC_ExteriorAmbient);
    ThingSpeak.setField(2, (float)tempC_Blackbody);
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
