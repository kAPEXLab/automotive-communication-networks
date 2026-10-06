# Exercise 1 - Ethernet Fundamentals

## Exercise Objective

Establish a basic Ethernet network between two computers and examine how communication progresses from physical link establishment to Ethernet frame transmission.

After completing this exercise, the following concepts should be clear:

- Physical Ethernet link establishment
- Link status, link speed and duplex mode
- IPv4 address and subnet mask configuration
- End-to-end connectivity verification using Ping
- Relationship between Ethernet, IPv4 and ICMP
- Ethernet frame encapsulation
- Source and destination MAC addresses
- EtherType and frame length
- Packet capture and analysis using Wireshark

---

## Experiments

1. Experiment 1.1 - Physical Link Establishment
2. Experiment 1.2 - IP Configuration
3. Experiment 1.3 - Ping Communication
4. Experiment 1.4 - Ethernet Frame Analysis

---

## General Hardware Requirements

- Two computers with Ethernet interfaces
- One unmanaged Ethernet switch
- Two Cat5e or Cat6 Ethernet cables
- Power adaptor for the Ethernet switch

---

## General Software Requirements

- Windows or Ubuntu/Linux operating system
- Command Prompt, PowerShell or Linux Terminal
- Wireshark

---

## Common Hardware Setup

```text
+------+        +-----------------+        +------+
| PC 1 |--------| Ethernet Switch |--------| PC 2 |
+------+        +-----------------+        +------+
```

### Connections

```text
PC 1 Ethernet Port ---- Switch Port 1
PC 2 Ethernet Port ---- Switch Port 2
```

---

## Important Lab Instructions

1. Use the same two Ethernet interfaces throughout all four experiments.
2. Disable Wi-Fi temporarily to ensure that test traffic uses the wired Ethernet connection.
3. Do not connect the experimental switch to the organisation network or Internet unless specifically instructed.
4. Use only the IP addresses assigned in this lab manual.
5. Record actual observations. Do not copy the example values provided in the expected-observation sections.
6. Restore the original network configuration after completing the exercise.

---

# Experiment 1.1 - Physical Link Establishment

## Objective

Establish and verify the physical Ethernet link between two computers connected through an Ethernet switch.

---

## Concepts Covered

- Ethernet Physical Layer
- Network interface
- Ethernet cable
- Link status
- Auto-negotiation
- Link speed
- Duplex mode
- Link and activity indicators

---

## Concept Note

Ethernet communication begins with physical connectivity.

When an Ethernet interface is connected to a switch:

1. The interface detects the connected link partner.
2. The two interfaces perform auto-negotiation, if supported and enabled.
3. A common link speed and duplex mode are selected.
4. The operating system reports the Ethernet interface as connected.
5. Link indicators become active.

An IP address is not required to establish the physical link. IP configuration will be performed in Experiment 1.2.

---

## Hardware Required

- PC 1
- PC 2
- Unmanaged Ethernet switch
- Two Cat5e or Cat6 Ethernet cables

---

## Hardware Setup

```text
PC 1  <---- Ethernet Cable ---->  Ethernet Switch
PC 2  <---- Ethernet Cable ---->  Ethernet Switch
```

---

## Pre-Experiment Check

Before connecting the cables, verify the following:

- Both computers are powered ON.
- The Ethernet switch is powered ON.
- The Ethernet interfaces are enabled.
- The cables are not visibly damaged.
- Wi-Fi is disabled temporarily.

---

## Procedure

### Step 1 - Observe the Disconnected State

Keep both Ethernet cables disconnected.

Check the Ethernet interface status on each computer.

#### Windows

Open PowerShell and execute:

```powershell
Get-NetAdapter
```

Alternatively, open:

```text
Control Panel
→ Network and Internet
→ Network Connections
```

#### Ubuntu/Linux

Open a terminal and execute:

```bash
ip link show
```

Record the Ethernet interface name.

Common examples include:

```text
eth0
enp2s0
enp3s0
eno1
```

> **Note:** The exact interface name depends on the computer. Use the actual interface name in subsequent Linux commands.

---

### Step 2 - Connect PC 1

Connect the Ethernet port of PC 1 to an available port on the switch.

