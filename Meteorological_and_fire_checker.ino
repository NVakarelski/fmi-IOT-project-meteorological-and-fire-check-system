#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME ""
#define BLYNK_AUTH_TOKEN ""
// enter credentials from Blynk profile and template
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

// enter network credentials to send information to Blynk
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = ""; //Wifi name

char pass[] = "";  //Wifi Password

Adafruit_BME280 bme;

BlynkTimer timer;

const int flamePin = 22;

int Flame = HIGH;

void myTimerEvent(){
  float temp = bme.readTemperature();

  float pressure = bme.readPressure() / 100.0F;

  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

  float humidity = bme.readHumidity();

  Flame = digitalRead(flamePin);

  Blynk.virtualWrite(V0, temp);

  Blynk.virtualWrite(V1, pressure);

  Blynk.virtualWrite(V2, altitude);

  Blynk.virtualWrite(V3, humidity);

  if (Flame == LOW)
  {
    Blynk.logEvent("FIRE");

    Blynk.virtualWrite(V4, "Fire in the house");
  }
  else
  {
    Blynk.virtualWrite(V4, "Everything is fine");
  }
}
void setup() {
  Serial.begin(9600);
 
  Wire.begin(18, 23);

  unsigned status;

  status = bme.begin(0x76);

  if (!status) {
      while (1) delay(10);
  }

  pinMode(flamePin, INPUT);

  Blynk.begin(auth, ssid, pass);

  timer.setInterval(1000L,myTimerEvent);
}

void loop() {
  Blynk.run();

  timer.run();
}
