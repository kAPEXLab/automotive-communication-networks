# Assignment 2: Multi-ECU Service-Oriented Communication Using SOME/IP

## Mini-Project: Virtual Vehicle Service Network Using Python and `someipy`

---

## 1. Purpose

In Assignment 1, you developed a basic SOME/IP system consisting of one service provider and one service consumer.

The system contained:

```text
Vehicle ECU
    |
    +-- VehicleSpeedService
            |
            +-- GetSpeed()
            |
            +-- SpeedChanged
```

The Instrument Cluster consumed the `VehicleSpeedService`.

Assignment 2 extends this concept into a **multi-ECU service-oriented vehicle system**.

Multiple Linux terminals shall represent different virtual ECUs.

The system shall contain:

- multiple virtual ECUs;
- multiple SOME/IP services;
- multiple Service Providers;
- multiple Service Consumers;
- methods;
- events;
- event groups;
- Service Discovery;
- request-response communication;
- publish-subscribe communication;
- multiple consumers of the same service;
- one consumer using multiple services;
- service failure and recovery experiments;
- Wireshark observation.

Unlike Assignment 1, complete implementation code is not provided.

You are expected to extend the knowledge gained in Assignment 1 and develop the required functionality.

---

## 2. Learning Outcomes

After completing Assignment 2, you should be able to:

1. Design a small multi-ECU service-oriented system.
2. Implement multiple SOME/IP services.
3. Implement multiple Service Providers.
4. Implement multiple Service Consumers.
5. Define and use Service IDs and Instance IDs.
6. Define and implement multiple methods.
7. Define and implement multiple events.
8. Define and use event groups.
9. Implement request-response communication.
10. Implement publish-subscribe communication.
11. Use SOME/IP Service Discovery with multiple services.
12. Demonstrate multiple consumers using the same service.
13. Demonstrate one consumer using multiple services.
14. Validate method input at the Service Provider.
15. Observe service failure and recovery.
16. Analyse multi-service SOME/IP communication using Wireshark.
17. Explain why particular vehicle functionality is implemented as a method or an event.

---

## 3. Prerequisite

Assignment 1 must be completed before starting Assignment 2.

The following Assignment 1 functionality shall remain operational:

```text
VehicleSpeedService

Service ID:
0x1234

Instance ID:
0x5678

Method:
GetSpeed()

Method ID:
0x0001

Event:
SpeedChanged

Event ID:
0x8001

Event Group ID:
0x0001
```

Assignment 2 shall extend this existing implementation.

Do not discard the Assignment 1 project and create an unrelated application.

---

## 4. Assignment 2 Vehicle Scenario

The virtual vehicle shall contain four logical ECUs.

```text
ECU 1: Vehicle ECU

ECU 2: Instrument Cluster ECU

ECU 3: Climate ECU

ECU 4: Central Display ECU
```

Each ECU shall run as a separate application.

At least four terminals shall therefore be used to represent the four ECU applications.

The required `someipy` daemon shall run separately according to the environment established in Assignment 1.

---

## 5. Virtual ECU Responsibilities

### ECU 1: Vehicle ECU

The Vehicle ECU shall provide:

```text
VehicleSpeedService
```

The service shall contain:

```text
Methods:

GetSpeed()
GetVehicleState()


Events:

SpeedChanged
OverspeedWarning
```

The Vehicle ECU is therefore a:

```text
Service Provider
```

---

### ECU 2: Instrument Cluster ECU

The Instrument Cluster shall consume:

```text
VehicleSpeedService
```

It shall:

```text
Call GetSpeed()

Call GetVehicleState()

Subscribe to SpeedChanged

Subscribe to OverspeedWarning
```

The Instrument Cluster is therefore a:

```text
Service Consumer
```

---

### ECU 3: Climate ECU

The Climate ECU shall provide:

```text
ClimateService
```

The service shall contain:

```text
Methods:

GetCabinTemperature()
SetTargetTemperature()


Event:

CabinTemperatureChanged
```

The Climate ECU is therefore a:

```text
Service Provider
```

---

### ECU 4: Central Display ECU

The Central Display shall consume both:

```text
VehicleSpeedService
```

