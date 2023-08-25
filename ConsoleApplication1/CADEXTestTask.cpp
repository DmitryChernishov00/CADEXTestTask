#define _USE_MATH_DEFINES
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string>
// Количество кривых
#define MIN_COUNT 3
#define MAX_COUNT 15

double tStep = M_PI / 180;

struct objTable{
    std::string name;
    std::string point;
    std::string derivative;
};

struct circleTable {
    std::string name;
    int radii;
};

void drawCircle(HDC hdc, int x0, int y0, int R, objTable* table, int i) // Нарисовать круг и вычислить 3D-точку и первую производную
{
    HPEN p1, p2 = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // Красное перо
    p1 = (HPEN)SelectObject(hdc, p2);  // Заносим перо в контекст рисования
    int x, y;
    double t = 0;
    MoveToEx(hdc, x0 + R, y0, NULL);
    do
    {
        x = x0 + (R * cos(t));
        y = y0 + (R * sin(t));
        LineTo(hdc, x, y);
        t += tStep;
    } while (t <= 2 * M_PI);
    // Вычисление 3D-точки
    t = M_PI / 4;
    x = x0 + (R * cos(t));
    y = y0 + (R * sin(t));
    table[i].point = "(" + std::to_string(x) + ", " + std::to_string(y) + ", 0)";
    // Вычисление производной (3D-вектора)
    int dx, dy;
    dx = -1 * R * sin(t);
    dy = R * cos(t);
    table[i].derivative = "(" + std::to_string(dx) + ", " + std::to_string(dy) + ", 0)";
    SelectObject(hdc, p1);   // Возвращаем старое перо
    DeleteObject(p2);       // Удаляем созданное перо
}

void drawEllipse(HDC hdc, int x0, int y0, int Rx, int Ry, objTable* table, int i)   // Нарисовать эллипс и вычислить 3D-точку и первую производную
{
    HPEN p1, p2 = CreatePen(PS_SOLID, 2, RGB(0, 0, 255)); // Синее перо
    p1 = (HPEN)SelectObject(hdc, p2);  // Заносим перо в контекст рисования
    int x, y;
    double t = 0;
    MoveToEx(hdc, x0 + Rx, y0, NULL);
    do
    {
        x = x0 + (Rx * cos(t));
        y = y0 + (Ry * sin(t));
        LineTo(hdc, x, y);
        t += tStep;
    } while (t <= 2 * M_PI);
    // Вычисление 3D-точки
    t = M_PI / 4;
    x = x0 + (Rx * cos(t));
    y = y0 + (Ry * sin(t));
    table[i].point = "(" + std::to_string(x) + ", " + std::to_string(y) + ", 0)";
    // Вычисление производной (3D-вектора)
    int dx, dy;
    dx = -1 * Rx * sin(t);
    dy = Ry * cos(t);
    table[i].derivative = "(" + std::to_string(dx) + ", " + std::to_string(dy) + ", 0)";
    SelectObject(hdc, p1);   // Возвращаем старое перо
    DeleteObject(p2);       // Удаляем созданное перо
}

void drawHelix(HDC hdc, int x0, int y0, int R, int z0, double rot, objTable* table, int i)  // Нарисовать спираль и вычислить 3D-точку и первую производную
{
    HPEN p1, p2 = CreatePen(PS_SOLID, 2, RGB(0, 255, 0)); // Зеленое перо
    p1 = (HPEN)SelectObject(hdc, p2);  // Заносим перо в контекст рисования
    int x, y;
    double z = 0;
    srand(time(NULL));
    double zStep = 0.2 + (rand() % 6) / 10;    // Шаг по Z от 0.2 до 0.5 
    double t = 0;
    MoveToEx(hdc, x0 + R, y0, NULL);
    do
    {
        x = x0 + (R * cos(t));
        y = y0 + (R * sin(t) * sin(rot) + z * cos(rot));
        LineTo(hdc, x, y);
        t += tStep;
        z += zStep;
    } while (z < z0);
    // Вычисление 3D-точки
    t = M_PI / 4;
    x = x0 + (R * cos(t));
    y = y0 + (R * sin(t));
    z = zStep * t;
    table[i].point = "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    // Вычисление производной (3D-вектора)
    int dx, dy;
    double dz;
    dx = -1 * R * sin(t);
    dy = R * cos(t);
    dz = zStep;
    table[i].derivative = "(" + std::to_string(dx) + ", " + std::to_string(dy) + ", " + std::to_string(dz) + ")";
    SelectObject(hdc, p1);   // Возвращаем старое перо
    DeleteObject(p2);       // Удаляем созданное перо
}

circleTable* addStruct(circleTable* table, const int amount)    //Добавить элемент в контейнер кругов
{
    if (amount == 0)
        table = new circleTable[amount + 1];
    else
    {
        circleTable* tempTable = new circleTable[amount + 1];
        for (int i = 0; i < amount; i++)
            tempTable[i] = table[i];
        delete[] table;
        table = tempTable;
    }
    return table;
}

