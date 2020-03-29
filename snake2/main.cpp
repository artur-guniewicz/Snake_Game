#include <iostream>
#include <conio.h>
#include <windows.h>
#include <dos.h>
#include <time.h>
#include <cstdio>

#define MAXSNAKESIZE 100
//#define MAXFRAMEX 119
//#define MAXFRAMEY 29

using namespace std;

int MAXFRAMEX = 119;
int MAXFRAMEY = 29;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // standard output handle (basic: it is an active buffer of console screen)
COORD CursorPosition; //structure that defines the coordinates of a character cell in a console screen buffer (basic: (0,0))

// typedef struct _COORD
// {
//   SHORT X;
//   SHORT Y;
// } COORD, *PCOORD;

void gotoxy(int x, int y)
{
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition); // sets the cursor position in the specified console screen buffer
}

void setcursor(bool visible, DWORD size)
{
    if(size == 0)
    {
        size = 20;
    }

    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible; // if visible = TRUE (1) -> visible
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}

// typedef struct _CONSOLE_CURSOR_INFO
// {
//   DWORD dwSize;
//   BOOL  bVisible;
// } CONSOLE_CURSOR_INFO, *PCONSOLE_CURSOR_INFO;


class Point
{
private:
    int x;
    int y;

public:
    Point()
    {
        x = y = 10;
    }

    Point(int x, int y)
    {
        this -> x = x;
        this -> y = y;
    }

    void SetPoint(int x, int y)
    {
        this -> x = x;
        this -> y = y;
    }

    int GetX()
    {
        return x;
    }

    int GetY()
    {
        return y;
    }

    void MoveUp()
    {
        y--;
        if(y < 0)
        {
            y = MAXFRAMEY;
        }
    }

    void MoveDown()
    {
        y++;
        if(y > MAXFRAMEY)
        {
            y = 0;
        }
    }

    void MoveLeft()
    {
        x--;
        if(x < 0)
        {
            x = MAXFRAMEX;
        }
    }

    void MoveRight()
    {
        x++;
        if(x > MAXFRAMEX)
        {
            x  = 0;
        }
    }

    void Draw(char ch = 'O')
    {
        gotoxy(x,y);
        cout << ch;
    }

    void Erase()
    {
        gotoxy(x,y);
        cout << " ";
    }

    void CopyPos(Point *p)
    {
        p -> x = x;
        p -> y = y;
    }

    int IsEqual(Point *p)
    {
        if(p -> x == x && p -> y == y)
            return 1;

        return 0;
    }

    void Debug()
    {
        cout << "(" << x << "," << y << ")";
    }
};

class Snake
{
private:
    Point *cell[MAXSNAKESIZE]; // array of points to represent snake
    int size; // current size of snake
    char dir; // current direction of snake
    Point fruit;
    int state; // bool representing of snake i.e. living, dead
    int started;
    int blink; // fruit blink
    int difficulty;

public:
    Snake()
    {
        size = 1; // default size
        cell[0] = new Point(20,5); // 20,5 is default position
        for(int i = 1; i < MAXSNAKESIZE; i++)
        {
            cell[i] = NULL;
        }

        fruit.SetPoint(rand()%MAXFRAMEX, rand()%MAXFRAMEY);
        state = 0; // 0 - is dead, 1 - is alive
        started = 0; // 0 - game is not running, 1 - game is running
        difficulty = 100;
    }

    void AddCell(int x, int y)
    {
        cell[size++] = new Point(x,y);
    }

    void TurnUp()
    {
        if(dir != 's') dir = 'w';
    }

    void TurnDown()
    {
        if(dir != 'w') dir = 's';
    }

    void TurnLeft()
    {
        if(dir != 'd') dir = 'a';
    }

    void TurnRight()
    {
        if(dir != 'a') dir = 'd';
    }

    void GameOver()
    {
        cout    <<  "   ___   ___  ___  ___  ____      ___   __ __  ____ ____ "             << endl
                <<  "  // \\\\ // \\\\ ||\\\\//|| ||        // \\\\  || || ||    || \\\\"   << endl
                <<  " (( ___ ||=|| || \\/ || ||==     ((   )) \\\\ // ||==  ||_//"          << endl
                <<  "  \\\\_|| || || ||    || ||___     \\\\_//   \\V/  ||___ || \\\\"      << endl;
    }

    void WelcomeScreen()
    {
        SetConsoleTextAttribute(console, 15);

        cout    << "           " <<  " _____  _   _   ___   _   __ _____"           << endl
                << "           " <<  "/  ___|| \\ | | / _ \\ | | / /|  ___|"        << endl
                << "           " <<  "\\ `--. |  \\| |/ /_\\ \\| |/ / | |_"         << endl
                << "           " <<  " `--. \\| . ` ||  _  ||    \\ |  __|"         << endl
                << "           " <<  "/\\__/ /| |\\  || | | || |\\  \\| |___ "      << endl
                << "           " <<  "\\____/ \\_| \\_/\\_| |_/\\_| \\_/\\____/ "   << endl
                << "           " <<  "       ---_ ......._-_--."                    <<  endl
                << "           " <<  "      (|\\ /      / /| \\  \\."               <<  endl
                << "           " <<  "      /  /     .'  -=-'   `."                 <<  endl
                << "           " <<  "     /  /    .'             )"                <<  endl
                << "           " <<  "   _/  /   .'        _.)   /"                 <<  endl
                << "           " <<  "  / o   o        _.-' /  .'"                  <<  endl
                << "           " <<  "  \\          _.-'    / .'*|"                 <<  endl
                << "           " <<  "   \\______.-'//    .'.' \\*|"                <<  endl
                << "           " <<  "    \\|  \\ | //   .'.' _ |*|"                <<  endl
                << "           " <<  "     `   \\|//  .'.'_ _ _|*|"                 <<  endl
                << "           " <<  "      .  .// .'.' | _ _ \\*|"                 <<  endl
                << "           " <<  "      \\`-|\\_/ /    \\ _ _ \\*\\."           <<  endl
                << "           " <<  "       `/'\\__/      \\ _ _ \\*\\."           <<  endl
                << "           " <<  "      /^|            \\ _ _ \\*"              <<  endl
                << "           " <<  "     '  `             \\ _ _ \\."             <<  endl
                << "           " <<  "                       \\_"                   <<  endl;
    }

