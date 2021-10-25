#pragma once

#include <SDL2/SDL.h>

#include <vector>

#include "Node.h"

class Grid {
        public:
                Grid(SDL_Renderer *renderer, const int &size);
                Grid() = default;
                ~Grid() = default;

                std::vector<Node> getNeighbors(const Node &node);
                Node getNode(const int &x, const int &y);
                void setParent(const Node &child, const Node &parent);
        private:
                int size;
                Node *values;

                std::vector<Node> walls;
};
