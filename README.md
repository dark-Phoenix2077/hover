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