and:

```text
ClimateService
```

For `VehicleSpeedService`, it shall:

```text
Call GetSpeed()

Call GetVehicleState()

Subscribe to SpeedChanged

Subscribe to OverspeedWarning
```

For `ClimateService`, it shall:

```text
Call GetCabinTemperature()

Call SetTargetTemperature()

Subscribe to CabinTemperatureChanged
```

The Central Display demonstrates that:

> One ECU application can consume services offered by more than one Service Provider.

---

## 6. Overall Architecture

The logical architecture is:

```text
                 VIRTUAL VEHICLE NETWORK


Vehicle ECU                              Climate ECU
Provider                                 Provider
    |                                        |
    |                                        |
VehicleSpeedService                    ClimateService
    |                                        |
    |                                        |
    +----------------+    +------------------+
                     |    |
                     v    v
                 Central Display
                    Consumer


Vehicle ECU
    |
    |
VehicleSpeedService
    |
    +-------------------------+
    |                         |
    v                         v
Instrument Cluster       Central Display
Consumer                 Consumer
```

This architecture demonstrates two important SOME/IP concepts:

```text
1. Multiple consumers can consume the same service.

2. One consumer can consume multiple services.
```

---

# 7. VehicleSpeedService Specification

The `VehicleSpeedService` developed in Assignment 1 shall be extended.

## Service Definition

| Parameter | Value |
|---|---|
| Service Name | `VehicleSpeedService` |
| Service ID | `0x1234` |
| Instance ID | `0x5678` |
| Major Version | `1` |
| Transport | UDP |

---

## Method 1: GetSpeed

Retain the Assignment 1 method.

| Parameter | Value |
|---|---|
| Method | `GetSpeed` |
| Method ID | `0x0001` |
| Request Payload | Empty |
| Response | Current vehicle speed |
| Data Type | `uint16` |
| Unit | km/h |

---

## Method 2: GetVehicleState

Add a second method:

```text
GetVehicleState()
```

Use:

| Parameter | Value |
|---|---|
| Method | `GetVehicleState` |
| Method ID | `0x0002` |
| Request Payload | Empty |
| Response | Vehicle state |
| Data Type | `uint8` |

Use the following states:

```text
0 = STOPPED

1 = MOVING
```

The state shall be determined from the existing simulated vehicle speed.

Use:

```text
If speed == 0:

    Vehicle State = STOPPED
```

Use:

```text
If speed > 0:

    Vehicle State = MOVING
```

---

## Event 1: SpeedChanged

Retain the Assignment 1 event.

| Parameter | Value |
|---|---|
| Event | `SpeedChanged` |
| Event ID | `0x8001` |
| Event Group ID | `0x0001` |
| Payload | Current vehicle speed |
| Data Type | `uint16` |
| Unit | km/h |

---

## Event 2: OverspeedWarning

Add:

```text
OverspeedWarning
```

Use:

| Parameter | Value |
|---|---|
| Event | `OverspeedWarning` |
| Event ID | `0x8002` |
| Event Group ID | `0x0002` |
| Payload | Current vehicle speed |
| Data Type | `uint16` |
| Unit | km/h |
| Trigger | Speed greater than 80 km/h |

The trigger condition shall be:

```text
current_speed_kmph > 80
```

Expected behaviour:

```text
Speed = 70 km/h
SpeedChanged generated
OverspeedWarning not generated
```

```text
Speed = 80 km/h
SpeedChanged generated
OverspeedWarning not generated
```

```text
Speed = 90 km/h
SpeedChanged generated
OverspeedWarning generated
```

```text
Speed = 100 km/h
SpeedChanged generated
OverspeedWarning generated
```

---

# 8. ClimateService Specification

Create a second SOME/IP service:

```text
ClimateService
```

## Service Definition

| Parameter | Value |
|---|---|
| Service Name | `ClimateService` |
| Service ID | `0x2234` |
| Instance ID | `0x0001` |
| Major Version | `1` |
| Transport | UDP |

---

## Method 1: GetCabinTemperature

Implement:

```text
GetCabinTemperature()
```

Use:

