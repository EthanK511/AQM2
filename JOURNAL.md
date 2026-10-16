---
title: "HackerCat"
author: "HackerCat"
description: "A wireless air-quality monitor that measures CO₂, temperature, and humidity, sends data to Firebase, and fits inside a custom 3D-printed enclosure."
created_at: "2026-05-03"
---

# Part selection

I'm going to use a collection of sensors and other parts to make a wireless monitor that measures CO₂, temperature, humidity, pressure, PM1, PM2.5, PM10, and maybe more.

<img width="366" height="252" alt="Screenshot 2026-10-16 11 43 01 AM" src="https://github.com/user-attachments/assets/481c7ead-0a4b-4ac3-91dc-aebe2a4b5754" />

**Total time spent: 0.2 hours**

# PCB schematic

I kind of forgot to journal for a while, but I made a bunch of progress! I added the sensors and the ESP, connected everything, and added a port and a lot of other small things to help make the project work.

<img width="815" height="564" alt="Screenshot 2026-10-16 11 40 37 AM" src="https://github.com/user-attachments/assets/50380a7c-a6ae-4661-b9b8-d7d6d3d472c9" />

**Total time spent: 4 hours**

# Finished the schematic and made a plan

At this point, I realized I had been pretty vague about the project, so here is a more detailed explanation.

For Rev. A of the ESP32 Air Quality Monitor, I planned to use an SCD41 for CO₂, temperature, and humidity; an SGP40 for VOCs; and a BME280 for pressure, temperature, and humidity. All three sensors use the same I²C bus through GPIO21 for SDA and GPIO22 for SCL. Each sensor has local 0.1 µF and 1 µF capacitors. The BME280 is configured for I²C mode, with CSB connected to 3V3 and SDO connected to GND.

The MCU and power system use an ESP32-WROOM, USB-C power and programming through a CH340C, a polyfuse, and an AMS1117-3.3 regulator. I also added bulk and 0.1 µF capacitors, along with a USBLC6-2SC6 TVS/ESD protection chip for VBUS and the USB data lines.

The ESP32 has proper decoupling, EN and BOOT buttons, and a 3V3 indicator LED. I also added a programming/debug header and test points for 3V3, GND, SDA, and SCL to make bring-up easier.

The software stack will use Wi-Fi and Firebase, with an HTML, CSS, and JavaScript frontend. The schematic was made in EasyEDA.

My next steps are to run ERC and DRC in EasyEDA, lay out the PCB, write the firmware, send JSON data to Firebase over HTTPS, build a website to read the data, and plot it.

Long term, I want to compare the readings against a known reference, tune the sampling intervals, and improve the enclosure and 3D design.

As an update since my last journal entry, I finished the schematic, updated some of the capacitors, and added more power-management parts to the USB-C section.

<img width="815" height="564" alt="Screenshot 2026-10-16 11 41 01 AM" src="https://github.com/user-attachments/assets/241060a0-7315-42a5-b950-6ecedb9540d8" />

**Total time spent: 2 hours**

# PCB layout

I started working on the PCB layout. I decided to begin with a 30 mm by 60 mm board outline, which I hope will give me enough room for all the parts and routing.

I placed the USB-C components, ESP32 parts, switches, headers, indicators, and the resistors and capacitors for everything I had added previously.

The next step is to research whether any of the sensors are sensitive to heat, airflow, or other things so I can place them in the best locations for accurate readings.

<img width="491" height="666" alt="Screenshot 2026-10-16 11 41 32 AM" src="https://github.com/user-attachments/assets/d67ac4d4-ec7f-4c56-8357-5e7741157900" />

**Total time spent: 1 hour**

# Finished the first PCB

## Sensor placement

The first thing I did was research what the sensors are sensitive to and try to figure out where to put them on the board. I found that the SCD41 is sensitive to outside heat and strong wind. The other sensors did not have any major sensitivities, so I placed them wherever they fit best.

## Routing

<img width="286" height="565" alt="Screenshot 2026-10-16 11 42 32 AM" src="https://github.com/user-attachments/assets/8b169849-09b8-4727-a970-83f92173e548" />

Next, I routed the board. I started with the communication lines and USB-C connections, then moved on to the 3V3 rail. I finished by adding a ground pour on both sides of the board.

## Rendering and finishing touches

