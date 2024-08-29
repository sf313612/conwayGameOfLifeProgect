#include <utility>
#include <vector>
#include <iostream>

#include "simulation.hpp"

void Simulation::draw(int whichColor)
{
    grid.draw(whichColor);
}

void Simulation::setCellValue(int row, int column, int value)
{
    grid.setValue(row, column, value);
}

int Simulation::countAliveNeighbors(int row, int column)
{
    int liveNeighbors = 0;
    std::vector<std::pair<int, int>> neighborOfSets = 
    { // масив пар значень які можуть набути сусіди живої клвтини
        {-1, 0}, // above
        {1, 0}, // below
        {0, -1}, // left
        {0, 1}, // right
        {-1, -1}, // diagonal upper left
        {-1, 1}, // diagonal upper right
        {1, -1}, // diagonal lower left
        {1, 1} // diagonal lower right
    };

    // цикл, що перебирає всіх сусідів клітинки
    for(const auto& offset : neighborOfSets)
    {
        int neighborRow = (row + offset.first + grid.getRows()) % grid.getRows(); // обчислюємо рядок сусідньої клітини
        int neighborColumn = (column + offset.second + grid.getColumns()) % grid.getColumns(); // обчислюємо стовпець сусідньої клітинки
        liveNeighbors += grid.getValue(neighborRow, neighborColumn); // перевіряємо значення і додаємо якщо 1
    }
    return liveNeighbors;
}

void Simulation::update()
{
    int row = 0;
    int column = 0;
    int liveNeighbors;
    int cellValue;
    if(isRunning())
    {
        aliveCells = 0;
        for(row = 0; row < grid.getRows(); row++)
        {
            for(column = 0; column < grid.getRows(); column++)
            {
                liveNeighbors = countAliveNeighbors(row, column);
                cellValue = grid.getValue(row, column);

                if(cellValue == 1){
                    // first rule
                    if(liveNeighbors > 3 || liveNeighbors < 2){
                        tempGrid.setValue(row, column, 0);
                    }
                    else{
                        tempGrid.setValue(row, column, 1);
                        aliveCells++;
                    }
                }
                else{
                    // second rule
                    if(liveNeighbors == 3){
                        tempGrid.setValue(row, column, 1);
                        aliveCells++;
                    }
                    else{
                        tempGrid.setValue(row, column, 0);
                    }
                }
                
            }
        }
        grid = tempGrid;

        if(aliveCells == 0){
            stop();
            std::cout << "" << std::endl;
            std::cout << "--------------------------Keep the life rhythm---------------------------------" << std::endl;
            std::cout << "Any live cell with two or three live neighbors survives to the next generation" << std::endl;
            std::cout << "Any dead cell with exactly three live neighbors becomes a live cell" << std::endl;
            std::cout << "-------------------------------------------------------------------------------" << std::endl;
        }
        else{
            generation++;
        }
    }
}

void Simulation::clearGrid()
{
    generation = 1;
    aliveCells = 0;
    if(isRunning()){
        grid.clear();
        stop();
    }
}

void Simulation::createRandomState()
{
    if(!isRunning()){
        grid.fillRandom();
        generation = 1;
        aliveCells = 0;
    }
}

void Simulation::changeCell(int row, int column)
{
    if(!isRunning())
    {
        grid.changeCell(row, column);
    }
}
