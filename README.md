# Nixie-NTP

### Nixie clock with NTP synchronisation

Like every amateur electrical engineer, I felt the need to make my own nixie
clock. However, I decided to add a bit of modern day tech and have the clock
set itself over NTP instead of requiring the user to manually adjust buttons.

The schematics provided detail the two boards - one control (arduino, ethernet)
and one display (shift registers, high voltage transistors).

## Partlist
#### (Digikey P/N in parens)

**Logic:**

- Atmega 328P (ATMEGA328-PU-ND / $3.01)
- Silvertel A9700 PoE module (N/A / $10)
- ENC28J60 ethernet PHY (ENC28J60-I/SO-ND / $2.83)
- RB1-125BHQ1A RJ45 jack with magnetics (N/A / $3.00)
- LM1117 3.3V regulator (LM1117T-3.3/NOPB-ND / $1.49)

**12-200V DC Boost:**
- 0.1uF 250V film cap (PCF1412CT-ND / $2.25)
- 4.7uF 450V alum cap (493-6196-1-ND / $1.51)
- MAX1771 DC-DC switching controller (MAX1771EPA+-ND / $7.23)
- 250V MOSFET (IRF644PBF-ND / $1.23)
- 100uF 20V tantalum cap (478-1746-1-ND / $2.52)
- 0.1uF 25V ceramic cap (490-1726-1-ND / $0.05)
- 10uF 25V tantalum cap (478-1762-1-ND / $1.02)
- 300V diode (ES2F-E3/52TGICT-ND / $0.40)
- 10k resistor (311-10.0KCRCT-ND / $0.02)
- 100uH inductor (M8603CT-ND / $0.81)
- 1.5M resistor (PT1.5MXCT-ND / $0.80)
- 0.05 ohm resistor (P50MCT-ND / $1.14)
- 5k trimmer pot (3214W-502ECT-ND / $3.18)

**Display Board**

- LS595 shift register with latch (296-1600-5-ND, $0.62)
- MPSA42 (or similar) high voltage transistors (MPSA42FS-ND, $0.22)
- Nixie tubes (Eagle footprint designed around the IN-14, YMMV)
- 1k resistors for regulating transistor base current
- 15k resistors for limiting nixie current

## Pretty pictures

![Pretty](/Pics/showcase.jpg?raw=true "It's alive")
![Logic](/Pics/logic.jpg?raw=true "Forward display")
![Display Logic](/Pics/display.jpg?raw=true "Display logic")
