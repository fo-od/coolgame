![coolgame logo](coolgame.svg)

> yeah this game is pretty cool  
> -_food_
***

so basically this game is a 2d platformer (very original) made with SDL in C++

# gameplay

the goal of the game isnt final right now, but its basically going to be moving through levels,
trying to get to some kind of win zone. i want it to be fast paced but feel smooth to control.

the entire idea of this game came from an unfinished scratch (scratch.mit.edu) platformer i made some time ago,
and ive had the idea stuck in my head for years.

# visuals

the game is going to be mainly black and white, with some color to make special things more visible when going fast.
the platforms and player arent going to be fancy, theyll just be rectangles/squares.
i will try my best to make it feel smooth though.

***

i dont plan on properly releasing this game, since this is a personal project to learn c++.
but if i end up liking it, ill try to put it on itch or something.

***

### building

ive only built this on linux & macOS (26) and i dont care enough to set up a vm to build it on windows.
but if there any issues for building on windows, feel free to make a pr.

required libraries (need to have installed on your system):

```
SDL3
SDL3_ttf
```

i use cmake for building, so youll also need to have that installed

and then you can just do

```bash
git clone https://github.com/fo-od/coolgame.git
cd coolgame
cmake -S . -B build
cmake --build build
```

and the executable will be somewhere in `build/` along with assets