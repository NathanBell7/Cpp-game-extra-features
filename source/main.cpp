#include <nds.h>

#include <stdio.h>

#include<gl2d.h>

#include<string>

#include <list>

#include <cmath>

using namespace std;

//256x192 pixels

volatile int frame = 0;

//----------------------------------------player class----------------------------------------//


class Player{

    private:

        int GRAVITY = 10;
        int velocity = 0;
        int velocity_increment = 1;
        bool moving_right = false;
        bool moving_left = false;
        bool jumping = false;
        bool falling = false;
        int x_position_centre;
        int y_position_centre;
        int x_position_left;
        int x_position_right;
        int y_position_top;
        int y_position_bottom;
        bool on_platform = true;
        char direction_facing ;


    public:

        Player(int x, int y, char facing){

            this->x_position_centre = x;
            this->x_position_left = x_position_centre-16;
            this->x_position_right = x_position_centre+16;

            this->y_position_centre = y;
            this->y_position_top = y_position_centre-16;
            this->y_position_bottom = y_position_centre+16;

            this->direction_facing = facing;

        }

        bool get_falling(){
            return falling;
        }

        int get_y_position_top(){
            return y_position_top;
        }

        int get_y_position_bottom(){
            return y_position_bottom;
        }

        int get_x_position_left(){
            return x_position_left;
        }

        int get_x_position_right(){
            return x_position_right;
        }

        int get_centre_x(){
            return x_position_centre;
        }

        int get_centre_y(){
            return y_position_centre;
        }

        bool get_jumping(){
            return jumping;
        }

        void display_position(){

            glBegin2D();/*opens gl for 2d creation*/

            glBoxFilled(x_position_left,y_position_top,x_position_right,y_position_bottom,RGB15(255, 255, 0));

            glEnd2D();/*ends gl for 2d creation*/

        }

        void set_all_y(int new_value){

            y_position_centre = new_value;
            y_position_top = new_value-16;
            y_position_bottom = new_value+16;


        }

        void set_falling(bool new_value){
            falling = new_value;
        }

        void set_all_x(int new_value){
            x_position_centre = new_value;
            x_position_left = new_value-16;
            x_position_right = new_value+16;
        }

         void update_jump_action(){
            if ((falling == false) && (jumping == false)){
                //onlu jump when not falling or jumping
                jumping = true;
                velocity = GRAVITY;

            }
        }

        void update_move_right_action(bool new_value){

            if(moving_left==false){
                moving_right = new_value;
                direction_facing = 'r';
            }    
        }

        void update_move_left_action(bool new_value){

            if(moving_right==false){
                moving_left = new_value;
                direction_facing = 'l';
            }
        }

        void character_movement(){

            if(moving_left == true){
                set_all_x(x_position_centre-4);
            }

            if(moving_right == true){
                set_all_x(x_position_centre+4);
            }

            if (jumping == true){
                set_all_y(y_position_centre-velocity);
                velocity -= velocity_increment;
                //if jump hits peak then it is falling
                if (velocity == 0){
                    jumping = false;
                    falling = true;
                }
            }

            if (falling == true){
                set_all_y(y_position_centre-velocity);
                velocity -= velocity_increment;
            }
        }

};



//----------------------------------------projectile class----------------------------------------//


class Projectile{

    private:

    int projectile_speed;
    char direction_facing;
    int x_position_centre;
    int x_position_left;
    int x_position_right;
    int y_position_centre;
    int y_position_top;
    int y_position_bottom;
    int damage;
    int width;
    int height;
    bool exist = true;



    public:

    Projectile(int projectile_speed, char direction_facing, int x_position_centre, int y_position_centre, int width, int height, int damage){

        if(direction_facing == 'r'){
            this->projectile_speed = projectile_speed;
        }
        else{
            this->projectile_speed = -(projectile_speed);
        }
        
        this->direction_facing = direction_facing;
        this->x_position_centre = x_position_centre;
        this->x_position_left = x_position_centre - (width/2);
        this->x_position_right = x_position_centre + (width/2);

        this->y_position_centre = y_position_centre;
        this->y_position_top = y_position_centre - (height/2);
        this->y_position_bottom = y_position_centre + (height/2);

        this->damage = damage;

    }


