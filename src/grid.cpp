#include <raylib.h>

#include "grid.hpp"

void Grid::draw(int whichColor)
{
    for(int row = 0; row < rows; row++) // перебір рядків сітки
    {
        for(int column = 0; column < columns; column++) // перебір колонок у поточному рядку
        {
            // якщо значення клітинки - 1, її буде заповнено кольором, якщо 0 - стає темносірою
            // використані тернарні оператори
            Color color;
            if(whichColor == 0){
                color = cells[row][column] ? Color{255, 255, 255, 255} : Color{9, 9, 9, 255};
            }
            else if(whichColor == 1){
                color = cells[row][column] ? Color{ 135, 60, 190, 255 }  : Color{9, 9, 9, 255};
            }
            else{
                color = cells[row][column] ? Color{ 0, 82, 172, 255 } : Color{9, 9, 9, 255};
            }
            DrawRectangle(column * cellSize + 11, row * cellSize + 11, cellSize - 1, cellSize - 1, color); // для кожної клітинки малюємо прямокутник
        }
    }
}

void Grid::setValue(int row, int column, int value)
{
    if(isInRange(row, column))
    {
        cells[row][column] = value;
    }
}

int Grid::getValue(int row, int column)
{
    if(isInRange(row, column))
    {
        return cells[row][column];
    }
    return 0;
}

bool Grid::isInRange(int row, int column)
{
    if(row >= 0 && row < rows && column >= 0 && column < columns)
    {
        return true;
    }
    return false;
}

void Grid::fillRandom()
{
    for(int row = 0; row < rows; row++) // перебір рядків
    {
        for(int column = 0; column < columns; column++) // перебір колонок
        {
            int randomValue = GetRandomValue(0, 4); // генеруємо числа від 0 до 4
            cells[row][column] = (randomValue == 4) ? 1 : 0; // якщо 4 - значення 1, інакше - 0
        } 
    }
}

void Grid::clear()
{
    for(int row = 0; row < rows; row++)
    {
        for(int column = 0; column < columns; column++)
        {
            cells[row][column] = 0;
        }
    }
}

void Grid::changeCell(int row, int column)
{
    if(isInRange(row, column)){
        cells[row][column] = !cells[row][column];
    }
}
