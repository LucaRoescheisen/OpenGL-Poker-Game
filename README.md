# C Poker Game with OpenGL

**Language:** C  
**Libraries:** OpenGL (Core Profile), GLAD, GLFW, FreeType  
**Tools:** CLion, CMake

---

## 1. Project Overview
This project is a high-performance graphical Poker engine (Texas Hold'em) implemented in C. This project utilizes a custom **OpenGL** pipeline to render a dynamic UI, handle texture mapping for card assets, and manage a real-time game loop.

---

## 2. Key Features

### Graphics & UI System
* **Custom Rendering Pipeline:** Built using Vertex Array Objects (**VAOs**) and Vertex Buffer Objects (**VBOs**) for efficient geometry rendering.
* **Text Rendering:** Integrated the **FreeType** library to generate glyph bitmaps, rendered via custom GLSL shaders.
* **Interactive UI:** Implemented a mouse-driven interface with collision detection for "Fold," "Call," and "Raise" buttons.
* **Texture Mapping:** Custom texture loader to map card face assets onto 2D quads.

### Game Logic & Engine
* **Scalable Table Logic:** Supports up to 8 players with individual state tracking (Active, Folded, All-in).
* **Hand Evaluation Engine:** A robust algorithm that evaluates the best 5-card combination from the player's hole cards and the community cards.
* **Kicker Resolution:** Automatically calculates "kickers" to break ties when players hold identical hand ranks (e.g., Two Pair vs. Two Pair).
* **Betting State Machine:** Handles the transition between rounds (Pre-flop, Flop, Turn, River) and manages pot distribution.

---

## 3. System Architecture
The software follows a modular design to separate low-level rendering from high-level game rules.

* **`GameState.c`**: Manages the game state and controls the flow of the application
* **`UserInterface.c`**: Manages rendering for buttons, mouse hovering, and UI layout.
* **`HandSolver.c` and `Prediction.c`**: The core rules engine, including card shuffling and hand ranking.
* **`textTexture.c`**: Handles the FreeType interface and glyph-to-screen mapping.

---
