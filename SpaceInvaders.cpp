#include <curses.h>
#include <vector>

#define WALL '#'
#define PLAYER '='
#define LASER '*'
#define EMPTY ' '

bool gameOver;
int x, y;
int x_old, y_old;

const int height = 20, width = 30;

void draw_window(int x_pos, int y_pos){
    mvhline(y_pos, x_pos, WALL, width);
    mvvline(y_pos, x_pos, WALL, height);
    mvvline(y_pos , x_pos + width, WALL, height);
}

class laser_beam{
    public:
        int x_laser = 0, y_laser = 0;

        void update(){
            if((y_laser - 1) > 1){
                y_laser -= 1;
            } else {
                y_laser = 100;
            }
        }
};

class Character{
    private:
        std::vector<laser_beam> lasers;

        void update_by_ref(int & y_laser){
            y_laser -= 1;
        }
    
        void move_right(){
            if ((x + 1) < width){
                x_old = x;
                x += 1;
            }
        }

        void move_left(){
            if ((x-1) > 0){
                x_old = x;
                x -= 1;
            }
        }

        void fire(){
            laser_beam las;
            las.x_laser =  x;
            las.y_laser = y - 1;
            lasers.push_back(las);
        }

    public:
        void draw(){
            mvaddch(y, x, PLAYER);
            mvaddch(y_old, x_old, EMPTY);

            for(laser_beam las : lasers){
                if(las.y_laser != 0)
                    mvaddch(las.y_laser, las.x_laser, LASER);
                if((las.y_laser + 1) != 0)
                    mvaddch((las.y_laser + 1), las.x_laser, EMPTY);
            }

            move(y, x);
        }

        void update(){
            for(int i = 0; i < lasers.size(); i++){
                update_by_ref(lasers[i].y_laser);
            }

            int ch;
            ch = getch();
            switch (ch){
                case 'q':
                    gameOver = true;
                    break;
                case 'x':
                    fire();
                    break;
                case KEY_RIGHT:
                    move_right();
                    break;
                case KEY_LEFT:
                    move_left();
                    break;
            }
        }
};

class Enemy{
    int x_enemy, y_enemy;
    bool right = true;

    public:
        void update(){
            if (right) {
                x_enemy += 1;
                right = false;
            } else {
                x_enemy -= 1;
                right = true;
            }
        }
};

int main(){
    // Initialise cursers
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();

    x = 10;
    y = 20;
    x_old = x - 1;
    y_old = y;
    Character spaceship;

    draw_window(0, 0);
    spaceship.draw();
    int ch;
    
    while(!gameOver){
        timeout(100);
        spaceship.update();
        spaceship.draw();
        refresh();
    }
    endwin(); // Close the curses window 

    return 0;
}