    void display_projectile(){

        glBegin2D();/*opens gl for 2d creation*/

        glBoxFilled(x_position_left,y_position_top,x_position_right,y_position_bottom,RGB15(255, 255, 0));

        glEnd2D();/*ends gl for 2d creation*/

    }


    void update_position(){

        x_position_centre += projectile_speed;
        x_position_left = x_position_centre - (width/2);
        x_position_right = x_position_centre + (width/2);

        if ((x_position_centre > 256)|(x_position_centre < 0)){
            exist = false;
        } 

    }

    int get_x_position_centre(){
        return x_position_centre;
    }

    int get_x_position_left(){
        return x_position_left;
    }

    int get_x_position_right(){
        return x_position_right;
    }

    int get_y_position_centre(){
        return y_position_centre;
    }

    int get_y_position_top(){
        return y_position_top;
    }

    int get_y_position_bottom(){
        return y_position_bottom;
    }

    int get_projectile_speed(){
        return projectile_speed;
    }

    bool get_exist(){
        return exist;
    }

    int get_damage(){
        return damage;
    }

    void set_exist(bool new_value){
        exist = new_value;
    }


};


//----------------------------------------ranged weapon class----------------------------------------//

class Weapon{

    private:

        //times are frame based

        int projectile_speed;
        int damage;
        int projectile_delay;
        int reload_time;
        int time_until_next_projectile = 0;
        bool reloading = false;
        int projectile_capacity;
        int x_position_centre;
        int y_position_centre;
        int current_projectile_amount;
        string weapon_type;
        char direction_facing;
        int time_until_reloaded;


    public:

        Weapon(int projectile_speed, int damage, int projectile_delay, int reload_time, int direction_facing,
         int projectile_capacity, int x_position_centre, int y_position_centre, string weapon_type){

            this->projectile_speed = projectile_speed;
            this->damage = damage;
            this->projectile_delay = projectile_delay;
            this->reload_time = reload_time;
            this->direction_facing = direction_facing;
            this->projectile_capacity = projectile_capacity;
            this->current_projectile_amount = projectile_capacity;

            this->x_position_centre = x_position_centre;
            this->y_position_centre = y_position_centre;

            this->weapon_type = weapon_type;

        }

        int get_time_until_next_projectile(){
            return time_until_next_projectile;
        }

        int get_time_until_reloaded(){
            return time_until_reloaded;
        }

        int get_current_projectile_amount(){
            return current_projectile_amount;
        }

        bool get_reloading(){
            return reloading;
        }

        void set_damage(int new_value){
            damage = new_value;
        }

        void update_time_until_reloaded(int new_time){
            time_until_reloaded = new_time;
        }

        void update_time_until_next_projectile(int new_time){
            time_until_next_projectile = new_time;
        }

        void move_weapon(int new_x, int new_y){

            if (new_x != x_position_centre){

                if(new_x > x_position_centre){
                    direction_facing = 'r';
                }
                
                else{
                    direction_facing = 'l';
                }
            }
            
            x_position_centre = new_x;
            
            y_position_centre = new_y;

        }

        void start_reload_timer(){
            reloading = true;
            time_until_reloaded = reload_time;
        }

        void reload_magazine(){
            current_projectile_amount = projectile_capacity;
            reloading = false;
        }

        bool shoot_projectile(){

            bool shooting = false;

            if ((time_until_next_projectile == 0)&(current_projectile_amount > 0)&(reloading == false)){
                //Projectile projectile(int x_position, int y_position, int damage, int projectile_speed, char direction_facing);
                time_until_next_projectile = projectile_delay;
                current_projectile_amount -= 1;
                shooting = true;
            }

            return shooting;

        }


        Projectile* create_projectile(){

            Projectile *new_projectile = new Projectile(projectile_speed,direction_facing, x_position_centre,y_position_centre,10,5,damage);

            return new_projectile;


        }

};

//----------------------------------------platform class----------------------------------------//