| Parameter | Value |
|---|---|
| Method | `GetCabinTemperature` |
| Method ID | `0x0001` |
| Request Payload | Empty |
| Response | Current cabin temperature |
| Data Type | `int16` |

Temperature shall be represented in tenths of a degree Celsius.

For example:

```text
240 = 24.0 degree Celsius

245 = 24.5 degree Celsius

250 = 25.0 degree Celsius
```

This allows the service to represent decimal temperatures using integers.

---

## Method 2: SetTargetTemperature

Implement:

```text
SetTargetTemperature()
```

Use:

| Parameter | Value |
|---|---|
| Method | `SetTargetTemperature` |
| Method ID | `0x0002` |
| Request | Requested target temperature |
| Request Data Type | `int16` |
| Response | Status |
| Response Data Type | `uint8` |

Temperature shall again be represented in tenths of a degree Celsius.

For example:

```text
220 = 22.0 degree Celsius

235 = 23.5 degree Celsius
```

The valid target temperature range shall be:

```text
16.0 degree Celsius to 30.0 degree Celsius
```

In the payload representation:

```text
160 to 300
```

Use the following response status:

```text
0 = ACCEPTED

1 = INVALID_TEMPERATURE
```

Example valid request:

```text
Request:

Target Temperature = 220


Meaning:

22.0 degree Celsius


Response:

ACCEPTED
```

Example invalid request:

```text
Request:

Target Temperature = 350


Meaning:

35.0 degree Celsius


Response:

INVALID_TEMPERATURE
```

---

## Event: CabinTemperatureChanged

Implement:

```text
CabinTemperatureChanged
```

Use:

| Parameter | Value |
|---|---|
| Event | `CabinTemperatureChanged` |
| Event ID | `0x8001` |
| Event Group ID | `0x0001` |
| Payload | Cabin temperature |
| Data Type | `int16` |

The Climate ECU shall simulate changing cabin temperature.

A simple sequence may be:

```text
24.0 degree Celsius

24.5 degree Celsius

25.0 degree Celsius

25.5 degree Celsius

26.0 degree Celsius
```

Whenever the simulated cabin temperature changes, the Climate ECU shall generate:

```text
CabinTemperatureChanged
```

Subscribed consumers shall receive the updated temperature.

---

# Task 11: Extend VehicleSpeedService

## Objective

Extend the Assignment 1 `VehicleSpeedService`.

Add:

```text
GetVehicleState()

OverspeedWarning
```

The existing functionality must remain operational:

```text
GetSpeed()

SpeedChanged
```

---

## Step 11.1: Extend the Interface

Modify:

```text
src/vehicle_speed_interface.py
```

Add:

```text
GET_VEHICLE_STATE_METHOD_ID = 0x0002

OVERSPEED_WARNING_EVENT_ID = 0x8002

OVERSPEED_EVENTGROUP_ID = 0x0002
```

Create the required method definition for:

```text
GetVehicleState
```

Create the required event and event group definitions for:

```text
OverspeedWarning
```

Add them to the existing:

```text
VehicleSpeedService
```

---

## Step 11.2: Define Vehicle State Payload

Create an appropriate serialisable payload for the vehicle state.

Use:

```text
uint8
```

with:

```text
0 = STOPPED
1 = MOVING
```

---

## Step 11.3: Implement GetVehicleState

Modify:

```text
src/vehicle_speed_service.py
```

Add a method handler for:

```text
GetVehicleState()
```

The handler shall:

```text
Receive request

Read current_speed_kmph

Determine vehicle state

Create response payload

Return response
```

Use:

```text
current_speed_kmph == 0
```

to represent:

```text
STOPPED
```

and:

```text
current_speed_kmph > 0
```

to represent:

```text
MOVING
```

---

## Step 11.4: Implement OverspeedWarning

Use the existing speed simulation.

After updating the vehicle speed:

```text
Send SpeedChanged
```

Then check:

```text
Is current speed greater than 80 km/h?
```

If yes:

```text
Send OverspeedWarning
```

If no:

```text
Do not send OverspeedWarning
```

The existing `SpeedChanged` event shall continue to be generated for all simulated speed changes.

---

