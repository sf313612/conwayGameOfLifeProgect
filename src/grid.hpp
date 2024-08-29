#pragma once // директива препроцесора для включення файлу лише один раз
#include <vector>

// необхідно створити сітку: розмір вікна 750, розмір клітинки 25 : 
// 750/25 = 30 - кількість потрібних клітинок

struct Grid
{
    public:
        Grid(int width, int height, int cellSize)
        : rows(height/cellSize), columns(width/cellSize), cellSize(cellSize), cells(rows, std::vector<int>(columns, 0)){}; // making vector of vectors filled with 0s
        void draw(int whichColor);
        void setValue(int row, int column, int value);
        int getValue(int row, int column);
        bool isInRange(int row, int column);
        int getRows() {return rows;}
        int getColumns() {return columns;}
        void fillRandom();
        void clear();
        void changeCell(int row, int column);
    private:
          int rows;
          int columns;
          int cellSize;
          std::vector<std::vector<int>> cells;
};