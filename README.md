# A gui app written in C++ to visualize pathfinding algorithms.

## Install

### Dependencies

#### Using APT :

`sudo apt install libgl1-mesa-dev`

#### Building

After opening a terminal in the directory of the project, you can build the the project :\
`make`

Additional options such as **clean**, **rebuild** and **reinstall** are also available in the Makefile.

## Usage

```
Usage: pathfinder [-a|--algorithm] <algorithm_name> [-s|--size] <vertices>

Available algorithms: dijkstra, astar

Vertices: positive integer (default: 1024)
```
