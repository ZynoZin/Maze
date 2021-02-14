#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <time.h>
#include <Windows.h>
#include <SDL_image.h>

#define widthWindow 1200
#define heightWindow 700
#define widthMaze 900
#define heightMaze 390
#define xPositionMaze 200
#define yPositionMaze 100
#define widthCell 30
#define columnNb widthMaze / widthCell
#define rowNb heightMaze / widthCell
#define cellsNumber columnNb *rowNb

typedef enum _Boolean
{
    False,
    True
} Boolean;

typedef struct _data
{
    Boolean isVisited;
    Boolean walls[4];
    Boolean discovered;
    int unvisitedNeighbourNumber;
    int connectedNeighbourNumber;
} data;

//stack-----------------------------------------------------------------------
//structs
typedef struct _cellCoordinates
{
    int rowIndex;
    int columnIndex;
} cellCoordinates;

typedef struct _cell
{
    cellCoordinates coord;
    int xPosition;
    int yPosition;
    struct _cell *unvisitedNeighbours; //array of unvisited neighbour cells
    struct _cell *connectedNeighbours; //array of undiscovered connected neighbour cells
    data *info;
} cell;

typedef cell maze[rowNb][columnNb];

typedef struct _node
{
    cell *topCell;
    struct _node *nextnode;
} node;

typedef node *stack;

//functions

/*stack functions*/
//to check if the stack is empty or not.
Boolean isEmpty(stack);
//to push a cell to the stack
void push(stack *, cell *);
//to pop a cell from the stack
cell *pop(stack *);
//-----------------------------------------------------------------

/*neighbours functions*/
//to  fill the unvisited neighbours list of a cell
void cellNeighbours(cell *, cell **);
//to check if unvisited neighbours is NULL or not.
Boolean checkNeighbours(cell);
//to pick a random neighbour from the the unvisited neighbours list
cell *randomNeighbour(cell);
//removes the wall between two cells.
void removeWall(cell *, cell *);

/*Generating the maze*/
//loads the parameteres of a cell
cell makeCell(int, int);
//initialize the maze grid with all the cells
void init(cell **);
//draws the maze while playing
void drawMazeInProgress(SDL_Renderer *, SDL_Texture *, cell **, SDL_Rect *);
//generates a new random maze and draw it
cell **generateMaze(SDL_Renderer *, SDL_Texture *, SDL_Rect *);
//draws the walls of a cell
void drawCell(SDL_Renderer *, cell);
//draws a grid
void drawMaze(SDL_Renderer *, cell **);

/*Solving the maze*/
//loads a stack filled with the cells representing the path from entry to exit
stack *stackPath(cell **, cell *, cell *);
//to fill the connected neighbours list of a cell
void connectedNeighbours(cell *, cell **);
//to draw the object to be moved
void drawObject(SDL_Renderer *renderer, SDL_Texture *texture, cell **maze, SDL_Rect *rect, SDL_Rect *secondScreenRect, int r, int g, int b);
//Moves the rectangle according to the path given
void drawPath(SDL_Renderer *renderer, SDL_Texture *texture, stack *Stack, cell **maze, SDL_Rect *secondScreenRect, int r, int g, int b);
//resets the connected neighbours of the cells contained in the grid
void reset(cell **);

//texture handelling and showing images
void createImage(SDL_Renderer *, char const *, SDL_Rect *);

//Main game loop
void gameLoop(SDL_Renderer *);
