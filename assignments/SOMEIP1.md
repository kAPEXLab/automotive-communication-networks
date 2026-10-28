# Assignment 1: Introduction to SOME/IP Using Python and `someipy`

## Mini-Project: Virtual Vehicle Speed Service Using SOME/IP

---

## 1. Purpose

This assignment provides the first hands-on exposure to **SOME/IP (Scalable service-Oriented MiddlewarE over IP)**.

The objective is to understand how **service-oriented communication** works between automotive software applications.

In this assignment, you will use Python and the `someipy` library to create a simple virtual automotive system consisting of:

- a **Vehicle ECU** acting as a Service Provider;
- an **Instrument Cluster ECU** acting as a Service Consumer;
- a SOME/IP **method** for request-response communication;
- a SOME/IP **event** for publish-subscribe communication;
- SOME/IP **Service Discovery**;
- Wireshark for observing SOME/IP communication.

You are **not expected to implement the SOME/IP protocol manually**.

The `someipy` library provides the required SOME/IP and SOME/IP Service Discovery functionality.

---

# 3. Mini-Project Scenario

Consider a simple vehicle containing two virtual ECUs.

## Vehicle ECU

The Vehicle ECU maintains a simulated vehicle-speed value.

It provides a SOME/IP service called:

```text
VehicleSpeedService
```

The service provides:

```text
Method : GetSpeed
Event  : SpeedChanged
```

## Instrument Cluster ECU

The Instrument Cluster consumes `VehicleSpeedService`.

It performs two operations:

1. Calls `GetSpeed()` when it wants to know the current vehicle speed.
2. Subscribes to `SpeedChanged` to automatically receive changing speed values.

The communication is conceptually:

```text
Vehicle ECU                              Instrument Cluster ECU
Service Provider                         Service Consumer

VehicleSpeedService
       |
       | <------------- GetSpeed() ----------------|
       |                                           |
       | --------------- Speed ------------------->|
       |                                           |
       | ------------- SpeedChanged -------------->|
```

---

# 4. Execution Environment

All components in Assignment 1 run on the **same Ubuntu Linux PC**.

Different Linux terminals are used to represent different components.

```text
Terminal 1
someipy daemon

Terminal 2
Vehicle ECU
VehicleSpeedService Provider

Terminal 3
Instrument Cluster ECU
VehicleSpeedService Consumer

Wireshark
Network observation
```

The `someipy` daemon manages the SOME/IP network communication and Service Discovery.

The Python applications communicate with the daemon, and the daemon handles the required SOME/IP network activities.

Conceptually:

```text
Python Application
        |
        v
     someipy
        |
        v
  someipy daemon
        |
        v
SOME/IP + SOME/IP-SD
        |
        v
     UDP / IP
```

> **Note:** Docker, virtual machines, Linux network namespaces, multiple physical computers and physical ECUs are not required in Assignment 1.

---

# 5. Software Requirements

The following software is required:

- Ubuntu Linux
- Python 3.8 or later
- Python `venv`
- `pip`
- `someipy`
- Wireshark
- Git

---

# 6. SOME/IP Interface Specification

All students shall use the following identifiers for Assignment 1.

## 6.1 Service

| Parameter | Value |
|---|---|
| Service Name | `VehicleSpeedService` |
| Service ID | `0x1234` |
| Instance ID | `0x5678` |
| Major Version | `1` |
| Transport | UDP |
| Provider IP | `127.0.0.1` |
| Provider Port | `3000` |
| Consumer IP | `127.0.0.1` |
| Consumer Port | `3002` |

---

## 6.2 Method

| Parameter | Value |
|---|---|
| Method Name | `GetSpeed` |
| Method ID | `0x0001` |
| Request Payload | Empty |
| Response Payload | Vehicle speed as `uint16` |
| Unit | km/h |

---

## 6.3 Event

| Parameter | Value |
|---|---|
| Event Name | `SpeedChanged` |
| Event ID | `0x8001` |
| Event Group ID | `0x0001` |
| Event Payload | Vehicle speed as `uint16` |
| Unit | km/h |
| Notification Period | 2 seconds |

---

## 6.4 Service Discovery

| Parameter | Value |
|---|---|
| SD Multicast Address | `224.224.224.245` |
| SD Port | `30490` |

---

# Task 0: Install and Verify the Development Environment

## Objective

Prepare the Ubuntu Linux system for SOME/IP application development.

No SOME/IP programming is required in this task.

---

## Step 0.1: Install Required Packages

