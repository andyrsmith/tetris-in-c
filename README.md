# Tetris

In this project I was doing a refresher course in C and wanted build a game in the terminal.  Used ncurses library for the display

## Building

Project is compatible on Linux OS and probably Mac(not sure).  Not compatible on Windows.

## Dependencies

Must have ncurses in order to build.  Will need to install libncurses5-dev on your system first unless if it is alreay installed.

Ubuntu install instructions

`
sudo apt-get install libncurses5-dev
`

Compile

`
make
`

That is it.

## Controls

space: rotate tetromio
arrow keys: move tetromi

## Todo

- game over screen
- improve scoring: switch scoring to the original nintendo scoring. 
- current scoring is 40 * completedLines

