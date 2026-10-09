# Exercise 3 - Ethernet Performance

## Exercise Objective

Measure and analyse Ethernet network performance under different operating conditions.

After completing this exercise, the following concepts should be clear:

- Throughput
- Link Speed
- Bandwidth Utilization
- Packet Loss
- Latency
- Duplex Modes
- Network Congestion
- Performance Troubleshooting

---

## Experiments

1. Experiment 3.1 - Throughput Measurement
2. Experiment 3.2 - Packet Loss Study
3. Experiment 3.3 - Duplex Mismatch Study

---

# Experiment 3.1 - Throughput Measurement

## Objective

Measure the actual throughput achievable over an Ethernet network using iperf3.

---

## Concepts Covered

- Throughput
- Bandwidth
- Link Speed
- Data Transfer Rate
- TCP Communication

---

## Theory

Ethernet interfaces advertise a certain link speed.

Examples:

```text
100 Mbps

1 Gbps

10 Gbps
```

Actual application throughput is usually lower than the theoretical link speed because of:

- Protocol overhead
- Packet headers
- Network stack processing
- Device limitations

Throughput measurement helps determine how much useful data can actually be transferred.

---

## Hardware Setup

```text
PC 1 ---------- Ethernet Switch ---------- PC 2
```

---

## Software Required

```text
iperf3
```

---

## IP Configuration

```text
PC 1 : 192.168.10.10

PC 2 : 192.168.10.20
```

---

## Procedure

### Step 1

Verify connectivity.

From PC 1:

```bash
ping 192.168.10.20
```

---

### Step 2

Start iperf3 server on PC 2.

```bash
iperf3 -s
```

Expected:

```text
Server listening on 5201
```

---

### Step 3

Start iperf3 client on PC 1.

```bash
iperf3 -c 192.168.10.20
```

---

### Step 4

Observe throughput statistics.

---

## Expected Observation

Example:

```text
Transfer: 1.10 GBytes

Bandwidth: 940 Mbits/sec
```

Actual values depend on:

- PC capability
- Network adapters
- Switch capability
- Background traffic

---

## Practical Activity

Record observations.

| Parameter | Observed Value |
|------------|------------|
| Link Speed | |
| Test Duration | |
| Data Transferred | |
| Average Throughput | |

---

## Analysis

Compare:

```text
Configured Link Speed

vs

Measured Throughput
```

---

## Key Learning

- Throughput is the actual data transfer rate.
- Throughput is usually lower than the advertised link speed.
- Ethernet overhead reduces usable bandwidth.

---

## Review Questions

1. What is throughput?
2. Why is throughput lower than link speed?
3. What factors influence throughput?

---

## Conclusion

Ethernet throughput was measured successfully using iperf3.

---

# Experiment 3.2 - Packet Loss Study

## Objective

Observe packet loss under different network conditions.

---

## Concepts Covered

- Packet Loss
- Reliability
- Retransmission
- Connectivity Quality

---

## Theory

Packet loss occurs when transmitted packets fail to reach their destination.

Packet loss may be caused by:

- Network congestion
- Faulty cables
- Software overload
- Duplex issues
- Interface errors

Reliable Ethernet communication requires minimal packet loss.

---

## Hardware Setup

```text
PC 1 ---------- Ethernet Switch ---------- PC 2
```

---

## Procedure

### Step 1

Verify communication.

```bash
ping 192.168.10.20
```

---

### Step 2

Start continuous ping.

Windows:

```cmd
ping -t 192.168.10.20
```

Linux:

```bash
ping 192.168.10.20
```

---

### Step 3

Observe normal communication.

---

### Step 4

Disconnect Ethernet cable for a few seconds.

Reconnect the cable.

Observe the ping output.

---

### Step 5

Record packet loss information.

---

## Expected Observation

During disconnection:

```text
Request Timed Out
```

or

```text
Destination Host Unreachable
```

After reconnection:

```text
Reply from 192.168.10.20
```

---

## Practical Activity

Record observations.

| Parameter | Value |
|------------|------------|
| Packets Sent | |
| Packets Received | |
| Packets Lost | |
| Packet Loss % | |

---

## Analysis

Observe:

- Communication before failure
- Communication during failure
- Communication after recovery

---

## Key Learning

- Packet loss indicates missing data.
- Physical cable issues can cause packet loss.
- Ethernet communication automatically recovers after link restoration.

---

## Review Questions

1. What is packet loss?
2. How can packet loss affect applications?
3. What causes packet loss?

---

## Conclusion

Packet loss behaviour was analysed successfully.

---

# Experiment 3.3 - Duplex Mismatch Study

## Objective

Study the impact of duplex mismatch on Ethernet performance.

---

## Concepts Covered

- Half Duplex
- Full Duplex
- Auto Negotiation
- Ethernet Errors
- Performance Degradation

---

## Theory

Ethernet communication may operate in:

```text
Half Duplex
```

or

```text
Full Duplex
```

Modern switched Ethernet normally uses:

```text
Full Duplex
```

If one device operates in Half Duplex and the other operates in Full Duplex:

```text
Duplex Mismatch
```

This leads to:

- Reduced throughput
- Increased retransmissions
- Network errors
- Poor performance

---

## Hardware Setup

```text
PC 1 ---------- Ethernet Switch ---------- PC 2
```

---

## Procedure

### Step 1

Verify current link settings.

#### Windows

```powershell
Get-NetAdapter
```

#### Linux

```bash
sudo ethtool <interface>
```

---

### Step 2

Record:

```text
Speed

Duplex Mode
```

---

### Step 3

If the network adapter supports manual configuration:

Configure:

```text
PC 1 : Full Duplex

PC 2 : Half Duplex
```

---

### Step 4

Run throughput test.

```bash
iperf3 -c 192.168.10.20
```

---

### Step 5

Observe:

- Throughput
- Packet loss
- Errors

---

### Step 6

Restore both interfaces to:

```text
Auto Negotiation
```

Repeat throughput measurement.

---

## Expected Observation

Duplex mismatch generally causes:

```text
Lower throughput

Higher errors

Reduced efficiency
```

---

## Practical Activity

Record observations.

| Parameter | Full Duplex | Duplex Mismatch |
|------------|------------|------------|
| Throughput | | |
| Packet Loss | | |
| Observed Errors | | |

---

## Analysis

Compare:

```text
Normal Operation

vs

Duplex Mismatch
```

---

## Key Learning

- Modern Ethernet relies on Full Duplex operation.
- Duplex mismatch degrades network performance.
- Auto Negotiation helps prevent configuration errors.

---

## Review Questions

1. What is the difference between Half Duplex and Full Duplex?
2. Why does duplex mismatch reduce throughput?
3. Why is Auto Negotiation important?

---

## Conclusion

The impact of duplex mismatch on Ethernet performance was analysed successfully.

---

# Exercise Summary

The following performance concepts should now be understood:

```text
Ethernet Link
      ↓
Bandwidth
      ↓
Throughput
      ↓
Packet Loss
      ↓
Duplex Configuration
      ↓
Network Performance
```

The concepts learned in this exercise form the foundation for:

- Ethernet troubleshooting
- Switch performance analysis
- VLAN performance
- Industrial Ethernet networks
- Automotive Ethernet networks
