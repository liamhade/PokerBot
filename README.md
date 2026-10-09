# PokerBot

The purpose of this project is to get familiar with C++ and mathematical simulation approaches by building a playable poker engine with monte-carlo simulation features.

# Future Designs

The Monte Carlo simulations are running very slow. A future iteration of this project could implement a LUT (lookup table) based approach, along with bitmasks and less bulky operations. But, for the scope of this project, what we have has been educational enough.

# Common Issues

Here are some issues that I ran into a lot during the development process. Hopefully the answers to these questions bring you peace in the development process.

## Accidently clicking the wrong debugger option

Sometimes I would click the gcc.exe debugger, instead of the g++.exe option. In that case, it's because VS Code has set the wrong option in tasks.json as the default.

## VS-Code debugger not working.

When you click the debug icon in the upper-right hand corner, you may find that the console unexpectedly quits. In my case, this was because I accidently chose gcc.exe instead of g++.exe. gcc is a compiler driver the defaults to C, while g++ defaults to compiling for C++.