Wait for a few seconds.

Observe:

- Link indicator on PC 1
- Link indicator on the corresponding switch port
- Ethernet interface status in the operating system

---

### Step 3 - Connect PC 2

Connect the Ethernet port of PC 2 to another available port on the switch.

Wait for a few seconds.

Observe:

- Link indicator on PC 2
- Link indicator on the corresponding switch port
- Ethernet interface status in the operating system

---

### Step 4 - Verify the Link Status

#### Windows

Execute:

```powershell
Get-NetAdapter
```

Example output:

```text
Name      Status   LinkSpeed
Ethernet  Up       1 Gbps
```

To check additional interface details, execute:

```powershell
Get-NetAdapter | Format-List Name, Status, LinkSpeed, MacAddress
```

#### Ubuntu/Linux

Replace `<interface>` with the actual Ethernet interface name.

Execute:

```bash
ip link show <interface>
```

Example:

```bash
ip link show enp2s0
```

Look for:

```text
state UP
```

If `ethtool` is available, execute:

```bash
sudo ethtool <interface>
```

Example:

```bash
sudo ethtool enp2s0
```

Look for:

```text
Speed: 1000Mb/s
Duplex: Full
Link detected: yes
```

---

### Step 5 - Observe Link Interruption

Disconnect the Ethernet cable from PC 1.

Observe:

- Change in the PC 1 link indicator
- Change in the corresponding switch-port indicator
- Change in the operating-system interface status

Reconnect the cable and verify that the link is re-established.

Repeat the same check for PC 2.

---

## Expected Observation

When the cable is connected correctly:

```text
Ethernet interface status : Connected/Up
Link detected             : Yes
Link indicator            : Active
Switch-port indicator     : Active
```

The displayed link speed may be:

```text
100 Mbps
1 Gbps
```

The negotiated value depends on the capabilities of the two link partners and the cable.

A blinking activity indicator normally shows that Ethernet traffic is being transmitted or received. The exact indicator colour and behaviour depend on the hardware.

---

## Practical Activity

Record the observed values.

| Parameter | PC 1 | PC 2 |
|---|---|---|
| Ethernet interface name | | |
| Status before cable connection | | |
| Status after cable connection | | |
| Link speed | | |
| Duplex mode, if displayed | | |
| Link indicator active | | |
| Switch-port indicator active | | |
| Status after cable disconnection | | |

---

## Observation Questions

1. Was an IP address required for physical link establishment?
2. How long did the interface take to report the link as active?
3. What link speed was negotiated?
4. Was full-duplex mode reported?
5. What happened when the cable was disconnected?
6. Did the link recover automatically after reconnection?

---

## Key Learning

- Physical connectivity is established before IP communication begins.
- Link status confirms that the local interface can detect its directly connected link partner.
- Auto-negotiation selects a mutually supported link configuration.
- A link indicator confirms physical connectivity, but it does not confirm correct IP configuration.
- Link establishment belongs to the lower layers of the communication stack.

---

## Troubleshooting

If the link is not established, check the following:

1. Confirm that the switch is powered ON.
2. Confirm that the Ethernet interface is enabled.
3. Push both cable connectors fully into their ports.
4. Try a different Ethernet cable.
5. Try a different switch port.
6. Check for damaged cable connectors.
7. Restart the network interface.
8. Check whether the Ethernet adaptor appears in the operating system.
9. Test the cable and switch port with another known working device.

---

## Recovery

1. Disconnect both Ethernet cables.
2. Power-cycle the Ethernet switch.
3. Enable the Ethernet interfaces on both computers.
4. Reconnect PC 1 and verify its link.
5. Reconnect PC 2 and verify its link.
6. Repeat the link-status checks.

---

## Conclusion

The physical Ethernet links between both computers and the Ethernet switch were established and verified successfully.

---

# Experiment 1.2 - IP Configuration

## Objective

Configure static IPv4 addresses on two computers and verify that both interfaces belong to the same IPv4 subnet.

---

## Concepts Covered

- IPv4 address
- Subnet mask
- Network portion
- Host portion
- Same-subnet communication
- Static IP configuration
- Interface configuration verification

---

## Concept Note