# Task 12: Extend the Instrument Cluster ECU

## Objective

Modify the Instrument Cluster so that it uses all functionality provided by `VehicleSpeedService`.

Modify:

```text
src/cluster_client.py
```

---

## Required Method Communication

The Instrument Cluster shall call:

```text
GetSpeed()
```

and:

```text
GetVehicleState()
```

---

## Required Event Communication

The Instrument Cluster shall subscribe to:

```text
SpeedChanged Event Group
```

and:

```text
OverspeedWarning Event Group
```

---

## Required Output

Normal operation may display:

```text
VehicleSpeedService AVAILABLE

Vehicle Speed : 50 km/h

Vehicle State : MOVING
```

When an event arrives:

```text
SpeedChanged notification received

Vehicle Speed : 60 km/h
```

When overspeed occurs:

```text
OVERSPEED WARNING

Vehicle Speed : 90 km/h
```

When vehicle speed becomes zero:

```text
Vehicle Speed : 0 km/h

Vehicle State : STOPPED
```

---

# Task 13: Define ClimateService

## Objective

Create the SOME/IP interface definition for the Climate ECU.

Create:

```text
src/climate_interface.py
```

---

## Required Service Definition

Define:

```text
ClimateService
```

with:

```text
Service ID:
0x2234

Instance ID:
0x0001

Major Version:
1
```

---

## Required Methods

Define:

```text
GetCabinTemperature()

Method ID:
0x0001
```

Define:

```text
SetTargetTemperature()

Method ID:
0x0002
```

---

## Required Event

Define:

```text
CabinTemperatureChanged

Event ID:
0x8001

Event Group ID:
0x0001
```

---

## Required Payload Definitions

Create suitable serialisable payloads for:

```text
Cabin Temperature

Target Temperature

SetTargetTemperature Response Status
```

The payload definitions shall use the data types specified earlier.

---

# Task 14: Develop the Climate ECU

## Objective

Create a second SOME/IP Service Provider.

Create:

```text
src/climate_service.py
```

This application represents:

```text
Climate ECU
```

---

## Required Climate ECU State

The Climate ECU shall maintain:

```text
Current Cabin Temperature

Target Temperature
```

For example:

```text
Current Cabin Temperature = 24.0 degree Celsius

Target Temperature = 22.0 degree Celsius
```

---

## Required Functionality

The Climate ECU shall:

1. connect to the `someipy` daemon;
2. create `ClimateService`;
3. offer `ClimateService`;
4. implement `GetCabinTemperature`;
5. implement `SetTargetTemperature`;
6. validate target temperature requests;
7. maintain current cabin temperature;
8. maintain target temperature;
9. simulate changing cabin temperature;
10. generate `CabinTemperatureChanged`.

---

## GetCabinTemperature Behaviour

When a consumer calls:

```text
GetCabinTemperature()
```

the Climate ECU shall return:

```text
Current Cabin Temperature
```

Example:

```text
GetCabinTemperature request received

Current Cabin Temperature = 24.5 degree Celsius

GetCabinTemperature response sent
```

---

## SetTargetTemperature Behaviour

When a consumer requests:

```text
SetTargetTemperature(23.0 degree Celsius)
```

the Climate ECU shall validate the value.

If valid:

```text
Target Temperature = 23.0 degree Celsius

Status = ACCEPTED
```

If invalid:

```text
Requested Target Temperature = 35.0 degree Celsius

Status = INVALID_TEMPERATURE
```

The Climate ECU shall not update its target temperature when the request is invalid.

---

## Cabin Temperature Simulation

Simulate changing cabin temperature.

For example:

```text
24.0 degree Celsius
24.5 degree Celsius
25.0 degree Celsius
25.5 degree Celsius
26.0 degree Celsius
```

Generate:

```text
CabinTemperatureChanged
```

as the temperature changes.

---

# Task 15: Develop the Central Display ECU

## Objective

Create an ECU that consumes services from multiple Service Providers.

Create:

```text
src/central_display.py
```

The application represents:

```text
Central Display ECU
```

---

## VehicleSpeedService Requirements

The Central Display shall discover and consume:

```text
VehicleSpeedService
```

