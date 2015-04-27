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

- Atmega 328P (ATMEGA328-PU-ND)
- Silvertel A9700 PoE module (N/A)
- ENC28J60 ethernet PHY (ENC28J60-I/SO-ND)
- RB1-125BHQ1A RJ45 jack with magnetics (N/A)
- LM1117 3.3V regulator (LM1117T-3.3/NOPB-ND)

**12-200V DC Boost:**
- 0.1uF 250V film cap (PCF1412CT-ND)
- 4.7uF 450V alum cap (493-6196-1-ND)
- MAX1771 DC-DC switching controller (MAX1771EPA+-ND)
- 250V MOSFET (IRF644PBF-ND)
- 100uF 20V tantalum cap (478-1746-1-ND)
- 0.1uF 25V ceramic cap (490-1726-1-ND)
- 10uF 25V tantalum cap (478-1762-1-ND)
- 300V diode (ES2F-E3/52TGICT-ND)
- 10k resistor (311-10.0KCRCT-ND)
- 100uH inductor (M8603CT-ND)
- 1.5M resistor (PT1.5MXCT-ND)
- 0.05 ohm resistor (P50MCT-ND)
- 5k trimmer pot (3214W-502ECT-ND)

**Display Board**

- LS595 shift register with latch (296-1600-5-ND)
- MPSA42 (or similar) high voltage transistors (MPSA42FS-ND)
- Nixie tubes (Eagle footprint designed around the IN-14, YMMV)
- 1k resistors for regulating transistor base current
- 15k resistors for limiting nixie current

## Pretty pictures

![Active](/Pics/active.jpg?raw=true "It's alive")
![Display](/Pics/disp-front.jpg?raw=true "Forward display")
![Display Logic](/Pics/disp-back.jpg?raw=true "Display logic")
![Logic](/Pics/logic.jpg?raw=true "Logic board")