A physical Ethernet link allows data to travel between connected interfaces. IPv4 addresses provide logical addresses used for communication at the Network Layer.

For two devices to communicate directly within this experiment:

- Each interface must have a unique IPv4 address.
- Both interfaces must use the same subnet mask.
- Both addresses must belong to the same IPv4 subnet.

The following configuration will be used:

```text
Network Address : 192.168.10.0
Subnet Mask     : 255.255.255.0
Prefix Length   : /24
```

With a `/24` prefix:

```text
Network portion : 192.168.10
Host portion    : Last octet
```

Therefore:

```text
PC 1 : 192.168.10.10
PC 2 : 192.168.10.20
```

belong to the same IPv4 subnet but have different host addresses.

---

## Hardware Setup

```text
+-------------------+                      +-------------------+
| PC 1              |                      | PC 2              |
| 192.168.10.10/24  |---- Ethernet Switch--| 192.168.10.20/24  |
+-------------------+                      +-------------------+
```

---

## IP Configuration

### PC 1

```text
IPv4 Address : 192.168.10.10
Subnet Mask  : 255.255.255.0
Prefix Length: 24
Gateway      : Leave blank
DNS Server   : Leave blank
```

### PC 2

```text
IPv4 Address : 192.168.10.20
Subnet Mask  : 255.255.255.0
Prefix Length: 24
Gateway      : Leave blank
DNS Server   : Leave blank
```

> **Note:** A default gateway and DNS server are not required because this is an isolated local network and both devices are in the same subnet.

---

## Procedure

## Method A - Windows Graphical Configuration

Perform the following on PC 1:

1. Open **Control Panel**.
2. Select **Network and Internet**.
3. Select **Network and Sharing Centre**.
4. Select **Change adapter settings**.
5. Right-click the Ethernet interface.
6. Select **Properties**.
7. Select **Internet Protocol Version 4 (TCP/IPv4)**.
8. Select **Properties**.
9. Select **Use the following IP address**.
10. Enter:

```text
IP address : 192.168.10.10
Subnet mask: 255.255.255.0
```

11. Leave the default gateway blank.
12. Leave the DNS server fields blank.
13. Select **OK**.

Repeat the procedure on PC 2 using:

```text
IP address : 192.168.10.20
Subnet mask: 255.255.255.0
```

---

## Method B - Windows PowerShell

Open PowerShell as Administrator.

First, identify the Ethernet interface:

```powershell
Get-NetAdapter
```

If required, remove an existing IPv4 address from the isolated test interface before applying the lab address.

Configure PC 1:

```powershell
New-NetIPAddress -InterfaceAlias "Ethernet" -IPAddress 192.168.10.10 -PrefixLength 24
```

Configure PC 2:

```powershell
New-NetIPAddress -InterfaceAlias "Ethernet" -IPAddress 192.168.10.20 -PrefixLength 24
```

> **Note:** Replace `"Ethernet"` with the actual interface alias if a different name is displayed.

---

## Method C - Ubuntu/Linux Terminal

Identify the interface:

```bash
ip link show
```

Replace `<interface>` with the actual Ethernet interface name.

Enable the interface:

```bash
sudo ip link set <interface> up
```

Remove existing addresses from the selected isolated test interface:

```bash
sudo ip addr flush dev <interface>
```

Configure PC 1:

```bash
sudo ip addr add 192.168.10.10/24 dev <interface>
```

Configure PC 2:

```bash
sudo ip addr add 192.168.10.20/24 dev <interface>
```

Example for PC 1:

```bash
sudo ip addr add 192.168.10.10/24 dev enp2s0
```

> **Important:** Configuration performed using the `ip` command is normally temporary and may be lost after a restart or network-service reconfiguration.

---

## Configuration Verification

### Windows

Execute:

```cmd
ipconfig
```

Locate the Ethernet adaptor and verify:

```text
IPv4 Address : 192.168.10.10 or 192.168.10.20
Subnet Mask  : 255.255.255.0
```

Alternatively, use PowerShell:

```powershell
Get-NetIPAddress -AddressFamily IPv4
```

### Ubuntu/Linux

Execute:

```bash
ip -4 addr show <interface>
```

Example:

```bash
ip -4 addr show enp2s0
```