It shall:

```text
Call GetSpeed()

Call GetVehicleState()

Subscribe to SpeedChanged

Subscribe to OverspeedWarning
```

---

## ClimateService Requirements

The Central Display shall independently discover and consume:

```text
ClimateService
```

It shall:

```text
Call GetCabinTemperature()

Call SetTargetTemperature()

Subscribe to CabinTemperatureChanged
```

---

## Required Behaviour

The Central Display must track service availability independently.

For example:

```text
VehicleSpeedService : AVAILABLE

ClimateService      : AVAILABLE
```

If only Vehicle ECU is running:

```text
VehicleSpeedService : AVAILABLE

ClimateService      : NOT AVAILABLE
```

If only Climate ECU is running:

```text
VehicleSpeedService : NOT AVAILABLE

ClimateService      : AVAILABLE
```

---

## Example Central Display Output

```text
==================================================
CENTRAL VEHICLE DISPLAY
==================================================

VehicleSpeedService : AVAILABLE
ClimateService      : AVAILABLE

Vehicle Speed       : 50 km/h
Vehicle State       : MOVING

Cabin Temperature   : 24.5 degree Celsius
Target Temperature  : 22.0 degree Celsius
```

When speed changes:

```text
Vehicle Speed Updated : 60 km/h
```

When overspeed occurs:

```text
OVERSPEED WARNING

Vehicle Speed : 90 km/h
```

When cabin temperature changes:

```text
Cabin Temperature Updated : 25.0 degree Celsius
```

---

# Task 16: Run the Multi-ECU System

## Objective

Run the four virtual ECUs simultaneously.

Use separate terminals so that the behaviour of each ECU can be observed independently.

---

## Terminal 1: Vehicle ECU

Run:

```text
vehicle_speed_service.py
```

This terminal represents:

```text
Vehicle ECU
```

---

## Terminal 2: Instrument Cluster ECU

Run:

```text
cluster_client.py
```

This terminal represents:

```text
Instrument Cluster ECU
```

---

## Terminal 3: Climate ECU

Run:

```text
climate_service.py
```

This terminal represents:

```text
Climate ECU
```

---

## Terminal 4: Central Display ECU

Run:

```text
central_display.py
```

This terminal represents:

```text
Central Display ECU
```

---

## someipy Daemon

The required `someipy` daemon shall also be running according to the setup established in Assignment 1.

Use an additional terminal for the daemon if required.

The important requirement is that the four application terminals visibly represent four different logical ECUs.

---

# Task 17: Demonstrate Multiple Service Discovery

## Objective

Observe how different SOME/IP services become available independently.

---

## Step 17.1: Start the Consumers First

Start:

```text
Instrument Cluster ECU
```

and:

```text
Central Display ECU
```

Do not start Vehicle ECU or Climate ECU yet.

Observe the consumers.

---

## Step 17.2: Start Vehicle ECU

Start:

```text
Vehicle ECU
```

Observe:

```text
VehicleSpeedService becomes available
```

The Instrument Cluster can now consume `VehicleSpeedService`.

The Central Display can also consume `VehicleSpeedService`.

At this point:

```text
VehicleSpeedService : AVAILABLE

ClimateService      : NOT AVAILABLE
```

---

## Step 17.3: Start Climate ECU

Start:

```text
Climate ECU
```

Observe:

```text
ClimateService becomes available
```

The Central Display should now indicate:

```text
VehicleSpeedService : AVAILABLE

ClimateService      : AVAILABLE
```

---

## Required Understanding

Explain:

> A consumer can independently discover different services, and one service becoming available does not imply that another service is also available.

---

# Task 18: Demonstrate Multiple Consumers

## Objective

Demonstrate that multiple consumers can use the same SOME/IP service.

Both:

```text
Instrument Cluster ECU
```

and:

```text
Central Display ECU
```

shall consume:

```text
VehicleSpeedService
```

---

## Required Method Demonstration

Both consumers shall be capable of calling:

```text
GetSpeed()
```

The Vehicle ECU shall receive requests from both consumers.

---

## Required Event Demonstration

Both consumers shall subscribe to:

```text
SpeedChanged
```

