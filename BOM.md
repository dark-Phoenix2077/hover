# Bill of Materials
## FC

* Note that MPU-6050 is significantly cheaper locally, and that is why I chose to use it despite cheaper alternatives

| Reference | Component | Qty | Package | LCSC | Price/Unit |
|---|---|---|---|---|---|
| U2 | STM32F103C8T6 | 1 | LQFP-48 | https://www.lcsc.com/product-detail/C8734.html | $1.65 |
| U3 | MPU-6050 | 1 | QFN-24 | https://www.lcsc.com/product-detail/C24112.html | $6.88 |
| U4 | BK2423 | 1 | QFN-20 | https://www.lcsc.com/search?q=BK2423 | ~$0.80 |
| U1 | AMS1117-3.3 | 1 | SOT-223 | https://www.lcsc.com/product-detail/C347222.html | ~$0.10 |
| Y1, Y2 | 16MHz Crystal | 2 | SMD 3225-4Pin | https://www.lcsc.com/product-detail/C13738.html | $0.30 |
| Q1–Q4 | SI2300 MOSFET | 4 | SOT-23 | https://www.lcsc.com/product-detail/C6285728.html | $0.01 |
| D1–D4 | 1N4148 Flyback Diode | 4 | SOD-323 | https://www.lcsc.com/product-detail/C2972761.html | $0.006 |
| J1 | Battery Connector (1S) | 1 | JST-PH 2pin | https://www.lcsc.com/product-detail/C295747.html | ~$0.08 |
| J2–J5 | Motor Connector | 4 | JST-PH 2pin | https://www.lcsc.com/product-detail/C295747.html | ~$0.08 |
| J6 | SWD Header | 1 | 2.54mm 1×4 | https://www.lcsc.com/product-detail/C124375.html | ~$0.05 |
| J7 | Charge Port | 1 | Barrel Jack SMD | www.lcsc.com/product-detail/C16214.html | ~$0.20 |
| SW1 | Slide Switch | 1 | SMD SPDT | https://www.lcsc.com/product-detail/C431541.html | ~$0.12 |
| TP1–TP5 | Test Point 1.5mm | 5 | Pad D1.5mm | https://www.lcsc.com/search?q=testpoint+1.5mm | ~$0.02 |
| C1, C3, C19, C21 | 10µF Cap | 4 | 0805 | https://www.lcsc.com/search?q=0805+capacitor | ~$0.002 |
| C2, C4–C10, C14–C18, C20, C24 | 100nF Cap | 15 | 0402 | https://www.lcsc.com/search?q=0402+capacitor | ~$0.002 |
| C11 | 1µF Cap | 1 | 0402 | https://www.lcsc.com/search?q=0402+capacitor | ~$0.002 |
| C12, C13, C22, C23 | 22pF Cap | 4 | 0402 | https://www.lcsc.com/search?q=0402+capacitor | ~$0.002 |
| R1, R6, R8, R10, R12 | 10kΩ Resistor | 5 | 0402 | https://www.lcsc.com/search?q=0402+resistor | ~$0.002 |
| R2, R3 | 4.7kΩ Resistor | 2 | 0402 | https://www.lcsc.com/search?q=0402+resistor | ~$0.002 |
| R4 | 22kΩ Resistor | 1 | 0402 | https://www.lcsc.com/search?q=0402+resistor | ~$0.002 |
| R5, R7, R9, R11 | 100Ω Resistor | 4 | 0402 | https://www.lcsc.com/search?q=0402+resistor | ~$0.002 |

## RC

