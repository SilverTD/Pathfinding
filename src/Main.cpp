#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

#include "Globals.h"
#include "Node.h"
#include "Grid.h"

enum State {
	START,
	TARGET,
	WALL
};

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

int
        frameCount,
        timerFPS,
        lastFrame,
        fps,
        lastTime,
        state = State::WALL;

bool
        running = true,
        mouseDown = false,
        startFinding = false;

Grid grid;
SDL_Rect grid_cursor_ghost;
SDL_Event event;
Node start, target;

std::vector<Node> open, closed;
std::vector<Node> path;

#define convert(x) (x / SIZE)

inline void init() {
        open.clear();
        closed.clear();
        path.clear();

        grid = Grid(renderer, ROWS, COLS, SIZE);
        start = Node(renderer, 0, 0, SIZE);
        target = Node(renderer, ROWS - 1, COLS - 1, SIZE);
        grid_cursor_ghost = {(ROWS - 1) / 2 * SIZE, (COLS - 1) / 2 * SIZE, SIZE, SIZE};
}

inline auto heuristic(const Node &start, const Node &end) -> float {
        return std::hypot(end.x - start.x, end.y - start.y);
}

inline void zoom(int&& size) {
        SIZE += size;
        ROWS = convert(SCREEN_WIDTH);
        COLS = convert(SCREEN_HEIGHT);

        init();
}

void update() {
        if (open.empty() || !startFinding) return;

        Node current = open[0];

        for (std::size_t i = 1; i < open.size(); ++i)
                if (open[i].f < current.f)
                        current = open[i];

        if (current == target) {
                current = grid.getNode(target.x, target.y);

                while (current != start) {
                        path.push_back(current);
                        current = grid.getNode(current.parentX, current.parentY);
                }

                std::reverse(path.begin(), path.end());
                startFinding = false;
                return;
        }

        open.erase(std::remove(open.begin(), open.end(), current), open.end());
        closed.push_back(current);

        for (auto &neighbor : grid.getNeighbors(current)) {
                if (neighbor.isWall || (std::find(closed.begin(), closed.end(), neighbor) != closed.end()))
                        continue;

                int newCost = current.g + heuristic(current, neighbor);
                if (newCost < neighbor.g || (std::find(open.begin(), open.end(), neighbor) == open.end())) {
                        neighbor.g = newCost;
                        neighbor.h = heuristic(neighbor, target);
                        neighbor.f = neighbor.g + neighbor.h;

                        grid.setParent(neighbor, current);

                        if (std::find(open.begin(), open.end(), neighbor) == open.end())
                                open.push_back(neighbor);
                }
        }
}

void handle_mouse_wheel(const int& y) {
	if (y > 0 && !startFinding && SIZE < 100) zoom(10);
	else if (y < 0 && !startFinding && SIZE >= 20) zoom(-10);
}

void handle_mouse_motion(const int& x, const int& y) {
	grid_cursor_ghost.x = x * SIZE;
        grid_cursor_ghost.y = y * SIZE;
	if (mouseDown && state == State::WALL && (!grid.checkExist(x, y)))
                grid.addWall(x, y);
}

void handle_mouse_button_down(const int& x, const int& y) {
	mouseDown = true;
        open.clear();
        closed.clear();
        path.clear();

	switch (state) {
		case State::START: start = Node(renderer, x, y, SIZE); break;
		case State::TARGET: target = Node(renderer, x, y, SIZE); break;
		case State::WALL:
			if (grid.checkExist(x, y)) {
				grid.removeWall(x, y);
				return;
			}
			grid.addWall(x, y);
			break;

	}
}

void handle_key_down(const int& e) {
	switch (e) {
		case SDLK_RETURN:
			startFinding = true;
                        open.push_back(start);
                        break;
                case SDLK_ESCAPE:
                        startFinding = false;
                        open.clear();
                        closed.clear();
                        path.clear();
                        grid.removeWalls();
                        break;
		case SDLK_1: state = State::START; break;
		case SDLK_2: state = State::TARGET; break;
		case SDLK_3: state = State::WALL; break;
	}
}

void input() {
        while (SDL_PollEvent(&event)) {
                switch (event.type) {
                        case SDL_QUIT: running = false; break;
			case SDL_MOUSEBUTTONUP: mouseDown = false; break;
			case SDL_MOUSEWHEEL: handle_mouse_wheel(event.wheel.y); break; 
                        case SDL_MOUSEMOTION: handle_mouse_motion(convert(event.motion.x), convert(event.motion.y)); break;
                        case SDL_MOUSEBUTTONDOWN: handle_mouse_button_down(convert(event.motion.x), convert(event.motion.y)); break;
                        case SDL_KEYDOWN: handle_key_down(event.key.keysym.sym); break;
                }
        }
}

void tick() {
        lastFrame = SDL_GetTicks();
        if (lastFrame >= lastTime + 1000) {
                lastTime = lastFrame;
                fps = frameCount;
                frameCount = 0;
        }
}

void draw() {
        SDL_SetRenderDrawColor(renderer, 192, 191, 192, 0xff);     // Draw grid_cursor_ghost.
        SDL_RenderFillRect(renderer, &grid_cursor_ghost);

        for (auto &node : closed) node.draw(255, 106, 73);
        for (auto &node : open) node.draw(36, 221, 96);
        for (auto &node : path) node.draw(85, 176, 254);

        grid.drawWalls();

        start.draw(63, 119, 255);
        target.draw(255, 34, 10);

        grid.draw();
}

void render() {
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(renderer);

        ++frameCount;
        timerFPS = SDL_GetTicks() - lastFrame;
        if (timerFPS < (1000 / 60)) {
                SDL_Delay((1000 / 60) - timerFPS);
        }

        draw();

        SDL_RenderPresent(renderer);
}

int main(int argc, char const *argv[]) {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
                std::cout << "SDL_Init() failed" << '\n';
        if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer) < 0)
                std::cout << "SDL_CreateWindowAndRenderer() failed" << '\n';
        SDL_SetWindowTitle(window, "Path Finding");

        init();

        while (running) {
                tick();
                update();
                input();
                render();
        };

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 0;
}