Expected address on PC 1:

```text
inet 192.168.10.10/24
```

Expected address on PC 2:

```text
inet 192.168.10.20/24
```

---

## Expected Observation

### PC 1

```text
IPv4 Address : 192.168.10.10
Subnet Mask  : 255.255.255.0
Prefix Length: /24
```

### PC 2

```text
IPv4 Address : 192.168.10.20
Subnet Mask  : 255.255.255.0
Prefix Length: /24
```

Both interfaces should report an active Ethernet link.

---

## Practical Activity

Record the actual configuration.

| Parameter | PC 1 | PC 2 |
|---|---|---|
| Interface name | | |
| IPv4 address | | |
| Subnet mask | | |
| Prefix length | | |
| Default gateway | | |
| Link status | | |

---

## Address Analysis

Complete the following:

| Item | Value |
|---|---|
| PC 1 IPv4 address | |
| PC 2 IPv4 address | |
| Subnet mask | |
| Prefix length | |
| Network address | |
| Host portion of PC 1 | |
| Host portion of PC 2 | |
| Same subnet: Yes/No | |

---

## Controlled Observation - Duplicate Address

> **Perform this activity only on the isolated lab network. Restore the correct address immediately after observation.**

Temporarily configure PC 2 with the same IPv4 address as PC 1:

```text
192.168.10.10/24
```

Observe whether the operating system reports a duplicate-address warning or whether communication becomes inconsistent.

Restore PC 2 to:

```text
192.168.10.20/24
```

---

## Controlled Observation - Different Subnet

Temporarily configure PC 2 as:

```text
IPv4 Address : 192.168.20.20
Subnet Mask  : 255.255.255.0
```

Compare the network portions:

```text
PC 1 network : 192.168.10.0/24
PC 2 network : 192.168.20.0/24
```

Do not expect direct IPv4 communication without a router between these subnets.

Restore PC 2 to:

```text
192.168.10.20/24
```

---

## Key Learning

- An IPv4 address is a logical Layer 3 address.
- A subnet mask separates the network portion from the host portion.
- Two interfaces in the same subnet must use unique host addresses.
- A physical link may be active even when the IPv4 configuration is incorrect.
- A default gateway is not needed for direct communication within one subnet.
- Physical connectivity and IPv4 configuration are separate requirements.

---

## Review Questions

1. Why must the two computers use unique IP addresses?
2. What does the `/24` prefix represent?
3. What is the network address for `192.168.10.10/24`?
4. Which part of `192.168.10.10/24` identifies the host?
5. Why is a default gateway not required in this experiment?
6. Can two devices have an active Ethernet link but still fail to exchange IPv4 packets?
7. What happens when the two computers are configured in different IPv4 subnets without a router?

---

## Troubleshooting

If the expected configuration is not displayed:

1. Confirm that the correct Ethernet interface was configured.
2. Verify that Wi-Fi is disabled.
3. Check for typing errors in the IPv4 address.
4. Verify that the subnet mask is `255.255.255.0`.
5. Confirm that PC 1 and PC 2 use different addresses.
6. Remove unexpected addresses from the test interface.
7. Check whether a network-management service has overwritten the temporary configuration.
8. Reconfigure the address and verify it again.

---

## Recovery

Restore the required settings:

### PC 1

```text
192.168.10.10/24
```

### PC 2

```text
192.168.10.20/24
```

Verify the final configuration before proceeding to Experiment 1.3.

---

## Conclusion

Static IPv4 addresses were configured successfully, and both Ethernet interfaces were placed in the same IPv4 subnet.

---

# Experiment 1.3 - Ping Communication

## Objective

Verify bidirectional IPv4 connectivity between the two computers using Ping and examine the effect of cable disconnection and reconnection.

---

## Concepts Covered

- ICMP Echo Request
- ICMP Echo Reply
- Reachability
- Round-trip time
- Packet transmission
- Packet loss
- Relationship between Ethernet, IPv4 and ICMP
- Link interruption and recovery

---

## Concept Note

Ping checks whether an IPv4 destination is reachable.

Ping normally sends an ICMP Echo Request to the destination. If the destination receives the request and is permitted to respond, it returns an ICMP Echo Reply.

