#include "tinygl-cpp.h"
#include <sys/time.h>
#include <iostream>
#include <queue>
#include <vector>
#include <stdlib.h>

using namespace std;
using namespace tinygl;

class Coord {
    public:
        int x, y;
        Coord(){}
        Coord(int x, int y) {
            this->x = x;
            this->y = y;
        }
};

#define MAX_RANGE 1000
class Edge {
    public:
        Coord source, target;
        float weight;
        Edge(Coord source, Coord target) {
            this->source = source;
            this->target = target;
            weight = ((float)(rand()%MAX_RANGE))/MAX_RANGE;
        }
};


class EdgeComparator {
    public:
        bool operator() (Edge x, Edge y) {
            return x.weight < y.weight;
        }
};



class Maze : public Window {
private:
    int N;
    int sz; // Size of square
    bool* touched;
    priority_queue<Edge, vector<Edge>, EdgeComparator> frontier;

public:
    Maze(int N, int sz, int sleep):Window(N*sz*2, N*sz*2, sleep) {
        this->N = N;
        this->sz = sz;
        touched = new bool[N*N];
        for (int i = 0; i < N*N; i++) {
            touched[i] = false;
        }
        // Add the lower left
        Coord a(0, 0);
        Coord b(1, 0);
        Coord c(0, 1);
        frontier.push(Edge(a, b));
        frontier.push(Edge(a, c));
        touched[0] = 0;
    }

    ~Maze() {
        delete[] touched;
    }

    void draw() override {
        if(frontier.size() > 0) {
            Edge e = frontier.top();
            frontier.pop();
            if (!touched[e.target.y*N + e.target.x]) {
                touched[e.target.y*N + e.target.x] = true;
                color(1, 1, 1);
                // Draw edge between points
                square(e.source.x*sz*2+sz/2, e.source.y*sz*2+sz/2, sz, sz);
                square(e.target.x*sz*2+sz/2, e.target.y*sz*2+sz/2, sz, sz);
                if (e.source.x == e.target.x) {
                    square(e.source.x*sz*2 + sz/2, 
                            e.source.y*sz*2+sz*(e.target.y-e.source.y) + sz/2, sz, sz);
                }
                else {
                    square(e.source.x*sz*2+sz*(e.target.x-e.source.x) + sz/2,
                             e.source.y*sz*2 + sz/2, sz, sz);
                }
                // Consider all four neighbors of e.target
                // If a neighbor is in bounds and it hasn't been touched,
                // add an edge from e.target to this neighbor to the frontier
                int x = e.target.x;
                int y = e.target.y;
                Coord right(x+1, y);
                if (x < N-1 && !touched[right.x + N*right.y]) {
                    frontier.push(Edge(e.target, right));
                }
                Coord left(x-1, y);
                if (x > 0 && !touched[left.x + N*left.y]) {
                    frontier.push(Edge(e.target, left));
                }
                Coord up(x, y-1);
                if (y > 0 && !touched[up.x + N*up.y]) {
                    frontier.push(Edge(e.target, up));
                }
                Coord down(x, y+1);
                if (y < N && !touched[down.x + N*down.y]) {
                    frontier.push(Edge(e.target, down));
                }
            }
        }
    }
};

int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Usage: ./maze <N> <square size> <sleep (optional)>\n";
        return 1;
    }
    int N = atoi(argv[1]);
    int res = atoi(argv[2]);
    int sleep = 0;
    if (argc > 3) {
        sleep = atoi(argv[3]);
    }
    Maze window(N, res, sleep);
    window.run();
    return 0;
}