Open a Linux terminal.

Run:

```bash
sudo apt update
sudo apt install -y python3 python3-pip python3-venv git wireshark
```

---

## Step 0.2: Verify Python

Run:

```bash
python3 --version
```

Then:

```bash
pip3 --version
```

Python 3.8 or later is required.

---

## Step 0.3: Create the Project Directory

Run:

```bash
mkdir -p ~/someip-python-assignment
cd ~/someip-python-assignment
```

Create the required directories:

```bash
mkdir -p src config captures images
```

The project should initially look like:

```text
someip-python-assignment/
├── src/
├── config/
├── captures/
└── images/
```

---

## Step 0.4: Create a Python Virtual Environment

Run:

```bash
python3 -m venv .venv
```

Activate it:

```bash
source .venv/bin/activate
```

Verify:

```bash
which python
```

The returned path should point to Python inside:

```text
someip-python-assignment/.venv/
```

---

## Step 0.5: Install `someipy`

Run:

```bash
python -m pip install --upgrade pip
python -m pip install someipy
```

Verify the installation:

```bash
python -m pip show someipy
```

Check that the `someipy` daemon is available:

```bash
which someipyd
```

Then:

```bash
someipyd --help
```

---

## Step 0.6: Verify Wireshark

Run:

```bash
wireshark --version
```

---

## Step 0.7: Verify Git

Run:

```bash
git --version
```

---

## Task 0 Completion Criteria

The following must be working before proceeding:

```text
Python             : WORKING
Python venv        : WORKING
someipy            : INSTALLED
someipyd           : AVAILABLE
Wireshark          : INSTALLED
Git                : INSTALLED
Project directory  : CREATED
```

---

# Task 1: Create the SOME/IP Service Interface

## Objective

Create the common definition of `VehicleSpeedService`.

The same interface definition will be used by both:

```text
Vehicle ECU
and
Instrument Cluster ECU
```

---

## Step 1.1: Create the Interface File

Create:

```text
src/vehicle_speed_interface.py
```

Add the following code:

```python
"""
Common definition of the Vehicle Speed SOME/IP service.
"""

from dataclasses import dataclass

from someipy import (
    Event,
    EventGroup,
    Method,
    ServiceBuilder,
    TransportLayerProtocol,
)

from someipy.serialization import SomeIpPayload, Uint16


# ---------------------------------------------------------------------------
# Service identification
# ---------------------------------------------------------------------------

SERVICE_ID = 0x1234
INSTANCE_ID = 0x5678
MAJOR_VERSION = 1


# ---------------------------------------------------------------------------
# Method identification
# ---------------------------------------------------------------------------

GET_SPEED_METHOD_ID = 0x0001


# ---------------------------------------------------------------------------
# Event identification
# ---------------------------------------------------------------------------

SPEED_CHANGED_EVENT_ID = 0x8001
SPEED_EVENTGROUP_ID = 0x0001


# ---------------------------------------------------------------------------
# Network configuration
# ---------------------------------------------------------------------------

INTERFACE_IP = "127.0.0.1"
PROVIDER_PORT = 3000
CONSUMER_PORT = 3002


# ---------------------------------------------------------------------------
# Payload
# ---------------------------------------------------------------------------

@dataclass
class SpeedPayload(SomeIpPayload):

    speed_kmph: Uint16

    def __init__(self, speed_kmph: int = 0):
        self.speed_kmph = Uint16(speed_kmph)


# ---------------------------------------------------------------------------
# GetSpeed Method
# ---------------------------------------------------------------------------

get_speed_method = Method(
    id=GET_SPEED_METHOD_ID,
    protocol=TransportLayerProtocol.UDP,
)


# ---------------------------------------------------------------------------
# SpeedChanged Event
# ---------------------------------------------------------------------------

speed_changed_event = Event(
    id=SPEED_CHANGED_EVENT_ID,
    protocol=TransportLayerProtocol.UDP,
)


speed_eventgroup = EventGroup(
    id=SPEED_EVENTGROUP_ID,
    events=[speed_changed_event],
)


# ---------------------------------------------------------------------------
# Vehicle Speed Service
# ---------------------------------------------------------------------------

vehicle_speed_service = (
    ServiceBuilder()
    .with_service_id(SERVICE_ID)
    .with_major_version(MAJOR_VERSION)
    .with_method(get_speed_method)
    .with_eventgroup(speed_eventgroup)
    .build()
)
```

---

## Step 1.2: Understand the Important Identifiers

