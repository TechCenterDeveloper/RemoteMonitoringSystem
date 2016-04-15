// This #include statement was automatically added by the Particle IDE.
#include "ThingSpeak/ThingSpeak.h"

// This #include statement was automatically added by the Particle IDE.
#include "spark-dallas-temperature/spark-dallas-temperature.h"

// This #include statement was automatically added by the Particle IDE.
#include "OneWire/OneWire.h"

#define ONE_WIRE_BUS D2  //data on pin d2

//ThingSpeakLibrary::ThingSpeak thingspeak ("KCVFGJJX8JWA34YG");
/* Thingspeak */
TCPClient client;
unsigned long myChannelNumber = 106736;
const char * myWriteAPIKey = "KCVFGJJX8JWA34YG";

double tempc1 = 0.0;
double tempc2 = 0.0;

OneWire ds = OneWire(ONE_WIRE_BUS);  // Setup a oneWire instance to communicate with any OneWire devices 
DallasTemperature sensors(&ds);  // Pass our oneWire reference to Dallas Temperature.

DeviceAddress sensorAddr1 = {0x28, 0xFF, 0xBF, 0x92, 0x61, 0x15, 0x02, 0x2F};   //this is the probe sensor address
DeviceAddress sensorAddr2 = {0x28, 0x62, 0xC5, 0x5E, 0x07, 0x00, 0x00, 0xF4};   //this is chip2 sensor address

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
    Time.zone(-5);
    Serial.begin(9600);    
    sensors.begin();    // IC defaults to 9 bit. If you have trouble consider changing to 12. 
    //Spark.variable("tempc1", &tempc1, DOUBLE);   //expose vars to web interface  
    
    Particle.publish ("Report", "Electron is online!");
    ThingSpeak.begin(client);   //thingspeak setup online
    
    if(!sensors.validAddress(sensorAddr1))
    {
        Serial.println("Unable to find address for Device 0 - probe");
    }
    if(!sensors.validAddress(sensorAddr2))
    {
        Serial.println("Unable to find address for Device 1 - probe");
    }
    //if (!sensors.getAddress(sensorAddr1, 0))
    //{
    //    Serial.println("Unable to find address for Device 0 - probe");
    //}
    //if (!sensors.getAddress(sensorAddr2, 1)) 
    //{
    //    Serial.println("Unable to find address for Device 1 - chip2");
    //}
    
    // show the addresses we found on the bus
    Serial.print("Device 0 Address: ");
    printAddress(sensorAddr1);
    Serial.println();

    Serial.print("Device 1 Address: ");
    printAddress(sensorAddr2);
    Serial.println();
    
    Serial.print("Device 0 Resolution: ");
    Serial.print(sensors.getResolution(sensorAddr1), DEC); 
    Serial.println();
    
    Serial.print("Device 1 Resolution: ");
    Serial.print(sensors.getResolution(sensorAddr2), DEC); 
    Serial.println();
    //byte i,j;
    ////byte present = 0;
    ////byte data[12];
    //byte addr[8];
    //
    //for(i = 0; i < 10; i++)
    //{
    //    if(ds.search(addr))
    //    {
    //        
    //        Serial.print("Addr ");
    //        Serial.print(i);
    //        Serial.print(": ");
    //        for(j = 0; j < 8; j++)
    //        {
    //            Serial.print(addr[j], HEX);
    //            Serial.print(" ");
    //        }
    //        if ( OneWire::crc8( addr, 7) != addr[7]) 
    //        {
    //            Serial.print("CRC is not valid!\r\n");
    //        }
    //        if ( addr[0] != 0x28) 
    //        {
    //            Serial.print("Device is not a DS18S20 family device.\r\n");
    //        }
    //        else
    //        {
    //            Serial.print("\r\n");
    //        }
    //    }
    //    else
    //    {
    //        Serial.print("No more addresses.\r\n");
    //        ds.reset_search();
    //        delay(1000);
    //        break;
    //    }
    //    
    //}

}   


void loop(void)
{
   
    
    //if ( !ds.search(addr)) 
    //{
    //    Serial.print("No more addresses.\r\n");
    //    ds.reset_search();
    //    delay(250);
    //    return;
    //}
    //
    //Serial.print("R=");
    //for( i = 0; i < 8; i++) 
    //{
    //    Serial.print(addr[i], HEX);
    //    Serial.print(" ");
    //}
    //
    //if ( OneWire::crc8( addr, 7) != addr[7]) 
    //{
    //    Serial.print("CRC is not valid!\r\n");
    //    return;
    //}
    //
    //if ( addr[0] != 0x10) 
    //{
    //    Serial.print("Device is not a DS18S20 family device.\r\n");
    //    return;
    //}
    
    
    //sensors.requestTemperaturesByAddress(sensorAddr1);        // Send the command to get temperatures from all sensors on the one wire bus
    //tempc1 = sensors.getTempCByIndex(0);     //  0 refers to the first IC on the wire 
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
    
    if(Time.hour() <= 16 && Time.hour() >= 8)
    {
        Serial.println("delay 5 min");
        delay(5*60*1000);
    }
    else
    {
        Serial.println("delay 1 hr");
        delay(60*60*1000);
    }
    
    
    
    //tempf = (tempc * 9.0 / 5.0) + 32.0;
    //Serial.print(tempf);
    //Serial.print("F   ");
    
    //rssi=WiFi.RSSI();
    //Serial.print(rssi);
    //Serial.println("dbm");
    
    //digitalWrite (7, HIGH);     //quick confidence flash
    //delay(100);
    //digitalWrite (7, LOW);
    //delay(100);

}

