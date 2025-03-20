#include <Arduino.h>
#include <WiFi.h>
#include <environment.h>
#include <Application.h>
#include <utils.h>

auto app = Application::getInstance();

void connectToWiFi() {
  	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    if (IS_DEBUG_MODE) {
        // DEBUG
        Serial.printf("%s - Connecting to Wi-Fi\n", DEVICE_ID);
        Serial.print("Connecting ");
    }

    int i = 0;
    char symbols[] = "..ooOO ";

    while (WiFi.status() != WL_CONNECTED) {

    	delay(200);

		if (IS_DEBUG_MODE) {
        	// DEBUG
        	if (i % 14 == 0) Serial.printf("\nWifi status: %s\nConnecting ", wl_status_to_string(WiFi.status()));
        	Serial.print(symbols[i%7]);
		}

    	++i;
    }

    if (IS_DEBUG_MODE) {
        // DEBUG
        Serial.println();
    }
}

void sensorLoop(void *) {
    app->sensorLoop();
}

void messageHandler(MQTTClient * client, char topic[], char payload[], int length) {
  app->messageHandler(topic, payload, length);
}

void setup() {
    Serial.begin(BAUD_RATE);

    Serial.printf("Setting up %s...\n", DEVICE_ID);

    // TODO -> remove when code is finished and functional
    delay(1000);

    if (IS_DEBUG_MODE) {
        // DEBUG
        Serial.println("Serial connection set up.");
        Serial.println("Setting up WiFi...");
    }

    WiFiClient wifi = WiFiClient();
    WiFi.mode(WIFI_STA);

    connectToWiFi();

    //pinMode(LIGHT, OUTPUT);

    // Create Application
    app->initialize(wifi, messageHandler);
    app->startup();

//    TaskHandle_t SensorLoopHandle;
//
//    xTaskCreatePinnedToCore(
//
//      // Function to implement the task
//      sensorLoop,
//
//      // Name of the task
//      "sensor_loop",
//
//      // Stack size in words
//      10000,
//
//      // Task input parameter
//      NULL,
//
//      // Priority of the task
//      0,
//
//      // Task handle.
//      &SensorLoopHandle,
//
//      // Core to use
//      0);
}

void loop() {
  	app->brokerLoop();

    if (WiFi.status() != WL_CONNECTED) {

      	if (IS_DEBUG_MODE) {
          	// DEBUG
          	Serial.printf("WiFi status: %s\n", wl_status_to_string(WiFi.status()));
      	}

		connectToWiFi();
    }
    if (!app->brokerStatus()) {
		if (IS_DEBUG_MODE) {
          	// DEBUG
          	Serial.printf("MQTT Broker status: %s\n", toString(app->brokerStatus()).c_str());
      	}

		app->reconnectBroker(messageHandler);
    };

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