### Service ID

```text
SERVICE_ID = 0x1234
```

Identifies:

```text
VehicleSpeedService
```

---

### Instance ID

```text
INSTANCE_ID = 0x5678
```

Identifies the particular instance of `VehicleSpeedService`.

---

### Method ID

```text
GET_SPEED_METHOD_ID = 0x0001
```

Identifies:

```text
GetSpeed()
```

---

### Event ID

```text
SPEED_CHANGED_EVENT_ID = 0x8001
```

Identifies:

```text
SpeedChanged
```

---

### Event Group ID

```text
SPEED_EVENTGROUP_ID = 0x0001
```

The consumer subscribes to this event group to receive the `SpeedChanged` event.

---

### Payload

```text
SpeedPayload
```

represents vehicle speed as:

```text
uint16
```

The value represents:

```text
km/h
```

---

## Step 1.3: Verify the Python File

Run:

```bash
cd ~/someip-python-assignment
source .venv/bin/activate
python -m py_compile src/vehicle_speed_interface.py
```

If there is no error message, the file has passed the Python syntax check.

---

# Task 2: Develop the Vehicle Speed Service Provider

## Objective

Develop the virtual **Vehicle ECU**.

The Vehicle ECU shall:

1. provide `VehicleSpeedService`;
2. implement the `GetSpeed` method;
3. maintain a simulated vehicle-speed value;
4. generate `SpeedChanged` notifications.

---

## Step 2.1: Create the Provider

Create:

```text
src/vehicle_speed_service.py
```

Add:

```python
"""
Virtual Vehicle ECU.

Provides:
1. GetSpeed method
2. SpeedChanged event
"""

import asyncio
import logging
from typing import Tuple

from someipy import (
    MessageType,
    Method,
    MethodResult,
    ReturnCode,
    ServerServiceInstance,
    ServiceBuilder,
    TransportLayerProtocol,
    connect_to_someipy_daemon,
)

from someipy.someipy_logging import set_someipy_log_level

from vehicle_speed_interface import (
    GET_SPEED_METHOD_ID,
    INSTANCE_ID,
    INTERFACE_IP,
    MAJOR_VERSION,
    PROVIDER_PORT,
    SERVICE_ID,
    SPEED_CHANGED_EVENT_ID,
    SPEED_EVENTGROUP_ID,
    SpeedPayload,
    speed_eventgroup,
)


current_speed_kmph = 0


async def get_speed_handler(
    input_data: bytes,
    client_address: Tuple[str, int],
) -> MethodResult:

    print()
    print("GetSpeed request received")
    print(f"Consumer address : {client_address[0]}:{client_address[1]}")
    print(f"Current speed    : {current_speed_kmph} km/h")

    response_payload = SpeedPayload(current_speed_kmph).serialize()

    result = MethodResult()
    result.message_type = MessageType.RESPONSE
    result.return_code = ReturnCode.E_OK
    result.payload = response_payload

    print(f"Response bytes   : {response_payload.hex(' ')}")
    print("GetSpeed response sent")

    return result


def build_provider_service():

    get_speed_method = Method(
        id=GET_SPEED_METHOD_ID,
        protocol=TransportLayerProtocol.UDP,
        method_handler=get_speed_handler,
    )

    return (
        ServiceBuilder()
        .with_service_id(SERVICE_ID)
        .with_major_version(MAJOR_VERSION)
        .with_method(get_speed_method)
        .with_eventgroup(speed_eventgroup)
        .build()
    )


async def simulate_speed(
    service_instance: ServerServiceInstance,
) -> None:

    global current_speed_kmph

    while True:

        current_speed_kmph += 10

        if current_speed_kmph > 100:
            current_speed_kmph = 0

        event_payload = SpeedPayload(
            current_speed_kmph
        ).serialize()

        print()
        print(
            f"Simulated speed changed to "
            f"{current_speed_kmph} km/h"
        )

        service_instance.send_event(
            SPEED_EVENTGROUP_ID,
            SPEED_CHANGED_EVENT_ID,
            event_payload,
        )

        print("SpeedChanged event sent")

        await asyncio.sleep(2)


async def main() -> None:

    set_someipy_log_level(logging.INFO)

    print("=" * 60)
    print("Virtual Vehicle ECU")
    print("VehicleSpeedService Provider")
    print("=" * 60)

    print(f"Service ID       : 0x{SERVICE_ID:04X}")
    print(f"Instance ID      : 0x{INSTANCE_ID:04X}")
    print(f"Method ID        : 0x{GET_SPEED_METHOD_ID:04X}")
    print(f"Event ID         : 0x{SPEED_CHANGED_EVENT_ID:04X}")
    print(f"Event-group ID   : 0x{SPEED_EVENTGROUP_ID:04X}")
    print(f"Provider endpoint: {INTERFACE_IP}:{PROVIDER_PORT}")

    print("=" * 60)

    print("Connecting to someipy daemon...")

    daemon = await connect_to_someipy_daemon()

    print("Connected to someipy daemon")

    provider_service = build_provider_service()

    provider_instance = ServerServiceInstance(
        daemon=daemon,
        service=provider_service,
        instance_id=INSTANCE_ID,
        endpoint_ip=INTERFACE_IP,
        endpoint_port=PROVIDER_PORT,
        ttl=5,
        cyclic_offer_delay_ms=2000,
    )

    print("Offering VehicleSpeedService...")

    await provider_instance.start_offer()

    print("VehicleSpeedService is being offered")

    speed_task = asyncio.create_task(
        simulate_speed(provider_instance)
    )

    try:

        await asyncio.Future()

    finally:

        speed_task.cancel()

        try:
            await speed_task
        except asyncio.CancelledError:
            pass

        await provider_instance.stop_offer()

        await daemon.disconnect_from_daemon()


if __name__ == "__main__":

    try:
        asyncio.run(main())

    except KeyboardInterrupt:
        print("\nVehicle Speed Service stopped")
```

