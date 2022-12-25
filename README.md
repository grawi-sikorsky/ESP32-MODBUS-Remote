# MODBUS Panel
This is a project of web panel for any supported solar charging controllers with MODBUS interface (RJ11 or RJ45).
It's able to gather all informations about solar charging system (charging voltage/current, battery states etc) store it in database, and display in web panel.

It have 3 modules:

# 1) ESP32-MODBUS-Remote

  It's written in C++ part of project for embedded modules like ESP32. It have to be connected with solar charger controller via MODBUS interface (2-4 wires).
  It's responsible for gathering all data from solar charger (like charging voltage, battery voltage, charging current, draw current etc). 
  Module is sending all informations via available wifi to backend part of project. Can be configured via web-panel.

# 2) ESP32-MODBUS-Backend [this]

Backend side of control panel written in Java with SpringBoot. Delivers API for Remote ESP32 to gather all informations sended by that module.
Stores data in docker postgreSQL database or H2 db (for developing purposes). 
Additionally it have API for Frontend part of the application to display all gathered informations about solar charging process.

# 3) ESP32-MODBUS-Frontend

Frontend written in TypeScript / Angular, shows all data and timeline.

## BUILD
todo.