The traffic is encapsulated as follows:

```text
Ethernet Frame
└── IPv4 Packet
    └── ICMP Echo Request or Echo Reply
```

A successful Ping in this isolated network confirms that:

- The physical links are active.
- The Ethernet interfaces can exchange frames.
- The IPv4 configuration supports direct communication.
- ICMP traffic is not being blocked.

A failed Ping does not always prove that the physical link is faulty. Incorrect addressing or firewall policy can also block the response.

---

## Preconditions

Before starting:

- Experiment 1.1 must be completed.
- Experiment 1.2 must be completed.
- PC 1 must use `192.168.10.10/24`.
- PC 2 must use `192.168.10.20/24`.
- Both Ethernet links must be active.
- Wi-Fi should remain disabled.

---

## Hardware Setup

```text
PC 1                                      PC 2
192.168.10.10/24                          192.168.10.20/24
       |                                         |
       +---------- Ethernet Switch --------------+
```

---

## Procedure

### Step 1 - Verify Local TCP/IP Configuration

On PC 1, test the local IPv4 stack.

#### Windows

```cmd
ping 127.0.0.1
```

#### Ubuntu/Linux

```bash
ping -c 4 127.0.0.1
```

Repeat on PC 2.

Expected result:

```text
Reply received
0% packet loss
```

---

### Step 2 - Ping PC 2 from PC 1

#### Windows

```cmd
ping 192.168.10.20
```

#### Ubuntu/Linux

```bash
ping -c 4 192.168.10.20
```

---

### Step 3 - Ping PC 1 from PC 2

#### Windows

```cmd
ping 192.168.10.10
```

#### Ubuntu/Linux

```bash
ping -c 4 192.168.10.10
```

---

## Expected Observation

A successful Windows result may appear as:

```text
Reply from 192.168.10.20: bytes=32 time<1ms TTL=128

Ping statistics for 192.168.10.20:
    Packets: Sent = 4, Received = 4, Lost = 0 (0% loss)
```

A successful Linux result may appear as:

```text
64 bytes from 192.168.10.20: icmp_seq=1 ttl=64 time=0.5 ms

4 packets transmitted, 4 received, 0% packet loss
```

Actual time and TTL values can differ depending on the operating system and hardware.

---

## Step 4 - Run Continuous Ping

### Windows

From PC 1:

```cmd
ping -t 192.168.10.20
```

Stop the command using:

```text
Ctrl+C
```

### Ubuntu/Linux

From PC 1:

```bash
ping 192.168.10.20
```

Stop the command using:

```text
Ctrl+C
```

---

## Step 5 - Disconnect the Cable

While continuous Ping is running:

1. Disconnect the Ethernet cable from PC 2.
2. Observe the Ping output.
3. Wait several seconds.
4. Reconnect the cable.
5. Observe whether replies resume automatically.

---

## Expected Observation During Disconnection

Depending on the operating system, one of the following may appear:

```text
Request timed out
```

```text
Destination host unreachable
```

```text
Network is unreachable
```

```text
100% packet loss
```

After reconnection and link recovery, Ping replies should normally resume.

Some initial packets may be lost while the link and address-resolution information recover.

---

## Step 6 - Record Ping Statistics

Stop continuous Ping and record:

- Number of packets transmitted
- Number of replies received
- Number of lost packets
- Packet-loss percentage
- Minimum round-trip time
- Maximum round-trip time
- Average round-trip time

---

## Practical Activity

### Bidirectional Connectivity

| Test | Result | Packets Sent | Packets Received | Packet Loss |
|---|---|---:|---:|---:|
| PC 1 to PC 2 | | | | |
| PC 2 to PC 1 | | | | |

### Cable Interruption

| Observation | Recorded Result |
|---|---|
| Output before cable disconnection | |
| Output after cable disconnection | |
| Link indicator after disconnection | |
| Output after cable reconnection | |
| Link indicator after reconnection | |
| Communication recovered automatically | |

### Round-Trip Time

| Direction | Minimum | Maximum | Average |
|---|---:|---:|---:|
| PC 1 to PC 2 | | | |
| PC 2 to PC 1 | | | |

---

## Interpretation

