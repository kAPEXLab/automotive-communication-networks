# 📘 Exercise 3 - Ethernet Performance

## 1. 🎯 Lab Objective

This exercise introduces Ethernet performance testing and explains how throughput, packet loss, and physical-layer issues affect network communication quality.

---

## 2. 🔖 Learning Outcomes

After completing this exercise, the reader will be able to:

- define throughput and distinguish it from link speed
- measure network performance using iperf3
- identify packet loss and explain its causes
- understand how duplex and cabling issues affect reliability
- analyse network health using practical observations

---

## 3. 📡 Equipment and Setup

### Hardware

- Two PCs
- Ethernet switch
- Ethernet cables

### Software

- iperf3
- ping
- Wireshark (optional for advanced analysis)

### Topology

```text
PC 1 ---------- Ethernet Switch ---------- PC 2
```

### IP Plan

```text
PC 1: 192.168.10.10 /24
PC 2: 192.168.10.20 /24
```

---

## 4. General Lab Instructions

1. Verify connectivity before starting performance tests.
2. Record the actual measured values from the lab equipment.
3. Keep the same Ethernet connection throughout the test.
4. Avoid background traffic during throughput measurement when possible.

---

# 📌 Experiment 3.1 - Throughput Measurement

## Objective

Measure the actual data transfer rate achievable over the Ethernet connection using iperf3.

## Background

The advertised Ethernet link speed is not always equal to the usable throughput. Actual data transfer is reduced by protocol overhead, processing delay, and device limitations.

## Procedure

### Step 1 - Verify connectivity

From PC 1:

```bash
ping 192.168.10.20
```

### Step 2 - Start the iperf3 server on PC 2

```bash
iperf3 -s
```

Expected output includes:

```text
Server listening on 5201
```

### Step 3 - Start the iperf3 client on PC 1

```bash
iperf3 -c 192.168.10.20
```

### Step 4 - Observe results

Record the values shown for:

- transfer size
- bandwidth
- time duration
- retransmissions (if shown)

## Expected Observation

The bandwidth reported by iperf3 will usually be lower than the theoretical link speed because of real-world overhead.

Example:

```text
Transfer: 1.10 GBytes
Bandwidth: 940 Mbits/sec
```

## Observation Table

| Parameter | Observed Value |
|-----------|----------------|
| Link Speed | |
| Test Duration | |
| Data Transferred | |
| Average Throughput | |
| Retransmissions | |

## Review Questions

1. What is throughput?
2. Why is throughput usually lower than the advertised link speed?
3. What factors influence measured throughput?

---

# 📌 Experiment 3.2 - Packet Loss Study

## Objective

Observe packet loss and understand how it affects communication quality.

## Background

Packet loss occurs when packets fail to reach their destination. Some common causes are congestion, faulty cabling, interface issues, and duplex mismatches.

## Procedure

### Step 1 - Check basic connectivity

```bash
ping 192.168.10.20
```

### Step 2 - Start continuous ping

#### Windows

```cmd
ping -t 192.168.10.20
```

#### Linux

```bash
ping 192.168.10.20
```

### Step 3 - Observe normal traffic

Notice the response times and whether any packets are lost.

### Step 4 - Introduce a fault condition

Disconnect a cable for a few seconds and reconnect it. Observe the Ping output during the interruption.

## Expected Observation

The key observations are as follows:

- successful replies under normal conditions
- delayed or failed replies during interruption
- packet loss when the link is unstable

## Observation Table

| Condition | Packets Sent | Packets Lost | Observations |
|-----------|--------------|--------------|--------------|
| Normal | | | |
| Cable disconnected briefly | | | |
| After reconnection | | | |

## Review Questions

1. What is packet loss?
2. What are common causes of packet loss?
3. Why is packet loss more serious than a small delay?

---

# 📌 Experiment 3.3 - Duplex and Link Quality Study

## Objective

Understand how duplex mismatch and cabling issues affect Ethernet performance.

## Background

Ethernet interfaces can operate in full-duplex or half-duplex mode. If both ends do not agree on the setting, communication may degrade, causing errors and packet loss.

## Procedure

1. Check the current interface status using the OS and, if available, ethtool.
2. Confirm link speed and duplex mode.
3. Compare the observed settings with the expected configuration.
4. Note whether any errors or collisions are reported.

### Linux example

```bash
sudo ethtool <interface>
```

### Windows example

```powershell
Get-NetAdapter | Format-List Name, Status, LinkSpeed, FullDuplex
```

## Expected Observation

The expected observations are as follows:

- a healthy link reports correct speed and duplex
- mismatch or instability may lead to errors or performance degradation

## Review Questions

1. What is duplex mode?
2. Why can a duplex mismatch cause packet loss?
3. How does link quality affect overall network reliability?

---

## Conclusion

This exercise demonstrates that Ethernet performance depends on more than just the presence of a link. Throughput, packet loss, and error behavior are affected by hardware, configuration, and environmental factors. It shows how to measure and interpret basic performance indicators in a real Ethernet network.
