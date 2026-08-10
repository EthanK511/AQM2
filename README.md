# AQM2
A portable, inexpensive, open source, low-power CO2/air quality monitor
<img width="1387" height="1068" alt="Untitled drawing" src="https://github.com/user-attachments/assets/792de066-c95c-437c-93c2-a4182aea65e4" />
[Oshwlab](https://oshwlab.com/eknorwegia/project_kpvsolpj)
[Easy Eda View Project](https://pro.easyeda.com/editor#id=3279f3b71d5047e581513a55c1dcb35e)
There are two versions of this device. The first version with the larger PCB shown above was the original version. This version turned out to be too expensive so I pivoted to a more cost-effective yet still really good version.
## Why
### AQM1
In 2024, I started my first iteration of this project (AQM). Back then, it was also called many other names, such as "like CO2 Cloud" and "Big Waste Of Money".
### AQM2
I wanted to start work on a second version because I had some issues with the first; the design could have used some improvement, and my coding skills are much better now.
#### Issues With AQM1
1. Big
2. Expensive
3. Connects with Dupont cables
4. High power consumption
5. Ugly Interface

I planned to fix all of these issues with the first version of AQM2, and for everything except the price, it was amazing. However, the $50 price tag for a single PCB was more than I wanted to spend. With the second version of AQM2, I fixed this and simplified the design a bunch, and relying on my soldering skills to assemble the PCBs in my garage. This allowed me to bring the cost down to just a few cents per PCB.
## How
All of these issues can be solved in 3 steps
1. Custom PCB
2. New Code
3. New Interface

The device works when the SCD 41 gets a reading of temperature, humidity, and CO2 levels from the air around it. It uses an SDA and SCD connection to transmit this data to the ESP. From there, the data is sent via Wi-Fi to a Firebase database and then forwarded to a webpage using some simple HTML.
## BOM

|No.|Quantity|Comment |Designator|Footprint                      |Value|Manufacturer Part|Manufacturer|Supplier Part|Supplier|
|---|--------|--------|----------|-------------------------------|-----|-----------------|------------|-------------|--------|
|1  |2       |0.1uF   |C1,C2     |C0603                          |0.1uF|                 |            |             |        |
|2  |1       |ESP32C3L|H1        |HDR-TH_7P-P2.54-V-M            |     |ESP32C3L         |hanxia(韩下)  |C32713273    |LCSC    |
|3  |1       |ESP32C3R|H2        |HDR-TH_7P-P2.54-V-M            |     |ESP32C3R         |hanxia(韩下)  |C32713273    |LCSC    |
|4  |1       |2.54-1*4|SCD41     |HDR-TH_4P-P2.54-V-M-1          |     |2.54-1*4         |ZHOURI(洲日)  |C5116483     |LCSC    |
|5  |2       |SK6812  |U1,U2     |LED-SMD_4P-L5.0-W5.0-LS5.4-TL-1|     |SK6812           |欧思科光电       |C5378720     |LCSC    |

|Mrf#                              |Mfr.                     |Order Qty.|Unit Price(USD)|Ext.Price(USD)                    |LCSC#   |Package       |Min|Mult                                                                                                                                                                                                                                                                                                                                                                             |SPQ |Availability|Stock Status|Customer Notes|Description                                   |Matched status|Product Link                                     |
|----------------------------------|-------------------------|----------|---------------|----------------------------------|--------|--------------|---|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----|------------|------------|--------------|----------------------------------------------|--------------|-------------------------------------------------|
|SK6812                            |OPSCO Optoelectronics    |15        |0.0893         |1.34                              |C5378720|SMD-4P,5.4x5mm|5  |5                                                                                                                                                                                                                                                                                                                                                                                |1000|8695        |In Stock    |              |SMD-4P,5.4x5mm LED Addressable, Specialty RoHS|Exact Matches |https://www.lcsc.com/product-detail/C5378720.html|
|CL10B104KB8NNNC                   |Samsung Electro-Mechanics|100       |0.0160         |1.60                              |C1591   |0603          |100|100                                                                                                                                                                                                                                                                                                                                                                              |4000|2718200     |In Stock    |              |100nF ±10% 50V Ceramic Capacitor X7R 0603     |Exact Matches |https://www.lcsc.com/product-detail/C1591.html   |