### Successful Ping

A successful reply shows that the destination responded to the ICMP Echo Request.

### Request Timed Out

The request was sent, but a reply was not received within the expected time.

Possible causes include:

- Firewall blocking ICMP
- Disconnected cable
- Destination interface disabled
- Incorrect destination address
- Destination not responding

### Destination Host Unreachable

A local system or network device reports that it cannot deliver the packet to the destination.

Possible causes include:

- Incorrect subnet configuration
- Missing route
- Incomplete address resolution
- Physical disconnection

---

## Key Learning

- Ping uses ICMP to test IPv4 reachability.
- A successful Ping involves Ethernet, IPv4 and ICMP.
- Round-trip time measures the time required for a request and its reply.
- Packet loss indicates that some requests or replies were not received.
- A physical link can recover automatically after cable reconnection.
- Ping failure must be diagnosed layer by layer rather than attributed only to the cable.

---

## Review Questions

1. Which protocol is used by Ping?
2. What is the purpose of an ICMP Echo Request?
3. What is the purpose of an ICMP Echo Reply?
4. What does round-trip time represent?
5. Why does Ping work without Internet connectivity in this experiment?
6. Which protocol layers are involved in a successful Ping?
7. What is the difference between `Request timed out` and `Destination host unreachable`?
8. Why might Ping fail even when the link indicators are active?
9. What happens to packet loss when the Ethernet cable is disconnected?
10. Why may the first Ping after reconnection take longer or fail?

---

## Troubleshooting

If Ping fails:

1. Verify the link indicators.
2. Check the Ethernet adaptor status.
3. Verify PC 1 is configured as `192.168.10.10/24`.
4. Verify PC 2 is configured as `192.168.10.20/24`.
5. Confirm that both addresses are unique.
6. Verify that both addresses belong to the same subnet.
7. Confirm that the correct destination address is being used.
8. Disable Wi-Fi temporarily.
9. Check whether the host firewall blocks ICMP.
10. Test the local TCP/IP stack using `ping 127.0.0.1`.
11. Reconnect the cables.
12. Try another switch port or cable.

> **Security Note:** If firewall changes are required, apply them only to the isolated lab network and restore the original security configuration after testing.

---

## Recovery

1. Stop continuous Ping.
2. Verify both cable connections.
3. Verify both link indicators.
4. Reapply the required IPv4 addresses.
5. Confirm the interfaces are enabled.
6. Repeat the local loopback test.
7. Repeat the bidirectional Ping test.

---

## Conclusion

Bidirectional IPv4 communication was verified using ICMP Ping. The effect of physical link interruption and automatic recovery was also observed.

---

# Experiment 1.4 - Ethernet Frame Analysis

## Objective

Capture and analyse Ethernet communication using Wireshark and identify the Ethernet, IPv4 and ICMP information associated with Ping traffic.

---

## Concepts Covered

- Packet capture
- Ethernet frame
- Source MAC address
- Destination MAC address
- EtherType
- IPv4 packet
- ICMP Echo Request
- ICMP Echo Reply
- Encapsulation
- Frame length
- Protocol layering

---

## Concept Note

Application and network data is transmitted over Ethernet through encapsulation.

For the Ping used in this experiment:

```text
Ethernet Frame
└── IPv4 Packet
    └── ICMP Message
        ├── Echo Request
        └── Echo Reply
```

The Ethernet header normally contains:

```text
Destination MAC Address
Source MAC Address
EtherType
```

For an IPv4 packet:

```text
EtherType = 0x0800
```

The complete on-wire Ethernet transmission also includes fields such as the preamble, Start Frame Delimiter and Frame Check Sequence. These fields are handled by the Ethernet hardware and may not be displayed in an ordinary Wireshark capture.

> **Important:** Do not assume that every physical-layer field will appear in Wireshark. Most standard computer network interfaces provide the captured frame to the operating system after certain physical-layer information has already been processed.

---

## Preconditions

Before starting:

- Bidirectional Ping must be working.
- PC 1 must use `192.168.10.10/24`.
- PC 2 must use `192.168.10.20/24`.
- Wireshark must be installed on PC 1.
- The correct Ethernet interface must be known.
- Wi-Fi should remain disabled.