| Reference | Component | Qty | Package | LCSC | Price/Unit |
|---|---|---|---|---|---|
| U4 | STM32F103C8T6 | 1 | LQFP-48 | https://www.lcsc.com/product-detail/C8734.html | $1.65 |
| U5 | BK2423 | 1 | QFN-20 | https://www.lcsc.com/search?q=BK2423 | ~$0.80 |
| U1 | AMS1117-3.3 | 1 | SOT-223 | https://www.lcsc.com/product-detail/C347222.html | ~$0.10 |
| Y2, Y3 | 16MHz Crystal | 2 | SMD 3225-4Pin | https://www.lcsc.com/product-detail/C13738.html | $0.30 |
| BZ1 | Buzzer | 1 | 12×9.5mm THT | — | — |
| D1 | LED Indicator | 1 | THT D1.8mm | — | — |
| J1 | Battery Connector (1S) | 1 | JST-PH 2pin | https://www.lcsc.com/product-detail/C295747.html | ~$0.08 |
| J2, J3 | Joystick Connector | 2 | 2.54mm 1×5 | — | — |
| J7 | SWD Header | 1 | 2.54mm 1×4 | https://www.lcsc.com/product-detail/C124375.html | ~$0.05 |
| J8 | Charge Port | 1 | Barrel Jack THT | https://www.lcsc.com/product-detail/C16214.html | ~$0.20 |
| SW1–SW8 | Push Button | 8 | THT 6mm | — | — |
| SW9 | Slide Switch | 1 | SMD SPDT | https://www.lcsc.com/product-detail/C431541.html | ~$0.12 |
| TP1 | Test Point 1.5mm (ANT) | 1 | Pad D1.5mm | https://www.lcsc.com/search?q=testpoint+1.5mm | ~$0.02 |
| C1, C3, C23, C31 | 10µF Cap | 4 | 0805 | https://www.lcsc.com/search?q=0805+capacitor | ~$0.002 |
| C2, C4, C14–C19, C24, C25, C28–C30 | 100nF Cap | 13 | 0402 | https://www.lcsc.com/search?q=0402+capacitor | ~$0.002 |
| C20 | 1µF Cap | 1 | 0402 | https://www.lcsc.com/search?q=0402+capacitor | ~$0.002 |
| C21, C22, C26, C27 | 22pF Cap | 4 | 0402 | https://www.lcsc.com/search?q=0402+capacitor | ~$0.002 |
| R1–R3 | 10kΩ Resistor | 3 | 0402 | https://www.lcsc.com/search?q=0402+resistor | ~$0.002 |
| R4 | 22kΩ Resistor | 1 | 0402 | https://www.lcsc.com/search?q=0402+resistor | ~$0.002 |
| R5, R6 | 100Ω Resistor | 2 | 0402 | https://www.lcsc.com/search?q=0402+resistor | ~$0.002 |

## Other

* This list covers physical assembly components not part of either PCB
* It is not completely final - I made this off the top of my head, there are probably other components I forgot to identify
* It does not include miscellaneous screws or 3D printed parts

| Component | Qty | Notes | Price (est.) |
|---|---|---|---|
| Brushed coreless DC motors (8020) | 4 | 8×20mm form factor, 1S compatible | ~$2.00 each |
| 55mm micro quad propellers (CW) | 4 | Match motor shaft diameter | ~$0.50 each |
| 55mm micro quad propellers (CCW) | 4 | Match motor shaft diameter | ~$0.50 each |
| 1S LiPo battery — 150mAh (drone) | 1 | Fits battery tray in drone frame | ~$4.00 |
| 1S LiPo battery — 800mAh (RC) | 1 | Fits battery bay in RC enclosure | ~$6.00 |
| 31.25mm wire for 2.4GHz antenna (drone) | 1 | Bare solid-core wire, soldered to ANT test point on FC PCB; length = λ/4 at 2.4GHz | ~$0.10 |
| 31.25mm wire for 2.4GHz antenna (RC) | 1 | Same as above, for RC PCB ANT test point | ~$0.10 |
| Silicone motor wire (26AWG) | ~1m | Connects motors to PCB JST connectors | ~$1.00 |
| 1S LiPo charger (USB) | 1 | Compatible with JST-PH or barrel jack input | ~$5.00 |