After finishing the ground pour, I ran DRC. There were a few issues, but thankfully I was able to fix them pretty quickly.

## JLCPCB

After resolving the DRC issues, I uploaded the Gerber files and BOM to JLCPCB. After configuring everything, I got a quote for around $200. I was honestly shocked.

I went back and tried changing some settings, removing more expensive components, and adjusting anything else I could. After going through the quote process again, the price was not much better. Either way, this was going to be expensive.

<img width="311" height="23" alt="Screenshot 2026-10-16 11 42 50 AM" src="https://github.com/user-attachments/assets/257a3778-8466-4e81-8ff1-34f89426c9a9" />

**Total time spent: 4 hours**

# Change of plans

<img width="311" height="23" alt="Screenshot 2026-10-16 11 42 50 AM" src="https://github.com/user-attachments/assets/1854fa7e-df4d-4444-a163-f7b3ec53679e" />

The board was too expensive. After thinking about it for a while, I realized that I already had a small SCD41 and an ESP32 development board. Instead of spending another 20 hours designing an expensive board, I decided to make a smaller PCB that connects the two parts, design a 3D-printed enclosure, and write software to handle the database stuff.

While it breaks my heart to throw away all that work, there was no way I was going to work another 20 hours just to pay for the grant.

Here is the new plan:

- Two rows of female pin headers for the ESP32 development board
- A 4-pin header for the SCD41
- Two 4.7K pull-up resistors on SDA and SCL
- A couple of decoupling capacitors
- Possibly mounting holes for the enclosure

**Total time spent: 0.1 Hours**

# The parts

I picked the new parts.

Instead of using my larger ESP32 development board, I decided to use a smaller and more compact ESP to save space, energy, and money. I also chose the unsoldered version because it was cheaper, and I can solder it myself. It's not that hard.

I chose the ESP32-C3. Next up was the SCD41. I had a few options. I could use a bare sensor and have JLCPCB solder it onto the board, solder it myself, or buy a premade board from Amazon or AliExpress with the resistors and capacitors already installed.

In the end, I decided to use the premade board for simplicity and because the price was similar.

<img width="366" height="252" alt="Screenshot 2026-10-16 11 43 01 AM" src="https://github.com/user-attachments/assets/9300663c-a773-4e4d-b8be-4e21d6851768" />

**Total time spent: 0.1 hours**

# Schematic, again

I whipped up this little schematic pretty quickly, and I think it looks okay. It's very simple, and I added two extra individually addressable LEDs to act as air-quality indicators and for debugging.

<img width="372" height="131" alt="Screenshot 2026-10-16 11 43 15 AM" src="https://github.com/user-attachments/assets/16541161-d781-4378-ab02-f30fc6dc4329" />

**Total time spent: 0.5 hours**

# Issues everywhere

Picture this: you're minding your own business, making a PCB design, and suddenly the community-made part refuses to connect to the SCD41 pin.

I had no idea why it was happening. I asked the internet, asked AI, and felt like I had tried everything I could think of. I considered making custom pins for the ESP and aligning them manually in the PCB editor, but that sounded like a huge headache.

<img width="305" height="459" alt="Screenshot 2026-10-16 11 43 57 AM" src="https://github.com/user-attachments/assets/303b30ea-08a5-42d1-a3c9-b50cd13479e9" />

<img width="372" height="185" alt="Screenshot 2026-10-16 11 43 28 AM" src="https://github.com/user-attachments/assets/914b9e53-8dde-4e37-8bd5-fb8155a858e9" />

**Total time spent: 0.5 hours**

# PCB layout

I decided to use small 2.54 mm connectors instead of including the actual schematic components. I will not be assembling those parts directly onto this board, so that approach should work fine.

I arranged the new connectors and made sure they had the correct spacing. Then I grouped them and placed them on the board.

Some of the nets started acting glitchy, so I had to redo a few small sections. I also decided that the board was small enough that it did not need a ground pour, so I routed the ground connections by hand.

I just need to add some artwork now.

<img width="358" height="506" alt="Screenshot 2026-10-16 11 43 39 AM" src="https://github.com/user-attachments/assets/5a11df9a-6b6c-46d6-9a73-a200345c3753" />

**Total time spent: 0.3 hours**

# Logos and artwork

