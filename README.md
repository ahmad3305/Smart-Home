# Smart-Home-Automation

This is a project for managing smart homes and devices. It's kind of like making your own backend for a smart home app, but in C++.  
You can handle users, homes, rooms, all sorts of devices (like lights, fans, locks, etc.), keep track of suppliers and inventory, and even see who uses how much energy.

---

## What does it do?

- **Accounts and Users:**  
  There are three types of accounts: Admin, User, and Guest. Each has different things they can do. Admins can manage more stuff like the workers, suppliers etc, Users are normal home owners, Guests are usually just visiting and have limited access.

- **Homes, Rooms, and Devices:**  
  A User can have one or more homes. Each home has rooms, and rooms have devices in them.  
  Devices are things like lights, fans, AC, heaters, WiFi routers, door locks, and window locks. (Currently, my company have only these :sweat_smile:)  
  Each device knows what type it is (like, a Light or a Fan) and you can make them do their own thing, for example, `adjustFeature()` is a function every device can have, but each device can adjust itself in its own way.
  
- **Energy Usage:**  
  The system also records how much energy each device uses, so you can check usage for a device, a room, or a whole home.

- **Supplier and Inventory:**  
  There’s a list of suppliers that provide device types. Suppliers keep an inventory, and inventories are stored in warehouses.  
  The system keeps track of which supplier sells which device type, how much its stock, warranty, and so on. (There is many-to-many between device types and suppliers and also some additional relational info, so I made association class between them)

- **Workers:**  
  There are workers, and each worker can be skilled with different device types. So, for example, you can know which worker to call if something breaks or needs installing. (There is many-to-many between device types and workers and also some additional relational info, so I made association class between them).

## Classes in the Project

Here’s basically what you’ll find in the code:

- **Account, Admin, User, Guest**: Account types and permissions.
- **Home, Room**: Places where devices live.
- **Device, Light, Fan, AC, Heater, Wifi, DoorLock, WindowLock, DeviceType**: All the devices and their types.  
  Devices inherit from a base Device class and each specialized device can have its own logic.
- **Supplier, Inventory, Warehouse**: For keeping track of who sells what, where things are, and how much is left.
- **Worker, WorkerDeviceType**: Workers and what device types they can handle.
- **DeviceTypeSupplier**: Keeps track of which supplier supplies which device type, with info like price, stock, warranty, etc.
- **EnergyUsage**: Records for tracking how much energy something is using.

---

## How are things connected?

- **A User owns Homes.** A Home can’t exist without a User.
- **Homes have Rooms.** Rooms can’t exist outside a Home.
- **Rooms have Devices.** Installed Devices can’t exist outside a Room.
- **Devices have a DeviceType.** DeviceType can exist on its own and is shared, so it’s not like the device “owns” it.
- **Suppliers supply DeviceTypes (through DeviceTypeSupplier).** One supplier can supply many device types, and one device type can be supplied by many suppliers.
- **Inventory belongs to a Supplier and is stored in a Warehouse.**
- **Workers can work with many DeviceTypes, and each DeviceType can be handled by many Workers.**
- **EnergyUsage is linked to Devices and Users.**

---
![OOP (4)](https://github.com/user-attachments/assets/2cd81cb0-bebb-4da0-b9b6-335efb2f49e6)


![flow chart](https://github.com/user-attachments/assets/dce77cde-bd21-4552-9a3b-60eb8c70b2e9)


