# Tetris

### Local multiplayer Tetris created in Unreal Engine using C++ instead of blueprints.

Set up in a data management manner, a grid of cells was implemented which hold required information as to whether spaces are filled or not. This way negated having to implement collison detectors and so the tick method only measures when a tetromino piece needs to move down the grid. The tetromino pieces themselves are created by simply toggling the visibilty and texture of square cells to create the famous shapes. This way allows the game manager to easily keep track of other placed tetrominos and subsequently whether lines have been created. Local multiplayer was set up by creating two grids and using pointers to use the same functions and methods without having to create identicle methods for both.
