# Pengine - C++ 2D Game Engine

## Overview

### Pengine

### Minigin
This projected started with Minigin, provided by my Programming 4 teacher.

Minigin is a very small project using [SDL2](https://www.libsdl.org/) and [glm](https://github.com/g-truc/glm) for 2D c++ game projects. It is in no way a game engine, only a barebone start project where everything sdl related has been set up. It contains glm for vector math, to aleviate the need to write custom vector and matrix classes.

[![Build Status](https://github.com/avadae/minigin/actions/workflows/msbuild.yml/badge.svg)](https://github.com/avadae/msbuild/actions)
[![GitHub Release](https://img.shields.io/github/v/release/avadae/minigin?logo=github&sort=semver)](https://github.com/avadae/minigin/releases/latest)

## Table of Contents

## Features

### ECS
The Entity Component System (ECS) is a design pattern that allows for a flexible and modular approach to building game objects. It decouples data (components) from behaviour (systems), enabling efficient data management and processing. Additionally it provides great cache locality which can make the program a lot faster.

P

### Event Queue
The EvenManager manages game events, allowing for decoupled communication between different parts of the game. Events can be dispatched and handled asynchronously. 

### Serialization / Deserialization

### ImGUI Debugging

## References
