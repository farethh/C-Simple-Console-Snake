#include <iostream>
#include <windows.h>
#include <cstdio>
#include <conio.h>
#include <time.h>

using namespace std;

char event_direction()  /// Zdarzenie czy został zmieniony kierunek
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            return '1';
        case 's':
            return '2';
        case 'd':
            return '3';
        case 'w':
            return '4';
        default:
            return 5;

        }
    }

}

void clear_screen()
{
    COORD cursorPosition;   cursorPosition.X = 0;   cursorPosition.Y = 0;   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void move_direction(char num, COORD& head, int snake_lenght, COORD tail[], HANDLE hConsole, bool& game, int& back) /// Poruszanie sie
{
    for (int i = snake_lenght; i > 0; i--)
        tail[i] = tail[i - 1];

    tail[0] = head;

    switch (num) /// zakaz cofania 
    {
    case '1':
        if (back == 3)
        {
            head.X++;
            break;

        }
        head.X--;
        back = 4;
        break;
    case '2':
        if (back == 1)
        {
            head.Y--;
            break;
        }
        head.Y++;
        back = 2;
        break;
    case '3':
        if (back == 4)
        {
            head.X--;
            break;

        }
        head.X++;
        back = 3;
        break;
    case '4':
        if (back == 2)
        {
            head.Y++;
            break;
        }
        head.Y--;
        back = 1;
        break;
    default:
        break;
    }
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), tail[0]);
    SetConsoleTextAttribute(hConsole, 235);
    cout << "S";
    for (int i = 0; i < snake_lenght - 1; i++)
    {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), tail[i + 1]);
        SetConsoleTextAttribute(hConsole, 170);
        cout << "S";
        SetConsoleTextAttribute(hConsole, 7);

    }
    /// czy byla kolizja
    for (int i = 0; i < snake_lenght; i++)
    {
        if ((tail[i].Y == head.Y) && (tail[i].X == head.X))
            game = false;
    }
}

void out_of_map(COORD &head, bool& game) /// czy gracz jest poza mapą
{   
    if (head.X > 38 ) head.X = 15;
    if ( head.X < 15 ) head.X = 38;
    if (head.Y < 2) head.Y = 29;
    if ( head.Y > 29) head.Y = 2;
}

void draw_map(HANDLE hConsole)  /// rysowanie mapy
{
    cout << "\n              ";
    SetConsoleTextAttribute(hConsole, 153);
    cout << "##########################";
    SetConsoleTextAttribute(hConsole, 7);

    for (int i = 0; i < 28; i++)
    {
        SetConsoleTextAttribute(hConsole, 7);
        cout << "\n              ";
        SetConsoleTextAttribute(hConsole, 153);
        cout << "#";
        SetConsoleTextAttribute(hConsole, 7);
        cout << "                        ";
        SetConsoleTextAttribute(hConsole, 153);
        cout << "#";
    }
    SetConsoleTextAttribute(hConsole, 7);
    cout << "\n              ";
    SetConsoleTextAttribute(hConsole, 153);
    cout << "##########################";
}

void game_over(COORD score) /// koniec gry / ile punktow
{
    score.X = 67; score.Y = 13;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), score);
    cout << "GAME OVER";
    score.X = 10; score.Y = 32;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), score);
}

void gen_fruit(COORD& fruit, COORD head, HANDLE hConsole, int& points, int& snake_lenght, double& time_sleep, COORD tail[]) /// losowanie owoca / tworzenie  nowego
{
    for (int i = 0; i < snake_lenght; i++)
    {
        if ((tail[i].Y == fruit.Y) && (tail[i].X == fruit.X))
        {
            fruit.X = rand() % 22 + 15;
            fruit.Y = rand() % 27 + 2;
            i = -1;
        }
    }

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), fruit);
    SetConsoleTextAttribute(hConsole, 204);
    cout << "O";
    SetConsoleTextAttribute(hConsole, 7);

    if ((head.X == fruit.X) && (head.Y == fruit.Y))
    {
        fruit.X = rand() % 22 + 15;
        fruit.Y = rand() % 27 + 2;
        snake_lenght++;
        points++;
        time_sleep *= 0.99;

    }
}

void points_val(int points, COORD score) /// wynik na prawo
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), score);
    cout << "SCORE: " << points;
}

int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    srand(time(NULL));

    COORD tail[200];

    COORD head;
    head.X = rand() % 20 + 16;
    head.Y = rand() % 26 + 3;

    COORD fruit;
    fruit.X = rand() % 22 + 15;
    fruit.Y = rand() % 27 + 2;

    COORD score;
    score.X = 60;
    score.Y = 15;

    DWORD t1, t2;
    t1 = GetTickCount64();
   
    int back = 2, snake_lenght = 1, points = 0;
    bool game = true;
    char num = '1' + rand() %4;
    double time_sleep = 60;

    do
    {
        clear_screen();
        draw_map(hConsole);
        out_of_map(head, game);

        if (_kbhit())
            num = event_direction();

        move_direction(num, head, snake_lenght, tail, hConsole, game, back);

        gen_fruit(fruit, head, hConsole, points, snake_lenght, time_sleep, tail);
        points_val(points, score);
        Sleep(time_sleep);

        t2 = GetTickCount64() - t1;
        cout <<"      TIME:  " << t2 / CLOCKS_PER_SEC;

    } while (game);
    game_over(score);
}
