#include "Grid.h"

#include "Globals.h"

#include <algorithm>

Grid::Grid(SDL_Renderer *renderer, const int &width, const int &height, const int &size) :
renderer(renderer), width(width), height(height), size(size) {
        values = new Node[width * height];

        for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                        values[y * width + x] = Node(renderer, x, y, size);
                }
        }
};

auto Grid::getNeighbors(const Node &node) -> std::vector<Node> {
        std::vector<Node> neighbors;

        for (int y = -1; y <= 1; ++y) {
                for (int x = -1; x <= 1; ++x) {
                        if (x == 0 && y == 0) continue;

                        int checkX = node.x + x;
                        int checkY = node.y + y;

                        if (checkX >= 0 && checkX < width && checkY >= 0 && checkY < height)
                                neighbors.push_back(getNode(checkX, checkY));
                }
        }

        return neighbors;
}

auto Grid::getNode(const int &x, const int &y) -> Node {
        return values[y * width + x];
}

auto Grid::checkExist(const int &x, const int &y) -> bool {
        return (std::find(walls.begin(), walls.end(), values[y * width + x]) != walls.end());
}

void Grid::setParent(const Node &child, const Node &parent) {
        values[child.y * width + child.x].setParent(parent);
}

void Grid::addWall(const int &x, const int &y) {
        values[y * width + x].isWall = true;
        walls.push_back(values[y * width + x]);
}

void Grid::removeWall(const int &x, const int &y) {
        values[y * width + x].isWall = false;
        walls.erase(std::remove(walls.begin(), walls.end(), values[y * width + x]), walls.end());
}

void Grid::removeWalls() {
        for (auto &wall : walls)
                values[wall.y * width + wall.x].isWall = false;
        walls.clear();
}

void Grid::drawWalls() {
        for (auto &wall : walls) wall.draw(44, 44, 44);
}

void Grid::draw() {
        SDL_SetRenderDrawColor(renderer, 226, 221, 221, 0xff);

        for (int x = -1; x < 1 + width * size - size; x += size)
                SDL_RenderDrawLine(renderer, x, 0, x, SCREEN_HEIGHT);
        for (int y = -1; y < 1 + height * size - size; y += size)
                SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
}
