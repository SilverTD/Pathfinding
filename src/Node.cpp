#include "Node.h"

Node::Node(SDL_Renderer *renderer, const int &x, const int &y, const int &size) :
renderer(renderer), x(x), y(y) {
        rect.w = rect.h = size;
        rect.x = x * size;
        rect.y = y * size;
};

void Node::draw(const uint8_t &r, const uint8_t &g, const uint8_t &b) {
        SDL_SetRenderDrawColor(renderer, r, g, b, 0xff); // Color rgba
        SDL_RenderFillRect(renderer, &rect);
}

void Node::setParent(const Node &parent) {
        parentX = parent.x;
        parentY = parent.y;
}

auto Node::operator==(const Node &other) const -> bool {
        return (x == other.x && y == other.y);
}

auto Node::operator!=(const Node &other) const -> bool {
        return (x != other.x || y != other.y);
}
