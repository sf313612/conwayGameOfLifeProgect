#include <raylib.h>
#include <iostream>

#include "simulation.hpp"

// ініціалізація структури кнопка
typedef struct Button
{
    Rectangle rect;
    Color color;
}Button;

Button startButton = {0};
Button exitButton = {0};

// ініціалізація кнопки
void initButtton(Button *button, Rectangle rect, Color color)
{
    button->rect = rect;
    button->color = color;
}

// визначає чи нажата кнопка
bool isClicked(Button button)
{
    return CheckCollisionPointRec(GetMousePosition(), button.rect);
}

// тип для зберігання станів гри // відповідно 1 - 2 - 3
typedef enum GameScreen { TITLE, GAMEPLAY, ENDING} GameScreen;

// using binary system to represent alive and dead cells
int main()
{
    Color back = {19, 19, 19, 255}; 
    // розміри сітки
    const int WINDOW_W = 750;
    const int WINDOW_H = 775;

    // розміри вікна
    const int WINDOW_WIDTH = 1200;
    const int WINDOW_HEIGHT = 795;
    const int CELL_SIZE = 25;
    int whichColor = 0;
    
    // створення камери
    Camera2D camera = { 0 };
    camera.target = (Vector2){ WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f};
    camera.offset = (Vector2){ WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f };
    camera.zoom = 1.0f;

    // initialize the button
    initButtton(&startButton, (Rectangle){WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 50, 200, 100}, back);
    initButtton(&exitButton, (Rectangle){920, 700, 150, 100}, back);
    bool startButtonCLicked = false;
    bool exitButtonCLicked = false;

    int FPS = 12; // frames per second // виставляємо 12, щоб симуляція не була занадто швидкою

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game of Life"); // створення вікна гри
    SetTargetFPS(FPS); // симуляція не відбуватиметься швидше за fps
    Simulation simulation(WINDOW_W, WINDOW_H, CELL_SIZE); // створення сітки

    GameScreen currentScreen = TITLE;

    // Simulationg Loop for Updating State of the Program
    while(!WindowShouldClose())
    {
        switch(currentScreen)
        {
            case TITLE:
            {
                // якщо натиснути start - перехід на вікно гри
                if(isClicked(startButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    startButtonCLicked = true;
                    std::cout << "" << std::endl;
                    std::cout << "---------------------------------Rules----------------------------------------" << std::endl;
                    std::cout << "Any live cell with two or three live neighbors survives to the next generation" << std::endl;
                    std::cout << "Any dead cell with exactly three live neighbors becomes a live cell" << std::endl;
                    std::cout << "-------------------------------------------------------------------------------" << std::endl;
                }

                if(startButtonCLicked == true){
                    currentScreen = GAMEPLAY;
                }
            }break;
            case GAMEPLAY:
            {
                // якщо покрутити колесо миші - відбувається збільшення/зменшення ігрового поля
                // відслідковування зміни поля камери zoom
                camera.zoom += ((float)GetMouseWheelMove() * 0.05f);

                // може максимально збільшити у 3 рази, зменшити у 2
                if (camera.zoom > 3.0f) camera.zoom = 3.0f;
                else if (camera.zoom < 0.2f) camera.zoom = 0.2f;

                // якщо клікнути лівою кнопкою миші, клітинка оживе
                if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                    Vector2 mousePosition = GetMousePosition();
                    int row = mousePosition.y / CELL_SIZE;
                    int column = mousePosition.x / CELL_SIZE;
                    simulation.changeCell(row, column);
                    camera.zoom = 1.0f;
                    whichColor = 0;
                    FPS = 12;
                    simulation.stop();
                }

                // якщо клікнути на праву кнопку миші чи на колесо - зміна кольору клітинок
                else if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
                    whichColor = 1;
                }
                else if(IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)){
                    whichColor = 2;
                }

                // якщо натиснути ENTER - симуляція відбувається
                else if(IsKeyPressed(KEY_ENTER)){
                    simulation.start();
                    SetWindowTitle("Game of Life is running ...");
                }

                // якщо натиснути пробіл - симуляція припиняється
                else if(IsKeyPressed(KEY_SPACE)){
                    simulation.stop();
                    SetWindowTitle("Game of Life has stopped.");
                }

                // якщо натиснути F - збільшується швидкість симуляції
                else if(IsKeyPressed(KEY_F)){
                    FPS += 2;
                    SetTargetFPS(FPS);
                }

                // якщо натиснути S - зменшується швидкість симуляції
                else if(IsKeyPressed(KEY_S)){
                    if(FPS > 5){
                        FPS -= 2;
                        SetTargetFPS(FPS);
                    }
                }

                // якщо натиснути R - створюється випадкова послідовність клітинок
                else if(IsKeyPressed(KEY_R)){
                    simulation.createRandomState();
                }

                // якщо натиснути C - поле очиститься
                else if(IsKeyPressed(KEY_C)){
                    simulation.clearGrid();
                    SetWindowTitle("Game of Life has stopped.");
                }

                // якщо натиснути на кнопку exit - перехід на вікно закінчення гри
                if(isClicked(exitButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    exitButtonCLicked = true;
                }

                if(exitButtonCLicked == true){
                    currentScreen = ENDING;
                }

                // Застосування правил гри Конвея "Життя"
                simulation.update();
            }break;
            case ENDING:
            {
                
            }break;
            default: break;
        }

        // Drawing using raylib
        BeginDrawing();
        ClearBackground(back);

        switch(currentScreen)
        {
            case TITLE:
            {
                DrawText("CONWAY'S GAME OF LIFE", 340, 280, 40, WHITE);
                DrawText("Embark on Life's Journey", 340, 480, 40, WHITE);
                DrawRectangleRec(startButton.rect, startButton.color);
                DrawText("Start", startButton.rect.x + startButton.rect.width / 2 - MeasureText("Start", 50) / 2, startButton.rect.y + startButton.rect.height / 2 - 20 / 2, 40, WHITE);

            }break;
            case GAMEPLAY:
            {
                BeginMode2D(camera);

                simulation.draw(whichColor);

                EndMode2D();

                DrawText("CONWAY'S GAME OF LIFE", 820, 30, 25, WHITE);
                DrawText("Generation", 820, 100, 20, WHITE);
                DrawRectangleRounded({980, 80, 170, 60}, 0.3, 6, Color{39, 39, 39, 255});

                DrawText("Alive Cells", 820, 170, 20, WHITE);
                DrawRectangleRounded({980, 150, 170, 60}, 0.3, 6, Color{39, 39, 39, 255});

                DrawText("Click cell to change it", 800, 250, 20, WHITE);
                DrawText("Press enter to start", 800, 290, 20, WHITE);
                DrawText("Press space to stop", 800, 330, 20, WHITE);
                DrawText("Press F to spid", 800, 370, 20, WHITE);
                DrawText("Press S to slow", 800, 410, 20, WHITE);
                DrawText("Press R for random", 800, 450, 20, WHITE);
                DrawText("Press C to clear", 800, 490, 20, WHITE);
                DrawText("Press left mouse button for white", 800, 530, 20, WHITE);
                DrawText("Press right mouse button for purple", 800, 570, 20, WHITE);
                DrawText("Press middle mouse button for blue", 800, 610, 20, WHITE);
                DrawText("Use wheel to zoom", 800, 650, 20, WHITE);

                // виведення номера генерації
                char generationTextForGen[10];
                sprintf(generationTextForGen, "%d", simulation.updateGeneration());

                DrawText(generationTextForGen, 1050, 100, 20, WHITE);

                // виведення кількості живих комірок
                char generationTextForCell[10];
                sprintf(generationTextForCell, "%d", simulation.countAliveCells());

                DrawText(generationTextForCell, 1050, 170, 20, WHITE);
                
                // кнопка виходу
                DrawRectangleRec(exitButton.rect, exitButton.color);
                DrawRectangleRounded({900, 700, 150, 60}, 0.3, 6, Color{39, 39, 39, 255});
                DrawText("Exit", 950, 720, 25, WHITE);

            }break;
            case ENDING:
            {
                DrawText("CONWAY'S GAME OF LIFE", 350, 260, 40, WHITE);
                DrawText("The Journey's End", 430, 360, 35, WHITE);
            }break;
            default: break;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}