When the Vehicle ECU generates:

```text
SpeedChanged
```

both consumers shall receive the notification.

Conceptually:

```text
                    Vehicle ECU
                         |
                         |
                  SpeedChanged
                    /         \
                   /           \
                  v             v
       Instrument Cluster   Central Display
```

---

## OverspeedWarning

Both consumers shall also subscribe to:

```text
OverspeedWarning
```

When:

```text
speed > 80 km/h
```

both consumers shall receive the warning.

---

# Task 19: Demonstrate One Consumer Using Multiple Services

## Objective

Demonstrate that one application can consume services provided by multiple ECUs.

The Central Display shall consume:

```text
VehicleSpeedService
```

from:

```text
Vehicle ECU
```

and:

```text
ClimateService
```

from:

```text
Climate ECU
```

Conceptually:

```text
Vehicle ECU                         Climate ECU
     |                                  |
     |                                  |
VehicleSpeedService                ClimateService
     |                                  |
     |                                  |
     +---------------+------------------+
                     |
                     v
              Central Display
```

---

## Information Required on Central Display

From `VehicleSpeedService`:

```text
Vehicle Speed

Vehicle State

SpeedChanged

OverspeedWarning
```

From `ClimateService`:

```text
Cabin Temperature

Target Temperature

CabinTemperatureChanged
```

The Central Display shall obtain this information through the corresponding SOME/IP service instead of independently duplicating the data.

---

# Task 20: Demonstrate Climate Method Validation

## Objective

Understand that SOME/IP provides communication, while the service application implements application-specific validation.

Use:

```text
SetTargetTemperature()
```

---

## Test 20.1: Normal Valid Value

Request:

```text
22.0 degree Celsius
```

Encoded value:

```text
220
```

Expected response:

```text
ACCEPTED
```

---

## Test 20.2: Lower Boundary

Request:

```text
16.0 degree Celsius
```

Encoded value:

```text
160
```

Expected response:

```text
ACCEPTED
```

---

## Test 20.3: Upper Boundary

Request:

```text
30.0 degree Celsius
```

Encoded value:

```text
300
```

Expected response:

```text
ACCEPTED
```

---

## Test 20.4: Invalid Low Value

Request:

```text
10.0 degree Celsius
```

Encoded value:

```text
100
```

Expected response:

```text
INVALID_TEMPERATURE
```

The existing target temperature shall remain unchanged.

---

## Test 20.5: Invalid High Value

Request:

```text
35.0 degree Celsius
```

Encoded value:

```text
350
```

Expected response:

```text
INVALID_TEMPERATURE
```

The existing target temperature shall remain unchanged.

---

## Required Understanding

Explain:

```text
SOME/IP transports the method request and response.

ClimateService application logic decides whether the requested temperature is valid.
```

---

# Task 21: Perform Service Failure and Recovery Experiments

## Objective

Observe how independently provided services behave when one provider stops.

---

## Experiment 21.1: Stop Climate ECU

Initially keep all applications running.

Stop:

```text
Climate ECU
```

Keep running:

```text
Vehicle ECU

Instrument Cluster ECU

Central Display ECU
```

---

## Observe VehicleSpeedService

Verify whether:

```text
GetSpeed

GetVehicleState

SpeedChanged

OverspeedWarning
```

continue functioning.

Observe what happens to:

```text
ClimateService
```

in the Central Display and `someipy` daemon.

Record the behaviour actually observed.

---

## Restart Climate ECU

Restart:

```text
Climate ECU
```

Observe the system.

Verify:

```text
GetCabinTemperature()

SetTargetTemperature()

CabinTemperatureChanged
```

after the Climate Service becomes usable again.

---

## Experiment 21.2: Stop Vehicle ECU

Keep running:

```text
Climate ECU

Instrument Cluster ECU

Central Display ECU
```

Stop:

```text
Vehicle ECU
```

Observe what happens to:

```text
VehicleSpeedService
```

Verify whether:

```text
ClimateService
```

continues to operate.

---

## Restart Vehicle ECU

Restart the Vehicle ECU.

Observe the consumers and daemon.

Verify the vehicle methods and events again.

---

