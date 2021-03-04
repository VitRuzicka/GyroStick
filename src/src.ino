#include <TFT_eSPI.h>
#include <SPI.h>
#include "WiFi.h"
#include <Wire.h>
#include "Button2.h"
#include "esp_adc_cal.h"
#include "bmp.h"
#include <analogWrite.h>


// #define TFT_MOSI            19
// #define TFT_SCLK            18
// #define TFT_CS              5
// #define TFT_DC              16
// #define TFT_RST             23
// #define TFT_BL              4   

#define JAS                 50
#define CONNECTED             1  //predelat az se esp pripoji pres esp-now
#define ADC_EN              14  
#define ADC_BATT             34
#define BUTTON_1            35
#define BUTTON_2            0
int vref = 1100;



TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library



void espDelay(int ms)
{
  esp_sleep_enable_timer_wakeup(ms * 1000);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
  esp_light_sleep_start();
}

void showMenu(){
  tft.setTextSize(2);
  static uint64_t timeStamp = 0;
  if (millis() - timeStamp > 1000) { //refreah rate kazduch 1000 ms
    timeStamp = millis();
    tft.fillScreen(TFT_BLACK);
    uint16_t v = analogRead(ADC_BATT);
    float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0) - 0.2;
    tft.drawString(String(battery_voltage) + "V", tft.width(), 0);
    if(CONNECTED){
  tft.pushImage(0, 0,  20, 15, wifi);
  }
  tft.setTextSize(1);
  tft.setTextDatum(BC_DATUM);
  tft.drawString(WiFi.macAddress(), tft.width()/2, tft.height());
  }
}



void setup() {
  //VYSTUPY
  pinMode(BUTTON_1, INPUT);
  pinMode(4, OUTPUT); //ovladani jasu



  //DISPLAY
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(0, 0);
  tft.setTextDatum(TR_DATUM);
  tft.setTextSize(2);
  tft.setSwapBytes(true);
  tft.pushImage(0, 0,  135, 240, logo);
  espDelay(1000);                 /////neblokujici nebo alepson doufam
  analogWrite(4, JAS);
  tft.fillScreen(TFT_BLACK);


  WiFi.mode(WIFI_MODE_STA); //soucast esp-now

}
void loop() {
  
showMenu();
}
