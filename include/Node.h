#pragma once

#include <SDL2/SDL.h>

#include <vector>

class Node {
        public:
                Node(SDL_Renderer *renderer, const int &x, const int &y, const int &size);
                Node() = default;
                ~Node() = default;

                void draw(const uint8_t &r, const uint8_t &g, const uint8_t &b);
                void setParent(const Node &parent);

                auto operator==(const Node &other) const -> bool;
                auto operator!=(const Node &other) const -> bool;

                int
                        x = 0,
                        y = 0,
                        g = 0,
                        parentX,
                        parentY;

                float
                        f = 0.0f,
                        h = 0.0f;
                bool
                        isWall = false;
        private:
                SDL_Rect rect;
                SDL_Renderer *renderer = nullptr;
};
