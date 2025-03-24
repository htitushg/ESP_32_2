# HomeIoT

<!-- TOC -->
* [MQTT](#mqtt)
* [Startup/Setup message](#startupsetup-message)
* [Class diagram](#class-diagram)
* [Setup sequence diagram](#setup-sequence-diagram)
<!-- TOC -->

---

## MQTT

```mermaid
---
title: MQTT topics/channels
---

flowchart LR
    system["`_SYSTEM NAME_`"]
    location_type["`_LOCATION TYPE_`"]
    location_id["`_LOCATION ID_`"]
    device_type["`_DEVICE TYPE_`"]
    device_id["`_DEVICE ID_`"]
    module["`_MODULE_`"]
    
    home["`**home**`"]
    
    kitchen["`**kitchen**`"]
    garden["`**garden**`"]
    room["`**room**`"]
    
    kitchen_id["`**1**`"]
    room1_id["`**2**`"]
    room2_id["`**3**`"]
    garden_id["`**4**`"]
    
    light["`**light**`"]
    water_plant["`**waterPlant**`"]
    
    device1["`**ESP32-af6f-43b1-a20e**`"]
    device2["`**ESP32-db87-47b7-998d**`"]
    device3["`**ESP32-808e-4c62-9c4f**`"]
    device4["`**ESP32-0158-4d61-85c2**`"]
    
    light_controller["`**lightController**`"]
    luminosity_sensor["`**luminositySensor**`"]
    water_valve["`**waterValve**`"]
    humidity_sensor["`**humiditySensor**`"]
    
    subgraph OUR SYSTEM NAME
        system
        home
    end
    subgraph LOCATION
        subgraph  
        location_type
        kitchen
        garden
        room
        end
        subgraph  
        location_id
        kitchen_id
        garden_id
        room1_id
        room2_id
        end
    end
    subgraph DEVICE 
       subgraph  
        device_type
        light
        water_plant
        end
        subgraph  
        device_id
        device1
        device2
        device3
        device4
        end 
    end
    subgraph SPECIFIC CHANNEL
        module
        light_controller
        luminosity_sensor
        water_valve
        humidity_sensor
    end

    system  --- location_type   --- location_id     --- device_type     --- device_id   --- module
    home    --- kitchen     --- kitchen_id  --- light           --- device1     --- light_controller & luminosity_sensor
    home    --- garden      --- garden_id   --- water_plant     --- device2     --- water_valve & humidity_sensor
    home    --- room        --- room1_id    --- light           --- device3     --- light_controller & luminosity_sensor
    home    --- room        --- room2_id    --- light           --- device4     --- light_controller & luminosity_sensor
```

## Startup/Setup message
```json
{
  "id": "ESP32-af6f-43b1-a20e",
  "type": "light",
  "location_id": 3,
  "location_type": "room",
  "location_name": "room 3",
  "modules": [
    {
      "name": "lightController",
      "value": "False"
    },
    {
      "name": "lightSensor",
      "value": "True"
    },
    {
      "name": "luminositySensor",
      "value": "150.0"
    },
    {
      "name": "presenceDetector",
      "value": "True"
    },
    {
      "name": "temperatureSensor",
      "value": "22.5"
    },
    {
      "name": "consumptionSensor",
      "value": "32.45"
    }
  ]
}
```

## Class diagram
```mermaid
---
title: Device class diagram
---
classDiagram
    class Application {
        # Application *app$
        # String location
        # unsigned int locationID
        # String root_topic
        # bool wait_for_setup
        # unsigned int publish_interval
        # unsigned long lastPublishTime
        # WiFiClient network
        # Broker *broker
        # IModule *lightController
        # IModule *lightSensor
        # IModule *luminositySensor
        # IModule *presenceDetector
        # IModule *temperatureSensor
        # IModule *consumptionSensor
        
        # isWaitingForSetup() bool
        # onSetupMessage(char payload[]) void
        # reset() void
        # setupModule(const char* name, const char* value) void
        # messageHandler(MQTTClient *client, char topic[], char payload[], int length) void$
        # unsubscribeAllTopics() void
        # setRootTopic() void

        + getInstance() Application*$
        + Application() Application
        + brokerLoop() void
        + startup() void
        + init(WiFiClient wifi) void
        + sensorLoop() void$
    }
    class Broker {
        # MQTTClient mqtt
        # WiFiClient wifi
        # String root_topic

        # Broker(WiFiClient network) Broker

        + newBroker(WiFiClient network, void cb(MQTTClient *client, char topic[], char bytes[], int length)) Broker*$
        + sub(const String &module_name) void
        + pub(const String &module_name, const String &value) void
        + unsub(const String &module_name) void
        + setRootTopic(const String &topic) void
        + loop() void
    }
    class IModule {
        <<interface>>
        # Broker *broker
        # String name
        + setValue(const char * value) void*
        + getValue() const String*
        + getValueReference() const void**
        + getName() String
    }
    class ModuleFactory {
        + newModule(Broker *broker, String type) IModule*$
    }
    class IObservable {
        <<interface>>
        + ~IObservable()*
        + Attach(IObserver *observer) void*
        + Detach(IObserver *observer) void*
        + Notify() void*
    }
    class IObserver {
        <<interface>>
        + ~IObserver()*
        + Update(const String &value) void*
    }
    class LightController {
        # Broker *broker
        # String name
        # bool value
        
        + LightController(Broker *broker, bool value) LightController
        + setValue(const char * value) void
        + getValue() const String
        + getValueReference() const * void
        + getName() String
        + Attach(IObserver *observer) void
        + Detach(IObserver *observer) void
        + Notify() void
        + Update(const String &value) void
    }
%%    class LightSensor {
%%        # Broker *broker
%%        # String name
%%        # bool value
%%
%%        + LightSensor(Broker *broker, bool value) LightSensor
%%        + setValue(const char * value) void
%%        + getValue() const String
%%        + getValueReference() const * void
%%        + getName() String
%%        + Attach(IObserver *observer) void
%%        + Detach(IObserver *observer) void
%%        + Notify() void
%%        + Update(const String &value) void
%%    }
    class PresenceDetector {
        # Broker *broker
        # String name
        # bool value

        + PresenceDetector(Broker *broker, bool value) PresenceDetector
        + setValue(const char * value) void
        + getValue() const String
        + getValueReference() const * void
        + getName() String
        + Attach(IObserver *observer) void
        + Detach(IObserver *observer) void
        + Notify() void
        + Update(const String &value) void
    }
%%    class LuminositySensor {
%%        # Broker *broker
%%        # String name
%%        # float value
%%
%%        + LuminositySensor(Broker *broker, float value) LuminositySensor
%%        + setValue(const char * value) void
%%        + getValue() const String
%%        + getValueReference() const * void
%%        + getName() String
%%        + Attach(IObserver *observer) void
%%        + Detach(IObserver *observer) void
%%        + Notify() void
%%        + Update(const String &value) void
%%    }
%%    class TemperatureSensor {
%%        # Broker *broker
%%        # String name
%%        # float value
%%
%%        + TemperatureSensor(Broker *broker, float value) TemperatureSensor
%%        + setValue(const char * value) void
%%        + getValue() const String
%%        + getValueReference() const * void
%%        + getName() String
%%        + Attach(IObserver *observer) void
%%        + Detach(IObserver *observer) void
%%        + Notify() void
%%        + Update(const String &value) void
%%    }
%%    class ConsumptionSensor {
%%        # Broker *broker
%%        # String name
%%        # float value
%%
%%        + ConsumptionSensor(Broker *broker, float value) ConsumptionSensor
%%        + setValue(const char * value) void
%%        + getValue() const String
%%        + getValueReference() const * void
%%        + getName() String
%%        + Attach(IObserver *observer) void
%%        + Detach(IObserver *observer) void
%%        + Notify() void
%%        + Update(const String &value) void
%%    }
    Application *.. IModule
    ModuleFactory ..> IModule
    Application --> ModuleFactory
    Application *.. Broker
    LightController ..|> IModule
%%    LightSensor ..|> IModule
    PresenceDetector ..|> IModule
%%    LuminositySensor ..|> IModule
%%    TemperatureSensor ..|> IModule
%%    ConsumptionSensor ..|> IModule
%%    LightController *.. Broker
%%    LightSensor *.. Broker
%%    PresenceDetector *.. Broker
%%    LuminositySensor *.. Broker
%%    TemperatureSensor *.. Broker
%%    ConsumptionSensor *.. Broker
    IObservable *.. IObserver
    IModule ..|> IObserver
    IModule ..|> IObservable
```

## Setup sequence diagram

```mermaid
sequenceDiagram
    autonumber
    box Teal Device
    participant S as Setup
    participant A as Application
    participant B as Broker
    participant LiC as LightController
    participant LiS as LightSensor
    participant PD as PresenceDetector
    participant LuS as LuminositySensor
    participant TS as TemperatureSensor
    participant CS as ConsumptionSensor
    end
    box Green Server
    participant MQTT as MQTT Broker
    participant Server as Go Application
    participant DB
    end
    S ->> A: Instanciate
    A ->> B: Instanciate
    B ->> MQTT: Connect
    A ->> B: Prepare `startup` message
    B ->> MQTT: Send `startup` message
    MQTT ->> Server: Relay `startup` message from Device
    Server ->> DB: Check if Device exists and creates it if necessary
    DB ->> Server: Send Device data back to prepare `setup` message
    Server ->> MQTT: Send `setup` message
    MQTT ->> B: Relay `setup` message
    B ->> A: Parse `setup` message
    A ->> A: Update `location` and `locationID`
    A ->> LiC: Update value
    A ->> LiS: Update value
    A ->> PD: Update value
    A ->> LuS: Update value
    A ->> TS: Update value
    A ->> CS: Update value
    A ->> S: Setup complete
```

```mermaid
sequenceDiagram
    autonumber
    box teal Light
        participant LC as Light Controller
        participant PD as Presence Detector
        participant LS as Luminosity Sensor
        participant TS as Temperature Sensor
        participant LB as Light Bulb
    end
    box magenta Server
        participant MQTT as MQTT Broker
    end
    box green User
        participant C as Client
    end
    
    LC --> MQTT : subscribe LightController channel
    
    loop updateSensors
        PD --> MQTT : send updated presence status
        LS --> MQTT : send updated luminosity
        TS --> MQTT : send updated temperature
    end
    
    C --> PD : client entered in room
    PD --> MQTT : presence detected
    PD --> LC : presence detected
    
    LC --> LS : get luminosity
    opt: Luminosity inferior than 100
        LC --> LB : turn on light
        LC --> MQTT : light turned on
    end
```