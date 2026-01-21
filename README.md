# C Poker Game with OpenGL

**Language:** C  
**Tools:** CLion, GLAD, GLFW

## 1. Project Overview
This project is a graphical poker game implemented in C using OpenGL, GLFW and GLAD. It provides a fully playable Texas Hold'em-style poker game with betting
rounds, hand evaluation, and a UI system with text rendering.

## Key Features
  -GUI
    - Custom UI elements using rectangles, textures, VAOs and VBOs.
    - Text rendering with Freetype and shaders.
    - Interactive betting buttons (Fold, Call, Raise).
  -Game Logic
    - Handles up to 8 players, including folded and all-in states.
    - Full 5-hand card evaluation.
    - Automatic calculation of kickers for tie-breaking.
    - Proper handling of betting rounds, pots and winners.
  -Inputs & Controls
    - Keyboard and mouse interaction to control betting amounts and player actions.
  -Graphics
    - OpenGL-based rendering of cards and text.
