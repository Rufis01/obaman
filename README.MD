# Obaman
## What is it?
Obaman is an experimental PS1 game and engine.

The main goal is to develop a basic, yet general and portable game engine. The game is a pacman clone and should provide a general idea of how to use this engine and should serve as a way to test it.

As of now there's **no working game**, only a couple of debug functionalities have been implemented.

## Why did I choose the PS1?
There are 3 main reasons:
1. I'm a former PSVita developer. I wanted a new target and the PS1 seemed like a good choice: the PS3 is hard to develop for, and I wasn't really interested in the PS2; The PSP is probably quite similar to the PSV and it's dead, so there's really no point; I don't own a PS4 or a PS5; Furthermore I had already attempted a similar project which targeted the PC (Windows + Linux) via OpenGL and it utterly failed, the code was a complete mess (even after some refactoring), which brings me to point 2.
2. It's a rather basic console (compared to modern standards) and that helps to keep me focused on the developement of the engine, without getting distracted by shaders, lights and all that modern grapical stuff. Plus it forces me to think how to make the project portable, since I'm constantly reminded that I'm not working with a modern machine with cross-platform libraries and such. If I want to make it portable I must come up with a good design. _No bodging allowed!_
3. It's a unique system, with unique challenges and quirks which not only make it an invaluable source of knowledge, but also keep my motivation and interest high.

## Building
To build obaman you'll need [psn00bsdk](https://github.com/Lameguy64/PSn00bSDK).
Then run:
```bash
   cmake --preset default .
   cmake --build ./build
   ```
This will create a runnable disc image in `./iso`.