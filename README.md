# 🍪 CHIP ATE
<img src="demo/splash_stm32.jpg" width="80%"> 

## 🤔 What?
CHIP ATE is a CHIP-8 Emulator implemented on both PC using SDL2 and STM32 with a custom PCB!

## 🤨 Why?
This was supposed to be my first step into console emulation, but then it became that for PCB design as well!

## Features
- **Portable Emulator Core**:  
  Implements the timers, memory, and CPU as per the [official CHIP-8 spec](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM), including all 34 instructions

- **Platform Abstraction Layer**:  
  Able to run on either PC or STM32 using a platform-agnostic API layer
  
- **Custom PCB**:  
  Assembled on a STM32-based board with an OLED screen, keypad, a buzzer, and power regulation
  
- **ROM Menu**:  
  Provides seamless loading in and out of the ROM catalog

## Diagrams

### System Architecture
<img src="demo/chip8_emulator.drawio.png" width="60%">

### Schematic
<img src="demo/schematic.png" width="60%">

### Routing and 3D Renders
<img src="demo/routing.png" width="20%"> <img src="demo/pcb_front.png" width="20%"> <img src="demo/pcb_back.png" width="20%">

## 📸 STM32 Demos

### Demo Video (click it!)
<a href="https://youtu.be/Bx0_Uua0Y5I">
  <img src="demo/space_invaders_stm32.jpg" alt="STM32 Demo" width="400"/>
</a>

### Tetris
<img src="demo/tetris_stm32.jpg" width="40%">

### Tic-Tac-Toe
<img src="demo/tic_tac_toe_stm32.jpg" width="40%">

### Pong
<img src="demo/pong_stm32.jpg" width="40%">

## 📸 PC Demos

### Demo Video (click it!)
<a href="https://youtu.be/zPvpcgly5Rw">
  <img src="demo/space_invaders_pc.png" alt="PC Demo" width="400"/>
</a>

### Startup Screen
<img src="demo/splash.png" width="40%">

### Menu
<img src="demo/menu.png" width="40%">

### Opcode Test
<img src="demo/opcodes.png" width="40%">

## Resources
- [Guide to Making a CHIP-8 Emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/) by Tobias V. Langhoff
- [Building a CHIP-8 Emulator in C++](https://austinmorlan.com/posts/chip8_emulator/) by Austin Morlan
- [chip8stm32 Repo](https://github.com/AlfonsoJLuna/chip8stm32) by AlfonsoJLuna
- [KiCad Tutorial Playlist](https://youtube.com/playlist?list=PL3bNyZYHcRSUhUXUt51W6nKvxx2ORvUQB&si=zccgU5XXFSJnHfGM) by Shawn Hymel
- [KiCad 7 PCB Layout in 5 Steps](https://youtu.be/3FGNw28xBr0?si=mEvIMN-HYlVow5y7) by The Design Graveyard
