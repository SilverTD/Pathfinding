#include "Grid.h"

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

void Grid::draw() {
        for (int x = 0; x < size; ++x) {
                for (int y = 0; y < size; ++y) {
                        values[x * size + y].draw(121, 120, 120, true);
                }
        }
}