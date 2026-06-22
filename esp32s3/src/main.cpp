#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>
#include <secrets.h>
#include <WiFi.h>

#define DHTPIN 4
#define DHTTYPE DHT22

#define LED_PIN 48
#define LED_COUNT 1


DHT dht(DHTPIN, DHTTYPE);
// NeoPixel constructor: number of LEDs, pin, color format (GRB) + signal frequency (800 kHz)
Adafruit_NeoPixel pixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println("DHT Test startet...");
  dht.begin();


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Verbinde mit WLAN");
  int attempts = 0;
  // wait for connection; abort after 20 attempts (~10 seconds)
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nWLAN-Verbindung fehlgeschlagen! SSID oder Passwort prüfen.");
    return;
  }
  Serial.println("\nWLAN verbunden! " + WiFi.localIP().toString());


  pixel.begin();
  pixel.setBrightness(50);

  // RSSI (Received Signal Strength Indicator): closer to 0 means stronger signal (e.g. -50 dBm good, -90 dBm weak)
  Serial.println("Signalstärke: " + String(WiFi.RSSI()) + " dBm");
}

void loop() {
 
  delay(10000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // defaults to Celsius; pass true for Fahrenheit

  // isnan() checks if the value is "Not a Number" — happens when the sensor doesn't respond
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Fehler beim Lesen des DHT-Sensors!");
    pixel.setPixelColor(0, pixel.Color(255, 0, 0)); // red = error
    pixel.show();
    return;
  }

 
  Serial.print("Luftfeuchtigkeit: ");
  Serial.print(humidity);
  Serial.print(" %  |  Temperatur: ");
  Serial.print(temperature);
  Serial.println(" *C");

  pixel.setPixelColor(0, pixel.Color(0, 255, 0)); // green = OK, reset after potential error state
  pixel.show();
}