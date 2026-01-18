# Linux Sensor Character Driver

## Overview
This project demonstrates a **Linux kernel character driver** with progressive features implemented day-wise:

- **Day 11**: Blocking read + poll/select interface
- **Day 12**: IOCTL interface for configuration
- **Day 13**: Async notification using SIGIO
- **Day 16**: Workqueue-based bottom-half driver
- **Day 17**: mmap / zero-copy driver
- **Day 18**: Sysfs interface for interval configuration
- **Upgrade-1**: Final integrated driver combining all features

## Features
- Real-time sensor value simulation
- Blocking & non-blocking reads
- Poll/select support for efficient IO
- Async SIGIO notifications
- mmap zero-copy interface
- Sysfs interface for interval control
- Workqueue based bottom-half processing

## How to Build
```bash
make          # Build all modules
sudo insmod upgrade-1/sensor_char.ko
sudo chmod 666 /dev/sensor_char

## How to Run User Programs
# Poll/select test
./user_app/user_poll.out

# Async SIGIO test
./user_app/user_async.out

# mmap zero-copy test
./user_app/user_mmap.out

# Blocking read test
./user_app/user_char.out

