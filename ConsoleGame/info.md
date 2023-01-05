## Projects performed during the Object-oriented programming course.

This course was aimed at studying OOP, programming patterns. During the course, a console game was created. All the code was written by me, no implementation examples were given in the course.

### Playing field
The playing field is a container for objects, the playing field is represented by a rectangular grid
The creation of a field of arbitrary size, control of the maximum number of objects on the field, the ability to add and remove objects on the field, the ability to copy the field is implemented.
### Units
Units is an object placed on the field. One unit is a squad. Implemented 3 types of units:
- Rouge 
- Warrior
- Mage 
Each unit has its own attack radius, health, damage, and two different weapons to choose from.
The "Strategy" pattern is implemented for weapons (WeaponBehavior)
The "Factory" pattern is used to create units


### Neutral objects
Implemented 4 neutral objects (Bird/MrWitcher/Bonfire/Grave), when interacting, they impose various effects on the unit.
 
### Base 

- The base is located on the field
- Can create units
- Keeps records of units
- On the base, you can choose an active unit

### Implemented the "Сommand" pattern (Command.h/cpp) for transmitting commands

### A Chain of responsibility has been created to receive commands from the user
- Handler - Handler
- Base handler - AbstractHandler

### The "Mediator" pattern is implemented. His role is assumed by GameMaster, which implements the gameCycle game cycle, in which certain commands are created and invoked


### Implemented a set of classes for logging actions and program status to a file/terminal
Switching between different logging is implemented using the "Proxy" pattern
of different recording formats using the "Adapter" pattern


### Saving and loading the program state
Implemented loading and saving the state of the program at any time of the game

Saving and loading are implemented using the "Memento" pattern (class Memento)
Correctness control is implemented

### Template classes
The template class of the game Game (singleton pattern) is implemented


### Two rules of the game are implemented
- Playing with bases:
    + For two players: the goal is to destroy the base and all units
    + For one player: destroy all NPCs
- A game without bases: the goal is to destroy all units. Units will spawn every turn if there are less than 3 of them

The transfer of a move between players is implemented using the "State" pattern
### Implemented a set of exceptions
- On user input
- To open a file
- To download from a file
- For access to the block

### The UML diagram is built

### The program is run through an executable file
_
This projects ware made during the learning process
