#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>

#define DHTPIN 4 // Pin, an dem der DHT-Sensor angeschlossen ist
#define DHTTYPE DHT22 // DHT 22 (AM2302)

#define LED_PIN 48
#define LED_COUNT 1

DHT dht(DHTPIN, DHTTYPE);
Adafruit_NeoPixel pixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  delay(1000); // Warte kurz, damit die serielle Verbindung stabil ist
  Serial.println("DHT Test startet...");
  dht.begin();

  pixel.begin();
  pixel.setBrightness(50);
  pixel.setPixelColor(0, pixel.Color(255, 255, 0)); // gelb = Start
  pixel.show();
}

void loop() {
  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Fehler beim Lesen des DHT-Sensors!");
    pixel.setPixelColor(0, pixel.Color(255, 0, 0)); // rot = Fehler
    pixel.show();
    return;
  }

  Serial.print("Luftfeuchtigkeit: ");
  Serial.print(humidity);
  Serial.print(" %  |  Temperatur: ");
  Serial.print(temperature);
  Serial.println(" *C");

  pixel.setPixelColor(0, pixel.Color(0, 255, 0)); // grün = OK
  pixel.show();
}