#pragma once

#include "grid.hpp"

struct Simulation
{
    public:
        Simulation(int width, int height, int cellSize)
        : grid(width, height, cellSize), tempGrid(width, height, cellSize), run(false), generation(1), aliveCells(0) {};
        void draw(int whichColor);
        void setCellValue(int row, int column, int value);
        int countAliveNeighbors(int row, int column);
        void update();
        bool isRunning() {return run;}
        void start(){run = true;}
        void stop(){run = false;}
        void clearGrid();
        void createRandomState();
        void changeCell(int row, int column);
        int updateGeneration() {return generation;}
        int countAliveCells() {return aliveCells;}
    private:
        Grid grid;
        Grid tempGrid;
        bool run;
        int generation;
        int aliveCells;
};