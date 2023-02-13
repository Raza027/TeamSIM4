					


-------------------------------------------------------------Accident Alert Device--------------------------------------------------------------
-------------------------------------------------------------------TEAM SIM4--------------------------------------------------------------------
Team Details :-

Name                                       Student id 

Omar Farooq Khan Suri                               202006216,

Ifra Khan                                  202006240,

Raza Ahmed                                 202005967,

Mohd Suhail                                  202006109,



	

Introduction:-

Atleast one out of 10 people killed on roads across the world is from India.
A total of 4,03,116 road accident cases were reported during 2021 and the fatalities in road accidents increased by 16.8% from 2020.
Timely help is not provided to the injured person and this results in more death cases.
Our device addresses this problem and provides a solution for this issue. It's functioning is such that it not only alerts the contacts of the person in need of help but also sends their location.

Description:-

It's a safety device which (although is not specific to any domain), can be attached to various other devices and programmed accordingly for the purpose of protection of the user. 
It can be attached to the helmet of a biker or to the body of a car. 

Working:- 

Whenever our device senses any sudden change in acceleration or heavy jerk on the object on which it is attached then immediately a buzzer activates and an SOS call is made to the emergency contact. In order to provide immediate help, the location of the accident is also messaged. If due to any reason a false SOS is sent then the user can cancel the SOS by deactivating the buzzer.

->ADXL335 Accelerometer detects the strike and sends the information to Arduino Nano.

->Arduino Nano checks the magnitude of the strike, if required then rings the buzzer

->If the buzzer gets activated then GPS NEO detects the location and a contact of the victim is alerted through sms and call using GSM800L.

->In case of false information, kill switch can cancel the process.



Future developments :-

We plan on reducing the weight of the product and making it more versatile, compact, portable and cheap so that even lower middle class people can afford it.

Components Used:-

ADXL335 Accelerometer,
Arduino nano with USB Cable,
GPS NEO,
SIM800L,
Buzzer,
Led, Resistors,
Jumper wires, header pins,
Kill Switch & 
3.7 V Cell.



