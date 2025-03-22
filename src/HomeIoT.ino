#include <Arduino.h>
#include <WiFi.h>
#include <environment.h>
#include <Application.h>
#include <utils.h>

auto app = Application::getInstance();

void connectToWiFi() {
  	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // DEBUG
    DEBUG_MODE_PRINTF("%s - Connecting to Wi-Fi\n", DEVICE_ID);
    DEBUG_MODE_PRINT("Connecting ");

    int i = 0;
    char symbols[] = "..ooOO ";

    while (WiFi.status() != WL_CONNECTED) {

    	delay(200);

        // DEBUG
        if (i % 14 == 0) DEBUG_MODE_PRINTF("\nWifi status: %s\nConnecting ", wl_status_to_string(WiFi.status()));
        DEBUG_MODE_PRINT(symbols[i%7]);

    	++i;
    }

    // DEBUG
    DEBUG_MODE_PRINTLN();
}

void sensorLoop(void *) {
    app->sensorLoop();
}

void messageHandler(MQTTClient * client, char topic[], char payload[], int length) {
  app->messageHandler(topic, payload, length);
}

void setup() {
//  	#ifdef DEBUG_MODE
    	Serial.begin(BAUD_RATE);
//    #endif

    Serial.printf("Initializing %s...\n", DEVICE_ID);

    DEBUG_MODE_PRINTF("Setting up %s...\n", DEVICE_ID);

    // TODO -> remove when code is finished and functional
    delay(1000);

    // DEBUG
    DEBUG_MODE_PRINTLN("Serial connection set up.");
    DEBUG_MODE_PRINTLN("Setting up WiFi...");

    WiFiClient wifi = WiFiClient();
    WiFi.mode(WIFI_STA);

    connectToWiFi();

    pinMode(LIGHT, OUTPUT);
    pinMode(LIGHT_VOLTAGE, OUTPUT);
    digitalWrite(LIGHT_VOLTAGE, HIGH);

    pinMode(PRESENCE_DETECTOR_PIN, INPUT);
    pinMode(PRESENCE_DETECTOR_VOLTAGE, OUTPUT);
    digitalWrite(PRESENCE_DETECTOR_VOLTAGE, HIGH);

    pinMode(TEMPERATURE_SENSOR_PIN, INPUT);
    pinMode(TEMPERATURE_SENSOR_VOLTAGE, OUTPUT);
    digitalWrite(TEMPERATURE_SENSOR_VOLTAGE, HIGH);

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

        // DEBUG
        DEBUG_MODE_PRINTF("WiFi status: %s\n", wl_status_to_string(WiFi.status()));

		connectToWiFi();
    }
    if (!app->brokerStatus()) {
        // DEBUG
        DEBUG_MODE_PRINTF("MQTT Broker status: %s\n", toString(app->brokerStatus()).c_str());

		app->reconnectBroker(messageHandler);
    };

    // Substract 500ms to a_publish_interval to make sure to print the message while sensorLoop is resetting
    // a_last_published_time every a_publish_interval
	if (millis() - app->getLastPublishedTime() > (app->getPublishInterval() - 500)) {

        // DEBUG
        DEBUG_MODE_PRINTLN("Running broker loop...");

        app->readInputSensor();

        app->resetTime();
    }

  	// TODO -> Remove when adding enough elements here
  	delay(500);
}
