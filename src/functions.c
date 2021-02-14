#include "functions.h"

//functions related to stack
Boolean isEmpty(stack Stack)
{
    return (Boolean)(Stack == NULL);
}
void push(stack *pointToStack, cell *Cell)
{

    node *nodeToPush;
    nodeToPush = (node *)malloc(sizeof(node));
    nodeToPush->topCell = (cell *)malloc(sizeof(cell));
    (nodeToPush->topCell) = Cell;
    nodeToPush->nextnode = *pointToStack;
    *pointToStack = nodeToPush;
}
cell *pop(stack *Stack)
{

    cell *Cell = NULL;
    Cell = (*Stack)->topCell;
    node *Node = *Stack;
    (*Stack) = (*Stack)->nextnode;
    free(Node);
    return Cell;
}

void cellNeighbours(cell *Cell, cell **maze)
{

    //init neighbours
    int i = Cell->coord.rowIndex;
    int j = Cell->coord.columnIndex;
    int unvisitedNeighboursNb = 0;

    cell top, right, bottom, left;

    Boolean topExists = i != 0 ? True : False;
    if (topExists)
        top = maze[i - 1][j];

    Boolean rightExists = j != columnNb - 1 ? True : False;
    if (rightExists)
        right = maze[i][j + 1];

    Boolean bottomExists = i != rowNb - 1 ? True : False;
    if (bottomExists)
        bottom = maze[i + 1][j];

    Boolean leftExists = j != 0 ? True : False;
    if (leftExists)
        left = maze[i][j - 1];

    //Cell.unvisitedNeighbours (array of unvisited neighbours to be filled)

    if (topExists && !top.info->isVisited)
    {
        unvisitedNeighboursNb++;

        *(Cell->unvisitedNeighbours + unvisitedNeighboursNb - 1) = top;
    }

    if (rightExists && !right.info->isVisited)
    {
        unvisitedNeighboursNb++;

        *(Cell->unvisitedNeighbours + unvisitedNeighboursNb - 1) = right;
    }

    if (bottomExists && !bottom.info->isVisited)
    {
        unvisitedNeighboursNb++;

        *(Cell->unvisitedNeighbours + unvisitedNeighboursNb - 1) = bottom;
    }
    if (leftExists && !left.info->isVisited)
    {
        unvisitedNeighboursNb++;

        *(Cell->unvisitedNeighbours + unvisitedNeighboursNb - 1) = left;
    }
    Cell->info->unvisitedNeighbourNumber = unvisitedNeighboursNb;
}
Boolean checkNeighbours(cell Cell)
{

    if (Cell.info->unvisitedNeighbourNumber != 0)
    {
        return True;
    }
    else
    {
        return False;
    }
}
cell *randomNeighbour(cell Cell)
{
    int randomNb = rand() % (Cell.info->unvisitedNeighbourNumber); //random number between 0 and neighboursNumber of the cell
    return &(Cell.unvisitedNeighbours)[randomNb];
}
void removeWall(cell *Cell1, cell *Cell2)
{
    if (Cell2->coord.rowIndex == Cell1->coord.rowIndex - 1)
    {
        Cell1->info->walls[0] = False;
        Cell2->info->walls[2] = False;
    }
    else if (Cell2->coord.columnIndex == Cell1->coord.columnIndex + 1)
    {
        Cell1->info->walls[1] = False;
        Cell2->info->walls[3] = False;
    }
    else if (Cell2->coord.rowIndex == Cell1->coord.rowIndex + 1)
    {
        Cell1->info->walls[2] = False;
        Cell2->info->walls[0] = False;
    }
    else if (Cell2->coord.columnIndex == Cell1->coord.columnIndex - 1)
    {
        Cell1->info->walls[3] = False;
        Cell2->info->walls[1] = False;
    }
}

