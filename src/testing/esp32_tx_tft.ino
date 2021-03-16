#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
//esp_now
#include <esp_now.h>
#include <WiFi.h>
//displej
#include <TFT_eSPI.h>
#include <SPI.h>
#include "WiFi.h"
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


TFT_eSPI tft = TFT_eSPI(135, 240);
MPU6050 mpu;

uint8_t broadcastAddress[] = {0x5C, 0xCF, 0x7F, 0xAB, 0xAE, 0x53};
#define LED_PIN 13 // číslo pinu s integrovanou LED diodou
#define INTERRUPT_PIN 15
bool dmpReady = false;  // když je DPM připraveno, obsahuje true
uint8_t mpuIntStatus;   // stav externího přerušení z DPM
uint8_t devStatus;      // stav poslední operace
uint16_t packetSize;    // velikost paketu z DPM
uint16_t fifoCount;     // počet bytů ve FIFO zásobníku
uint8_t fifoBuffer[64]; // FIFO zásobník
float offset;
bool CONNECTED = false;

typedef struct struct_message {
  byte roll;
  byte pitch;
  byte throttle;
  byte yaw;
  bool arm;
  byte settings;
  byte aux1;
} struct_message;

struct_message zprava;


// proměnné důležité pro výpočet
Quaternion q;           // [w, x, y, z]         kvaternion
VectorFloat gravity;    // [x, y, z]            vektor setrvačnosti
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll úhly

volatile bool mpuInterrupt = false;     // true pokud DMP vyvolalo přerušení




////    FUNC



void ICACHE_RAM_ATTR dmpDataReady() {
  mpuInterrupt = true;
}

void dataOdeslana(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    CONNECTED = true ;
  }
  else {
    CONNECTED = false;
  }

}


void showMenu() {
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK);
  uint16_t v = analogRead(ADC_BATT);
  float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0) - 0.2;
  tft.drawString(String(battery_voltage) + "V", tft.width(), 0);
  if (CONNECTED) {
    tft.pushImage(0, 0,  20, 15, wifi);
  }
  tft.setTextSize(1);
  tft.setTextDatum(BC_DATUM);
  tft.drawString(WiFi.macAddress(), tft.width() / 2, tft.height());
}




//////      SETUP






void setup() {


  Wire.begin();
  Serial.begin(115200);
  mpu.initialize();

  
  pinMode(INTERRUPT_PIN, INPUT);
  // ověříme připojení k MPU-6050
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  
  pinMode(LED_PIN, OUTPUT);  //indikace cteni dat s gyra
  digitalWrite(LED_PIN, LOW);
  devStatus = mpu.dmpInitialize();  //inicializace dmp


  // supply your own gyro offsets here, scaled for min sensitivity
  //  mpu.setXGyroOffset(220);
  //  mpu.setYGyroOffset(76);
  //  mpu.setZGyroOffset(-85);
  //  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip


  // ujistíme se, že funguje
  if (devStatus == 0) {

    mpu.CalibrateAccel(6);   //kalibrace na 0
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();




    mpu.setDMPEnabled(true);  //aktivace dmp procesoru

    
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    Serial.println(F("DMP pripraveno, cekani na preruseni..."));
    dmpReady = true;

    // zjistíme si, jak velké pakety DMP vrací
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    //doslo k chybe pripojeni k mpu - vypsat hlasku
  }



  ////   ESP-NOW



  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(dataOdeslana);

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }


  unsigned long predchoziCas = 0;
  const long refreshRate = 1000; //cas obnoveni displaye v ms


}





/////             LOOP




void loop() {

  unsigned long dobaOdSpusteni = millis();
  // pokud není DMP připravené, nebudeme dělat nic
  if (!dmpReady) return;

  // zde provádíme náš kód, cyklus ověřuje, zda nemá DMP připravena nějaká data
  while (!mpuInterrupt && fifoCount < packetSize) {
    if (mpuInterrupt && fifoCount < packetSize) {      //uprava zde snaha o to dostat se z nekonečné smyčky, pokud by to nefungovalo, je třeba to upravit
      fifoCount = mpu.getFIFOCount();
    }
  }

  // resetujeme proměnnou informující o přerušení vyvolané z DMP a získáme INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // získáme velikost FIFO zásobníku
  fifoCount = mpu.getFIFOCount();

  // zjistíme, zda nedošlo k přetečené zásobníku
  // pokud k němu dojde, je třeba optimalizovat kód v cyklu výše,
  // případně přerušit provádění mezi delšími výpočty, je-li to třeba
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    // vyčistíme zásobník
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));

    // pokud je vše v pořádku, zpracujeme data z DMP
  } else if (mpuIntStatus & 0x02) {
    // čekání na správnou délku dat
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // přečteme paket ze zásobníku
    mpu.getFIFOBytes(fifoBuffer, packetSize);

    // pokud je v zásobníku víc než jeden paket, tímto zajistíme, že bude přečten v dalším cyklu
    fifoCount -= packetSize;

    // naplnění proměnných s vypočítanými hodnotami
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    ypr[0] = ypr[0] * 180 / M_PI; //prevod z radianu na stupne
    ypr[1] = ypr[1] * 180 / M_PI;
    ypr[2] = ypr[2] * 180 / M_PI;

    if (dobaOdSpusteni > 18 * 1000 && offset == 0.0) {  //18 vterin po spusteni se vynuluje YAW
      Serial.println("nulovani gyra");
      offset = ypr[0];
    }

    ypr[0] = ypr[0] - offset;
    zprava.roll = ypr[2];
    zprava.pitch = ypr[1];
    zprava.throttle = random(); //dodelat!!!!!!! vcetne prvni kalibrace halla
    zprava.yaw = ypr[0];
    zprava.arm = random();   //dodelat chovani armu
    zprava.settings = random();
    zprava.aux1 = random(1, 250);

  }

  if (dobaOdSpusteni - predchoziCas >= refreshRate) { //obnoveni displaye
    predchoziCas = cas;
    showMenu();
  }

}
