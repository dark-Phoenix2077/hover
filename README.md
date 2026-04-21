(NOTE: I UPDATED THE PCBs TODAY BECAUSE I MANAGED TO GET KICAD WORKING AND NOT CONSIDER SOME VIAS AS NOT CONNECTING)

# Hover
A fully custom mini racing/stunt drone with ML-assisted balancing, automatic stunts, and some level of autonomous operation without costly cameras or GPS.

<img width="826" height="598" alt="Screenshot 2026-04-16 at 4 11 55 pm" src="https://github.com/user-attachments/assets/a1b0b4b3-ac11-4cea-b6a8-a776079b2c0b" />

## Why I built this

I wanted a challenge that forced me to learn PCB design and embedded firmware from 
scratch, but also let me apply the area I already know well - machine learning. Designing
a drone from a blank schematic, writing every driver by hand, and then training a custom 
tiny ML model felt like the right way to do all of that at once. My goal was simply to push my first ever hardware project as far as I could, not to have a completely polished project.

## What the project is

Hover is a tiny brushed micro quadcopter built entirely from custom hardware and
firmware. There is no off-the-shelf flight controller, no commercial RC receiver, and no
pre-written driver code. Everything was written/designed from scratch by me.

The system has two PCBs:

Flight Controller (FC): A 4-layer X-shaped PCB with a motor in each corner. It runs an
STM32F103C8T6 at 72MHz, an ICM-42688-P IMU over SPI, a BK2423 2.4GHz RF transceiver, 
and four brushed motor drive circuits.

<img width="779" height="767" alt="Screenshot 2026-04-16 at 4 12 30 pm" src="https://github.com/user-attachments/assets/1dffbb4b-f9aa-4ede-8cdd-d61854352744" />

Remote Control (RC): A 2-layer PCB inside a 3D-printed enclosure. It uses
the same STM32F103C8T6 and BK2423 RF ICs. It has two analog joysticks (ADC + DMA) for
throttle, yaw, pitch, and roll, and eight buttons which are used for arming, auto take-off, auto land,
return-to-home, a 360-degree flip trigger (one of my goals for this was to have some kind of smart one-button trick, and for this goal I settled on the 360 flip), and three spares for future use.

<img width="1189" height="591" alt="Screenshot 2026-04-16 at 4 14 29 pm" src="https://github.com/user-attachments/assets/61232256-d98c-4058-ad8b-a3a2be643225" />

apart from that, this repository also includes:

ML Pipeline: A training and conversion pipeline for an onboard stabilisation model.
Training data will be collected from real flight telemetry once the hardware is
bought. The model will be converted to TFLite and dropped directly into the
FC firmware, which already has the inference code in place and waiting.

CAD: Both the drone body and RC enclosure were designed in Onshape. The drone frame
is a minimal 2-shell design with motor mounts, battery tray, and canopy, with both shells 
sandwiching the PCB in the middle. The RC enclosure uses the same 2-shell design with joystick 
cutouts, button extenders, and a battery bay below the PCB.

<img width="772" height="758" alt="Screenshot 2026-04-16 at 4 15 18 pm" src="https://github.com/user-attachments/assets/a5593ae2-4e91-42bd-a6a6-59e929a232c1" />

<img width="809" height="526" alt="Screenshot 2026-04-16 at 4 16 22 pm" src="https://github.com/user-attachments/assets/afe04248-f896-4809-ae5b-ef4413dce47a" />


Firmware: Firmware created for both the FC and RC in STM32 CubeMX and CubeIDE, all firmware should work but is untested as I do not have the hardware right now. additionally, the ML inference file is writtenbut will not work until I create the .tflite model file.

BOM: A BOM generated from kicad for the FC & the RC and also other parts (excluding 3D printed parts and assorted screws). I will likely be purchasing all of these myself, and this is not completely final.

## Notices and warnings

PCB: Both PCBs have been designed, passed DRC
(minus a known KiCad via-recognition quirk that was manually verified both by me and with
a local PCB supplier I know well and confirmed not an issue, see below), but have not been 
ordered yet - I am waiting for after the design check to be passed before ordering my boards. All firmware is written against 
the schematic, not a tested board.

PCB Warnings: Both boards carry a small number of DRC
warnings: silkscreen readability overlaps (cosmetic, non-functional) and unconnected
track-end warnings caused by a KiCad quirk where certain vias are not recognised as
connected. As I just mentioned, they were manually reviewed by me, and by a local supplier
to be not an issue.

ML model: The inference firmware and training pipeline are
complete, but the actual trained model does not exist yet. It cannot be trained until
flight data is collected from the physical drone as training data. Everything is written 
to accept a .tflit file e drop-in with no firmware changes required.

3D-printed enclosures: Both enclosures are functional first
drafts. Ergonomics and tolerances have not been iterated on. I plan to mostly leave the 
drone shell as-is with a bit of surface sanding, and the controller cad was a very quick mockup,
being a cuboid with the 4 corners sticking out (see the step file and/or the journals, it is a 
bit hard to explain) as I thought it would be easiest to cad right now - this is modeled 
after the controller for another drone I own.

Firmware: All firmware is untested, I am waiting for the actual PCBs to be ordered/arrive to test.


# Bill of Materials
FC

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

RC

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

Other

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
