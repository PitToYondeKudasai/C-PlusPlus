#include <curses.h>
#include <vector>
#include <string>

#define WALL '#'
#define PLAYER '='
#define LASER '*'
#define ENEMY 'O'
#define EMPTY ' '

bool gameOver;
int x, y;
int x_old, y_old;
int score;

const int height = 20, width = 30;

void draw_window(int x_pos, int y_pos){
    mvhline(y_pos, x_pos, WALL, width);
    mvvline(y_pos, x_pos, WALL, height);
    mvvline(y_pos , x_pos + width, WALL, height);
}

class laser_beam{
    public:
        int x_laser = 0, y_laser = 0;
        bool hit = false;

        void update(){
            if((y_laser - 1) > 1){
                y_laser -= 1;
            } else {
                y_laser = -10;
            }
        }
};

class enemy{
    public:
        int x_enemy, y_enemy;
        bool right = true;
        bool destroyed = false;

        void update(){
            /*
            if (right) {
                x_enemy += 1;
                right = false;
            } else {
                x_enemy -= 1;
                right = true;
            }
            */
        }
};

class Character{
    private:
        std::vector<laser_beam> lasers;
        std::vector<enemy> enemies;

        void update_by_ref(laser_beam & las, bool hit){
            if(hit){
                las.y_laser = -10;
            } else {
                las.y_laser -= 1;
            }
        }

        void destroy_by_ref(bool & destroyed){
            destroyed = true;
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

        void hit_enemy(laser_beam & las){
            for(int i = 0; i < enemies.size(); i++){
                if(!enemies[i].destroyed){
                    if((enemies[i].x_enemy == las.x_laser) && (enemies[i].y_enemy == (las.y_laser + 1))){
                        destroy_by_ref(enemies[i].destroyed);
                        update_by_ref(las, true);
                        score += 1;
                    }
                }
            }
        }

    public:
        void initialise_enemies(){
            for(int i = 3; i < 27; i++){
                enemy e;
                e.x_enemy = i;
                e.y_enemy = 3;
                enemies.push_back(e);
                e.x_enemy = i;
                e.y_enemy = 4;
                enemies.push_back(e);
            }
        }

        void draw(){
            mvaddch(y, x, PLAYER);
            mvaddch(y_old, x_old, EMPTY);

            for(enemy en : enemies){
                if(en.destroyed){
                    mvaddch(en.y_enemy, en.x_enemy, EMPTY);
                } else {
                    mvaddch(en.y_enemy, en.x_enemy, ENEMY);
                }
            }

            for(laser_beam las : lasers){
                if(las.y_laser != 0)
                    mvaddch(las.y_laser, las.x_laser, LASER);
                if(las.y_laser + 1 != 0)
                    mvaddch((las.y_laser + 1), las.x_laser, EMPTY);
            }

            mvprintw(22, 15, "%d",score);
            move(y, x);
        }

        void update(){
            for(int i = 0; i < lasers.size(); i++){
                update_by_ref(lasers[i], false);
                hit_enemy(lasers[i]);
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
    score = 0;
    Character spaceship;
    spaceship.initialise_enemies();

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