//functions related to Draw
void drawCell(SDL_Renderer *renderer, cell Cell)
{
    if ((Cell.info)->walls[0])
        SDL_RenderDrawLine(renderer, Cell.xPosition, Cell.yPosition, Cell.xPosition + widthCell, Cell.yPosition);
    if ((Cell.info)->walls[1])
        SDL_RenderDrawLine(renderer, Cell.xPosition + widthCell, Cell.yPosition, Cell.xPosition + widthCell, Cell.yPosition + widthCell);
    if ((Cell.info)->walls[2])
        SDL_RenderDrawLine(renderer, Cell.xPosition + widthCell, Cell.yPosition + widthCell, Cell.xPosition, Cell.yPosition + widthCell);
    if ((Cell.info)->walls[3])
        SDL_RenderDrawLine(renderer, Cell.xPosition, Cell.yPosition + widthCell, Cell.xPosition, Cell.yPosition);
}
void drawMaze(SDL_Renderer *renderer, cell **maze)
{
    for (int i = 0; i < rowNb; i++)
        for (int j = 0; j < columnNb; j++)
            drawCell(renderer, maze[i][j]);
}

//Generating the maze
cell makeCell(int yPos, int xPos)
{
    cell Cell;
    Cell.xPosition = 150 + (xPos * widthCell);
    Cell.yPosition = 120 + (yPos * widthCell);
    Cell.coord.rowIndex = yPos;
    Cell.coord.columnIndex = xPos;
    Cell.info = (data *)malloc(sizeof(data));
    (Cell.info)->walls[0] = True;
    (Cell.info)->walls[1] = True;
    (Cell.info)->walls[2] = True;
    (Cell.info)->walls[3] = True;
    Cell.info->isVisited = False;
    Cell.info->discovered = False;
    Cell.info->unvisitedNeighbourNumber = 0;
    Cell.unvisitedNeighbours = (cell *)malloc(4 * sizeof(cell));
    Cell.connectedNeighbours = (cell *)malloc(4 * sizeof(cell));
    return Cell;
}
void init(cell **grid)
{
    for (int i = 0; i < rowNb; i++)
    {
        for (int j = 0; j < columnNb; j++)
        {
            grid[i][j] = makeCell(i, j);
        }
    }
}
void createImage(SDL_Renderer *renderer, char const *path, SDL_Rect *rect)
{
    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;
    if ((surface = IMG_Load(path)) == NULL)
    {
        printf("Error: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if ((texture = SDL_CreateTextureFromSurface(renderer, surface)) == NULL)
    {
        printf("Error: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_RenderCopy(renderer, texture, NULL, rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
void drawMazeInProgress(SDL_Renderer *renderer, SDL_Texture *texture, cell **grid, SDL_Rect *secondScreenRect)
{

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, secondScreenRect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    drawMaze(renderer, grid);
    SDL_RenderPresent(renderer);
    SDL_Delay(20);
}

cell **generateMaze(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *secondScreenRect)
{
    srand(time(NULL));
    cell **grid = (cell **)malloc(rowNb * sizeof(cell *));
    for (int i = 0; i < rowNb; i++)
    {
        grid[i] = (cell *)malloc(columnNb * sizeof(cell));
    }
    init(grid);
    stack Stack = NULL;
    int c, d;
    cell *cellCurrent;
    cell *chosenCell;

    (grid[0][0].info)->isVisited = True;
    grid[0][0].info->walls[3] = False;
    push(&Stack, &grid[0][0]);
    while (!isEmpty(Stack))
    {
        cellCurrent = pop(&Stack);
        cellNeighbours(cellCurrent, grid);
        if (checkNeighbours(*cellCurrent))
        {

            push(&Stack, cellCurrent);
            chosenCell = randomNeighbour(*cellCurrent);
            removeWall(cellCurrent, chosenCell);
            chosenCell->info->isVisited = True;
            if (chosenCell->coord.columnIndex == columnNb - 1 && chosenCell->coord.rowIndex == rowNb - 1)
                chosenCell->info->walls[1] = False;
            if (chosenCell->coord.columnIndex == columnNb - 1 && chosenCell->coord.rowIndex == 0)
                chosenCell->info->walls[1] = False;
            push(&Stack, chosenCell);
            drawMazeInProgress(renderer, texture, grid, secondScreenRect);
        }
    }
    return grid;
}

void connectedNeighbours(cell *Cell, cell **maze)
{

    int i = Cell->coord.rowIndex;
    int j = Cell->coord.columnIndex;
    int connectedNeighboursNb = 0;

    cell top, right, bottom, left;

    Boolean topIsConnected = Cell->info->walls[0] == False ? True : False;
    if (topIsConnected)
        top = maze[i - 1][j];

    Boolean rightIsConnected = Cell->info->walls[1] == False && (Cell->coord.columnIndex != columnNb - 1 || Cell->coord.rowIndex != 0) && (Cell->coord.columnIndex != columnNb - 1 || Cell->coord.rowIndex != rowNb - 1) ? True : False;
    if (rightIsConnected)
        right = maze[i][j + 1];

    Boolean bottomIsConnected = Cell->info->walls[2] == False ? True : False;
    if (bottomIsConnected)
        bottom = maze[i + 1][j];

    Boolean leftIsConnected = Cell->info->walls[3] == False && (Cell->coord.columnIndex != 0 || Cell->coord.rowIndex != 0) ? True : False;
    if (leftIsConnected)
        left = maze[i][j - 1];

    if (bottomIsConnected && !bottom.info->discovered)
    {
        connectedNeighboursNb++;

        *(Cell->connectedNeighbours + connectedNeighboursNb - 1) = bottom;
    }
    if (leftIsConnected && !left.info->discovered)
    {
        connectedNeighboursNb++;

        *(Cell->connectedNeighbours + connectedNeighboursNb - 1) = left;
    }
    if (topIsConnected && !top.info->discovered)
    {
        connectedNeighboursNb++;

        *(Cell->connectedNeighbours + connectedNeighboursNb - 1) = top;
    }
    if (rightIsConnected && !right.info->discovered)
    {
        connectedNeighboursNb++;

        *(Cell->connectedNeighbours + connectedNeighboursNb - 1) = right;
    }
    Cell->info->connectedNeighbourNumber = connectedNeighboursNb;
}
stack *stackPath(cell **maze, cell *enterCell, cell *exitCell)
{
    cell *currentCell = enterCell;
    stack *Stack = (stack *)malloc(sizeof(stack));
    *Stack = NULL;
    push(Stack, enterCell);

    while ((!isEmpty(*Stack)) && (currentCell->coord.rowIndex != exitCell->coord.rowIndex || currentCell->coord.columnIndex != exitCell->coord.columnIndex))
    {
        currentCell = pop(Stack);
        connectedNeighbours(currentCell, maze);
        if (currentCell->info->discovered == False)
        {
            push(Stack, currentCell);
            currentCell->info->discovered = True;
            for (int i = 0; i < currentCell->info->connectedNeighbourNumber; i++)
            {
                push(Stack, currentCell->connectedNeighbours + i);
            }
        }
    }
    pop(Stack);

    return Stack;
}

void reset(cell **grid)
{
    for (int i = 0; i < rowNb; i++)
    {
        for (int j = 0; j < columnNb; j++)
        {
            grid[i][j].info->discovered = False;
            grid[i][j].info->connectedNeighbourNumber = 0;
        }
    }
}

void drawObject(SDL_Renderer *renderer, SDL_Texture *texture, cell **maze, SDL_Rect *rect, SDL_Rect *secondScreenRect, int r, int g, int b)
{
    SDL_RenderCopy(renderer, texture, NULL, secondScreenRect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    drawMaze(renderer, maze);
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderDrawRect(renderer, rect);
    SDL_RenderFillRect(renderer, rect);
    Sleep(100);
    SDL_RenderPresent(renderer);
}
void drawPath(SDL_Renderer *renderer, SDL_Texture *texture, stack *Stack, cell **maze, SDL_Rect *secondScreenRect, int r, int g, int b)
{
    cell *currentCell;
    SDL_Rect rect;
    rect.w = widthCell - 2 * (widthCell / 5);
    rect.h = widthCell - 2 * (widthCell / 5);
    rect.x = maze[0][0].xPosition + widthCell / 5 - widthCell;
    rect.y = maze[0][0].yPosition + widthCell / 5;
    drawObject(renderer, texture, maze, &rect, secondScreenRect, r, g, b);
    rect.x = maze[0][0].xPosition + widthCell / 5;
    rect.y = maze[0][0].yPosition + widthCell / 5;
    drawObject(renderer, texture, maze, &rect, secondScreenRect, r, g, b);
    while (!isEmpty(*Stack))
    {
        currentCell = pop(Stack);
        if (currentCell->info->discovered == True)
        {
            rect.x = currentCell->xPosition + widthCell / 5;
            rect.y = currentCell->yPosition + widthCell / 5;
            drawObject(renderer, texture, maze, &rect, secondScreenRect, r, g, b);
        }
    }
    rect.x = currentCell->xPosition + widthCell / 5 + widthCell;
    rect.y = currentCell->yPosition + widthCell / 5;
    drawObject(renderer, texture, maze, &rect, secondScreenRect, r, g, b);
}
void gameLoop(SDL_Renderer *renderer)
{
    cell **grid = NULL;
    stack *Stack = NULL;
    int x, y;
    boolean isInFirstScreen = True;
    boolean isMazeGenerated = False;
    SDL_Texture *texture = NULL;
    SDL_Surface *surface = NULL;
    SDL_Rect firstScreenRect = {0, 0, widthWindow, heightWindow};
    SDL_Rect secondScreenRect = {0, 0, widthWindow, heightWindow};
    SDL_Rect startButtonRect = {650, 400, 350, 150};
    SDL_Rect quitRect = {966, 590, 200, 80};
    SDL_Rect homeRect = {15, 590, 200, 75};
    SDL_Rect generateRect = {493, 520, 200, 80};
    SDL_Rect solveExit1Rect = {283, 590, 200, 80};
    SDL_Rect solveExit2Rect = {653, 595, 200, 70};
    createImage(renderer, "pictures/firstScreen.png", &firstScreenRect);
    if ((surface = IMG_Load("pictures/secondScreen.png")) == NULL)
    {
        printf("Error: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if ((texture = SDL_CreateTextureFromSurface(renderer, surface)) == NULL)
    {
        printf("Error: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_FreeSurface(surface);
    SDL_Event event;
    int done = 0;
    while (!done)
    {

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    x = event.motion.x;
                    y = event.motion.y;
                    //start Button
                    if (isInFirstScreen == True && x >= startButtonRect.x && x <= startButtonRect.x + startButtonRect.w)
                    {
                        if (y >= startButtonRect.y && y <= startButtonRect.y + startButtonRect.y)
                        {
                            createImage(renderer, "pictures/secondScreen.png", &secondScreenRect);
                            isInFirstScreen = False;
                        }
                    }
                    //quit Button
                    if (x >= quitRect.x && x <= quitRect.x + quitRect.w)
                        if (y >= quitRect.y && y <= quitRect.y + quitRect.y)
                            exit(0);
                    //home Button
                    if (x >= homeRect.x && x <= homeRect.x + homeRect.w)
                    {
                        if (y >= homeRect.y && y <= homeRect.y + homeRect.y)
                        {
                            createImage(renderer, "pictures/firstScreen.png", &firstScreenRect);
                            isInFirstScreen = True;
                            isMazeGenerated = False;
                        }
                    }
                    //generate Button
                    if (x >= generateRect.x && x <= generateRect.x + generateRect.w)
                    {
                        if (y >= generateRect.y && y <= generateRect.y + generateRect.y)
                        {
                            isMazeGenerated = True;
                            free(grid);
                            grid = generateMaze(renderer, texture, &firstScreenRect);
                        }
                    }

                    //solve exit1 Button
                    if (x >= solveExit1Rect.x && x <= solveExit1Rect.x + solveExit1Rect.w)
                    {
                        if (y >= solveExit1Rect.y && y <= solveExit1Rect.y + solveExit1Rect.y)
                        {
                            if (isMazeGenerated == True)
                            {
                                Stack = stackPath(grid, &grid[rowNb - 1][columnNb - 1], &grid[0][0]);
                                drawPath(renderer, texture, Stack, grid, &secondScreenRect, 254, 155, 170);
                                reset(grid);
                            }
                        }
                    }
                    //solve exit 2 Button
                    if (x >= solveExit2Rect.x && x <= solveExit2Rect.x + solveExit2Rect.w)
                    {
                        if (y >= solveExit2Rect.y && y <= solveExit2Rect.y + solveExit2Rect.y)
                        {
                            if (isMazeGenerated == True)
                            {
                                Stack = stackPath(grid, &grid[0][columnNb - 1], &grid[0][0]);
                                drawPath(renderer, texture, Stack, grid, &secondScreenRect, 175, 214, 218);
                                reset(grid);
                            }
                        }
                    }
                }
                break;
                //Quit
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    done = 1;
                break;
            case SDL_QUIT:
                done = 1;
                break;
            default:
                break;
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(20);
    }
    SDL_DestroyTexture(texture);
}