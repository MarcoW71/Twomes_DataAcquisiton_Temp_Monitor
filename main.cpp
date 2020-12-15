#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <esp_wifi.h>
#include <esp_bt_main.h>

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  4        /* Time ESP32 will go to sleep (in seconds) */
#define TIME_TO_CONVERSION 1     /* Time ESP32 will go to sleep for conversion(in seconds) */


RTC_DATA_ATTR bool wake_after_conversion = 0;

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup(){
  Serial.begin(115200);
  delay(1000); //Take some time to open up the Serial Monitor(testing only)
  
  //esp_wifi_stop(); //disable wifi
  //esp_bluedroid_disable(); //disable bluetooth

  if (wake_after_conversion == 0){
  sensors.begin(); //initialize sensor
  sensors.setWaitForConversion(false); //disable the wait for conversion
  sensors.requestTemperatures(); //start conversion
  
  esp_sleep_enable_timer_wakeup(TIME_TO_CONVERSION * uS_TO_S_FACTOR);
  
  //Serial.println(wake_after_conversion);
 // Serial.println("Going to sleep now");
  Serial.flush();
  wake_after_conversion = 1;
  
  esp_deep_sleep_start();
  }
  
  else{ 
  sensors.begin();
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
  
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  
 //Serial.println(wake_after_conversion);
 // Serial.println("Going to sleep now");
  Serial.flush();
  wake_after_conversion = 0;
  
  esp_deep_sleep_start();
  }
}

void loop(){
  //This is not going to be called
}