class Platform{

    private:

        int x_position_centre;
        int y_position_centre;
        int x_position_left;
        int x_position_right;
        int y_position_top;
        int y_position_bottom;

    public:

    Platform(int x_centre, int y_centre, int width, int height){
        this->x_position_centre = x_centre;
        this->x_position_left = x_position_centre-(width/2);
        this->x_position_right = x_position_centre+(width/2);

        this->y_position_centre = y_centre;
        this->y_position_top = y_position_centre-(height/2);
        this->y_position_bottom = y_position_centre+(height/2);
    }

    bool detect_collision_player(Player player){
        bool contact = false;
        // if player's bottom is inside platform
        if ((player.get_x_position_left()<x_position_right)
            &(player.get_x_position_right()>x_position_left)
            &(player.get_y_position_bottom()>y_position_top)
            &(player.get_y_position_bottom()<y_position_bottom)){
                contact = true;
            }
        
        return contact;
    }

    int get_y_position_top(){
        return y_position_top;
    }

    int get_y_position_bottom(){
        return y_position_bottom;
    }

    int get_x_position_left(){
        return x_position_left;
    }

    int get_x_position_right(){
        return x_position_right;
    }

    void display_position(){

        glBegin2D();/*opens gl for 2d creation*/

        glBoxFilled(x_position_left,y_position_top,x_position_right,y_position_bottom,RGB15(255, 0, 0));

        glEnd2D();/*ends gl for 2d creation*/

    }

};


//----------------------------------------cursor class----------------------------------------//


class Cursor{

    private:

    int x_position_centre;
    int y_position_centre;
    int x_position_left;
    int x_position_right;
    int y_position_top;
    int y_position_bottom;
    bool moving_left = false;
    bool moving_right = false;

    public:

        Cursor(int x, int y){

            this->x_position_centre = x;
            this->y_position_centre = y;

            this->x_position_left = x_position_centre-20;
            this->x_position_right = x_position_centre+20;

            this->y_position_top = y - 5;
            this->y_position_bottom = y + 5;
            
        }

        int get_x_position_centre(){
            return x_position_centre;
        }

        void set_all_x(int new_value){
            x_position_centre = new_value;
            x_position_left = new_value-20;
            x_position_right = new_value+20;
        }

        void update_move_left(bool new_value){
            moving_left = new_value;
        }

        void update_move_right(bool new_value){
            moving_right = new_value;
        }

        void display_position(){

            glBegin2D();/*opens gl for 2d creation*/

            glBoxFilled(x_position_left,y_position_top,x_position_right,y_position_bottom,RGB15(0, 0, 255));

            glEnd2D();/*ends gl for 2d creation*/

        }

        void movement(){
            //if cursor is on middle or right option, then can move left
            if ((moving_left==true) & ((x_position_centre == 128)|(x_position_centre == 198))){
                set_all_x(x_position_centre-70);
                moving_left = false;
            }

            //if cursor is on left or middle option, then can move right
            if ((moving_right==true) & ((x_position_centre == 58)|(x_position_centre == 128))){
                set_all_x(x_position_centre+70);
                moving_right = false;
            }

        }
        
};

//----------------------------------------wall class----------------------------------------//

class Wall{

private:

    int x_position_centre;
    int y_position_centre;
    int x_position_left;
    int x_position_right;
    int y_position_top;
    int y_position_bottom;


public:

    Wall(int x, int y, int width, int height){

        this->x_position_centre = x;
        this->y_position_centre = y;
        this->x_position_left = x_position_centre - (width/2);
        this->x_position_right = x_position_centre + (width/2);
        this->y_position_top = y_position_centre - (height/2);
        this->y_position_bottom = y_position_centre + (height/2);

    }


    int get_y_position_top(){
        return y_position_top;
    }

    int get_y_position_bottom(){
        return y_position_bottom;
    }

    int get_x_position_left(){
        return x_position_left;
    }

    int get_x_position_right(){
        return x_position_right;
    }


