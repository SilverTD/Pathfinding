#include "Grid.h"

#include "Globals.h"

#include <algorithm>

Grid::Grid(SDL_Renderer *renderer, const int &width, const int &height) :
renderer(renderer), width(width), height(height) {
        values = new Node[width * height];

        Node temp;
        for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                        temp = Node(renderer, x, y, false);
                        values[y * width + x] = temp;
                }
        }
};

std::vector<Node> Grid::getNeighbors(const Node &node) {
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

Node Grid::getNode(const int &x, const int &y) {
        return values[y * width + x];
}

void Grid::setParent(const Node &child, const Node &parent) {
        values[child.y * width + child.x].setParent(parent);
}

void Grid::addWall(const int &x, const int &y) {
        values[y * width + x].isWall = true;
        walls.push_back(values[y * width + x]);
}

bool Grid::checkExist(const int &x, const int &y) {
        return (std::find(walls.begin(), walls.end(), values[y * width + x]) != walls.end());
}

void Grid::removeWall(const int &x, const int &y) {
        values[y * width + x].isWall = false;
        walls.erase(std::remove(walls.begin(), walls.end(), values[y * width + x]), walls.end());
}

void Grid::removeWalls() {
        for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                        values[y * width + x].isWall = false;
                }
        }
        walls.clear();
}

void Grid::drawWall() {
        for (auto &wall : walls) wall.draw(44, 44, 44);
}

void Grid::draw() {
        SDL_SetRenderDrawColor(renderer, 226, 221, 221, 0xff);

        for (int x = -1; x < 1 + width * SIZE - SIZE; x += SIZE)
                SDL_RenderDrawLine(renderer, x, 0, x, SCREEN_HEIGHT);
        for (int y = -1; y < 1 + height * SIZE - SIZE; y += SIZE)
                SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
}
