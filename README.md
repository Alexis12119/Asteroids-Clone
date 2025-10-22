# 🚀 Geodroid

A complete classic Asteroids game built with Raylib in C, featuring smooth vector-based gameplay, collision detection, comprehensive menu system, and persistent leaderboards.

## 🎮 Features

- **Classic Gameplay**: Rotate, thrust, and shoot to destroy asteroids
- **Progressive Difficulty**: Asteroids split into smaller, faster pieces
- **Strategic Difficulty System**: Bullet-limited gameplay with asteroid speed scaling
  - **Easy Mode**: 15 bullets, slower asteroids (30-80 speed)
  - **Normal Mode**: 10 bullets, standard asteroids (50-150 speed)
  - **Hard Mode**: 5 bullets, blazing-fast asteroids (100-200 speed)
- **Resource Management**: Bullets decrement on fire, increment on hits (up to difficulty limit)
- **Game Over Conditions**: No lives remaining OR zero bullets remaining
- **Smooth Physics**: Vector-based movement with realistic momentum and velocity capping
- **Score System**: Points for destroying asteroids (Large: 20, Medium: 50, Small: 100)
- **Lives System**: 3 lives with respawn mechanics and safe positioning
- **Screen Wrapping**: Seamless edge-to-edge gameplay
- **Complete Menu System**: Main menu, settings, leaderboard, and pause functionality
- **Persistent Leaderboards**: Top 10 high scores with names and dates
- **Audio System**: Retro sound effects for shooting and explosions
- **Settings Persistence**: Sound, difficulty, and volume settings saved to disk
- **Pause System**: ESC to pause/resume during gameplay
- **Name Entry**: Enter your name for high score records

## 🎯 Controls

### Main Game

- **LEFT/RIGHT Arrow**: Rotate ship
- **UP Arrow**: Thrust forward
- **SPACE**: Shoot bullets
- **ESC**: Pause game

### Menus

- **UP/DOWN Arrows**: Navigate menu items
- **ENTER**: Select menu item (including EXIT to quit game)
- **Mouse Click**: Click BACK buttons to navigate
- **ESC**: Pause game (during gameplay only)

### Settings

- **S**: Toggle sound on/off
- **D**: Cycle difficulty (Easy/Normal/Hard)
- **LEFT/RIGHT Arrows**: Adjust volume

## 🎵 Audio Assets

The game includes retro sound effects for an authentic arcade experience:

- **Laser Sound**: Classic pew-pew shooting effect
- **Explosion Sound**: Satisfying asteroid destruction audio
- **Audio Toggle**: Enable/disable sounds in settings menu
- **Volume Control**: Adjustable master volume

