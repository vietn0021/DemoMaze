#include "pch.h"
#include "Maze.h"
#include <stack>
#include <queue>  // Thêm thư viện cho BFS
#include <algorithm> // Để sử dụng reverse()
void Maze::movePlayer(int dx, int dy)
{
    int newX = playerX + dx;
    int newY = playerY + dy;

    // Kiểm tra nếu người chơi không ra ngoài mê cung
    if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
        // Kiểm tra nếu không có tường giữa vị trí hiện tại và vị trí mới
        if (!grid[playerY][playerX].walls[dx == 1 ? 1 : dx == -1 ? 3 : dy == 1 ? 2 : 0]) {
            playerX = newX;
            playerY = newY;
        }
    }
}
Maze::Maze(int w, int h) : width(w), height(h), grid(h, std::vector<Cell>(w)) {
    srand(static_cast<unsigned>(time(nullptr)));
    generate(); // Sinh mê cung
    createEntranceAndExit();
    playerX = 0;  // Bắt đầu ở lối vào (0, 0)
    playerY = 0;
}

void Maze::generate() {
    createPathFromStartToEnd();
    huntAndKill();
}

void Maze::createEntranceAndExit()
{
    // Lối vào ở cạnh trên (hàng đầu tiên)
    grid[0][0].walls[0] = false; // Loại bỏ tường trên của ô đầu tiên

    // Lối ra ở cạnh dưới (hàng cuối cùng)
    grid[height - 1][width - 1].walls[2] = false; // Loại bỏ tường dưới của ô cuối cùng
}

void Maze::createPathFromStartToEnd()
{
    std::stack<std::pair<int, int>> stack;
    stack.push({ 0, 0 }); // Bắt đầu từ ô lối vào
    grid[0][0].visited = true;

    while (!stack.empty()) {
        std::pair<int, int> current = stack.top();
        int x = current.first;
        int y = current.second;

        // Nếu đến được ô cuối cùng, dừng lại
        if (x == width - 1 && y == height - 1)
            break;

        // Tìm các ô hàng xóm chưa được thăm
        std::vector<std::pair<int, int>> neighbors;
        findUnvisitedNeighbors(x, y, neighbors);

        if (!neighbors.empty()) {
            std::pair<int, int> next = neighbors[rand() % neighbors.size()];
            int nx = next.first;
            int ny = next.second;

            // Phá tường giữa ô hiện tại và ô hàng xóm
            removeWall(x, y, nx, ny);

            // Đánh dấu ô hàng xóm là đã thăm và thêm vào stack
            grid[ny][nx].visited = true;
            stack.push({ nx, ny });
        }
        else {
            // Nếu không còn ô hàng xóm, quay lui
            stack.pop();
        }
    }
}

void Maze::resetMaze()
{
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x] = Cell(); // Thiết lập lại từng ô
        }
    }
    generate(); // Tạo lại mê cung mới
}

void Maze::resetPlayerPosition()
{
    playerX = 0;  // Vị trí cột ban đầu
    playerY = 0;  // Vị trí hàng ban đầu
}

void Maze::huntAndKill() {
    int x = rand() % width;
    int y = rand() % height;

    while (true) {
        if (killPhase(x, y)) continue;
        if (!huntPhase(x, y)) break;
    }
}

bool Maze::killPhase(int& x, int& y) {
    std::vector<std::pair<int, int>> neighbors;
    while (true) {
        findUnvisitedNeighbors(x, y, neighbors);
        if (neighbors.empty()) return false;

        std::pair<int, int> neighbor = neighbors[rand() % neighbors.size()];
        int nx = neighbor.first;
        int ny = neighbor.second;

        removeWall(x, y, nx, ny);

        x = nx;
        y = ny;
        grid[y][x].visited = true;
    }
}

bool Maze::huntPhase(int& x, int& y) {
    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x) {
            if (!grid[y][x].visited) {
                std::vector<std::pair<int, int>> neighbors;
                findVisitedNeighbors(x, y, neighbors);
                if (!neighbors.empty()) {
                    std::pair<int, int> neighbor = neighbors[rand() % neighbors.size()];
                    int nx = neighbor.first;
                    int ny = neighbor.second;

                    removeWall(x, y, nx, ny);
                    grid[y][x].visited = true;
                    return true;
                }
            }
        }
    }
    return false;
}

void Maze::findUnvisitedNeighbors(int x, int y, std::vector<std::pair<int, int>>& neighbors) {
    neighbors.clear();
    if (y > 0 && !grid[y - 1][x].visited) neighbors.emplace_back(x, y - 1);
    if (x < width - 1 && !grid[y][x + 1].visited) neighbors.emplace_back(x + 1, y);
    if (y < height - 1 && !grid[y + 1][x].visited) neighbors.emplace_back(x, y + 1);
    if (x > 0 && !grid[y][x - 1].visited) neighbors.emplace_back(x - 1, y);
}

void Maze::findVisitedNeighbors(int x, int y, std::vector<std::pair<int, int>>& neighbors) {
    neighbors.clear();
    if (y > 0 && grid[y - 1][x].visited) neighbors.emplace_back(x, y - 1);
    if (x < width - 1 && grid[y][x + 1].visited) neighbors.emplace_back(x + 1, y);
    if (y < height - 1 && grid[y + 1][x].visited) neighbors.emplace_back(x, y + 1);
    if (x > 0 && grid[y][x - 1].visited) neighbors.emplace_back(x - 1, y);
}

void Maze::removeWall(int x1, int y1, int x2, int y2) {
    if (x1 == x2) {
        if (y1 < y2) {
            grid[y1][x1].walls[2] = false;
            grid[y2][x2].walls[0] = false;
        }
        else {
            grid[y1][x1].walls[0] = false;
            grid[y2][x2].walls[2] = false;
        }
    }
    else {
        if (x1 < x2) {
            grid[y1][x1].walls[1] = false;
            grid[y2][x2].walls[3] = false;
        }
        else {
            grid[y1][x1].walls[3] = false;
            grid[y2][x2].walls[1] = false;
        }
    }
}