## Required Understanding

Explain:

> VehicleSpeedService and ClimateService are provided independently. A problem with one Service Provider should not be interpreted as proof that an unrelated service provided by another ECU has also failed.

Only describe service disappearance and rediscovery behaviour that you can actually demonstrate using application output, daemon output or network capture.

---

# Task 22: Observe the Multi-ECU System Using Wireshark

## Objective

Observe SOME/IP communication involving multiple services, methods, events and consumers.

---

## Step 22.1: Start a Clean Capture

Start Wireshark before launching the complete system.

Use the appropriate network interface.

For a localhost-based setup, inspect the loopback interface.

---

## Step 22.2: Start the Applications

Start:

```text
someipy daemon

Instrument Cluster ECU

Central Display ECU

Vehicle ECU

Climate ECU
```

This order makes it easier to observe consumers waiting before their required services become available.

---

## Step 22.3: Observe Service Discovery

Identify Service Discovery traffic associated with:

```text
VehicleSpeedService
```

and:

```text
ClimateService
```

Verify the corresponding Service IDs:

```text
VehicleSpeedService

Service ID = 0x1234
```

```text
ClimateService

Service ID = 0x2234
```

---

## Step 22.4: Observe VehicleSpeedService Methods

Locate communication associated with:

```text
GetSpeed()
```

Identify:

```text
REQUEST

RESPONSE
```

Locate communication associated with:

```text
GetVehicleState()
```

Identify:

```text
REQUEST

RESPONSE
```

---

## Step 22.5: Observe VehicleSpeedService Events

Locate:

```text
SpeedChanged
```

notifications.

Verify:

```text
Event ID = 0x8001
```

Locate:

```text
OverspeedWarning
```

notifications.

Verify:

```text
Event ID = 0x8002
```

---

## Step 22.6: Observe ClimateService Methods

Locate:

```text
GetCabinTemperature()
```

Identify:

```text
REQUEST

RESPONSE
```

Locate:

```text
SetTargetTemperature()
```

Identify:

```text
REQUEST

RESPONSE
```

Observe both:

```text
ACCEPTED
```

and:

```text
INVALID_TEMPERATURE
```

cases at application level and correlate them with relevant method exchanges.

---

## Step 22.7: Observe ClimateService Event

Locate:

```text
CabinTemperatureChanged
```

notifications.

Verify:

```text
ClimateService ID = 0x2234

Event ID = 0x8001
```

Remember that:

```text
VehicleSpeedService
```

also has an event with:

```text
Event ID = 0x8001
```

Therefore, Event ID alone is not sufficient to determine which service generated the event.

The Service ID must also be considered.

---

## Step 22.8: Analyse Representative Packets

For selected packets, identify as many of the following fields as Wireshark displays:

```text
Source IP

Destination IP

Source Port

Destination Port

Service ID

Method ID

Event ID

Client ID

Session ID

Protocol Version

Interface Version

Message Type

Return Code

Payload
```

---

## Step 22.9: Save the Capture

Save the complete capture as:

```text
captures/assignment2-multi-ecu.pcapng
```

---

# Task 23: Explain the Service Design

## Objective

Demonstrate that methods and events were selected intentionally based on the communication requirement.

Students must be able to explain every service element.

---

## GetSpeed

```text
GetSpeed()
```

is a method.

Explain:

- who requests the information;
- who responds;
- when the information is required;
- why request-response is suitable.

---

## GetVehicleState

```text
GetVehicleState()
```

is a method.

Explain:

- why the consumer requests the state;
- why a response is expected;
- what information is returned.

---

## SpeedChanged

```text
SpeedChanged
```

is an event.

Explain:

- who detects the speed change;
- who publishes the notification;
- why subscribed consumers should receive updates.

---

## OverspeedWarning

```text
OverspeedWarning
```

is an event.

Explain:

- who detects the overspeed condition;
- why the Vehicle ECU should generate the warning;
- why consumers should not need to continuously poll for the warning.

---

## GetCabinTemperature

```text
GetCabinTemperature()
```

is a method.

Explain why a consumer may request the current temperature when required.

---

## SetTargetTemperature

```text
SetTargetTemperature()
```

