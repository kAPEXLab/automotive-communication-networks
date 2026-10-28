# BASIC SOME/IP ASSIGNMENTS: CLIENT-SERVER DEMO

## Overview

Seven observation assignments using the **SOME/IP client-server demo**.

Students do **not** write or modify any code. They will:

- Operate the client menu
- Observe the client and server program windows
- Capture and analyse packets using Wireshark
- Record SOME/IP header values
- Compare requests, responses, errors and notifications
- Observe SOME/IP Service Discovery (SD)
- Decode selected SOME/IP frames manually

---

# Setup

## The Demo

Two programs run on the lab PC:

- **Server ECU:** Offers the `VehicleInfo` service.
- **Client ECU:** Uses the `VehicleInfo` service and is operated using a numbered menu.

Both programs print every message as a labelled header table together with its raw bytes.

## VehicleInfo Service

**Service ID:** `0x1234`  
**Interface Version:** `0x01`

| ID | Name | Type | Payload |
|---|---|---|---|
| `0x0001` | GetSpeed | REQUEST → RESPONSE | Response: `uint16` km/h |
| `0x0002` | SetHeadlight | REQUEST → RESPONSE | Request: `uint8`, `0 = off`, `1 = on` |
| `0x0003` | Horn | REQUEST_NO_RETURN | None |
| `0x8001` | SpeedChanged | NOTIFICATION every 1 s | `uint16` km/h |

## UDP Ports

| Communication | UDP Port |
|---|---:|
| Methods and Events | `30509` |
| Service Discovery | `30490` |

---

# Start Every Assignment Like This

1. The instructor starts the **server** and **client** in two windows on the lab PC.
2. In Wireshark, capture on **"Adapter for loopback traffic capture"** using the filter:

   ```text
   udp.port == 30509 || udp.port == 30490
   ```

3. Right-click a port `30509` packet and select:

   ```text
   Decode As... → UDP port 30509 → SOMEIP
   ```

4. Save each capture using the following naming convention:

   ```text
   A<N>_<rollno>.pcapng
   ```

   Example:

   ```text
   A1_12345.pcapng
   ```

---

# SOME/IP Header Reference

Every SOME/IP message starts with a **16-byte header**, read in **big-endian** format.

| Bytes | Field |
|---|---|
| `0-1` | Service ID |
| `2-3` | Method ID (Event ID if the top bit is set) |
| `4-7` | Length = 8 + payload bytes |
| `8-9` | Client ID |
| `10-11` | Session ID |
| `12` | Protocol Version |
| `13` | Interface Version |
| `14` | Message Type |
| `15` | Return Code |
| `16 onward` | Payload |

## Message Type Codes

| Code | Message Type |
|---|---|
| `0x00` | REQUEST |
| `0x01` | REQUEST_NO_RETURN |
| `0x02` | NOTIFICATION |
| `0x80` | RESPONSE |
| `0x81` | ERROR |

## Return Codes

| Code | Return Code |
|---|---|
| `0x00` | E_OK |
| `0x01` | E_NOT_OK |
| `0x02` | E_UNKNOWN_SERVICE |
| `0x03` | E_UNKNOWN_METHOD |
| `0x08` | E_WRONG_INTERFACE_VERSION |

---

# Assignment 1: The 16-Byte Header

## Aim

Read every field of a SOME/IP request and its response, and verify your reading against Wireshark.

## Steps

1. In the client, choose **1 GetSpeed** once.
2. Copy the raw bytes of the **REQUEST** and **RESPONSE** from the client window.
3. Find the same two packets in Wireshark.
4. Expand:

   ```text
   SOME/IP Protocol
   ```

## Observation Table

| Field | Bytes | REQUEST Value | RESPONSE Value | Changed? Why? |
|---|---|---|---|---|
| Service ID | `0-1` | | | |
| Method ID | `2-3` | | | |
| Length | `4-7` | | | |
| Client ID | `8-9` | | | |
| Session ID | `10-11` | | | |
| Protocol Version | `12` | | | |
| Interface Version | `13` | | | |
| Message Type | `14` | | | |
| Return Code | `15` | | | |
| Payload | `16 onward` | | | |

## Questions

1. The REQUEST has `Length = 8`. Why is it 8 and not 16 or 0?
2. Convert the RESPONSE payload to km/h. Does it match what the client printed?
3. Which four fields together form the **Message ID** and **Request ID**?
4. In Wireshark, what are the UDP source and destination ports of the REQUEST? Which one belongs to the server?

---

# Assignment 2: Matching Requests and Responses

## Aim

Observe how a client determines which response belongs to which request using the **Client ID** and **Session ID**.

---

## Part 2A: One Client

### Steps

1. Choose **1 GetSpeed** three times.
2. Record each request's Session ID.
3. Record the Session ID of the corresponding response.

### Observation Table

| Call | Request Session ID | Response Session ID | Speed (km/h) |
|---:|---|---|---:|
| 1 | | | |
| 2 | | | |
| 3 | | | |

