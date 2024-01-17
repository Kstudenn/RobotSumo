# Robot Sumo competition

This program was created to control previously builded 3D printed robot which took part in 'Konar Arena' competition organized by Wrocław University of Science and Technology.

## Competition

Robot's job was to find its opponents with his sensors and try pushing them off ring

## Robot's tasks

To participate in competition robot needed to accomplish few tasks.It needed to search for opponent, attack it while not falling off arena.
It needed to search for opponent, attack it while not falling off arena.
Engines was programed to accelerate by changing duration between 1/0 signals send to engines.
There were two sets of sensors. One set programed to recognize arena which helps to not leave it.
Second set was programmed to search for opponent on the ring.
If enemy is found algorithm proceeds to use engines to move towards opponent and try pushing him off the ring.
Whole program will start by getting communicate through bluetooth signal.

## Technologies

Technologies used in this project:
Arduino IDE, C++