---

## Hardware Setup

```text
PC 1                                      PC 2
Wireshark                                 Ping destination
192.168.10.10/24                          192.168.10.20/24
       |                                         |
       +---------- Ethernet Switch --------------+
```

---

## Software Required

- Wireshark
- Command Prompt, PowerShell or Linux Terminal

---

## Procedure

### Step 1 - Record MAC Addresses

Before starting Wireshark, record the MAC address of each Ethernet interface.

#### Windows

Execute:

```cmd
getmac /v
```

Alternatively:

```powershell
Get-NetAdapter | Format-Table Name, MacAddress, Status
```

#### Ubuntu/Linux

Execute:

```bash
ip link show <interface>
```

Look for:

```text
link/ether
```

Example:

```text
link/ether 3c:52:82:11:22:33
```

Record the actual MAC addresses.

---

### Step 2 - Start Wireshark

1. Open Wireshark on PC 1.
2. Identify the wired Ethernet interface.
3. Confirm that the interface shows activity.
4. Double-click the Ethernet interface to start packet capture.

> **Note:** Select the wired Ethernet interface, not Wi-Fi, loopback or a virtual adaptor.

---

### Step 3 - Generate Ping Traffic

From PC 1, Ping PC 2.

#### Windows

```cmd
ping 192.168.10.20
```

#### Ubuntu/Linux

```bash
ping -c 4 192.168.10.20
```

---

### Step 4 - Stop the Capture

After the Ping command completes:

1. Return to Wireshark.
2. Select the red **Stop** button.
3. Save the capture if required.

Suggested filename:

```text
Exercise_1_4_Ping_Capture.pcapng
```

---

### Step 5 - Apply an ICMP Display Filter

Enter the following in the Wireshark display-filter bar:

```text
icmp
```

Apply the filter.

The packet list should show ICMP Echo Request and Echo Reply packets.

---

### Step 6 - Select an ICMP Echo Request

Select one packet labelled:

```text
Echo (ping) request
```

Expand:

```text
Ethernet II
```

Record:

- Destination MAC address
- Source MAC address
- EtherType

Expand:

```text
Internet Protocol Version 4
```

Record:

- Source IP address
- Destination IP address
- Time to Live
- Protocol

Expand:

```text
Internet Control Message Protocol
```

Record:

- ICMP type
- ICMP code
- Identifier
- Sequence number

---

### Step 7 - Select the Matching Echo Reply

Select the corresponding packet labelled:

```text
Echo (ping) reply
```

Compare it with the Echo Request.

Observe:

- Source and destination MAC addresses
- Source and destination IPv4 addresses
- ICMP type
- Sequence number
- Frame length

---

## Expected Observation

### ICMP Echo Request

```text
Ethernet Source MAC      : PC 1 MAC address
Ethernet Destination MAC : PC 2 MAC address
EtherType                : IPv4 (0x0800)
IPv4 Source              : 192.168.10.10
IPv4 Destination         : 192.168.10.20
Upper-layer protocol     : ICMP
ICMP message             : Echo Request
```

### ICMP Echo Reply

```text
Ethernet Source MAC      : PC 2 MAC address
Ethernet Destination MAC : PC 1 MAC address
EtherType                : IPv4 (0x0800)
IPv4 Source              : 192.168.10.20
IPv4 Destination         : 192.168.10.10
Upper-layer protocol     : ICMP
ICMP message             : Echo Reply
```

The exact frame length may vary with the operating system and Ping implementation.

---

## Practical Activity 1 - Record Interface Addresses

| Parameter | PC 1 | PC 2 |
|---|---|---|
| Ethernet interface name | | |
| MAC address | | |
| IPv4 address | | |

---

## Practical Activity 2 - Analyse the Echo Request

| Field | Observed Value |
|---|---|
| Frame number | |
| Frame length | |
| Source MAC address | |
| Destination MAC address | |
| EtherType | |
| Source IPv4 address | |
| Destination IPv4 address | |
| IP protocol | |
| IP Time to Live | |
| ICMP type | |
| ICMP code | |
| ICMP identifier | |
| ICMP sequence number | |

---

## Practical Activity 3 - Analyse the Echo Reply