void bubbleSort(circleTable* table, int amount) // Сортировка
{
    std::string tempStr;
    int tempRad;
    bool nonsort;
    do {
        nonsort = false;
        for (int i = 0; i < amount - 1; i++)
        {
            if (table[i].radii > table[i + 1].radii)
            {
                nonsort = true;
                tempRad = table[i].radii;
                tempStr = table[i].name;
                table[i].radii = table[i + 1].radii;
                table[i].name = table[i + 1].name;
                table[i + 1].radii = tempRad;
                table[i + 1].name = tempStr;
            }
        }
    } while (nonsort);
}

int main()
{
    HWND hwnd;
    char Title[1024];
    GetConsoleTitle(Title, 1024); // Имя окна
    hwnd = FindWindow(NULL, Title); // hwnd окна
    RECT rc;
    GetClientRect(hwnd, &rc);
    int iWidth = rc.right;
    int iHeight = rc.bottom;
    HDC hdc = GetDC(hwnd);    // Получаем контекст для рисования
    int x0, y0, R, Rx, Ry, Z, count, choise;    // Random
    int tryagain = 0;
    do {
        system("cls");
        srand(time(NULL));
        count = MIN_COUNT + rand() % (MAX_COUNT - MIN_COUNT + 1);    // Случайное количество кривых
        objTable* mainTable = new objTable[count + 1];
        circleTable* circleTab = 0;
        int sumRadii = 0;   // Суммарный радиус
        int circleCount = 0, ellipseCount = 0, helixCount = 0;  // Счетчик фигур
        for (int i = 0; i < count; i++)
        {
            // Случайные координаты и радиус
            x0 = (iWidth / 2) + rand() % (iWidth / 2);
            y0 = rand() % iHeight;
            // Случайная кривая
            choise = rand() % 3;
            if (choise == 0)    // Круг
            {
                circleTab = addStruct(circleTab, circleCount);
                circleCount++;
                R = (iWidth / 30) + rand() % (iWidth / 5 - iWidth / 30 + 1);    // Радиус
                mainTable[i].name = "Circle" + std::to_string(circleCount);
                drawCircle(hdc, x0, y0, R, mainTable, i);
                circleTab[circleCount - 1].name = mainTable[i].name;
                circleTab[circleCount - 1].radii = R;
                sumRadii += circleTab[circleCount - 1].radii;   // 6. Вычислить сумму радиусов всех кривых во втором контейнере
            }
            if (choise == 1)    // Эллипс
            {
                ellipseCount++;
                Rx = (iWidth / 30) + rand() % (iWidth / 5 - iWidth / 30 + 1);   // Радиус по X
                Ry = (iHeight / 30) + rand() % (iHeight / 5 - iHeight / 30 + 1);    // Радиус по Y
                mainTable[i].name = "Ellipse" + std::to_string(ellipseCount);
                drawEllipse(hdc, x0, y0, Rx, Ry, mainTable, i);
            }
            if (choise == 2)    // Спираль
            {
                helixCount++;
                R = (iWidth / 30) + rand() % (iWidth / 5 - iWidth / 30 + 1);    // Радиус
                Z = 20 + rand() % (iHeight / 2 - 19);   // Предел длины спирали по оси Z
                double rotation = (rand() % 629) / 100; // Угол вращения покруг оси X
                mainTable[i].name = "Helix" + std::to_string(helixCount);
                drawHelix(hdc, x0, y0, R, Z, rotation, mainTable, i);
            }
        }
        // Вывод контейнера, содержащего кривые
        std::cout << "List of objects   t=Pi/4" << std::endl << "Object    ||  3D-point    ||  First derivative" << std::endl;
        for (int i = 0; i < count; i++) {
            std::cout << mainTable[i].name << "   " << mainTable[i].point << "   " << mainTable[i].derivative << std::endl;
        }
        std::cout << "Total curves: " << count << std::endl << std::endl;
        // Сортировка контейнера, содержащего круги
        bubbleSort(circleTab, circleCount);
        // Вывод контейнера, содержащего круги
        std::cout << "List of circles" << std::endl;
        if (circleCount == 0)
            std::cout << "No circles generated. Please, try again" << std::endl;
        else
        {
            std::cout << "Object   ||  Radii" << std::endl;
            for (int i = 0; i < circleCount; i++) {
                std::cout << circleTab[i].name << "     " << circleTab[i].radii << std::endl;
            }
        }
        std::cout << "Total sum of circle radii: " << sumRadii<<std::endl; // Суммарный радиус
        std::cout << "Try again? (1 - yes, 0 - no): ";
        std::cin >> tryagain;
        std::cin.get();
    } while (tryagain != 0);
    ReleaseDC(hwnd, hdc);    // Освобождаем контекст рисования
    return 0;
}