---

## Step 2.2: Check the Provider

Run:

```bash
python -m py_compile src/vehicle_speed_service.py
```

No output means that the Python syntax check passed.

Do not start the provider yet.

---

# Task 3: Develop the Instrument Cluster Consumer

## Objective

Develop the virtual **Instrument Cluster ECU**.

The Instrument Cluster shall:

1. search for `VehicleSpeedService`;
2. wait until the service becomes available;
3. invoke `GetSpeed`;
4. receive the speed response;
5. subscribe to `SpeedChanged`;
6. receive event notifications.

---

## Step 3.1: Create the Consumer

Create:

```text
src/cluster_client.py
```

Add:

```python
"""
Virtual Instrument Cluster ECU.

Consumes VehicleSpeedService.
"""

import asyncio
import logging

from someipy import (
    ClientServiceInstance,
    MessageType,
    ReturnCode,
    connect_to_someipy_daemon,
)

from someipy.someipy_logging import set_someipy_log_level

from vehicle_speed_interface import (
    CONSUMER_PORT,
    GET_SPEED_METHOD_ID,
    INSTANCE_ID,
    INTERFACE_IP,
    SERVICE_ID,
    SpeedPayload,
    speed_eventgroup,
    vehicle_speed_service,
)


def speed_event_callback(
    event_id: int,
    event_payload: bytes,
) -> None:

    try:

        speed_message = SpeedPayload().deserialize(
            event_payload
        )

        print()
        print("SpeedChanged notification received")
        print(f"Event ID      : 0x{event_id:04X}")
        print(f"Payload bytes : {event_payload.hex(' ')}")
        print(
            f"Vehicle speed : "
            f"{speed_message.speed_kmph.value} km/h"
        )

    except Exception as error:

        print(
            f"Could not decode event payload: {error}"
        )


async def call_get_speed(
    client_instance: ClientServiceInstance,
) -> None:

    while True:

        try:

            print()
            print("Calling GetSpeed...")

            result = await client_instance.call_method(
                GET_SPEED_METHOD_ID,
                b"",
            )

            if result.message_type == MessageType.RESPONSE:

                print("GetSpeed response received")

                print(
                    f"Return code   : {result.return_code}"
                )

                print(
                    f"Payload bytes : "
                    f"{result.payload.hex(' ')}"
                )

                if result.return_code == ReturnCode.E_OK:

                    speed_message = (
                        SpeedPayload().deserialize(
                            result.payload
                        )
                    )

                    print(
                        f"Vehicle speed : "
                        f"{speed_message.speed_kmph.value} km/h"
                    )

            elif result.message_type == MessageType.ERROR:

                print(
                    "Provider returned SOME/IP error"
                )

        except Exception as error:

            print(
                f"GetSpeed call failed: {error}"
            )

        await asyncio.sleep(5)


async def main() -> None:

    set_someipy_log_level(logging.INFO)

    print("=" * 60)
    print("Virtual Instrument Cluster ECU")
    print("VehicleSpeedService Consumer")
    print("=" * 60)

    print(
        f"Required Service ID  : "
        f"0x{SERVICE_ID:04X}"
    )

    print(
        f"Required Instance ID : "
        f"0x{INSTANCE_ID:04X}"
    )

    print(
        f"Consumer endpoint    : "
        f"{INTERFACE_IP}:{CONSUMER_PORT}"
    )

    print("=" * 60)

    print("Connecting to someipy daemon...")

    daemon = await connect_to_someipy_daemon()

    print("Connected to someipy daemon")

    client_instance = ClientServiceInstance(
        daemon=daemon,
        service=vehicle_speed_service,
        instance_id=INSTANCE_ID,
        endpoint_ip=INTERFACE_IP,
        endpoint_port=CONSUMER_PORT,
    )

    client_instance.register_callback(
        speed_event_callback
    )

    print("Searching for VehicleSpeedService...")

    while not await client_instance.is_available():

        print(
            "VehicleSpeedService is not yet available"
        )

        await asyncio.sleep(1)

    print()
    print("VehicleSpeedService is AVAILABLE")

    print("Subscribing to SpeedChanged...")

    client_instance.subscribe_eventgroup(
        speed_eventgroup,
        5.0,
    )

    print("Event-group subscription requested")

    method_task = asyncio.create_task(
        call_get_speed(client_instance)
    )

    try:

        await asyncio.Future()

    finally:

        method_task.cancel()

        try:
            await method_task
        except asyncio.CancelledError:
            pass

        await daemon.disconnect_from_daemon()


if __name__ == "__main__":

    try:
        asyncio.run(main())

    except KeyboardInterrupt:
        print("\nInstrument Cluster stopped")
```

