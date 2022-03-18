#include <nds.h>

#include <stdio.h>

#include<gl2d.h>

#include<string>

#include <list>

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

        void set_falling(bool new_value){
            falling = new_value;
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

        void set_all_x(int new_value){
            x_position_centre = new_value;
            x_position_left = new_value-16;
            x_position_right = new_value+16;
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

};



//----------------------------------------projectile class----------------------------------------//


class Projectile{

    private:

    int projectile_speed;
    char direction_facing;
    int x_position;
    int x_position_left;
    int x_position_right;
    int y_position;
    int y_position_top;
    int y_position_bottom;
    int damage;
    int width;
    int height;

    public:

    Projectile(int projectile_speed, char direction_facing, int x_position, int y_position, int width, int height, int damage){

        if(direction_facing == 'r'){
            this->projectile_speed = projectile_speed;
        }
        else{
            this->projectile_speed = -(projectile_speed);
        }
        
        this->direction_facing = direction_facing;
        this->x_position = x_position;
        this->x_position_left = x_position - (width/2);
        this->x_position_right = x_position + (width/2);

        this->y_position = y_position;
        this->y_position_top = y_position - (height/2);
        this->y_position_bottom = y_position + (height/2);

        this->damage = damage;

    }


    void display_projectile(){

        glBegin2D();/*opens gl for 2d creation*/

        glBoxFilled(x_position_left,y_position_top,x_position_right,y_position_bottom,RGB15(255, 255, 0));

        glEnd2D();/*ends gl for 2d creation*/


    }


    void update_position(){

        x_position += projectile_speed;
        x_position_left = x_position - (width/2);
        x_position_right = x_position + (width/2);

        if ((x_position > 256)|(x_position < 0)){
            //delete this;   delete object if off screen
        } 

    }

    int get_centre_x(){
        return x_position;
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
        int x_position;
        int y_position;
        int current_projectile_amount;
        string weapon_type;
        char direction_facing;
        int time_until_reloaded;


    public:

        Weapon(int projectile_speed, int damage, int projectile_delay, int reload_time, int direction_facing, int projectile_capacity, int x_position, int y_position, string weapon_type){

            this->projectile_speed = projectile_speed;
            this->damage = damage;
            this->projectile_delay = projectile_delay;
            this->reload_time = reload_time;
            this->direction_facing = direction_facing;
            this->projectile_capacity = projectile_capacity;
            this->current_projectile_amount = projectile_capacity;

            this->x_position = x_position;
            this->y_position = y_position;

            this->weapon_type = weapon_type;

        }


        void move_weapon(int new_x, int new_y){

            if (new_x != x_position){

                if(new_x > x_position){
                    direction_facing = 'r';
                }
                
                else{
                    direction_facing = 'l';
                }
            }
            

            x_position = new_x;
            
            y_position = new_y;

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

            Projectile *new_projectile = new Projectile(projectile_speed,direction_facing, x_position,y_position,10,5,damage);

            return new_projectile;


        }

        int get_time_until_next_projectile(){
            return time_until_next_projectile;
        }

        void update_time_until_next_projectile(int new_time){
            time_until_next_projectile = new_time;
        }

        int get_time_until_reloaded(){
            return time_until_reloaded;
        }

        void update_time_until_reloaded(int new_time){
            time_until_reloaded = new_time;
        }

        int get_current_projectile_amount(){
            return current_projectile_amount;
        }

        bool get_reloading(){
            return reloading;
        }

        void start_reload_timer(){
            reloading = true;
            time_until_reloaded = reload_time;
        }

        void reload_magazine(){
            current_projectile_amount = projectile_capacity;
            reloading = false;
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

    int centre_x;
    int centre_y;
    int x_position_left;
    int x_position_right;
    int y_position_top;
    int y_position_bottom;
    bool moving_left = false;
    bool moving_right = false;

    public:

        Cursor(int x, int y){

            this->centre_x = x;
            this->centre_y = y;

            this->x_position_left = centre_x-20;
            this->x_position_right = centre_x+20;

            this->y_position_top = y - 5;
            this->y_position_bottom = y + 5;
            
        }

        int get_centre_x(){
            return centre_x;
        }

        void set_all_x(int new_value){
            centre_x = new_value;
            x_position_left = new_value-20;
            x_position_right = new_value+20;
        }

        void display_position(){

            glBegin2D();/*opens gl for 2d creation*/

            glBoxFilled(x_position_left,y_position_top,x_position_right,y_position_bottom,RGB15(0, 0, 255));

            glEnd2D();/*ends gl for 2d creation*/

        }

        void movement(){
            //if cursor is on middle or right option, then can move left
            if ((moving_left==true) & ((centre_x == 128)|(centre_x == 198))){
                set_all_x(centre_x-70);
                moving_left = false;
            }

            //if cursor is on left or middle option, then can move right
            if ((moving_right==true) & ((centre_x == 58)|(centre_x == 128))){
                set_all_x(centre_x+70);
                moving_right = false;
            }

        }

        void update_move_left(bool new_value){
            moving_left = new_value;
        }

        void update_move_right(bool new_value){
            moving_right = new_value;
        }
};

//----------------------------------------wall class----------------------------------------//

class Wall{

private:

    int centre_x;
    int centre_y;
    int x_position_left;
    int x_position_right;
    int y_position_top;
    int y_position_bottom;




public:

    Wall(int x, int y, int width, int height){

        this->centre_x = x;
        this->centre_y = y;
        this->x_position_left = centre_x - (width/2);
        this->x_position_right = centre_x + (width/2);
        this->y_position_top = centre_y - (height/2);
        this->y_position_bottom = centre_y + (height/2);


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

    int cursor_location = cursor.get_centre_x();

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

    Platform platform3(128,100,30,10);


    char starting_direction = 'r';

    int starting_x = 128;

    int starting_y = 180;

    bool tracking_weapon_delay = false;

    bool tracking_reload = false;

    Player player(starting_x,starting_y,starting_direction);

    Wall left_wall(-7,96,10,192);

    Wall right_wall(262,96,10,192);


    //int projectile_speed, int damage, int projectile_delay, int reload_time, int direction_facing, int projectile_capacity, int x_position, int y_position

    Weapon weapon(10,3,10,300,'r',12,starting_x,starting_y,"ranged");

    std::list<Projectile*> list_of_projectiles;

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
        

        scanKeys();

        //display sprites

        floor.display_position();

        player.display_position();

        platform1.display_position();

        platform2.display_position();

        platform3.display_position();

        left_wall.display_position();

        right_wall.display_position();

        for(Projectile* projectile : list_of_projectiles){
            projectile->display_projectile();
            projectile->update_position();
            iprintf("\n");
            iprintf("proj x %i",projectile->get_centre_x());
        }

        //checking what keys have been pressed

        if(keysDown() & KEY_A){
            player.update_jump_action();
        }

        if(keysDown() & KEY_B){
            bool shooting = weapon.shoot_projectile();
            if (shooting){
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


        glFlush(0);

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
