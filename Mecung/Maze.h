#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>
class Maze
{
public:
    int playerX, playerY;
    void movePlayer(int dx, int dy);

    struct Cell {
        bool visited = false;
        bool walls[4] = { true, true, true, true }; // Top, Right, Bottom, Left
    };

    int width, height;
    std::vector<std::vector<Cell>> grid;

    Maze(int w = 20, int h = 20);
    void generate();
    void createEntranceAndExit();
    void createPathFromStartToEnd();
    void resetMaze();
    void resetPlayerPosition();

private:
    void huntAndKill();
    bool killPhase(int& x, int& y);
    bool huntPhase(int& x, int& y);
    void findUnvisitedNeighbors(int x, int y, std::vector<std::pair<int, int>>& neighbors);
    void findVisitedNeighbors(int x, int y, std::vector<std::pair<int, int>>& neighbors);
    void removeWall(int x1, int y1, int x2, int y2);
};