**Asset Credits:**
- Sound Effects and Music: [OpenGameArt.org](https://opengameart.org/)
  - explosion.wav
  - laser.wav
  - spaced.wav
  - Icy Game Over.mp3

## 🎯 Difficulty System

The game features three difficulty levels that affect both bullet economy and asteroid behavior:

### Easy Mode

- **15 Bullets**: Most forgiving resource management
- **Asteroid Speed**: 30-80 units (slower, more time to react)
- **Best for**: Learning the game or casual play

### Normal Mode

- **10 Bullets**: Balanced challenge
- **Asteroid Speed**: 50-150 units (original game feel)
- **Best for**: Standard gameplay experience

### Hard Mode

- **5 Bullets**: Intense strategic gameplay
- **Asteroid Speed**: 100-200 units (very fast, requires precision)
- **Best for**: Expert players seeking maximum challenge

### Bullet Mechanics

- **Firing**: Each shot consumes 1 bullet
- **Replenishment**: Destroying asteroids gives you bullets back (up to your difficulty limit)
- **Game Over**: When bullets reach zero, the game ends (in addition to losing all lives)
- **Display**: Current bullet count shown in-game as "Bullets: X"

This system adds strategic depth - you must aim carefully and manage your limited ammunition while facing increasingly aggressive asteroids!

## 🛠️ Building & Running

### Prerequisites

- **Raylib**: Install via your package manager
  - Arch Linux: `pacman -S raylib`
  - Ubuntu/Debian: `sudo apt install libraylib-dev`
  - macOS: `brew install raylib`
  - Windows: Download from [raylib.com](https://www.raylib.com/)

### Build Instructions

```bash
# Clone or navigate to project directory
cd Geodroid

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake -S .. -B .

# Build the game
cmake --build .

# Run the game
./Geodroid
```

### Alternative Quick Build
```bash
cmake -B build -S . && cmake --build build && ./build/Geodroid
```

## 📁 Project Structure

```
Geodroid/
├── src/
│   ├── main.c          # Game loop and state management
│   ├── player.c/.h     # Player ship mechanics
│   ├── asteroid.c/.h   # Asteroid spawning and behavior
│   ├── bullet.c/.h     # Bullet system
│   ├── menu.c/.h       # Menu system and UI
│   ├── leaderboard.c/.h # High score management
│   ├── settings.c/.h   # Settings persistence
│   └── constants.h     # Game constants
├── data/               # Save files directory
│   ├── scores.dat      # High scores data
│   └── settings.dat    # User settings
├── assets/             # Sound effects and graphics
│   ├── laser.wav       # Shooting sound effect
│   ├── explosion.wav   # Asteroid destruction sound
│   ├── *.png           # Asteroid sprites
│   └── license.txt     # Asset licensing information
├── CMakeLists.txt      # Build configuration
└── README.md          # This file
```

## 🎨 Game Mechanics

### Ship Physics

- **Rotation**: Smooth 360° rotation
- **Thrust**: Acceleration-based movement with momentum
- **Friction**: Natural deceleration when not thrusting
- **Velocity Cap**: Maximum speed prevents infinite acceleration
- **Screen Wrap**: Instant teleportation across edges

### Asteroid System

- **Sizes**: Large (40px), Medium (25px), Small (15px)
- **Splitting**: Large → 2 Medium, Medium → 2 Small with random offsets
- **Movement**: Random velocity and rotation
- **Safe Spawning**: Minimum distance from player start position

### Scoring & Progression

- Strategic gameplay rewards destroying asteroids
- Smaller pieces worth more points
- High scores persist between game sessions
- Name entry for leaderboard records

### Bullet System

- **Limited Ammunition**: Difficulty determines starting bullets (5-15)
- **Consumption**: Each shot reduces bullet count by 1
- **Replenishment**: Successful asteroid hits restore bullets (capped at difficulty limit)
- **Critical Resource**: Running out of bullets causes game over
- **Strategic Element**: Encourages precise shooting and tactical positioning

## 🎮 Menu System

### Main Menu

- **Play Game**: Start new game session
- **Leaderboard**: View top 10 high scores
- **Settings**: Adjust game options
- **Exit**: Select with arrows, press ENTER to quit the game

### Pause Menu (ESC during gameplay)

- **Resume**: Continue current game
- **Restart**: Start new game
- **Main Menu**: Return to main menu
- **Settings**: Adjust options
- **BACK Button**: Click to return to main menu

### Settings Menu

- **Sound Toggle**: Enable/disable audio (S key)
- **Difficulty**: Cycle through Easy/Normal/Hard modes (D key)
  - Easy: 15 bullets, slower asteroids
  - Normal: 10 bullets, standard speed
  - Hard: 5 bullets, fast asteroids
- **Volume**: Adjust master volume (Left/Right arrows)
- **BACK Button**: Click to return to main menu

### Leaderboard

- **Top 10 Scores**: Persistent high score table
- **Player Names**: Custom names for records
- **Dates**: When scores were achieved
- **Automatic Sorting**: Highest scores first
- **BACK Button**: Click to return to main menu

## 🐛 Known Issues & Edge Cases Handled

- ✅ No negative lives display
- ✅ No negative bullet count
- ✅ Bullet count capping at difficulty limits
- ✅ Safe asteroid spawning (away from player)
- ✅ Velocity capping prevents infinite acceleration
- ✅ Proper collision detection prevents multiple hits
- ✅ Score overflow protection (long long type)
- ✅ Floating-point precision in bullet lifetime
- ✅ Menu navigation prevents invalid states
- ✅ File I/O error handling for save data
- ✅ Input debouncing prevents accidental actions
- ✅ Difficulty persistence across game sessions

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 🙏 Acknowledgments

- Built with [Raylib](https://www.raylib.com/) - A simple and easy-to-use library to enjoy videogames programming
- Inspired by the classic Atari Asteroids game
- **Assets**: Sound effects and music from [OpenGameArt.org](https://opengameart.org/)
- Thanks to the Raylib community for excellent documentation and examples
- Special thanks to the open-source community for providing free game assets

---

**Enjoy the complete Geodroid experience!** 🎮🚀

_This game includes a full menu system, persistent leaderboards, and professional polish - everything you'd expect from a commercial game release._