is a method.

Explain:

- why the consumer initiates the operation;
- why a request payload is necessary;
- why a response status is required.

---

## CabinTemperatureChanged

```text
CabinTemperatureChanged
```

is an event.

Explain:

- which ECU detects the temperature change;
- which ECU publishes the event;
- why subscribed consumers receive the updated value automatically.

---

# Assignment 2 Viva Questions

1. How many logical ECUs are present in the system?
2. Which ECUs are Service Providers?
3. Which ECUs are Service Consumers?
4. Can the same ECU be both a Service Provider and Service Consumer?
5. What is the purpose of `VehicleSpeedService`?
6. What is its Service ID?
7. What is its Instance ID?
8. What methods does it provide?
9. What events does it provide?
10. What is the purpose of `GetVehicleState()`?
11. Why is `GetVehicleState()` a method?
12. How is vehicle state calculated?
13. What is the Method ID of `GetVehicleState()`?
14. What is the purpose of `OverspeedWarning`?
15. What condition generates `OverspeedWarning`?
16. Why is no warning generated at exactly 80 km/h?
17. What is the Event ID of `OverspeedWarning`?
18. What is its Event Group ID?
19. What is the purpose of `ClimateService`?
20. What is the Service ID of `ClimateService`?
21. What does `GetCabinTemperature()` return?
22. Why is cabin temperature represented in tenths of a degree Celsius?
23. What does `SetTargetTemperature()` do?
24. What is the valid target temperature range?
25. What happens when an invalid temperature is requested?
26. Is temperature-range validation performed by SOME/IP or by the Climate application?
27. What is `CabinTemperatureChanged`?
28. Why is it implemented as an event?
29. Which ECU consumes both `VehicleSpeedService` and `ClimateService`?
30. Which service is consumed by both Instrument Cluster and Central Display?
31. Can two different services use the same Method ID?
32. Can two different services use the same Event ID?
33. Why must Service ID be considered along with Method ID or Event ID?
34. What happens when the Climate ECU stops?
35. Should `VehicleSpeedService` stop when the Climate ECU stops?
36. What happens when the Vehicle ECU stops?
37. Should `ClimateService` stop when the Vehicle ECU stops?
38. What role does Service Discovery play in the multi-ECU system?
39. How do you distinguish the two services in Wireshark?
40. What is the difference between a method and an event?
41. What is an Event Group?
42. Why does the consumer subscribe to an Event Group?
43. What information is contained in a SOME/IP payload?
44. What is the role of the `someipy` daemon?
45. What does the Central Display demonstrate about service-oriented architecture?

---

# Expected Final Understanding

At the end of Assignment 2, you should understand the system as a collection of independently provided and consumed services.

```text
Vehicle ECU
    |
    +-- VehicleSpeedService
            |
            +-- GetSpeed()
            |
            +-- GetVehicleState()
            |
            +-- SpeedChanged
            |
            +-- OverspeedWarning


Climate ECU
    |
    +-- ClimateService
            |
            +-- GetCabinTemperature()
            |
            +-- SetTargetTemperature()
            |
            +-- CabinTemperatureChanged
```

Two different consumer relationships exist.

First:

```text
VehicleSpeedService
        |
        +----------------------+
        |                      |
        v                      v
Instrument Cluster        Central Display
```

This demonstrates:

> Multiple consumers can use the same service.

Second:

```text
VehicleSpeedService --------+
                            |
                            v
                      Central Display
                            ^
                            |
ClimateService -------------+
```

This demonstrates:

> One consumer can use multiple services.

The overall communication principles are:

```text
Provider
    |
    +-- Defines a service
    |
    +-- Offers the service
    |
    +-- Implements methods
    |
    +-- Generates events


Consumer
    |
    +-- Discovers the service
    |
    +-- Invokes methods
    |
    +-- Receives responses
    |
    +-- Subscribes to event groups
    |
    +-- Receives event notifications
```

The goal of Assignment 2 is therefore:

> **Design, implement, run, observe and explain a multi-ECU service-oriented vehicle system in which multiple virtual ECUs communicate using SOME/IP methods, events and Service Discovery.**
