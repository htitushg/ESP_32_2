#include <Arduino.h>
#include <WiFi.h>
#include <environment.h>
#include <Application.h>
#include <utils.h>

std::shared_ptr<Application> app = Application::getInstance();

void sensorLoop(void *) {
    app->sensorLoop();
}

void setup() {
    Serial.begin(BAUD_RATE);
    delay(5000);

    if (IS_DEBUG_MODE) {
        // DEBUG
        Serial.println("Serial connection set up.");
        Serial.println("Setting up WiFi...");
    }

    WiFiClient wifi = WiFiClient();
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    if (IS_DEBUG_MODE) {
        // DEBUG
        Serial.printf("%s - Connecting to Wi-Fi\n", DEVICE_ID);
        Serial.print("Connecting ");
    }

    int i = 0;
    char symbols[] = ".oO";
    while (WiFi.status() != WL_CONNECTED) {
    delay(200);

	if (IS_DEBUG_MODE) {
        // DEBUG
        if (i % 13 == 0) Serial.printf("\nWifi status: %s\nConnecting ", wl_status_to_string(WiFi.status()));
        Serial.print(symbols[i%3]);
	}

    ++i;
    }

    if (IS_DEBUG_MODE) {
        // DEBUG
        Serial.println();
    }

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

    // Substract 500ms to a_publish_interval to make sure to print the message while sensorLoop is resetting
    // a_last_published_time every a_publish_interval
	if (millis() - app->getLastPublishedTime() > (app->getPublishInterval() - 500)) {

	    if (IS_DEBUG_MODE) {
            // DEBUG
            Serial.println("Running broker loop...");
	    }
    }

  	// TODO -> Remove when adding enough elements here
  	delay(500);
}