    void display_position(){

        glBegin2D();/*opens gl for 2d creation*/

        glBoxFilled(x_position_left,y_position_top,x_position_right,y_position_bottom,RGB15(0, 0, 255));

        glEnd2D();/*ends gl for 2d creation*/

    }


    bool detect_collision_player(Player player){
        bool contact = false;
        // if player's bottom is inside platform
        if ((player.get_x_position_left()<x_position_right)
            &(player.get_x_position_right()>x_position_left)){
                contact = true;
            }
        
        return contact;
    }

};

//----------------------------------------enemy class----------------------------------------//

class Enemy{


    private:

        int x_position_centre;
        int x_position_left;
        int x_position_right;
        int y_position_top;
        int y_position_bottom;
        int y_position_centre;
        int width;
        int height;
        bool moving = false;
        int GRAVITY = 7;
        int speed;
        bool jumping = true;
        bool falling = false;
        int velocity;
        int velocity_increment = 1;
        bool exist = true;
        int health;
        int damage;



    public:

        Enemy(int x,int y, int width, int height, int speed, int damage, int health){

            this->width = width;
            this->height = height;

            this->x_position_centre = x;
            this->x_position_left = x_position_centre - (width/2);
            this->x_position_right = x_position_centre + (width/2);
            

            this->y_position_centre = y;
            this->y_position_top = y-(height/2);
            this->y_position_bottom = y+(height/2);

            this->speed = speed;
            this->velocity = GRAVITY;

            this->damage = damage;
            this->health = health;

        }

        void update_all_y(int new_y){

            y_position_centre = new_y;
            y_position_top = new_y-(height/2);
            y_position_bottom = new_y+(height/2);

        }

        void update_all_x(int new_x){

            x_position_centre = new_x;
            x_position_left = x_position_centre - (width/2);
            x_position_right = x_position_centre + (width/2);

        }

        void movement_calculations(int frame, bool move_horizontally){

            if (frame%2 == 0){
                
                if (move_horizontally == true){
                    update_all_x(x_position_centre+speed);
                }
                

                if (jumping == true){
                    update_all_y(y_position_centre-velocity);
                    velocity -= velocity_increment;
                    //if jump hits peak then it is falling
                    if (velocity == 0){
                        jumping = false;
                        falling = true;
                    }
                }

                if (falling == true){
                    update_all_y(y_position_centre-velocity);
                    velocity -= velocity_increment;
                    if (velocity == -(GRAVITY)-1){
                        falling = false;
                        jumping = true;
                        velocity = GRAVITY;
                    }
                }
            }
            
        }

        void display_position(){

            glBegin2D();/*opens gl for 2d creation*/

            glBoxFilled(x_position_left,y_position_top,x_position_right,y_position_bottom,RGB15(0, 0, 255));

            glEnd2D();/*ends gl for 2d creation*/

        }


        int get_x_position_centre(){
            return x_position_centre;
        }

        int get_x_position_left(){
            return x_position_left;
        }

        int get_x_position_right(){
            return x_position_right;
        }

        int get_y_position_centre(){
            return y_position_centre;
        }

        int get_y_position_top(){
            return y_position_top;
        }

        int get_y_position_bottom(){
            return y_position_bottom;
        }

        bool get_exist(){
            return exist;
        }

        int get_width(){
            return width;
        }

        int get_damage(){
            return damage;
        }

        void set_health(int new_value){
            health = new_value;
        }

        int get_health(){
            return health;
        }

        void set_exist(bool new_value){
            exist = new_value;
        }

        void set_all_x(int new_value){
            x_position_centre = new_value;
            x_position_left = new_value-16;
            x_position_right = new_value+16;
        }

};

void Vblank() {
	frame++;
}



//----------------------------------------main menu method----------------------------------------//