---

## Step 3.2: Check the Consumer

Run:

```bash
python -m py_compile src/cluster_client.py
```

No output means the Python syntax check passed.

---

# Task 4: Configure the `someipy` Daemon

## Objective

Configure the component responsible for SOME/IP network communication and Service Discovery.

---

## Step 4.1: Create the Configuration File

Create:

```text
config/someipyd.json
```

Add:

```json
{
    "sd_address": "224.224.224.245",
    "sd_port": 30490,
    "log_level": "DEBUG",
    "interface": "127.0.0.1",
    "use_tcp": false
}
```

---

## Step 4.2: Understand the Configuration

### `sd_address`

```text
224.224.224.245
```

Multicast address used for Service Discovery.

### `sd_port`

```text
30490
```

UDP port used for Service Discovery.

### `interface`

```text
127.0.0.1
```

Network address used for this local experiment.

### `log_level`

```text
DEBUG
```

Enables detailed daemon diagnostic output.

### `use_tcp`

```text
false
```

For the Linux environment used in this assignment, the Python applications communicate with the daemon using the normal Linux local communication mechanism.

---

# Task 5: Run the Complete SOME/IP Application

## Objective

Run all components and establish SOME/IP communication.

For the first execution, start the applications in this order:

```text
1. someipy daemon

2. Vehicle ECU Provider

3. Instrument Cluster Consumer
```

---

## Terminal 1: Start the Daemon

Open Terminal 1.

Run:

```bash
cd ~/someip-python-assignment
source .venv/bin/activate
someipyd --config config/someipyd.json
```

Keep Terminal 1 running.

---

## Terminal 2: Start the Vehicle ECU

Open Terminal 2.

Run:

```bash
cd ~/someip-python-assignment
source .venv/bin/activate
python src/vehicle_speed_service.py
```

Expected application-level output should include information similar to:

```text
Virtual Vehicle ECU
VehicleSpeedService Provider

Service ID       : 0x1234
Instance ID      : 0x5678
Method ID        : 0x0001
Event ID         : 0x8001
Event-group ID   : 0x0001

Connected to someipy daemon

VehicleSpeedService is being offered

Simulated speed changed to 10 km/h
SpeedChanged event sent
```

The exact daemon log messages may vary according to the installed `someipy` version.

---

## Terminal 3: Start the Instrument Cluster

Open Terminal 3.

Run:

```bash
cd ~/someip-python-assignment
source .venv/bin/activate
python src/cluster_client.py
```

Expected application-level sequence:

```text
Virtual Instrument Cluster ECU

Connected to someipy daemon

Searching for VehicleSpeedService...

VehicleSpeedService is AVAILABLE

Subscribing to SpeedChanged...

Calling GetSpeed...

GetSpeed response received
Vehicle speed : 20 km/h
```

