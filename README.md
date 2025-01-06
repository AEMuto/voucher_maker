# ESP32 Voucher Generator

## Project Overview
This repository contains the source code for a project designed to integrate an ESP32 device into a voucher-based internet access system. This project is built around two main components:

1. **Raspberry Pi 3B+:**
   - Runs OpenWrt with OpenNDS to provide a captive portal.
   - Handles voucher validation and internet access control.
   - Includes a Python-based MQTT server for communication with the ESP32.
2. **ESP32 (LilyGO T-Display-S3):**
   - Generates unique internet access vouchers.
   - Displays vouchers on its built-in screen.
   - Prints vouchers to a network-connected printer.
   - Sends voucher data to the Raspberry Pi via MQTT.

## Roadmap / TODOs
- [x] **WiFi Configuration Portal:**
  - [x] Implement a web interface for configuring WiFi credentials.
  - [x] Securely store credentials using the ESP32's non-volatile storage (NVS).
- [ ] **Raspberry Pi Integration:**
  - [ ] Finalize MQTT communication between the ESP32 and the Raspberry Pi.
- [ ] **Printing Integration:**
  - [ ] Implement network printing for vouchers.
  - [ ] Test compatibility with multiple printer models.
- [ ] **UI Enhancements:**
  - [ ] Add additional details such as expiration times to vouchers.
- [ ] **Security Enhancements:**
  - [ ] Secure MQTT communication.

### Key Features
- **Voucher Management:**
  - Generate unique internet access vouchers.
  - Display vouchers on the ESP32 screen and print them to a network printer.
  - Send vouchers to the Raspberry Pi via MQTT for captive portal integration.
- **WiFi Configuration Portal:**
  - Allows users to configure WiFi credentials through a web interface hosted by the ESP32.
  - Credentials are securely stored using the ESP32's non-volatile storage (NVS).
- **QR Code Display:**
  - Displays the network credentials and configuration portal URL as a QR code on the ESP32's screen.
- **Captive Portal Integration:**
  - Utilizes OpenNDS on the Raspberry Pi to manage guest internet access.

## Getting Started
### Prerequisites
- **Hardware:**
  - ESP32 (LilyGO T-Display-S3 recommended).
  - Raspberry Pi 3B+ with OpenWrt installed.
- **Software:**
  - PlatformIO (installed as a VSCode extension or standalone).
  - Python environment for the MQTT server on the Raspberry Pi.

### Installation
1. Clone this repository:
   ```sh
   git clone <repository-url>
   cd voucher_maker
   ```
2. Open the project in your preferred IDE (e.g., VSCode with PlatformIO).
3. Configure the `platformio.ini` file if necessary (e.g., to change upload settings).
4. Build and upload the firmware to your ESP32:
   ```sh
   platformio run --target upload
   ```

### Usage
1. **Power On:** Ensure the ESP32 is powered on and connected.
2. **WiFi Configuration:**
   - When no WiFi credentials are found, the ESP32 starts as an access point.
   - Scan the QR code displayed on the ESP32 screen using a mobile device.
   - Follow the link to the configuration portal and fill out the form to set WiFi credentials.
3. **Voucher Generation:**
   - Trigger voucher generation (e.g., via a button press).
   - View the voucher code on the ESP32 screen or print it to a network printer.
   - The voucher is sent to the Raspberry Pi for captive portal integration.

## License
This project is licensed under the [MIT License](LICENSE).