int menu(){

    videoSetMode(MODE_5_3D);//set video mode 

    consoleDemoInit();

    glScreen2D();

    lcdMainOnTop();

    bool running = true;

    Cursor cursor(58,144);

    while(running){

        cursor.display_position();

        scanKeys();

        if(keysDown() & KEY_A){
            running = false;
        }

        //check if left key is pressed

        if(keysDown() & KEY_LEFT){
            cursor.update_move_left(true);
        }


        if(keysUp() & KEY_LEFT){
            cursor.update_move_left(false);
        }

        //check if right key is pressed

        if(keysDown() & KEY_RIGHT){
            cursor.update_move_right(true);
        }


        if(keysUp() & KEY_RIGHT){
            cursor.update_move_right(false);
        }


        cursor.movement();//tries to move cursor

        glFlush(0);

        swiWaitForVBlank();//wait for next frame

        consoleClear();//clear bottom screen of text

    }

    int cursor_location = cursor.get_x_position_centre();

    int choice;

    if (cursor_location == 58){
        choice = 1;
    }

    else if (cursor_location == 128){
        choice = 2;
    }

    else{
        choice = 3;
    }

    return choice;
}



//----------------------------------------bullet enemy collision method----------------------------------------//



bool projectile_enemy_collision(Projectile *projectile, Enemy *enemy){

    if ((((projectile)->get_x_position_left()) < ((enemy)->get_x_position_right()))&
    (((projectile)->get_x_position_left()) > ((enemy)->get_x_position_left()))&
    ((((projectile)->get_y_position_bottom() < (enemy)->get_y_position_bottom())&
    ((projectile)->get_y_position_bottom() >(enemy)->get_y_position_top()))|
    (((projectile)->get_y_position_top() < (enemy)->get_y_position_bottom())&
    ((projectile)->get_y_position_top() >(enemy)->get_y_position_top()))|
    (((projectile)->get_y_position_centre() < (enemy)->get_y_position_bottom())&
    ((projectile)->get_y_position_centre() >(enemy)->get_y_position_top())))){
        return true;
    }
    return false;
}



//----------------------------------------area 1 method----------------------------------------//



