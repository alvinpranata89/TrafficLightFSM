# TrafficLightFSM

This is what I have learned from one of the embedded system course from EmbeddedExpertIO regarding Finite State Machine (FSM) implementation on STM32 microcontroller using Nucleo F401RE board. The coding is in bare metal style, so I did not use CMSIS to configure the pin. All is done manually bit by bit using the provided datasheet, reference manual, and Cortx M4 processor user guide.

In this project we are implementing the concept of a traffic light system simulation based on 2 presence sensors connected to PC0 and PC1 as the input, and 6 LEDs connected to PA4-PA9 representing 2 traffic lights at an intersection as the output. 

<img width="941" height="595" alt="image" src="https://github.com/user-attachments/assets/ce395054-ea21-46b0-bf18-275de4caea6e" />

# input mapping
| PC0 | PC1 |    Meaning             |
| --- | ----|----------------------- |
|  0  |  0  | No car on either road  |
|  0  |  1  | Car detected on east road  |
|  1  |  0  | Car detected on west road  |
|  1  |  1  | Car detected on both side  |

PC0 : East car sensor
PC1 : West car sensor
