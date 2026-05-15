# 🏰 Magical Pets Kingdom

A 2D action RPG built with **C++ and SFML**, where players collect, train, and battle magical pets in an enchanted kingdom. Developed as an Object-Oriented Programming semester project.

![C++](https://img.shields.io/badge/C++-17-blue?logo=cplusplus)
![SFML](https://img.shields.io/badge/SFML-2.5.1-green)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)
![License](https://img.shields.io/badge/License-MIT-yellow)

## 📸 Screenshots

<table align="center">
<tr>
<td align="center">
<b>Main Menu</b><br><br>
<img src="https://github.com/user-attachments/assets/42650fc2-0e6d-4603-80f3-c62328a1babc" width="400"/>
</td>

<td align="center">
<b>Arena Mode Selection</b><br><br>
<img src="https://github.com/user-attachments/assets/a8cf60be-6826-4e56-9422-78214ef0c81c" width="400"/>
</td>
</tr>

<tr>
<td align="center">
<b>Pet Selection</b><br><br>
<img src="https://github.com/user-attachments/assets/bb1b3ca9-eb4c-45f1-8f75-3ea249508be6" width="400"/>
</td>

<td align="center">
<b>Gameplay</b><br><br>
<img src="https://github.com/user-attachments/assets/d7fbc1fa-ad1a-426f-8d96-5416a5b185d7" width="400"/>
</td>
</tr>

<tr>
<td align="center">
<b>Player Input</b><br><br>
<img src="https://github.com/user-attachments/assets/14f3e09b-bf7c-4c2d-84be-4cfee3a4623c" width="400"/>
</td>
</tr>
</table>
## 🎮 Features

### 🐉 4 Unique Magical Pets
| Pet | HP | ATK | SPD | Special Abilities |
|-----|-----|-----|-----|-------------------|
| **Dragon** | 200 | 20 | 1.0 | Fire Breath, Meteor Storm |
| **Phoenix** | 150 | 15 | 0.6 | Healing Flame, Rebirth |
| **Unicorn** | 100 | 15 | 1.5 | Magic Shield, Horn Strike |
| **Griffin** | 130 | 18 | 1.3 | Sky Dive, Talon Storm |

### ⚔️ Arena Battle System
- **1v1 Mode** — Real-time combat with projectiles, abilities, and movement
- **2v2 Mode** — Team battles with dual pet controls
- **Item Usage** — Use potions and boosts mid-battle (keys 1-5)
- **Dynamic AI** — Enemy pets chase, dodge, and use abilities strategically

### 🏋️ Training Camps
- 4 training courses: Obstacle Course (HP), Combat Drill (ATK), Agility Run (SPD), Shield Practice (DEF)
- Pets earn **EXP** and **level up** through training
- Trained stats **persist across battles**

### 🛒 Shop System
- 5 purchasable items: Healing Potion, Mana Potion, Speed Boost, Attack Boost, Defense Shield
- Dynamic restocking with random inventory
- Coins earned through battle victories (50 for 1v1, 75 for 2v2)

### 🏛️ Guild & Team Management
- Create and manage your guild with up to 4 pets
- Assign combat roles: **Tank** (+20% HP), **Healer** (+10 HP), **Damage Dealer** (+15% ATK)
- Role bonuses automatically apply in battle

### 🏆 Dynamic Leaderboard
- Win/loss tracking with file persistence
- Ranked display with gold/silver/bronze tiers
- Win percentage calculation

### 💾 Save/Load System
- Auto-save after battles, training, and purchases
- Saves: player profile, coins, inventory, pet training stats, guild data
- Load progress from the main menu

## 🛠️ Tech Stack
- **Language:** C++17
- **Graphics/Audio:** SFML 2.5.1
- **IDE:** Visual Studio 2022
- **Build:** MSBuild (x64 Debug/Release)

## 📁 Project Structure
```
MagicalKingdom/
├── assets/
│   ├── images/        # Pet sprites, backgrounds, UI elements
│   ├── audio/         # Sound effects, background music
│   └── fonts/         # Game fonts
├── Pet.h              # Base Pet class (abstract)
├── Dragon.h           # Dragon subclass
├── Phoenix.h          # Phoenix subclass
├── Unicorn.h          # Unicorn subclass
├── Griffin.h          # Griffin subclass
├── Battle.cpp/h       # Core battle mechanics
├── Battle1v1.h        # 1v1 arena battle
├── Battle2v2.h        # 2v2 team battle
├── ArenaBattle.h      # Arena base class
├── GameManager.h      # Singleton game manager
├── GameManage.cpp     # Main game logic & GUI
├── Item.h/cpp         # Item hierarchy (5 types)
├── Shop.h/cpp         # Shop system
├── Training.h/cpp     # Training camp system
├── Guild.h/cpp        # Guild management
├── LeaderBoard.h/cpp  # Dynamic leaderboard
├── menu.cpp           # Main menu system
└── MagicalKingdom.cpp # Pet implementations & healing
```

## 🎯 OOP Concepts Demonstrated
- **Inheritance & Polymorphism** — Pet hierarchy with virtual abilities
- **Abstraction** — Pure virtual functions in base Pet class
- **Encapsulation** — Private data with getters/setters
- **Singleton Pattern** — GameManager centralized state
- **Dynamic Memory** — Manual new/delete with cleanup in destructors
- **File I/O** — Save/load with text file serialization
- **Exception Handling** — try-catch around all critical operations

## 🚀 Getting Started

### Prerequisites
- Visual Studio 2022 (with C++ Desktop Development)
- SFML 2.5.1 (64-bit, VS 2017 compatible)

### Setup
1. Clone the repository:
   ```bash
   git clone https://github.com/Bilalmughal-07/MagicalPetKingdom.git
   ```
2. Open `MagicalKingdom.sln` in Visual Studio
3. Set SFML paths in Project Properties:
   - **Include Directories:** `<SFML_PATH>/include`
   - **Library Directories:** `<SFML_PATH>/lib`
4. Add SFML `/bin` to system PATH (or copy DLLs to output folder)
5. Set Configuration to **Debug | x64**
6. Set Working Directory to `$(ProjectDir)`
7. Build and run (F5)

### Controls
| Action | 1v1 Controls | 2v2 Player 2 |
|--------|-------------|--------------|
| Move | WASD | Arrow Keys |
| Shoot | Space | L |
| Ability 1 | U | N |
| Ability 2 | I | M |
| Use Items | 1-5 | — |

## 👨‍💻 Author
**Muhammad Bilal** — 24I-3168 — SE-A

## 📄 License
This project is for educational purposes as part of the OOP course curriculum.
