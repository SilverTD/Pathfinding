#include "Grid.h"

#include "Globals.h"

#include <algorithm>

Grid::Grid(SDL_Renderer *renderer, const int &size) :
renderer(renderer), size(size) {
        values = new Node[size * size];

        Node temp;
        for (int x = 0; x < size; ++x) {
                for (int y = 0; y < size; ++y) {
                        temp = Node(renderer, x, y, false);
                        values[x * size + y] = temp;
                }
        }
};

std::vector<Node> Grid::getNeighbors(const Node &node) {
        std::vector<Node> neighbors;

        for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                        if (x == 0 && y == 0) continue;

                        int checkX = node.x + x;
                        int checkY = node.y + y;

                        if (checkX >= 0 && checkX < size && checkY >= 0 && checkY < size)
                                neighbors.push_back(getNode(checkX, checkY));
                }
        }

        return neighbors;
}

Node Grid::getNode(const int &x, const int &y) {
        return values[x * size + y];
}

void Grid::setParent(const Node &child, const Node &parent) {
        values[child.x * size + child.y].setParent(parent);
}

void Grid::addWall(const int &x, const int &y) {
        values[x * size + y].isWall = true;
        walls.push_back(values[x * size + y]);
}

bool Grid::checkExist(const int &x, const int &y) {
        return (std::find(walls.begin(), walls.end(), values[x * size + y]) != walls.end());
}

void Grid::removeWall(const int &x, const int &y) {
        values[x * size + y].isWall = false;
        walls.erase(std::remove(walls.begin(), walls.end(), values[x * size + y]), walls.end());
}

void Grid::removeWalls() {
        walls.clear();
}

void Grid::drawWall() {
        for (auto &wall : walls) wall.draw(44, 44, 44);
}

void Grid::draw() {
        SDL_SetRenderDrawColor(renderer, 226, 221, 221, 0xff);

        for (int i = -1; i < 1 + size * SIZE; i += SIZE) {
                SDL_RenderDrawLine(renderer, i, 0, i, SCREEN_HEIGHT);
                SDL_RenderDrawLine(renderer, 0, i, SCREEN_WIDTH, i);
        }
}
