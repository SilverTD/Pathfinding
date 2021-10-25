#include "Node.h"

Node::Node(SDL_Renderer *renderer, const int &x, const int &y, const bool &isWall) :
renderer(renderer), x(x), y(y), isWall(isWall) {
        rect.w = rect.h = SIZE;
        rect.x = x * SIZE;
        rect.y = y * SIZE;
};

void Node::draw(const uint8_t &r, const uint8_t &g, const uint8_t &b, const uint8_t &a, const bool &outline) {
        SDL_SetRenderDrawColor(renderer, r, g, b, a); // Color rgba
        if (outline) {
                SDL_RenderDrawRect(renderer, &rect);
                return;
        }
        SDL_RenderFillRect(renderer, &rect);
}

void Node::setParent(const Node &parent) {
        parentX = parent.x;
        parentY = parent.y;
}

bool Node::operator == (const Node &other) const {
        return (x == other.x && y == other.y);
}

bool Node::operator != (const Node &other) const {
        return (x != other.x && y != other.y);
}
