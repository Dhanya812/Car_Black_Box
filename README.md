 Car_Black_Box
 Overview
The Car Black Box is an embedded system project built using the PIC18F4580 microcontroller, designed to log and display vehicle data, including speed, gear events, and timestamps. The system uses an External EEPROM (I2C) to store critical data and a CLCD for real-time dashboard display.

Features
✔ Logs gear change events with timestamps and speed data
✔ Stores the latest 10 events, replacing the oldest when full
✔ Matrix Keypad Menu with four options:

View Log – Displays the latest 10 recorded events
Clear Log – Erases all stored event logs
Set Time – Adjusts real-time clock (RTC) settings
Download Log – Exports event data via UART (Tera Term)
✔ Real-Time Clock (RTC) integration for accurate timekeeping
✔ CLCD Display for real-time data visualization
Technical Specifications
🔹 Microcontroller: PIC18F4580
🔹 Storage: External EEPROM (I2C protocol)
🔹 Display: Character LCD (CLCD)
🔹 Input: Matrix Keypad
🔹 Communication: UART (for log export), I2C (for EEPROM)
🔹 Power Supply: 5V DC

System Architecture
1️⃣ Data Logging: On gear change, logs speed, time, and gear position to EEPROM
2️⃣ Real-Time Display: Shows current data on the CLCD
3️⃣ Memory Management: Stores a rolling history of the last 10 events
4️⃣ Data Export: Logs can be downloaded via UART (Tera Term)
5️⃣ User Interface: Controlled via matrix keypad menu

How to Use
1️⃣ Boot up the system – The dashboard displays real-time vehicle data.
2️⃣ Use the Matrix Keypad to enter the menu.
3️⃣ Select options:

View logs, clear logs, set time, or download logs.
4️⃣ View logs on CLCD or download them via UART.
