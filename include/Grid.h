#pragma once

#include <SDL2/SDL.h>

#include <vector>

#include "Node.h"

class Grid {
        public:
                Grid(SDL_Renderer *renderer, const int &width, const int &height, const int &size);
                Grid() = default;
                ~Grid() = default;

                std::vector<Node> getNeighbors(const Node &node);
                Node getNode(const int &x, const int &y);
                void setParent(const Node &child, const Node &parent);
                void addWall(const int &x, const int &y);
                bool checkExist(const int &x, const int &y);
                void removeWall(const int &x, const int &y);
                void removeWalls();
                void drawWalls();
                void draw();
        private:
                int width, height, size;

                Node *values;

                std::vector<Node> walls;

                SDL_Renderer *renderer;
};