I finished the artwork! I made a custom logo in Canva, imported it into EasyEDA after removing the background with a cool Raycast extension for Mac, and then added some text and an HC logo.

After that, I was finished.

<img width="363" height="715" alt="Screenshot 2026-10-16 11 44 10 AM" src="https://github.com/user-attachments/assets/2f822fe9-bc1f-48b7-8b32-cd07532732ea" />

**Total time spent: 0.5 hours**

# Final edits

I found all the parts in the JLCPCB library that I needed to order, but the website did not give me a shipping cost, so I have no idea how much the final order will cost.

I will probably have to resubmit after I get the grant so I can get enough money for shipping.

I also filled out the “I need funding” section and added the PCB to my cart. I had to spend at least $2 on the actual components, so I decided to increase the number of PCBs to account for possible soldering mistakes. It was only about a dollar more to get an extra 15 boards.

<img width="363" height="47" alt="Screenshot 2026-10-16 11 44 28 AM" src="https://github.com/user-attachments/assets/8c9e643f-494d-4a18-943b-e9604ffe82c8" />

**Total time spent: 0.5 hours**

# GitHub

I made the GitHub repository and added a README. I also exported both EasyEDA project files for the two PCBs, added photos, and included a license.

<img width="367" height="586" alt="Screenshot 2026-10-16 11 44 38 AM" src="https://github.com/user-attachments/assets/3820d76d-8b2c-44d9-ab34-51ec864b9368" />

**Total time spent: 0.5 hours**

# CAD

I started working on the CAD design!

While sketching the Seeed Studio XIAO ESP32-C3, I tried to find the correct location for the USB-C port opening. Unfortunately, it did not line up. At some point, I must have messed something up.

The next step is to figure out what went wrong and fix it.

<img width="362" height="474" alt="Screenshot 2026-10-16 11 44 47 AM" src="https://github.com/user-attachments/assets/056cb06b-1dff-4ea3-81d2-8770ea4f0551" />

**Total time spent: 0.2 hours**

# Board dimensions

After more tries than I care to admit, I had to subtract 2.54 mm from the board width, which was originally 17.9 mm. That finally gave me the correct width so the board would fit.

Yay!

<img width="297" height="455" alt="Screenshot 2026-10-16 11 44 59 AM" src="https://github.com/user-attachments/assets/0a646fd6-50b1-4131-8d97-4aa3079b01b3" />

**Total time spent: 0.7 hours**

# Finished the PCB, hopefully

I decided to manually map out the sizes of the two components that would plug into the board. I added their basic outlines to the board so I could align everything more accurately.

After doing that, I realized the board was too short. I made it a little longer, rearranged some parts, and now I think everything should fit together properly.

The next step is designing the case. I want to make two versions: one with only plug-in capabilities, where the ESP plugs into an outlet or another power source, and another version with a space for a battery and room to plug in some kind of cable.

<img width="347" height="582" alt="Screenshot 2026-10-16 3 29 47 PM" src="https://github.com/user-attachments/assets/5aa6ec7b-bf61-4cc0-9051-ad65e5738ba5" />

**Total time spent: 0.6 hours**

# Finished the case

I made the case look nicer by rounding the edges. I also added an opening for the USB-C port and small ventilation holes near the sensor so air can pass through.

I thinned the walls near the LEDs so they can be seen through the white plastic I plan to use.

Finally, I added a friction-fit top with a lip. The top sits inside the case with a very small tolerance so it stays in place.

<img width="372" height="265" alt="Screenshot 2026-10-16 3 30 01 PM" src="https://github.com/user-attachments/assets/a571bef7-6d75-4c90-b7c6-56a7d3329698" />

**Total time spent: 1.1 hours**

# Firmware and Firebase

I wrote code that wakes the device every five minutes, reads the temperature, humidity, and CO₂ levels, connects to Wi-Fi and Firebase, sends the data to Firebase, and then goes back to deep sleep.

I added a bunch of checks to make sure everything works properly. I also had to create a Firebase database and configure the Realtime Database and authentication settings so the device could connect without getting blocked.

Overall, it was not too difficult, and it was a well-spent Saturday morning.

<img width="198" height="562" alt="Screenshot 2026-10-16 3 30 08 PM" src="https://github.com/user-attachments/assets/3b845ee0-9a01-4b36-871e-d086d08f29ee" />

**Total time spent: 3.8 hours**
