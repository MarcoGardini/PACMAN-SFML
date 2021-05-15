# PACMAN-SFML
 
Pacman Classic, remade in C++.
Here you can find both the source code and a zip file, containing the executable and all necessary files to run the game.
This project helps understanding the ghost behaviour of the original Pacman, which is carefully simulated and can be followed by activating the colored targets with the T key.

Take a look at https://www.gamasutra.com/view/feature/132330/the_pacman_dossier.php to finds a detailed report on the inner workings of Pacman.

## DEPENDANCIES

SFML is a 2D-graphics/multimedia library written in C++. 
You can find it and download it at

https://www.sfml-dev.org/

As PACMAN doesn't need crazy performance to work properly, I've kept the VS project configuration as Debug: as such you need to link SFML's Debug libraries. 

## CONTROLS

WASD/Directional Arrow - Move Pacman  
M - Mute all sounds  
N - Unmute all sounds  
T - Show Ghosts Targets  
G - Show Tile Grid  


## License

Copyright 2021 Marco Gardini

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

