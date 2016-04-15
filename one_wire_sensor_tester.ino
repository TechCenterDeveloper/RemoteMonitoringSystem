// This #include statement was automatically added by the Particle IDE.
#include "OneWire/OneWire.h"

// This #include statement was automatically added by the Particle IDE.
#include "spark-dallas-temperature/spark-dallas-temperature.h"

#define UPDATE_INTERVAL 15000    //update one wire bus every 3 sec

#define ONE_WIRE_BUS D2  //data on pin d2

double tempc1 = 0.0;

unsigned long lastUpdate = 0;

int numberOfDevices; // Number of temperature devices found

DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address

// Comment this out for normal operation
SYSTEM_MODE(SEMI_AUTOMATIC);  // skip connecting to the cloud for (Electron) testing

OneWire ds = OneWire(ONE_WIRE_BUS);  // Setup a oneWire instance to communicate with any OneWire devices 
DallasTemperature sensors(&ds);  // Pass our oneWire reference to Dallas Temperature.

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

void printTemperature(DeviceAddress deviceAddress);
void printTemperature(DeviceAddress deviceAddress)
{
    // method 1 - slower
    //Serial.print("Temp C: ");
    //Serial.print(sensors.getTempC(deviceAddress));
    //Serial.print(" Temp F: ");
    //Serial.print(sensors.getTempF(deviceAddress)); // Makes a second call to getTempC and then converts to Fahrenheit
    
    // method 2 - faster
    float tempC = sensors.getTempC(deviceAddress);
    Serial.print("Temp C: ");
    Serial.print(tempC);
    Serial.print(" Temp F: ");
    Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
}


void setup() 
{
    // start serial port
    Serial.begin(9600);
    Serial.println("Read One-Wire Device Address Demo");
    
    // locate devices on the bus
    Serial.print("Locating devices...");
    sensors.begin();
    Serial.print("Found ");
    numberOfDevices = sensors.getDeviceCount();
    Serial.print(sensors.getDeviceCount(), DEC);
    Serial.println(" devices.");
    
    // report parasite power requirements
    Serial.print("Parasite power is: "); 
    if (sensors.isParasitePowerMode()) 
    {
        Serial.println("ON");
    }
    else 
    {
        Serial.println("OFF");
    }
    
      // Loop through each device, print out address
    for(int i=0;i<numberOfDevices; i++)
    {
        // Search the wire for address
        if(sensors.getAddress(tempDeviceAddress, i))
        {
        	Serial.print("Found device ");
        	Serial.print(i, DEC);
        	Serial.print(" with address: ");
        	for(int j = 0; j < 8; j++)
        	{
        	    Serial.print("0x");
        	    Serial.print(tempDeviceAddress[j], HEX);
        	    Serial.print(" ");
        	}
        	//printAddress(tempDeviceAddress);
        	Serial.println();
        	
        	//Serial.print("Setting resolution to ");
        	//Serial.println(TEMPERATURE_PRECISION, DEC);
        	
        	//// set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
        	//sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
        	
        	Serial.print("Resolution is: ");
        	Serial.print(sensors.getResolution(tempDeviceAddress), DEC); 
        	Serial.println();
        	
        	if (OneWire::crc8(tempDeviceAddress, 7) != tempDeviceAddress[7]) 
        	{
                Serial.println("CRC is not valid!");
                //return;
            }
        }
        else
        {
        	Serial.print("Found ghost device at ");
        	Serial.print(i, DEC);
        	Serial.print(" but could not detect address. Check power and cabling. \r\n");
        }
    }
}

void loop() 
{
    
    unsigned long now = millis();
    // change the 3000(ms) to change the operation frequency
    
    if((now - lastUpdate) > UPDATE_INTERVAL)
    {
        lastUpdate = now;
        //byte i;
        byte present = 0;
        //byte addr[8];

        if ( !ds.search(tempDeviceAddress)) 
        {
            Serial.println("No more addresses.");
            Serial.println();
            Serial.println();
            ds.reset_search();
            //delay(250);
            return;
        }

        // if we get here we have a valid address in addr[]
        // you can do what you like with it
        // see the Temperature example for one way to use
        // this basic code.
    
        // this example just identifies a few chip types
        // so first up, lets see what we have found
    
        // the first ROM byte indicates which chip family
        switch (tempDeviceAddress[0]) 
        {
            case 0x10:
                Serial.println("Chip = DS1820/DS18S20 Temp sensor");
            break;
            case 0x28:
                Serial.println("Chip = DS18B20 Temp sensor");
            break;
            case 0x22:
                Serial.println("Chip = DS1822 Temp sensor");
            break;
            case 0x26:
                Serial.println("Chip = DS2438 Smart Batt Monitor");
            break;
            case 0x3B:
                Serial.println("Chip = MAX31850 K-Thermocouple");
            break;
            default:
                Serial.println("Device type is unknown.");
            // Just dumping addresses, show them all
            //return;  // uncomment if you only want a known type
        }

        // Now print out the device address
        Serial.print("ROM = ");
        for(int i = 0; i < 8; i++) 
        {
            Serial.print("0x");
            Serial.print(tempDeviceAddress[i],HEX);
            Serial.print(" ");
        }
        
        Serial.println();
        // Show the CRC status
        // you should always do this on scanned addresse
        if (OneWire::crc8(tempDeviceAddress, 7) != tempDeviceAddress[7]) 
        {
            Serial.println("CRC is not valid!");
            return;
        }
        else
        {
            Serial.println("CRC is valid!");
        }
        
        switch(tempDeviceAddress[0])
        {
            case 0x10:
                Serial.print("Resolution is: ");
                Serial.print(sensors.getResolution(tempDeviceAddress), DEC); 
                Serial.print("\r\n");
            break;
            case 0x28:
                Serial.print("Resolution is: ");
                Serial.print(sensors.getResolution(tempDeviceAddress), DEC); 
                Serial.print("\r\n");
            break;
            case 0x22:
                Serial.print("Resolution is: ");
                Serial.print(sensors.getResolution(tempDeviceAddress), DEC); 
                Serial.print("\r\n");
            break;
            case 0x26:
                Serial.println("Resolution is: N/A ");
            break;
            case 0x3B:
                Serial.println("Resolution is: 14-bit 0.25C ");
            break;
            default:
                Serial.println("No resolution for an unknown device");
        }
        
        
        Serial.print("Requesting temperatures...");
        sensors.requestTemperatures(); // Send the command to get temperatures
        Serial.println("DONE");
        
        printTemperature(tempDeviceAddress); // Use a simple function to print out the data
        
        Serial.println();
    
        ds.reset(); // clear bus for next use

    }
    
    //// call sensors.requestTemperatures() to issue a global temperature 
    //// request to all devices on the bus
    //Serial.print("Requesting temperatures...");
    //sensors.requestTemperatures(); // Send the command to get temperatures
    //Serial.println("DONE");
    //
    //// Loop through each device, print out temperature data
    //for(int i=0;i<numberOfDevices; i++)
    //{
    //    // Search the wire for address
    //    if(sensors.getAddress(tempDeviceAddress, i))
    //	{
    //		// Output the device ID
    //		Serial.print("Temperature for device: ");
    //		Serial.println(i,DEC);
    //		
    //		// It responds almost immediately. Let's print out the data
    //		printTemperature(tempDeviceAddress); // Use a simple function to print out the data
    //	} 
    //	//else ghost device! Check your power requirements and cabling
    //	
    //}
    //
    //delay(30*1000);
}