As the simulated speed changes, notifications should also appear.

For example:

```text
SpeedChanged notification received
Vehicle speed : 30 km/h

SpeedChanged notification received
Vehicle speed : 40 km/h
```

The actual speed values depend on when the applications are started.

---

# Task 6: Demonstrate Method Request-Response Communication

## Objective

Understand how the `GetSpeed()` method works.

---

## Step 6.1: Observe the Consumer

In Terminal 3, observe:

```text
Calling GetSpeed...
```

This means that the Instrument Cluster is requesting the current vehicle speed.

---

## Step 6.2: Observe the Provider

Terminal 2 should show application-level output similar to:

```text
GetSpeed request received
Current speed : 40 km/h
GetSpeed response sent
```

---

## Step 6.3: Observe the Response

Return to Terminal 3.

The consumer should show:

```text
GetSpeed response received
Vehicle speed : 40 km/h
```

---

## Method Communication Sequence

```text
Instrument Cluster                    Vehicle ECU

       |                                  |
       |------ GetSpeed REQUEST --------->|
       |                                  |
       |<----- GetSpeed RESPONSE ---------|
       |         speed = 40               |
       |                                  |
```

---

## Understand the Sequence

The communication occurs as follows:

1. The Instrument Cluster requires the current vehicle speed.
2. The Instrument Cluster invokes `GetSpeed()`.
3. SOME/IP carries a request towards the Vehicle ECU.
4. The request identifies the required service and method.
5. The Vehicle ECU executes the `GetSpeed` handler.
6. The provider reads `current_speed_kmph`.
7. The speed value is serialised into the response payload.
8. The Vehicle ECU returns a SOME/IP response.
9. The Instrument Cluster receives the response.
10. The consumer deserialises the payload.
11. The Instrument Cluster displays the current speed.

---

## Important Concept

`GetSpeed` is a **method**.

A method follows:

```text
REQUEST
   ↓
Processing
   ↓
RESPONSE
```

The consumer initiates the communication.

---

# Task 7: Demonstrate Event Publish-Subscribe Communication

## Objective

Understand the `SpeedChanged` event.

---

## Step 7.1: Observe Subscription

After discovering the service, the consumer subscribes to:

```text
Event Group ID = 0x0001
```

The event group contains:

```text
SpeedChanged
Event ID = 0x8001
```

---

## Step 7.2: Observe Notifications

The provider changes the simulated speed periodically:

```text
10 km/h
20 km/h
30 km/h
40 km/h
...
```

After every simulated speed update, the provider sends a `SpeedChanged` event.

The consumer should display output similar to:

```text
SpeedChanged notification received
Vehicle speed : 30 km/h
```

---

## Event Communication Sequence

```text
Instrument Cluster                    Vehicle ECU

       |                                   |
       |---- Subscribe Event Group ------->|
       |                                   |
       |<----- SpeedChanged: 20 -----------|
       |                                   |
       |<----- SpeedChanged: 30 -----------|
       |                                   |
       |<----- SpeedChanged: 40 -----------|
```

---

## Understand Event vs Event Group

Do not confuse an **Event** with an **Event Group**.

### Event

The actual notification being sent is:

```text
SpeedChanged
```

Its identifier is:

```text
Event ID = 0x8001
```

---

### Event Group

The consumer subscribes using:

```text
Event Group ID = 0x0001
```

The Event Group logically contains the `SpeedChanged` event.

---

## Important Concept

After subscription, the provider can send notifications without the consumer having to call `GetSpeed()` for each speed change.

The communication therefore follows:

```text
Subscribe
    ↓
Notification
    ↓
Notification
    ↓
Notification
    ↓
...
```

---

# Task 8: Compare Method and Event Communication

## Objective

Experimentally understand the difference between a SOME/IP method and a SOME/IP event.

---

## Experiment A: Method

Observe:

```text
GetSpeed()
```

The sequence is:

```text
Consumer
    |
    | REQUEST
    v
Provider
    |
    | RESPONSE
    v
Consumer
```

The consumer initiates the communication.

---

## Experiment B: Event

Observe:

```text
SpeedChanged
```

After subscription:

```text
Provider
    |
    | NOTIFICATION
    v
Consumer

Provider
    |
    | NOTIFICATION
    v
Consumer

Provider
    |
    | NOTIFICATION
    v
Consumer
```

The consumer does not invoke `GetSpeed()` for every notification.

---

## Comparison

