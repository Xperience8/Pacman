# Pacman
Windows, Unreal Engine, Solo Project

My Pacman is a clone of old original Pacman transformed into 3D with a lot of new features. Pacman consists from two modes. The first mode is top down mode. Player is using top camera for controlling Pacman. He sees big area around Pacman and he knows when ghosts are coming toward him. The second mode is first person mode. Player is using first person camera for controlling Pacman. He sees only what Pacman sees. Therefore he doesn't know where exactly are ghosts. Player needs to eat all spheres which are inside the level. There are two types of sphere, which give player some new ability. The first type gives player ability to shoot for limited type. The second type gives player ability to eat ghosts for limited type.

Pathfinding

Ghosts are not supposed to go back. They can only move forward or change their direction to the left or right. Pathfinding in unreal engine doesn't support that, therefore I've created custom pathfinding system based on Djikstra algorithm.

Height map

Height map of the level is used to automatically spawn all spheres inside the maze and create pathfinding data. Pacman uses only two heights. The first height is floor, which is used for moving. The second height are walls, which are used to block movement. When spheres are spawned into the level, some of them will be changed for special spheres.

AI

Ghost works in 3 states. The first state is moving around his own safe place and ignoring player. The second state is chasing player. This state is different for every ghost. This two states are periodically repeated for a while. The third state is used, when player eats power up, which gives him ability to eat ghost. In this state he is running away from the player. There are 4 ghosts which are chasing player. The first type is Blinky(red). Blinky is moving always toward Pacman. Therefore Blinky is trying to hit him from the back. The second type is Pinky(pink). Pinky is moving always toward one node before Pacman's current location. Therefore Pinky is trying to hit him from the front. The third type is Inky(cyan). When Blinky is alive, Inky's target point is in the direction from Blinky toward Pacman multiplied by two. When Blinky is dead, Inky will use Blinky's behavior. The last type is Clyde(orange). Clyde is chasing player only if he is nearly Clyde's safe place.