---

## Part 2B: Two Clients

### Steps

1. The instructor opens a second client window so that two clients communicate with the same server.
2. Choose **1 GetSpeed** from each client.
3. In Wireshark, add **Client ID** and **Session ID** as columns:

   ```text
   Right-click field → Apply as Column
   ```

### Observation Table

| Client | Client ID | UDP Source Port | Session IDs Seen |
|---|---|---|---|
| Client 1 | | | |
| Client 2 | | | |

## Questions

1. Does the server ever change the Client ID or Session ID it received? Why not?
2. In Part 2B, both clients use Client ID `0x0010` and start at Session ID `0x0001`. How does each client still get its own reply? Why must every client ECU in a real car have a unique Client ID?
3. After `0xFFFF`, which value does the Session ID wrap to? Why is `0x0000` avoided for requests?
4. If a reply arrives with a Session ID the client never sent, what should the client do with it?

---

# Assignment 3: Payloads and Fire-and-Forget

## Aim

Send data to the server and compare a method that returns a response with a method that does not.

## Steps

Perform the following client menu actions:

1. Choose **2 Headlight ON**
2. Choose **3 Headlight OFF**
3. Choose **4 Horn**
4. Record what is transmitted in each direction.

## Observation Table

| Action | Request Message Type | Request Payload | Request Length | Reply? | Reply Payload | Server Window Shows |
|---|---|---|---|---|---|---|
| Headlight ON | | | | | | |
| Headlight OFF | | | | | | |
| Horn | | | | | | |

## Questions

1. Why is the SetHeadlight request's Length `9` while the GetSpeed request's Length is `8`?
2. Horn uses `REQUEST_NO_RETURN (0x01)`. How does the client know the horn actually sounded? Is that acceptable for a horn?
3. Name one vehicle function that should use `REQUEST_NO_RETURN` and one that must use `REQUEST` with a `RESPONSE`. Give a reason for each.
4. The SetHeadlight response echoes the new state. Why is that more useful than an empty response?

---

# Assignment 4: Errors and Return Codes

## Aim

Make the server reject requests and determine exactly what was wrong by examining the reply.

## Steps

1. Choose menu option **7**.
2. Choose menu option **8**.
3. Choose menu option **9**.
4. Stop the server using:

   ```text
   Ctrl+C
   ```

5. Choose menu option **1**.

## Observation Table

| Menu | What Was Wrong | Reply Message Type | Return Code (Hex) | Return Code Name |
|---|---|---|---|---|
| 7 | Method `0x0099` | | | |
| 8 | Interface version `0x02` | | | |
| 9 | Service `0x5678` | | | |
| 1, server stopped | | | | |

## Questions

1. In menu 8, which header byte did the client change? What does a mismatch there mean between two real ECUs?
2. An error reply copies the Session ID of the request. Why is that important?
3. With the server stopped, no SOME/IP reply arrives. What does the client report, and what does Wireshark show instead? Look for **ICMP**.
4. Predict: if a fire-and-forget request such as Horn used an unknown Method ID, would the server send an ERROR? Use what you observed in Assignment 3 to explain.

---

# Assignment 5: Events and Subscription

## Aim

Receive data that the client did not request each time and control it using **SOME/IP Service Discovery (SD)**.

## Steps

### Step 1: Subscribe

Choose:

```text
5 Subscribe
```

Do nothing for **15 seconds** and watch both program windows.

### Step 2: Subscribe and Unsubscribe

Choose **5 Subscribe** again.

Wait **3 seconds**, then choose:

```text
6 Unsubscribe
```

### Step 3: Analyse SOME/IP-SD

In Wireshark, find the SD packets on port:

```text
30490
```

Expand:

```text
SOME/IP-SD
    ├── Entries Array
    └── Options Array
```

## Observation Table

| Item | Value |
|---|---|
| SD packet sent by the client: entry type | |
| Eventgroup ID | |
| TTL (s) | |
| Endpoint option: IP address and port | |
| SD packet sent by the server: entry type | |
| Event Message Type | |
| Event ID | |
| Event Client ID | |
| Number of events received in Step 1 before they stopped | |
| What stopped them in Step 1 | |
| What stopped them in Step 2, and how fast | |

## Questions

1. The Event ID is `0x8001`. What does the top bit (`0x8000`) tell you?
2. Why does an event carry Client ID `0x0000` while a response carries `0x0010`?
3. Why does the subscribe message carry an endpoint option? What would happen without it?
4. Unsubscribing is a Subscribe with TTL `0`. Why is a TTL needed at all if the client can unsubscribe?

---

# Assignment 6: Decode Frames by Hand

## Aim

Decode SOME/IP messages from raw bytes alone, as you would when analysing a log file.

> **Rule:** Complete this assignment without using the PC to decode the frames.

All five frames come from the `VehicleInfo` demo.

For each frame:

1. Decode the fields.
2. Fill in the observation table.
3. Write one sentence describing what happened.

## Raw Frames

### F1

