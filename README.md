# Nixie-NTP

### Nixie clock with NTP synchronisation

Like every amateur electrical engineer, I felt the need to make my own nixie
clock. However, I decided to add a bit of modern day tech and have the clock
set itself over NTP instead of requiring the user to manually adjust buttons.

The schematics provided detail the two boards - one control (arduino, ethernet)
and one display (shift registers, high voltage transistors).

## Partlist
**Logic board:**

- Atmega 328P (along with 16MHz crystal, 2x 22pf caps)
- Silvertel A9700 PoE module
- ENC28J60 ethernet PHY
- RB1-125BHQ1A RJ45 jack with magnetics
- LM1117 3.3V regulator (for ENC28J60)

**Display Board**

- LS595 shift register with latch
- MPSA42 (or similar) high voltage transistors
- Nixie tubes (Eagle footprint designed around the IN-14, YMMV)
- Some sort of high voltage power supply (~200V)

## Pretty pictures

![Active](/Pics/active.jpg?raw=true "It's alive")
![Display](/Pics/disp-front.jpg?raw=true "Forward display")
![Display Logic](/Pics/disp-back.jpg?raw=true "Display logic")
![Logic](/Pics/logic.jpg?raw=true "Logic board")