| Characteristic | `GetSpeed` | `SpeedChanged` |
|---|---|---|
| SOME/IP mechanism | Method | Event |
| Communication model | Request-response | Publish-subscribe |
| Initial communication initiated by | Consumer | Consumer subscribes, then provider sends notifications |
| Consumer behaviour | Calls method | Subscribes |
| Provider behaviour | Responds | Publishes notifications |
| Typical usage | Obtain information when required | Receive changing information |
| Example | Give me the current speed | The speed has changed |

---

## Think About

Consider vehicle speed, which can change continuously.

If the Instrument Cluster repeatedly asks:

```text
What is the speed?
What is the speed?
What is the speed?
What is the speed?
```

the communication model is different from:

```text
Tell me whenever the speed changes.
```

The first represents the idea of a method.

The second represents the idea of an event subscription.

---

# Task 9: Observe SOME/IP Communication Using Wireshark

## Objective

Connect the behaviour observed in the Python applications with the actual network communication.

---

## Step 9.1: Start Wireshark

Run:

```bash
wireshark
```

Select the network interface being used for this experiment.

Because the applications in this assignment use localhost, inspect the Linux loopback interface, normally:

```text
lo
```

---

## Step 9.2: Apply a Display Filter

A useful starting filter is:

```text
udp.port == 30490 || udp.port == 3000 || udp.port == 3002
```

If your Wireshark installation recognises the SOME/IP dissectors directly, you can also try:

```text
someip || someipsd
```

---

## Step 9.3: Perform a Clean Capture

Stop the existing applications before performing the capture.

Use the following order:

```text
1. Start Wireshark capture

2. Start someipy daemon

3. Start Instrument Cluster consumer

4. Wait for a few seconds

5. Start Vehicle ECU provider

6. Observe service availability

7. Observe GetSpeed request-response

8. Observe SpeedChanged notifications

9. Allow several notifications to occur

10. Stop the provider

11. Stop the Wireshark capture
```

---

## Step 9.4: Save the Capture

Save the capture as:

```text
captures/assignment1-someip.pcapng
```

---

## Step 9.5: Observe Service Discovery

Inspect the Service Discovery traffic.

Look for messages corresponding to operations such as:

```text
Offer Service
```

If visible in your capture, also identify operations such as:

```text
Find Service
Subscribe Event Group
Subscribe Event Group ACK
```

Only report messages that you can actually identify in your own capture.

---

## Step 9.6: Observe Method Communication

Locate the SOME/IP message corresponding to:

```text
GetSpeed REQUEST
```

Then locate the corresponding:

```text
GetSpeed RESPONSE
```

---

## Step 9.7: Observe Event Communication

Locate SOME/IP notifications corresponding to:

```text
SpeedChanged
```

There should be several notifications because the speed changes periodically.

---

## Step 9.8: Inspect SOME/IP Information

For selected SOME/IP packets, identify as many of the following fields as Wireshark displays:

```text
Source IP

Destination IP

Source Port

Destination Port

Service ID

Method ID / Event ID

Client ID

Session ID

Protocol Version

Interface Version

Message Type

Return Code

Payload
```

---

## Correlate Application and Network Behaviour

You should now be able to connect:

```text
Python Application
        |
        v
    someipy API
        |
        v
  someipy daemon
        |
        v
    SOME/IP
        |
        v
     UDP/IP
        |
        v
    Wireshark
```

For example:

```text
Python:

Calling GetSpeed...

        ↓

Wireshark:

SOME/IP REQUEST

        ↓

Provider:

GetSpeed request received

        ↓

Provider:

GetSpeed response sent

        ↓

Wireshark:

SOME/IP RESPONSE

        ↓

Consumer:

Vehicle speed : 40 km/h
```

---

# Task 10: Perform and Demonstrate SOME/IP Experiments

## Objective

Verify your understanding by changing the execution conditions and observing how the system behaves.

Do not modify the SOME/IP protocol implementation.

Observe the behaviour of your applications and network communication.

---

## Experiment 10.1: Start Consumer Before Provider

### Procedure

Start:

```text
someipy daemon
```

Then start:

```text
Instrument Cluster Consumer
```

Do **not** start the provider immediately.

Observe the consumer.

It should remain waiting for the required service.

Now start:

```text
Vehicle ECU Provider
```

Observe the consumer again.

---

### Explain

You should be able to explain:

1. Why could the consumer not immediately use `VehicleSpeedService`?
2. What changed when the provider started?
3. What is meant by service availability?
4. What role did SOME/IP Service Discovery play?

