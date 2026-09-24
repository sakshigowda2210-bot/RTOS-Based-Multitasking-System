# RTOS-Based Multitasking System

Run in Wokwi: https://wokwi.com/projects/472432111311544321

## Overview

An RTOS-based embedded system project using ESP32 and FreeRTOS (Arduino framework, C++). The project runs 7 independent tasks and coordinates them using a queue, a mutex and an event group.

## Tasks (7)

- LED task
- Sensor task
- Monitor task
- Button task
- Event task
- System Status task
- UART task

## Synchronization and Communication

- **Queue:** the Sensor task sends data to the Monitor task
- **Mutex:** protects shared Serial output
- **Event Group:** a button press signals the Event task

## Pins

- LED: GPIO 2
- Button: GPIO 4

Note: sensor values are simulated using `random()`; no physical sensor is used.

## Technologies Used

- ESP32
- C++ (Arduino framework)
- FreeRTOS
- Tasks
- Queues
- Mutex
- Event Groups
- GPIO
- UART
- Wokwi

## Project Structure

| File | Description |
|---|---|
| `sketch.ino` | Main application and FreeRTOS task implementation |
| `diagram.json` | Wokwi circuit configuration |
| `wokwi-project.txt` | Wokwi project configuration |

## Simulation

The project was developed and tested using the Wokwi online simulator, without physical hardware.

### Wokwi Circuit

![Wokwi Circuit](freertos.jpeg)

### Simulation Output

![Simulation Output](freertos2.png)

## Learning Outcomes

- FreeRTOS task creation and scheduling
- Inter-task communication using queues
- Protecting shared resources with a mutex
- Task signalling using event groups
- Embedded firmware debugging using simulation

## Author

**Sakshi**

Electronics and Communication Engineering

