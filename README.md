# 🐍 Console Snake Game in C++

A feature-rich, console-based implementation of the classic Snake game written in C++. This project utilizes Object-Oriented Programming (OOP) principles and the Windows Console API to create an interactive gaming experience with dynamic difficulty, obstacles, and color-coded graphics.

## 🎮 Features

* **Classic Gameplay:** Smooth movement using Arrow keys or WASD controls.
* **Dynamic Difficulty:** Choose from Beginner, Intermediate, or Advanced speeds.
* **Leveling System:** The game gets harder as you play!
    * Every **5 food items** eaten, the level increases.
    * New obstacles are generated dynamically upon leveling up.
* **Obstacles:** Randomly generated obstacles (`#`) that the snake must avoid.
* **Persistent High Scores:** Scores are saved to a local file (`scores.txt`), allowing you to track your personal best over time.
* **Colorful Graphics:** Uses Windows Console API to render the snake, food, and borders in different colors for better visibility.
* **Pause Functionality:** Need a break? Pause and resume the game at any time.

## 🛠️ Tech Stack

* **Language:** C++
* **Libraries:**
    * `windows.h` (Console manipulation, cursor hiding, colors)
    * `conio.h` (Input handling: `_kbhit`, `_getch`)
    * `vector` (Dynamic array management)
    * `fstream` (File handling)

> **⚠️ Note:** This game is designed specifically for **Microsoft Windows** operating systems due to the use of `<windows.h>` and `<conio.h>`.

## 🕹️ Controls

| Action | Key(s) |
| :--- | :--- |
| **Move Up** | `Arrow Up` or `W` |
| **Move Down** | `Arrow Down` or `S` |
| **Move Left** | `Arrow Left` or `A` |
| **Move Right** | `Arrow Right` or `D` |
| **Pause Game** | `P` |
| **Resume Game** | `R` (When paused) |

## 🚀 How to Run

### Prerequisites
1.  A C++ Compiler (MinGW or TDM-GCC recommended).
2.  A Windows Operating System.

### Installation & Compilation

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/your-username/snake-game-cpp.git](https://github.com/your-username/snake-game-cpp.git)
    cd snake-game-cpp
    ```

2.  **Compile the code:**
    If you are using GCC/MinGW:
    ```bash
    g++ main.cpp -o snake_game.exe
    ```

3.  **Run the game:**
    ```bash
    ./snake_game.exe
    ```

## 🧩 Code Structure

The project is built using a modular class structure:
* **`Board`**: Manages the grid, rendering, boundaries, and obstacle generation.
* **`Food`**: Handles food generation logic, ensuring it doesn't spawn on obstacles or the snake.
* **`Snake`**: Manages the snake's body (using a vector of positions), movement logic, collision detection, and growth.

## 🔮 Future Improvements

* Implement a leaderboard to show top 5 players instead of just the high score.
* Add power-ups (e.g., speed reduction, shorten body).
* Port to Linux/macOS using the `ncurses` library.

## 📄 License

This project is open-source and available under the [MIT License](LICENSE).

---
By Shane Christian