void area1(){

    videoSetMode(MODE_5_3D);//set video mode 

    consoleDemoInit();//setup sub screen as text output main screen

    glScreen2D();

    lcdMainOnBottom(); //set main screen to bottom

    lcdMainOnTop(); //set main screen to top


    Platform floor(128,202,256,20);

    Platform platform1(30,160,30,10);

    Platform platform2(226,160,30,10);

    Platform platform3(128,130,30,10);


    char starting_direction = 'l';

    int starting_x = 128;

    int starting_y = 180;

    bool tracking_weapon_delay = false;

    bool tracking_reload = false;

    Player player(starting_x,starting_y,starting_direction);

    Wall left_wall(-7,96,10,192);

    Wall right_wall(262,96,10,192);

    //int projectile_speed, int damage, int projectile_delay, int reload_time, int direction_facing, int projectile_capacity, int x_position, int y_position

    Weapon weapon(10,10,10,100,'r',12,starting_x,starting_y,"ranged");

    //create lists for pointers to projectiles and enemies

    std::list<Projectile*> list_of_projectiles;

    std::list<Enemy*> list_of_enemies;


    int health_of_base = 1000;

    frame = 0;

    bool running = true;
    
	while(running) {


        //debug text
        iprintf("bottom of player model %i",player.get_y_position_bottom());
        iprintf("\n");
        iprintf("left of player model %i",player.get_x_position_left());
        
        iprintf("\n");
        iprintf("if player is falling %i",player.get_falling());
        
        iprintf("\n");
        iprintf("number of projectiles %i",weapon.get_current_projectile_amount());

        iprintf("\n");
        iprintf("time until next projectile %i",weapon.get_time_until_next_projectile());

        iprintf("\n");
        iprintf("time until reloaded %i",weapon.get_time_until_reloaded());

        iprintf("\n");
        iprintf("reloading %i",weapon.get_reloading());

        iprintf("\n");
        iprintf("number of projectiles %i",list_of_projectiles.size());

        iprintf("\n");
        iprintf("health of base %i",health_of_base);

        scanKeys();

        //display sprites

        floor.display_position();

        player.display_position();

        platform1.display_position();

        platform2.display_position();

        platform3.display_position();

        left_wall.display_position();

        right_wall.display_position();

        //update projectile objects via pointer

        for(std::list<Projectile*>::iterator projectile_iterator = list_of_projectiles.begin(); projectile_iterator != list_of_projectiles.end();){

            (*projectile_iterator)->display_projectile();//show location
            (*projectile_iterator)->update_position();//move projectile

            iprintf("\n");
            iprintf("proj x %i",(*projectile_iterator)->get_x_position_centre());//print centre

            for(std::list<Enemy*>::iterator enemy_iterator = list_of_enemies.begin(); enemy_iterator != list_of_enemies.end();){

                //if projectile collides with an enemy
                if (projectile_enemy_collision(*projectile_iterator,*enemy_iterator)){
                    
                    (*projectile_iterator)->set_exist(false);//projectile no longer exists

                    (*enemy_iterator)->set_health((*enemy_iterator)->get_health()-(*projectile_iterator)->get_damage());//change health of enemy by damage of projectile

                    if((*projectile_iterator)->get_projectile_speed()<0){
                        (*enemy_iterator)->set_all_x((*enemy_iterator)->get_x_position_centre()-10);
                    }

                    else if (((*projectile_iterator)->get_projectile_speed()>0)&((*enemy_iterator)->get_x_position_centre()!=(256 - ((*enemy_iterator)->get_width())/2))){
                        (*enemy_iterator)->set_all_x((*enemy_iterator)->get_x_position_centre()+10);
                    }

                    if (((*enemy_iterator)->get_health()) <= 0){//if enemy health is less than 0, then it should no longer exist
                        (*enemy_iterator)->set_exist(false);
                    }
                }

                ++enemy_iterator;

            }

            //if projectile is not on screen, erase from list
            if(((*projectile_iterator)->get_exist())==false){
                projectile_iterator = list_of_projectiles.erase(projectile_iterator);
            }

            else{//else increment iterator
                ++projectile_iterator;
            }

        }

        //logic loop for existing enemies

        for(std::list<Enemy*>::iterator it = list_of_enemies.begin(); it != list_of_enemies.end();){

            iprintf("\n");
            iprintf("enemy health%i",(*it)->get_health());

            (*it)->display_position();//show location

            if ((*it)->get_x_position_centre() != 256 - ((*it)->get_width())/2){
                (*it)->movement_calculations(frame,true);//move projectile horizontally and vertically
            }

            else{
                (*it)->movement_calculations(frame,false);//move projectile only vertically
                health_of_base -= (*it)->get_damage();
            }

            if (((*it)->get_exist())==false){
                it = list_of_enemies.erase(it);
            }
            else{
                ++it;
            }
            
        }

        //checking what keys have been pressed

        if(keysDown() & KEY_A){
            player.update_jump_action();
        }

        if(keysDown() & KEY_B){
            bool shooting = weapon.shoot_projectile();//handles delay and returns bool if shooting has occured
            if (shooting){
                //if weapon is shooting a new projectile, create a new pointer object and add to list of projectiles and track delay until next shot
                Projectile *new_projectile = weapon.create_projectile();
                list_of_projectiles.insert(list_of_projectiles.begin(), new_projectile);
                tracking_weapon_delay = true;
            }
            
        }

        if(keysHeld() & KEY_LEFT){
            player.update_move_left_action(true);
        }

        if(keysUp() & KEY_LEFT){
            player.update_move_left_action(false);
        }

        if(keysHeld() & KEY_RIGHT){
            player.update_move_right_action(true);
        }

        if(keysUp() & KEY_RIGHT){
            player.update_move_right_action(false);
        }

        if(keysDown() & KEY_START){
            running = false;
        }

        if(keysDown() & KEY_R){
            if (weapon.get_current_projectile_amount() != 12){
                if (tracking_reload == false){
                    weapon.start_reload_timer();
                    tracking_reload = true;
                }
            }
            
        }

        weapon.move_weapon(player.get_centre_x(),player.get_centre_y());
        player.character_movement();

        //checking if player can shoot another bullet via delay

        if(tracking_weapon_delay == true){
            weapon.update_time_until_next_projectile(weapon.get_time_until_next_projectile()-1);
            if (weapon.get_time_until_next_projectile() == 0){
                tracking_weapon_delay = false;
            }
        }


        //check to see if need to auto start_reload_timer due to empty magazine

        if(weapon.get_current_projectile_amount() == 0){
            if (tracking_reload == false){
                weapon.start_reload_timer();
                tracking_reload = true;
            }
        }


        //checking if tracking reload and if the reload is complete whilst udating reload timer

        if(tracking_reload == true){
            weapon.update_time_until_reloaded(weapon.get_time_until_reloaded()-1);
            if(weapon.get_time_until_reloaded() == 0){
                tracking_reload = false;
                weapon.reload_magazine();
            }
        }

        

        //collision detection for platforms

        //if on platform

        //if player's bottom is inside platform and if not jumping, set falling to false
        if (floor.detect_collision_player(player) == true){
            if(player.get_jumping() == false){
                player.set_falling(false);
                player.set_all_y(floor.get_y_position_top()-15);
                
            }
        }

        if (platform1.detect_collision_player(player) == true){
            if(player.get_jumping() == false){
                player.set_falling(false);
                player.set_all_y(platform1.get_y_position_top()-15);
                
            }
        }

        if (platform2.detect_collision_player(player) == true){
            if(player.get_jumping() == false){
                player.set_falling(false);
                player.set_all_y(platform2.get_y_position_top()-15);
                
            }
        }

        if (platform3.detect_collision_player(player) == true){
            if(player.get_jumping() == false){
                player.set_falling(false);
                player.set_all_y(platform3.get_y_position_top()-15);
                
            }
        }

        //if not on platform

        //if player's bottom is not inside platform and player is not jumping, set falling to true
        if ((floor.detect_collision_player(player) == false)
            &(platform1.detect_collision_player(player) == false)
            &(platform2.detect_collision_player(player) == false)
            &(platform3.detect_collision_player(player) == false)){
                if(player.get_jumping() == false){
                    player.set_falling(true);
                }
        }

        //collision detection for walls

        if(left_wall.detect_collision_player(player)==true){
            player.set_all_x(left_wall.get_x_position_right()+17);
        }

        if(right_wall.detect_collision_player(player)==true){
            player.set_all_x(right_wall.get_x_position_left()-17);
        }


        if(frame%300 == 0){
            Enemy *test_enemy = new Enemy(10,187,20,20,1,1,30);

            list_of_enemies.insert(list_of_enemies.begin(),test_enemy);

        }

        if (health_of_base < 1){
            running = false;
        }


        glFlush(0);

        Vblank();

        swiWaitForVBlank();//wait for next frame

        consoleClear();//clear bottom screen of text

    }

}



