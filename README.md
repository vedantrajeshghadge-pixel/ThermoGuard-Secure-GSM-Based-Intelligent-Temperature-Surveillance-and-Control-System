# ThermoGuard: Secure GSM-Based Intelligent Temperature Surveillance and Control System

An intelligent GSM-based embedded temperature surveillance and control system developed using **LPC2148** that provides real-time temperature monitoring, secure threshold management, automated SMS alerts, and authenticated remote and local configuration.

---

## 📌 Overview

**ThermoGuard** is a real-time embedded temperature monitoring and control system designed for environments where temperature must be continuously monitored and maintained within safe limits.

The system uses an **LM35 temperature sensor** interfaced with the **LPC2148 ARM7 microcontroller**. The measured temperature is displayed on a **16×2 LCD** and continuously compared with a predefined temperature set point stored in **AT24C256 EEPROM**.

When the temperature exceeds the configured threshold, the system activates a **buzzer** and sends an alert SMS to the authorized mobile number through the **M660A GSM module**.

ThermoGuard also provides secure configuration through:

* 📱 GSM-based SMS commands
* ⌨️ Keypad-based local access
* 🔐 Four-digit passkey authentication
* ⚡ External interrupt-based local configuration

---

## 🎯 Aim

To develop an intelligent and secure GSM-based temperature surveillance and control system that provides:

* Real-time temperature monitoring
* Temperature threshold management
* Automatic over-temperature alerts
* Secure remote configuration
* Secure local configuration
* SMS-based sensor information
* EEPROM-based configuration storage
* Unauthorized access detection

---

## ⚙️ System Architecture

The ThermoGuard system is built around the **LPC2148 ARM7 microcontroller**, which coordinates temperature sensing, display, GSM communication, EEPROM storage, keypad input, external interrupts, and alert generation.

<img width="2720" height="2560" alt="system_block_diagram_updated" src="https://github.com/user-attachments/assets/0617d45e-105e-4935-bce6-1115d3ef6efe" />



**Figure 1. ThermoGuard System Architecture**

---

## 🧰 Hardware Requirements

| Component              | Purpose                            |
| ---------------------- | ---------------------------------- |
| **LPC2148**            | Main ARM7 microcontroller          |
| **GSM Module (M660A)** | SMS communication                  |
| **16×2 LCD**           | Temperature and status display     |
| **LM35**               | Temperature sensing                |
| **Buzzer**             | Temperature alert indication       |
| **AT24C256**          | EEPROM configuration storage       |
| **Switch**             | External interrupt trigger         |
| **Keypad**             | Password and local set-point entry |

---

## 💻 Software Requirements

* **Embedded C**
* **Keil µVision**
* **Flash Magic**
* UART interrupt programming
* I²C communication

---

## 🧠 Project Modules

### 1️⃣ Temperature Monitoring Module

The **LM35 temperature sensor** provides temperature information to the LPC2148 through its ADC interface.

The microcontroller:

* Reads the sensor value.
* Processes the temperature information.
* Displays the current temperature on the LCD.
* Compares the temperature with the stored set point.

---

### 2️⃣ 📺 LCD Module

The 16×2 LCD is used to display:

* Current temperature
* Temperature set point
* System status
* Authentication status
* Access-denied messages
* Local configuration information

---

### 3️⃣ 🔔 Buzzer Module

The buzzer provides a local warning when the measured temperature exceeds the configured temperature threshold.

```text
Current Temperature > Set Point
              │
              ▼
         Buzzer ON
```

---

### 4️⃣ 📱 GSM Module

The **M660A GSM module** provides SMS-based communication between the ThermoGuard system and the authorized user.

The GSM module is controlled through UART communication using AT commands.

The system supports:

* SMS reception
* SMS transmission
* Remote set-point modification
* Mobile-number modification
* Sensor-information requests
* Alert SMS generation

---

### 5️⃣ 💾 EEPROM Module

The **AT24C256 EEPROM** is used for storing important configuration information.

Stored information can include:

* Temperature set point
* Authorized mobile number
* Security passkey

The EEPROM is interfaced using **I²C communication**.

**Byte Write** and **Byte Read** operations are used for EEPROM access.

---

### 6️⃣ ⌨️ Keypad Module

The keypad provides local user input for:

* Security password
* New temperature set point
* Local configuration

---

