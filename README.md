# TrafficLightFSM

This is what I have learned from one of the embedded system course from EmbeddedExpertIO regarding Finite State Machine (FSM) implementation on STM32 microcontroller using Nucleo F401RE board. The coding is in bare metal style, so I did not use HAL library to configure the pin. All is done manually bit by bit using CMSIS and the provided datasheet, reference manual, and Cortex M4 processor user guide. This project is built using STM32CubeIDE

In this project we are implementing the concept of a traffic light system simulation based on 2 presence sensors connected to PC0 and PC1 as the input, and 6 LEDs connected to PA4-PA9 representing 2 traffic lights at an intersection as the output. 

<img width="941" height="595" alt="image" src="https://github.com/user-attachments/assets/ce395054-ea21-46b0-bf18-275de4caea6e" />

## The rule for this traffic light system is as follows:

1. If no cars are coming, stay in green light
2. If cars are only coming in 1 direction, move to that direction and stay in green
3. If cars are coming in both directions, cycle through the Light on each side (Red-Green-Yellow-Red)
4. To change from green to red, turn on yellow light for exactly 5 seconds
5. Green light should last for at least 30 seconds
    
---    
## In this case we can translate the requirements into a Finite State Machine (FSM) as follows:  

   | FSM      |Description                                    |
   | ---------|-----------------------------------------------|
   |go_north  |Red Light at East and Green Light at North|
   |wait_north|Red Light at East and Yellow Light at North|
   |go_east   |Green Light at East and Red Light at North|
   |wait_east |Yellow Light at East and Red Light at North|

## Input Mapping

PC0 : East car sensor\
PC1 : North car sensor

| PC0 | PC1 |    Meaning             |
| --- | ----|----------------------- |
|  0  |  0  | No car on either road  |
|  0  |  1  | Car detected on north road  |
|  1  |  0  | Car detected on east road  |
|  1  |  1  | Car detected on both side  |

## FSM Diagram

<img width="911" height="340" alt="image" src="https://github.com/user-attachments/assets/f572c6ce-a196-49ce-9681-57c1e51bc8fe" />


| |PC1,PC0|PC1,PC0|PC1,PC0|PC1,PC0|
|-|-|-|-|-|
|State|00|01|10|11|
|go_north(100001,30)|go_north|wait_north|go_north|wait_north|
|wait_north(100010,5)|go_east|go_east|go_east|go_east
|go_east(001100,30)|go_east|go_east|wait_east|wait_east|
|wait_east(010100,5)|go_north|go_north|go_north|go_north|

## Output and FSM Mapping

| FSM | PC9 | PC8 | PC7 | PC6 | PC5 | PC4 |HEX for GPIO Port|Description|
| --- | - | --- | --- | --- | --- | --- |---|-|
|go_north  | 1 | 0 | 0 | 0 | 0 | 1|0x210|Red Light at East and Green Light at North|
|wait_north| 1 | 0 | 0 | 0 | 1 | 0|0x220|Red Light at East and Yellow Light at North|
|go_east   | 0 | 0 | 1 | 1 | 0 | 0|0x0C0|Green Light at East and Red Light at North|
|wait_east | 0 | 1 | 0 | 1 | 0 | 0|0x140|Yellow Light at East and Red Light at North|

## Code Flow
At first the code will start at **go_north** state machine, then it will continuously scanning the input of the sensor (PC0 and PC1). Based on the Output and FSM Mapping table, the state machine and LED output will be driven accordingly. The 30 second and 5 second delay during green and yellow light is handled by the Systick register of the Cortex M4  
