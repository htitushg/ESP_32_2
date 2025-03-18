#include <Arduino.h>
#include <WiFi.h>
#include <environment.h>
#include <Application.h>

std::shared_ptr<Application> app = Application::getInstance();

void sensorLoop(void *) {
  while (true) {
    app->sensorLoop();
  }
}

void setup() {
  Serial.begin(BAUD_RATE);
  delay(5000);

  // DEBUG
  Serial.println("Serial connection set up.");

  // DEBUG
  Serial.println("Setting up WiFi...");

  WiFiClient wifi = WiFiClient();
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // DEBUG
  Serial.printf("%s - Connecting to Wi-Fi\n", DEVICE_ID);
  Serial.print("Connecting ");

  int i = 0;
  char symbols[] = ".oO";
  // WiFi.localIP().toString() == "0.0.0.0"
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);

    if (i % 13 == 0) Serial.printf("\nWifi status: %d\nConnecting ", WiFi.status());

    // DEBUG
    Serial.print(symbols[i%3]);
    ++i;
  }

  // DEBUG
  Serial.println();

  //pinMode(LIGHT, OUTPUT);

  // Create Application
  app->initialize(wifi);
  app->startup();

  TaskHandle_t SensorLoopHandle;

  xTaskCreatePinnedToCore(

      // Function to implement the task
      sensorLoop,

      // Name of the task
      "sensor_loop",

      // Stack size in words
      10000,

      // Task input parameter
      NULL,

      // Priority of the task
      0,

      // Task handle.
      &SensorLoopHandle,

      // Core to use
      0);
}

void loop() {
  app->brokerLoop();

  // TODO -> Remove when adding enough elements here
  delay(500);
}