//----------------------------------------area 2 method----------------------------------------//



void area2(){

    videoSetMode(MODE_5_3D);//set video mode 

    consoleDemoInit();//setup sub screen as text output main screen

    glScreen2D();

    lcdMainOnTop(); //set main screen to bottom

    bool running = true;

    while(running){

        iprintf("Area 2");

        scanKeys();

        if(keysDown() & KEY_START){
            running = false;
        }

        glFlush(0);

        swiWaitForVBlank();//wait for next frame

        consoleClear();//clear bottom screen of text

    }

}



//----------------------------------------area 3 method----------------------------------------//



void area3(){

    videoSetMode(MODE_5_3D);//set video mode 

    consoleDemoInit();//setup sub screen as text output main screen

    glScreen2D();

    lcdMainOnTop(); //set main screen to bottom

    bool running = true;

    while(running){

        iprintf("Area 3");

        scanKeys();

        if(keysDown() & KEY_START){
            running = false;
        }

        glFlush(0);

        swiWaitForVBlank();//wait for next frame

        consoleClear();//clear bottom screen of text

    }

}



//----------------------------------------main game loop method----------------------------------------//



int main(void) {

    while(1){

        int cursor_location = menu();

        if (cursor_location == 1){
            area1();
        }

        else if (cursor_location == 2){
            area2();
        }

        else if (cursor_location == 3){
            area3();
        }

    }

	return 0;

}
