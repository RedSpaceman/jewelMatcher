JewelMatcher v1.0 
Author: Sam Corcoran
Email: samuel.corcoran@gmail.com

------------
Game Outline
------------

Player may click adjacent jewels on the grid to switch their positions. If a line of three or more like-coloured jewels is created, the jewels are removed from the grid and the player's score is increased accordingly. If no matching set is created, the jewels switch back to their original places. When jewels are removed, jewels above them collapse downwards to fill the empty spaces. A game lasts for 60 seconds upon program execution, after which player input is inhibited.

----------------
Running The Game
----------------

The directory containing the game's compiled .exe must also contain a directory 'images', containing the art assets for the game. 

The game uses a free truetype font '#44v2' ( www.1001freefonts.com ) which must be contained by the .exe's directory.

---------
Libraries
---------

The game dynamically links to the following libraries, which must be kept alongside the .exe:

Rendering:
SDl.dll (http://www.libsdl.org/download-1.2.php)

Image Loading:
SDL_image.dll (http://www.libsdl.org/projects/SDL_image/)
zlib1.dll (accompanies SDL_image.dll)
libjpeg-8.dll (accompanies SDL_image.dll)
libpng15-15.dll (accompanies SDL_image.dll)

Text Rendering:
SDL_ttf.dll (http://www.libsdl.org/projects/SDL_ttf/)
libfreetype-6.dll(accompanies SDL_image.dll)

--------------
Datastructures
--------------

The player interacts with a 'Grid' object, composed of a series of 'Socket' objects. The 'Grid' acts as the board on which play takes place, while 'Sockets' represent positions or slots which can contain 'Jewel' objects. 

These 'Jewel' objects maintain a type/colour variable, which allows them to be matched as sets for scoring, and indicates which jewel image asset should be displayed at its location. Jewels also mantain locational information separate to that of the 'Socket' objects which contain them so that Jewels transitioning between two sockets may be rendered at intermediate locations.

When 'Grid' objects detect that their rows or columns contain matching sets of jewels, 'ColorGroup' objects are generated to track which 'Sockets' are concerned, and allow the total number of matching sets being scored at one time to be a factor in score calculation.

----------------------
Implemented Game Rules
----------------------

* Grid size hard-coded as 8*8
* Grid is populated randomly with jewels such that no existing color groups exist, which is handled by discarding 
* A game lasts for the duration of a 60 second timer, displayed on the screen
* Throughout the game, the player's current score is displayed on the screen
* Adjacent jewels may be clicked in sequence to switch their positions
* Switched jewels animate the process of moving to each others' sockets, and general game logic is not executed until animation is completed
* If switched jewels do not result a colour group (three or more like-coloured jewels in a line), the jewels are switched back, after animation but before gameplay continues
* On each iteration, if the grid's sockets all contain jewels and no jewels are currently moving, colour groups are detected and removed from the grid for scoring. This includes both after a legal jewel-switch by the player, or after columns in the grid have collapsed downwards and potentially created new colour groups
* Scoring is performed for all colour groups present on the board at the moment of scoring
* When jewels are removed, jewels above them in the column collapse downwards to fill the vacant sockets. This is handled by empty sockets continuously requesting/stealing a jewel from the socket above, with sockets in the top row of the grid generating new jewels to satisfy their vacancy
* Jewels can only be 'stolen' (during grid collapse) if the above socket's jewel is not currently moving between sockets
* Sockets in top row (with no sockets to 'steal' jewels from) generate new Jewel objects to fill their vacancy, which drop in from above the grid
* Gameplay is preventing from continuing while sockets are empty
* Gameplay is preventing from continuing while jewels are moving
* Pressing the 'p' key pauses the game's animation
* Once the timer expires, the player is told the game is over
* Jewels on Grid continue to collapse/be scored once game is over, though user interaction is prevented, to allow final moves to be fully scored
* Jewels images are drawn on screen in their grid locations
* Sockets are drawn on screen in a simplistic fashion , with different images for 'default socket' 'mouse-over socket' and 'selected socket', to help players understand what they can select or have selected

------------
Program Flow
------------

The following is a rough guide to the sequence of events occurring:

1. SDL initialisation and loading of assets
2. Create Grid object and its sockets
3. Populate Grid's sockets with Jewels objects of random colours, detecting if the generated grid contains existing color groups. If so, the sockets with matching jewels are instructed to generate new Jewels, a scramble intended to reduce the total number of complete grid regenerations required; if the grid continues to contain color groups then the whole grid undergoes regeneration.
4. Main game loop
4a. Handle events (mouse movement, clicks, button presses of 'p' pause button
4b. Check gamestate before continuing (if game is over, or paused)
4c. Calculate delta-time (time since last iteration), animationTime (whether animation will be given zero timestep of full fixed timestep), and time elapsed since the beginning of the game. 
4d. Check whether sockets all contain jewels, and initiate another iteration of board collapse if not
4e. Check whether jewels are currently moving between sockets
4f. Detect colour groups on the grid, removing and scoring any that are found
4g. Providing game timer has not run out, undo a jewel switch if an illegal switch was made on the last iteration
4h. Otherwise, if a mouse un-click occurred on the grid, at the relevant socket to the set of selected sockets (if two jewels are now selected, check adjacency and switch them)
4i. Draw background
4j. Draw grid (socket assets, jewel assets)
4k. Draw HUD (timer and score screen overlays) and other on-screen text
5. Clean-up: free surfaces, deconstruct objects.

---------
Time-Step
---------

To ensure a regularity in speed of animation, the game uses a fixed timeStep. A cumulative total of delta-time on each game loop iteration is compared to the fixed timestep size, described as its 'animation time'. Functions responsible for game advancement over time (e.g. animation) are given a delta-time of zero until the fixed timestep threshold is surpassed the cumulative delta-time, at which point 'animation time' is made equal to the fixed timestep for a single iteration, and the cumulative total of delta-time is reset.

---------------------------------
Un-Implemented Features and Fixes
---------------------------------

Bug fixes
---------
* Restructure handling of 'pause' so timer does not continue counting down despite on-screen representation being frozen
* Text rendering auto-centring needs tweaking as 'Game Over' message appears off-centre

Game Rules
----------
* Detect colour-groups which intersect and merge them together (if colours match) for additional points

Interaction
-----------
* Click and drag for jewel switching: if player clicks a jewel, and holds down the button, jewel should move with the mouse cursor. If jewel is released on an adjacent socket, those jewels should be switched.
* Splash screen with 'play' button so that gameplay does not begin immediately
* New-game key for initiating another game during or after the current one

Visual
------
* Alpha fade-in for jewels arriving in top row from above grid
* Particle effect to occur in the place of jewels as they are removed, to assist understanding of jewel removal
* Add brief delay when jewels are removed from grid, before grid collapsing occurs, to emphasize the moment
* Number of points earned from removed group should appear in text over centre of where the groups were removed from, before momentarily fading out, so player is better connected to the points they earned
* Particle effect on dynamite fuse, interpolated along distance based on how much time is left
* End-game graphic, with 'final score' overlay
* Animation speed for collapsing grid separate from that of jewel-switching, and increased slightly

Audio
-----
* Add switching sounds
* Add jewel-removal/point-scoring sounds
* Add end of game sounds 






























