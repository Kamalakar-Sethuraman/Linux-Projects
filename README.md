# Linux Sensor Character Driver

**Project Type:** Linux Kernel Module / Character Device Driver  
**Kernel Version Tested:** 6.x  
**Author:** KAMAL  
**Location:** Chennai, India  

## Overview
This project demonstrates a Linux character device driver simulating a hardware sensor with advanced kernel features such as:

- Blocking and non-blocking I/O using wait queues and `poll/select`.
- Asynchronous notification to user-space applications using `SIGIO`.
- Timer-based periodic data generation.
- IOCTL and sysfs interfaces for runtime configuration.
- Zero-copy communication between kernel and user-space using `mmap`.

It is designed as a learning and professional project to showcase kernel programming, synchronization, and device communication techniques, suitable for embedded Linux roles in Tier-1 companies.

## Features
- Incrementing sensor values periodically via kernel timer.
- User-space applications can read sensor data in blocking/non-blocking mode.
- Supports asynchronous notification (`SIGIO`) and polling.
- Configurable parameters through IOCTL or sysfs interface.
- Zero-copy memory sharing via `mmap`.
- Fully synchronized using spinlocks and wait queues.

## Directory Structure
# Linux Sensor Character Driver

**Project Type:** Linux Kernel Module / Character Device Driver  
**Kernel Version Tested:** 6.x  
**Author:** KAMAL  
**Location:** Chennai, India  

## Overview
This project demonstrates a Linux character device driver simulating a hardware sensor with advanced kernel features such as:

- Blocking and non-blocking I/O using wait queues and `poll/select`.
- Asynchronous notification to user-space applications using `SIGIO`.
- Timer-based periodic data generation.
- IOCTL and sysfs interfaces for runtime configuration.
- Zero-copy communication between kernel and user-space using `mmap`.

## Features
- Incrementing sensor values periodically via kernel timer.
- User-space applications can read sensor data in blocking/non-blocking mode.
- Supports asynchronous notification (`SIGIO`) and polling.
- Configurable parameters through IOCTL or sysfs interface.
- Zero-copy memory sharing via `mmap`.
- Fully synchronized using spinlocks and wait queues.

## Directory Structure
# Linux Sensor Character Driver

**Project Type:** Linux Kernel Module / Character Device Driver  
**Kernel Version Tested:** 6.x  
**Author:** KAMAL  
**Location:** Chennai, India  

## Overview
This project demonstrates a Linux character device driver simulating a hardware sensor with advanced kernel features such as:

- Blocking and non-blocking I/O using wait queues and `poll/select`.
- Asynchronous notification to user-space applications using `SIGIO`.
- Timer-based periodic data generation.
- IOCTL and sysfs interfaces for runtime configuration.
- Zero-copy communication between kernel and user-space using `mmap`.

It is designed as a learning and professional project to showcase kernel programming, synchronization, and device communication techniques, suitable for embedded Linux roles in Tier-1 companies.

## Features
- Incrementing sensor values periodically via kernel timer.
- User-space applications can read sensor data in blocking/non-blocking mode.
- Supports asynchronous notification (`SIGIO`) and polling.
- Configurable parameters through IOCTL or sysfs interface.
- Zero-copy memory sharing via `mmap`.
- Fully synchronized using spinlocks and wait queues.

## Directory Structure
linux-sensor-character-driver/
# │
# ├── day_11/ # Blocking read + poll driver
# ├── day_12/ # Added IOCTL interface
# ├── day_13/ # Async notification (SIGIO)
# ├── day_16/ # Workqueue-based driver
# ├── day_17/ # mmap / zero-copy driver
# ├── day_18/ # Sysfs interface for interval configuration
# ├── upgrade-1/ # Project upgrade: advanced features, IOCTL + mmap
# ├── user_app/ # Sample user-space applications
# │ ├── user_poll.c
# │ ├── user_async.c
# │ └── user_mmap.c
# ├── Makefile
# └── README.md
