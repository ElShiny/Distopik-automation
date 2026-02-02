# Distopik automation
This is the main repository of Distopik automation Hardware and Firmware. 

This project seeks to create a universal remote control system for vintage audio processing machines (compressors, mixers, audio limiters...).

### Hardware
Hardware designs are made with Altium and are located in the `Hardware` folder.

They are split into three main parts:
- **PiVO Shield**: PiVO shield (or Pi Volume Operations shield) contains two power supplies, RS485 transceiver and main data and power connectors. It mounts onto a Raspberry Pi 4, which allows it to send data to the other modules. Communication system used is MODBUS RTU over RS485.
<p align="center">
    <img src="/images/20260201_230423.jpg" width="500" >
</p>

- **UNIREL-SW and UNIREL-POT**: UNIREL is short for universal relay module. It comes in two flavours: SW (switch) and POT (potentiometer). The SW version is used to control switches and buttons, while the POT version is used to control analog potentiometers. Each module contains a STM32 microcontroller and an RS485 transceiver. Multiple UNIREL modules can be connected in series to expand the number of controllable elements.
<p align="center">
    <img src="/images/20260201_230548.jpg" width="300"> <img src="/images/20260202_143303.jpg" width="300">
</p>

- **Frontend Controls**: Frontend controls are used to provide user input at the control pannel of the audio processing machine. They can be used if the user doesn't want to use a remote interface. They connect to the Raspberry Pi and send their positions to it. If a user changes their value remotely, then the frontend control will be updated to display the new value.
Currently we have three types of frontend controls:
  - Rotary encoders with LED ring indicators: ACE-LED Ring
  - Motorized faders: MottPott
  - Push buttons with LED indicators

<p align="center">
    <img src="/images/20260201_230358.jpg" width="500" >
</p>