```text
12 34 00 01 00 00 00 0A 00 10 00 01 01 01 80 00 00 32
```

### F2

```text
12 34 00 02 00 00 00 09 00 10 00 02 01 01 00 00 01
```

### F3

```text
12 34 00 03 00 00 00 08 00 10 00 04 01 01 01 00
```

### F4

```text
12 34 00 99 00 00 00 08 00 10 00 05 01 01 81 03
```

### F5

```text
12 34 80 01 00 00 00 0A 00 00 00 07 01 01 02 00 00 4B
```

## Observation Table

| Frame | Method / Event | Length | Client ID | Session ID | Message Type | Return Code | Payload Value | What Happened |
|---|---|---|---|---|---|---|---|---|
| F1 | | | | | | | | |
| F2 | | | | | | | | |
| F3 | | | | | | | | |
| F4 | | | | | | | | |
| F5 | | | | | | | | |

---

## Challenge: Decode a SOME/IP-SD Frame

The following frame was sent to UDP port `30490`.

Identify:

- Entry type
- TTL
- IP address in the endpoint option
- Port in the endpoint option

### Raw Frame

```text
FF FF 81 00 00 00 00 30
00 00 00 08 01 01 02 00
C0 00 00 00 00 00 00 10
06 00 00 10 12 34 00 01
01 00 00 0A 00 00 00 01
00 00 00 0C 00 09 04 00
7F 00 00 01 00 11 C3 CB
```

### Challenge Observation

| Field | Decoded Value |
|---|---|
| Entry Type | |
| TTL | |
| Endpoint IP Address | |
| Endpoint Port | |

---

# Assignment 7: The Full Message Sequence

## Aim

Follow a complete conversation between two ECUs from start to finish and measure how quickly the server answers.

## Steps

### Step 1: Start Capture

Start a new Wireshark capture.

### Step 2: Run the Automatic Demo

The instructor runs:

```bash
python someip_client.py --demo
```

The demo performs every menu action once and in order.

### Step 3: Stop and Analyse

When the demo finishes:

1. Stop the Wireshark capture.
2. List every SOME/IP packet in order.
3. List every SOME/IP-SD packet in order.

---

## Observation Table 1: Message Sequence

| No. | Time (s) | From → To (Port) | Message Type or SD Entry | Method / Event | Session ID | What It Means |
|---:|---:|---|---|---|---|---|
| 1 | | | | | | |
| 2 | | | | | | |
| 3 | | | | | | |
| ... | | | | | | |

---

## Observation Table 2: Counts and Timing

| Item | Value |
|---|---|
| Number of REQUEST messages | |
| Number of RESPONSE messages | |
| Number of ERROR messages | |
| Number of REQUEST_NO_RETURN messages | |
| Number of NOTIFICATION (event) messages | |
| Number of SOME/IP-SD messages | |
| Response time for the first GetSpeed (ms) | |
| Time between two consecutive events (ms) | |

---

## Sequence Diagram

Using the completed observation table, draw a sequence diagram containing two vertical lifelines:

```text
Client ECU                          Server ECU
    │                                   │
    │                                   │
    │                                   │
    │                                   │
```

Add **one arrow per message** and label each arrow with:

- Message Type
- Method/Event name
- Session ID where applicable

---

## Questions

1. Which request had no partner message? Why?
2. How many requests received an `ERROR` instead of a `RESPONSE`? Name the return code of each.
3. The response time is well under 1 ms in this demo. Why would it be longer between two real ECUs on a vehicle network?
4. Why do events arrive approximately 1 second apart, while responses arrive as soon as a request is sent?

---

# Submission Requirements

For **each assignment**, submit:

- ✅ Completed observation table(s)
- ✅ Answers to all questions
- ✅ Saved Wireshark capture

Use the capture filename format:

```text
A<N>_<rollno>.pcapng
```

For **Assignment 7**, also submit:

- ✅ Sequence diagram

---

# Assignment Summary

| Assignment | Topic |
|---:|---|
| **1** | The 16-byte header |
| **2** | Matching requests and responses |
| **3** | Payloads and fire-and-forget |
| **4** | Errors and return codes |
| **5** | Events and subscription |
| **6** | Decode frames by hand |
| **7** | The full message sequence |

---

# Suggested Assignment Order

The recommended learning sequence is:

```text
Assignment 1
The 16-byte SOME/IP Header
        │
        ▼
Assignment 6
Decode Frames by Hand
        │
        ▼
Assignment 2
Request-Response Matching
        │
        ▼
Assignment 3
Payloads & REQUEST_NO_RETURN
        │
        ▼
Assignment 4
Errors & Return Codes
        │
        ▼
Assignment 5
Events & Subscription
        │
        ▼
Assignment 7
Complete SOME/IP Conversation
```

## Recommended Order

**Assignments 1 and 6 first** to understand the SOME/IP header and raw-frame decoding, followed by **Assignments 2–5** to observe protocol behaviour, and finally **Assignment 7** to analyse the complete client-server conversation.