---

## Experiment 10.2: Observe Request-Response

Keep all required processes running.

Wait for:

```text
Calling GetSpeed...
```

Observe:

```text
Instrument Cluster
```

Then:

```text
Vehicle ECU
```

Then:

```text
Instrument Cluster
```

Identify the corresponding request and response in Wireshark.

---

### Explain

Answer verbally during the demonstration:

```text
Who initiated the communication?

Which Method ID was used?

What did the request ask for?

What did the provider return?

What was contained in the response payload?
```

---

## Experiment 10.3: Observe Multiple Events

Allow the provider to generate at least five speed changes.

For example:

```text
20 km/h

30 km/h

40 km/h

50 km/h

60 km/h
```

Observe the corresponding notifications in the Instrument Cluster terminal.

Locate the notifications in Wireshark.

---

### Explain

You should be able to explain:

> Why was a new `GetSpeed()` request not required before every `SpeedChanged` notification?

---

## Experiment 10.4: Stop the Consumer

Keep these running:

```text
someipy daemon
Vehicle ECU Provider
```

Stop the consumer using:

```text
Ctrl+C
```

Observe the Vehicle ECU.

---

### Explain

The Vehicle ECU's simulated speed generation belongs to the **Vehicle ECU application**.

Stopping the Instrument Cluster does not mean that the Vehicle ECU stops generating its local simulated speed.

This illustrates an important separation:

```text
Application Data Generation
            |
            | different responsibility
            v
SOME/IP Communication
```

SOME/IP communicates the data.

SOME/IP does not generate the vehicle speed.

---

## Experiment 10.5: Change the Event Period

Open:

```text
src/vehicle_speed_service.py
```

Locate:

```python
await asyncio.sleep(2)
```

Temporarily change it to:

```python
await asyncio.sleep(1)
```

Restart the provider and consumer.

Observe:

- provider terminal;
- consumer terminal;
- frequency of `SpeedChanged` notifications;
- corresponding Wireshark traffic.

---

### Explain

You should be able to explain:

> Why did the notification frequency change?

The simulation and notification timing are controlled by the application logic used in this assignment.

---

## Restore the Original Value

After the experiment, restore:

```python
await asyncio.sleep(2)
```

---

## Experiment 10.6: Stop and Restart the Provider

Keep these running:

```text
someipy daemon
Instrument Cluster Consumer
```

Stop the provider using:

```text
Ctrl+C
```

Observe:

```text
Instrument Cluster terminal
someipy daemon terminal
Wireshark
```

Now restart the provider:

```bash
python src/vehicle_speed_service.py
```

Observe the behaviour again.

---

### Important Observation Rule

Document and explain what you **actually observe**.

Do not claim that a service disappeared, was rediscovered or generated a particular SOME/IP-SD message unless that behaviour can be demonstrated using:

- application output;
- daemon output; or
- Wireshark capture.

---

# Assignment 1 Learning Progression

```text
Task 0
Install and verify environment
        ↓
Task 1
Define the SOME/IP service
        ↓
Task 2
Develop Service Provider
        ↓
Task 3
Develop Service Consumer
        ↓
Task 4
Configure someipy daemon
        ↓
Task 5
Run the complete application
        ↓
Task 6
Understand Method
REQUEST → RESPONSE
        ↓
Task 7
Understand Event
SUBSCRIBE → NOTIFICATION
        ↓
Task 8
Compare Method vs Event
        ↓
Task 9
Observe communication using Wireshark
        ↓
Task 10
Experiment and demonstrate understanding
        ↓
Assignment 1 Complete
```

---

# Expected Final Understanding

After completing Assignment 1, you should be able to explain the complete communication path:

```text
Vehicle Application
        |
        v
VehicleSpeedService
        |
        v
someipy
        |
        v
SOME/IP / SOME/IP-SD
        |
        v
IP Network
        |
        v
someipy
        |
        v
Instrument Cluster Application
```

You should also be able to clearly explain the difference between:

```text
GetSpeed()

Consumer asks:
"What is the current speed?"

REQUEST → RESPONSE
```

and:

```text
SpeedChanged

Consumer subscribes:
"Tell me whenever the speed changes."

SUBSCRIBE → NOTIFICATION → NOTIFICATION → ...
```

The goal of Assignment 1 is therefore not merely to make the Python programs execute successfully.

The assignment is complete when you can:

> **Develop, run, observe and explain a basic SOME/IP service-oriented communication system.**