### 7️⃣ ⚡ External Interrupt Module

The external interrupt switch is used to initiate secure local configuration.

When the switch is pressed:

```text
External Interrupt
        │
        ▼
Local Set Point Mode
        │
        ▼
Password Authentication
```

---

### 8️⃣ 🔐 Security Module

ThermoGuard uses authentication for both remote and local configuration.

#### Remote Authentication

The received SMS must:

1. Originate from the authorized mobile number.
2. Contain the correct four-digit passkey.
3. Follow the correct command syntax.

#### Local Authentication

The user must activate the external interrupt and enter the correct four-digit password through the keypad.

---

## 📡 GSM Communication

The GSM module can be tested using the following AT commands:

```text
AT
ATE0
AT+CMGF=1
AT+CNMI=2,1,0,0,0
AT+CMGD=1
AT+CMGR=1
AT+CMGS="Mobile Number"
```

### Sending an SMS

The basic SMS transmission sequence is:

```text
AT+CMGS="Mobile Number"
        │
        ▼
       '>'
        │
        ▼
  Enter Message
        │
        ▼
   Ctrl + Z (0x1A)
        │
        ▼
      SMS Sent
```

GSM interfacing is implemented using **UART interrupt-based communication**.

---

## 📲 SMS Command Format

ThermoGuard uses the following command format:

```text
XXXXCDDDD...$
```

Where:

| Field  | Description                 |
| ------ | --------------------------- |
| `XXXX` | Four-digit security passkey |
| `C`    | Command identifier          |
| `D...` | Command data                |
| `$`    | End-of-command character    |

### Command Types

| Command | Function                        |
| ------- | ------------------------------- |
| `T`     | Change temperature set point    |
| `M`     | Change authorized mobile number |
| `I`     | Request sensor information      |

---

## 🌡️ Temperature Set-Point Modification

To change the temperature set point:

```text
0786T38$
```

Where:

```text
0786 → Passkey
T    → Temperature command
38   → New temperature set point
$    → End marker
```

After successful authentication, the new set point is stored in EEPROM.

---

## 📞 Mobile Number Modification

To change the authorized mobile number:

```text
0786M9866666699$
```

Where:

```text
0786          → Passkey
M             → Mobile-number command
9866666699    → New mobile number
$             → End marker
```

The new number is stored in EEPROM and used for future alert messages.

---

## 📊 Sensor Information Request

To request current sensor information:

```text
0786I$
```

The system verifies the passkey and sends the current sensor information through SMS.

---

## 🔄 Remote Monitoring Workflow

```text
            START
               │
               ▼
      Initialize Peripherals
               │
               ▼
       Read LM35 Temperature ◄──────────────┐
               │                            │
               ▼                            │
          Display on LCD                    │
               │                            │
               ▼                            │
      Read Set Point from EEPROM            │
               │                            │
               ▼                            │
       Compare Temperature                  │
               │                            │
        ┌──────┴──────┐                     │
        │             │                     │
      Above         Normal                  │
     Set Point       Range ─────────────────┘
        │
        ▼
    Buzzer ON
        │
        ▼
    Send Alert SMS
        │
        ▼
       Check Incoming SMS
               │
               ▼
       Validate Mobile Number
               │
               ▼
        Validate Passkey
               │
               ▼
        Validate Command
               │
       ┌───────┼───────┐
       ▼       ▼       ▼
       T       M       I
       │       │       │
   Set Point  Mobile  Sensor
    Update    Update  Information
       │       │       │
       └───────┼───────┘
               ▼
       Continue Monitoring
```

---

## ⌨️ Local Set-Point Modification

ThermoGuard also supports local temperature set-point modification.

The process begins when the external interrupt switch is pressed.

### Local Access Workflow

```text
External Interrupt
        │
        ▼
Local Set Point Mode
        │
        ▼
Enter 4-Digit Password
        │
   ┌────┴────┐
   │         │
 Correct    Wrong
   │         │
   ▼         ▼
Enter New   Access
Set Point   Denied
   │         │
   ▼         ▼
Store in    Send Unauthorized
EEPROM      Access SMS
   │
   ▼
Send Confirmation SMS
   │
   ▼
Return to Monitoring
```

### Successful Authentication

If the password is correct:

* LCD allows new set-point entry.
* New temperature set point is stored in EEPROM.
* Confirmation SMS is sent to the authorized mobile number.
* System returns to normal monitoring.

