# 🐦 Flappy Bird Clone (C++ & SFML)

A **Flappy Bird-style game** developed in **C++** using **SFML (Simple and Fast Multimedia Library)**.  
The project demonstrates **Object-Oriented Programming (OOP)** concepts such as **inheritance, polymorphism, composition, and association**.

---

## 📌 Overview
This project is a remake of the classic **Flappy Bird game**, where the player controls a bird that must fly between pipes without colliding.  
The game keeps track of the score, handles collisions, and allows restarting after game over.

---

## 🚀 Features
- 🎮 **Bird mechanics**: Gravity, jumping (flapping), and collision detection  
- 🏗️ **Pipes**: Randomly generated gaps, moving left with increasing difficulty  
- 📊 **Score system**: Increases when the player successfully passes a pipe  
- 💀 **Game Over screen** with restart option (press Space)  
- 🖼️ Built with **SFML Graphics, Events, and Text Rendering**  
- 🧑‍💻 Demonstrates **OOP concepts**:  
  - Inheritance & Run-time Polymorphism (virtual functions)  
  - Composition (Bird & Pipe inside the game class)  
  - Association (Bird drawing on the window)  

---

## 🛠️ Tech Stack
- **Language**: C++  
- **Library**: [SFML](https://www.sfml-dev.org/) (Graphics, Events, Input)  
- **OOP Concepts**: Inheritance, Polymorphism, Composition, Association  

---

## 📂 Project Structure
- `FlappyBirdGame` – Base class handling window, score, events  
- `Bird` – Handles player mechanics (gravity, jump, collision bounds)  
- `Pipe` – Represents obstacles with top and bottom parts  
- `MyFlappyGame` – Derived class, game loop with pipes, collisions, scoring  
- `main.cpp` – Entry point  

---

