# PokerBot

The purpose of this project is to get familiar with C++ and mathematical simulation approaches by building a playable poker engine with monte-carlo simulation features.

*Disclaimer*: While I tried to do the entirety of this project by hand, there were certain times where I used AI either to help debug, write the body of a function (marked in the code), or even help structure this README. With that being said, the architecture and implementation are 95% my own, with some online research mixed in (see the side-pot note in `main.cpp`). 

## Features

- Playable Texas Hold'em in the terminal against a computer opponent
- Hand evaluation and pot dispersal, including side pots
- A bot that uses Monte Carlo simulation to estimate its odds of winning
- Optional display of the bot's internals (its hand and estimated odds) for debugging

## Prerequisites

- A C++ compiler: g++ 13 or newer
  - **Windows:** install MinGW-w64 (e.g. via [MSYS2](https://www.msys2.org/): `pacman -S mingw-w64-ucrt-x86_64-gcc`) and add its `bin` folder to your PATH.
  - **macOS:** `xcode-select --install` (or `brew install gcc`)
  - **Linux:** `sudo apt install g++` (or your distro's equivalent)
- Git
- Optional: VS Code with the C/C++ extension, plus `gdb` for debugging

Check your compiler with `g++ --version`.

## Installation

```
git clone <repo-url>
cd PokerBot
```

## Building and running

**From the command line:**

```
g++ -g src/*.cpp -o bin/main.exe
./bin/main.exe
```

On macOS/Linux you can drop the `.exe` suffix.

**From VS Code:** open the folder, press `Ctrl+Shift+B`, and pick **"C/C++: g++.exe for main.cpp"**. To debug, go to Run and Debug and pick **"C/C++: g++.exe build and debug main.cpp"**. Make sure you choose the **g++** option, not gcc (see [Common Issues](#common-issues)).

## Usage

The game starts with you and one bot, each with a $100 stack and blinds of $0.50 / $1. Players and stacks are set in `src/main.cpp`.

On your turn, the game shows your stack, your hand, and the minimum bet. Then it lists the actions you can take:

- Type the number of the action you want and press Enter.
- If you choose to bet or raise, you'll be asked for an amount. It must be at least the minimum bet and no more than your stack.

To see what the bot is thinking, construct it with `show_internals` set to `true` in `src/main.cpp`:

```cpp
Bot p2(100, "James Dean", true);
```

## Running tests

Each file in `tests/` is its own program. To build and run one (swap in `test_HandEvaluator`, `test_PotDispersal`, or `test_PredictionEngine`):

```
g++ -g src/Action.cpp src/CardHandler.cpp src/ComputerPlayer.cpp src/DeckHandler.cpp src/HandEvaluator.cpp src/HumanPlayer.cpp src/InputHandler.cpp src/Poker.cpp src/PotDispersal.cpp src/PredictionEngine.cpp tests/test_PredictionEngine.cpp -o bin/test_PredictionEngine.exe
./bin/test_PredictionEngine.exe
```

**From VS Code:** run **Terminal → Run Task → "run all tests"** to build and run every test file in order (it's also the default *Run Test Task*). Each file also has its own **"run test_…"** task, and Run and Debug has a **"build and debug test_…"** configuration for each one, plus **"Build and debug all tests"** to debug all of them at once.

## Project structure

```
include/   Header files
src/       Game engine, players, hand evaluation, and Monte Carlo prediction
tests/     Test files
docs/      Specs, PRDs, and design diagrams
bin/       Build output (executables are git-ignored)
```

# Future Designs

The Monte Carlo simulations are running very slow. A future iteration of this project could implement a LUT (lookup table) based approach, along with bitmasks and less bulky operations. But, for the scope of this project, what we have has been educational enough.

# Common Issues

Here are some issues that I ran into a lot during the development process. Hopefully the answers to these questions bring you peace in the development process.

## Accidently clicking the wrong debugger option

Sometimes I would click the gcc.exe debugger, instead of the g++.exe option. In that case, it's because VS Code has set the wrong option in tasks.json as the default.

## VS-Code debugger not working.

When you click the debug icon in the upper-right hand corner, you may find that the console unexpectedly quits. In my case, this was because I accidently chose gcc.exe instead of g++.exe. gcc is a compiler driver the defaults to C, while g++ defaults to compiling for C++.
