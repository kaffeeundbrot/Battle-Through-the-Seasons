# Season Catcher 🌸☀️🍂❄️

An exciting OpenGL-based game where you catch seasonal items as the seasons change!

## Game Overview

Control a basket at the bottom of the screen to catch falling seasonal items and earn points. Avoid bad items to preserve your lives and progress through all four beautiful seasons!

## How to Play

### Controls
- **Arrow Keys (← →)** - Move your basket left and right
- **Space** - Pause/resume the game (or restart after game over)
- **ESC** - Exit the game

### Objective
Catch the colorful seasonal items to score points while avoiding dark-colored bad items. Each season has unique themed items!

### Scoring System
- **Good Items (Colorful)**: +10 points
- **Bad Items (Dark)**: -1 life
- **Starting Lives**: 3
- **Game Over**: When you run out of lives

## The Four Seasons

The game features four beautiful seasons that automatically change as you earn points. Each season has specific items to catch and avoid:

### 🌸 Spring (0-49 points)
- **Background**: Fresh green
- **Good Items**: Rice/Oats, Milk Bottle, Honey, Apple
- **Bad Items**: Crumpled Paper, Spoiled Fruit, Broken Furniture
- **Theme**: New beginnings and growth

### ☀️ Summer (50-99 points)
- **Background**: Bright blue sky
- **Good Items**: Plastic Bottles, Drink Cans, Candy Wrappers, Flip Flops
- **Bad Items**: Teddy Bear, Flowers, Kitchen Utensil, Jams
- **Theme**: Warmth and beach fun

### 🍂 Autumn (100-149 points)
- **Background**: Warm orange tones
- **Good Items**: Pumpkins, Jam Jars, Corn Cobs, Bread Loaf
- **Bad Items**: Crumpled Paper, Spoiled Fruit, Broken Furniture
- **Theme**: Harvest and transition

### ❄️ Winter (150+ points)
- **Background**: Cool icy blue
- **Good Items**: Hot Cocoa, Cookies, Gift Box, Scarf
- **Bad Items**: Crumpled Paper, Spoiled Fruit, Broken Furniture
- **Theme**: Cozy winter holidays

## Building and Running

### Prerequisites
- C++ compiler (clang++ or g++)
- FreeGLUT library
- OpenGL and GLU libraries
- pkg-config

### Compilation
```bash
make
```

### Running
```bash
make run
```

Or directly:
```bash
./season_catcher
```

### Cleaning Build Files
```bash
make clean
```

## Game Features

✅ Four distinct seasons with unique visual themes  
✅ Specific seasonal items with text labels (Rice/Oats, Milk Bottle, Hot Cocoa, etc.)  
✅ Decorative bear characters (Mama Bear, Papa Bear, Baby Bear) in the background  
✅ Smooth basket controls with arrow keys  
✅ Dynamic falling item system  
✅ Collision detection  
✅ Lives and scoring system  
✅ Pause/resume functionality  
✅ Game over detection and restart  
✅ Visual feedback for score, lives, and current season  

## Tips for High Scores

1. **Stay Centered**: Position yourself in the middle to react to items from both sides
2. **Prioritize Good Items**: Focus on catching colorful items over avoiding bad ones
3. **Watch the Pattern**: Items spawn randomly, but staying alert helps
4. **Use Pause Wisely**: Take a break if the screen gets too crowded
5. **Season Progression**: Each new season is an achievement - try to reach Winter!

## Technical Details

- **Language**: C++
- **Graphics**: OpenGL with FreeGLUT
- **Resolution**: 800x600 pixels
- **Frame Rate**: ~60 FPS
- **Physics**: Simple gravity-based falling mechanics

## Credits

Created as a seasonal arcade game demonstration using OpenGL and C++.

Enjoy catching the seasons! 🎮
