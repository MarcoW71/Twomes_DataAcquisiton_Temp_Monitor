#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup(){
  Serial.begin(115200);
  delay(1000); //Take some time to open up the Serial Monitor

  sensors.begin();
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.println(" ");
      //Als de sensor niet is verbonden zal de temperatuur -127 zijn, deze waarde zal waarschijnlijk niet voorkomen als deze gebruikt wordt. Daarom wordt deze weggefu
    if (temperatureC == -127){
      Serial.println("Sensor niet verbonden");
    }
    else{ 
      Serial.print(temperatureC);
      Serial.println("ºC");
    }
  /*
  First we configure the wake up source
  We set our ESP32 to wake up every 5 seconds
  */
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  /*
  Now that we have setup a wake cause and if needed setup the
  peripherals state in deep sleep, we can now start going to
  deep sleep.
  In the case that no wake up sources were provided but deep
  sleep was started, it will sleep forever unless hardware
  reset occurs.
  */
  Serial.println("Going to sleep now");
  Serial.flush(); 
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void loop(){
  //This is not going to be called
}