### Failed Authentication

If the password is incorrect:

* LCD displays **"Access Denied"**.
* Unauthorized local access alert SMS is sent.
* System returns to normal monitoring mode.

---

## 🧪 Module Testing

Each module can be tested individually before complete system integration.

### LCD Testing

Test:

* Character display
* String display
* Integer display

### Keypad Testing

Read keypad values and display the pressed key on the LCD.

### EEPROM Testing

Test:

* Byte Write
* Byte Read

Write data to EEPROM and read it back for verification.

### External Interrupt Testing

Verify the external interrupt using an LED or equivalent indication.

### UART Testing

Test:

* Character transmission
* String transmission
* String reception

UART interrupt functionality should be used.

### GSM Testing

Verify GSM communication using AT commands before integrating the GSM module with the complete application.

---

## 📸 Hardware Implementation

### 🔧 Complete Hardware Setup

<img width="1600" height="1200" alt="image" src="https://github.com/user-attachments/assets/dc759f2b-bbfe-45ed-88ab-6adbcb5f6abc" />



**Figure 2. Actual ThermoGuard Hardware Setup**

---

## 🖥️ System Output

### 🌡️ Temperature Monitoring

<img width="1600" height="1200" alt="image" src="https://github.com/user-attachments/assets/569a8857-403a-4623-be72-ce1f0a7d6279" />

**Figure 3.Temperature Monitoring Output**

---


## 📱 GSM SMS Output

<img width="1080" height="1160" alt="image" src="https://github.com/user-attachments/assets/42b9561d-c1a9-45a7-a55d-e0abb4a18453" />


**Figure 5. GSM-Based SMS Communication**

---

## 🚀 Features

* ✔ Real-time temperature monitoring
* ✔ LM35-based temperature sensing
* ✔ LPC2148 ARM7 microcontroller
* ✔ GSM-based SMS communication
* ✔ Automatic over-temperature SMS alerts
* ✔ Buzzer-based temperature warning
* ✔ Secure four-digit passkey authentication
* ✔ Remote temperature set-point modification
* ✔ Remote mobile-number modification
* ✔ Remote sensor-information request
* ✔ EEPROM-based configuration storage
* ✔ Keypad-based local configuration
* ✔ External interrupt-based local access
* ✔ Unauthorized local access detection
* ✔ UART interrupt-based GSM communication
* ✔ I²C-based EEPROM communication
* ✔ 16×2 LCD monitoring interface

---

## ✨ Applications

ThermoGuard can be used in temperature-sensitive environments such as:

* 🏭 Industrial plants
* 🧪 Laboratories
* ❄️ Cold storage facilities
* 🖥️ Server rooms
* 💊 Pharmaceutical storage
* 🏥 Medical equipment environments
* 🌾 Agricultural storage facilities

---

## 💡 Future Enhancements

Possible future improvements include:

* IoT/cloud-based temperature monitoring
* Web-based monitoring dashboard
* Mobile application integration
* Multiple temperature sensors
* Temperature data logging
* Historical temperature graphs
* Email and push notifications
* Automatic cooling-system control
* Battery backup
* Advanced authentication mechanisms
* Cloud-based data storage

---

## 🏁 Conclusion

**ThermoGuard: Secure GSM-Based Intelligent Temperature Surveillance and Control System** demonstrates the integration of embedded systems with GSM-based remote communication and secure access control.

The system combines the **LPC2148 ARM7 microcontroller, LM35 temperature sensor, M660A GSM module, 16×2 LCD, AT24C256 EEPROM, keypad, external interrupts, buzzer, UART, I²C, and ADC**.

The project provides real-time temperature surveillance, automatic alert generation, secure remote configuration, and authenticated local set-point modification.

It demonstrates practical implementation of **Embedded C, ARM7 architecture, ADC, UART interrupts, GSM communication, I²C, EEPROM, keypad interfacing, LCD interfacing, and external interrupts**.

---

## 👨‍💻 Author

### **MADHURI KAKUMANI**

**Electronics and Communication Engineering**

**Andhra Loyola Institute Engineering and Technology**

---

## 📜 License

This project is developed for **academic and educational purposes**.

You are free to study, modify, and enhance the project with proper credit to the original author.

---

⭐ **If you find this project useful, consider giving it a star on GitHub!**