    void StartMenu()
    {
        menu:

        system("cls");

        cout    << "SNAKE!" << endl
                << "1. START" << endl
                << "2. OPTIONS" << endl
                << "3. EXIT" << endl;

        switch(getch())
        {
        case '1':
            break;

        case '2':
            options:
            system("cls");

            cout    << "OPTIONS:" << endl
                    << "1. DIFFICULTY" << endl
                    << "2. SIZE OF MAP" << endl
                    << "3. BACK TO MENU" << endl;

            switch(getch())
            {
            case '1':
                system("cls");

                cout    << "DIFFICULTY:" << endl
                        << "1. EASY" << endl
                        << "2. MEDIUM" << endl
                        << "3. HARD" << endl
                        << "4. PRO" << endl
                        << "5. IMPOSSIBLE" << endl
                        << "6. BACK TO OPTIONS" << endl;

                switch(getch())
                {
                case '1':
                    difficulty = 200;
                    break;

                case '2':
                    difficulty = 100;
                    break;

                case '3':
                    difficulty = 50;
                    break;

                case '4':
                    difficulty = 20;
                    break;

                case '5':
                    difficulty = 1;
                    break;

                case '6':
                    goto options;
                    break;
                }
                goto options;
                break;

            case '2':
                system("cls");

                cout    << "SIZE OF MAP:" << endl
                        << "1. SMALL" << endl
                        << "2. REGULAR" << endl
                        << "3. BIG" << endl
                        << "4. BACK TO OPTIONS" << endl;

                switch(getch())
                {
                case '1':
                    MAXFRAMEX = 39;
                    MAXFRAMEY = 9;
                    fruit.SetPoint(rand()%MAXFRAMEX, rand()%MAXFRAMEY);
                    break;

                case '2':
                    MAXFRAMEX = 119;
                    MAXFRAMEY = 29;
                    fruit.SetPoint(rand()%MAXFRAMEX, rand()%MAXFRAMEY);
                    break;

                case '3':
                    MAXFRAMEX = 219;
                    MAXFRAMEY = 49;
                    fruit.SetPoint(rand()%MAXFRAMEX, rand()%MAXFRAMEY);
                    break;

                case '4':
                    goto options;
                    break;
                }
                goto options;
                break;

            case '3':
                goto menu;
                break;
            }
            break;

        case '3':
            exit(1);
            break;
        }
    }

    void Move()
    {
        // clear screen
        system("cls");

        if(state == 0)
        {
            if(!started)
            {
                WelcomeScreen();
                cout << "\n\nPress any key to start";
                getch();
                StartMenu();
                started = 1;
                state = 1;
            }
            else
            {
                GameOver();
                cout    << "\nYour score: " << size-1 << endl
                        << "\nPress any key to restart" << endl
                        << "or \"e\" to back to menu..." << endl;

                switch(getch())
                {
                case 'e':
                    StartMenu();
                    break;
                default:
                    break;
                }

                state = 1;
                size = 1;
            }
        }

        // making snake body follow its head
        for(int i = size-1; i > 0; i--)
        {
            cell[i-1] -> CopyPos(cell[i]);
        }

        // turning snake's head
        switch(dir)
        {
        case 'w':
            cell[0] -> MoveUp();
            break;

        case 's':
            cell[0] -> MoveDown();
            break;

        case 'a':
            cell[0] -> MoveLeft();
            break;

        case 'd':
            cell[0] -> MoveRight();
            break;

        default:
            break;
        }

        if(SelfCollision())
            state = 0;

        // collision with fruit
        if(fruit.GetX() == cell[0] -> GetX() && fruit.GetY() == cell[0] -> GetY())
        {
            AddCell(0,0);
            fruit.SetPoint(rand()%MAXFRAMEX, rand()%MAXFRAMEY);
        }

        // drawing snake
        for(int i = 0; i < size; i++)
            cell[i] -> Draw();

        SetConsoleTextAttribute(console, 242);

        fruit.Draw('#'); // fruit without blinking

      /*  if(!blink)
        {
            fruit.Draw('#');
        }
        blink = !blink; */

        SetConsoleTextAttribute(console, 252);

        Sleep(difficulty); // speed of snake
    }

    int SelfCollision()
    {
        for(int i = 1; i < size; i++)
        {
            if(cell[0] -> IsEqual(cell[i]))
                return 1;
        }

        return 0;
    }

    void Debug()
        {
            for(int i = 0; i < size; i++)
            {
                cell[i] -> Debug();
            }
        }
};

int main()
{
    setcursor(0,0);
    srand((unsigned)time(NULL));

    Snake snake;
    char op = '1';
    do
    {
        if(kbhit())
        {
            op = getch();
        }

        switch(op)
        {
        case 'w':
        case 'W':
            snake.TurnUp();
            break;

        case 's':
        case 'S':
            snake.TurnDown();
            break;

        case 'a':
        case 'A':
            snake.TurnLeft();
            break;

        case 'd':
        case 'D':
            snake.TurnRight();
            break;

        default:
            break;
        }

        snake.Move();
    }
    while(op!='e');


    return 0;
}