| Field | Observed Value |
|---|---|
| Frame number | |
| Frame length | |
| Source MAC address | |
| Destination MAC address | |
| EtherType | |
| Source IPv4 address | |
| Destination IPv4 address | |
| IP protocol | |
| IP Time to Live | |
| ICMP type | |
| ICMP code | |
| ICMP identifier | |
| ICMP sequence number | |

---

## Practical Activity 4 - Compare Request and Reply

| Item | Echo Request | Echo Reply |
|---|---|---|
| Source MAC | | |
| Destination MAC | | |
| Source IPv4 address | | |
| Destination IPv4 address | | |
| ICMP type | | |
| Sequence number | | |
| Frame length | | |

---

## Analysis

Answer the following using the captured packet:

1. Did the Echo Request use PC 1 as the Ethernet source?
2. Did the Echo Reply reverse the source and destination MAC addresses?
3. Did the Echo Reply reverse the source and destination IPv4 addresses?
4. Was EtherType `0x0800` displayed?
5. Did the matching request and reply use the same sequence number?
6. Was the ICMP message carried inside an IPv4 packet?
7. Was the IPv4 packet carried inside an Ethernet frame?
8. Did Wireshark display the Ethernet preamble?
9. Did Wireshark display the Frame Check Sequence?
10. Was the displayed frame length identical for request and reply?

---

## Optional Observation - ARP Before ICMP

Clear the address-resolution cache and repeat the capture only if permitted in the lab environment.

### Windows

Open Command Prompt as Administrator:

```cmd
arp -d *
```

### Ubuntu/Linux

Replace `<interface>` with the actual interface:

```bash
sudo ip neigh flush dev <interface>
```

Start a new Wireshark capture and Ping PC 2 again.

Apply the display filter:

```text
arp or icmp
```

Observe whether ARP communication appears before the ICMP Echo Request.

Possible ARP sequence:

```text
Who has 192.168.10.20?
Tell 192.168.10.10
```

This optional activity introduces the relationship between an IPv4 address and a MAC address. Detailed ARP analysis can be performed in Exercise 2.

---

## Useful Wireshark Display Filters

### Show ICMP traffic

```text
icmp
```

### Show ARP traffic

```text
arp
```

### Show traffic involving PC 2

```text
ip.addr == 192.168.10.20
```

### Show ICMP traffic involving PC 2

```text
icmp and ip.addr == 192.168.10.20
```

### Show IPv4 Ethernet frames

```text
eth.type == 0x0800
```

---

## Key Learning

- Wireshark separates captured data into Ethernet, IPv4 and ICMP layers.
- MAC addresses identify interfaces for local Layer 2 delivery.
- IPv4 addresses provide logical Layer 3 addressing.
- EtherType `0x0800` identifies an IPv4 payload.
- Ping uses ICMP Echo Request and Echo Reply messages.
- Source and destination addresses reverse between the request and reply.
- Encapsulation allows an ICMP message to be carried inside IPv4 and Ethernet.
- Physical-layer information such as the preamble is not normally visible in a standard software capture.
- The Frame Check Sequence may not be visible because it is commonly processed or removed by the network interface before capture.

---

## Review Questions

1. What is the purpose of a MAC address?
2. What is the purpose of an IPv4 address?
3. What does EtherType `0x0800` represent?
4. What protocol is encapsulated inside the IPv4 packet during Ping?
5. What is the difference between an ICMP Echo Request and Echo Reply?
6. Why do the source and destination addresses reverse in the reply?
7. What is encapsulation?
8. Why might the Ethernet preamble not appear in Wireshark?
9. Why might the Frame Check Sequence not appear in Wireshark?
10. What information connects an Echo Reply to its corresponding Echo Request?
11. Why may ARP traffic appear before the first ICMP packet?
12. Which information belongs to Layer 2 and which belongs to Layer 3?

---

## Troubleshooting

If no packets are captured:

1. Confirm that the correct Ethernet interface is selected.
2. Confirm that the capture started before the Ping command.
3. Verify that Ping communication is working.
4. Disable Wi-Fi temporarily.
5. Ensure that no restrictive capture filter was configured.
6. Remove the display filter and check the complete packet
