#include <nds.h>

#include <stdio.h>

#include<gl2d.h>

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


    public:

        Player(int x, int y){

            x_position_centre = x;
            x_position_left = x_position_centre-16;
            x_position_right = x_position_centre+16;

            y_position_centre = y;
            y_position_top = y_position_centre-16;
            y_position_bottom = y_position_centre+16;


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
            moving_right = new_value;
            
        }

        void update_move_left_action(bool new_value){
            moving_left = new_value;
            
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
        x_position_centre = x_centre;
        x_position_left = x_position_centre-(width/2);
        x_position_right = x_position_centre+(width/2);

        y_position_centre = y_centre;
        y_position_top = y_position_centre-(height/2);
        y_position_bottom = y_position_centre+(height/2);
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

            centre_x = x;
            centre_y = y;

            x_position_left = centre_x-20;
            x_position_right = centre_x+20;

            y_position_top = y - 5;
            y_position_bottom = y + 5;
            
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

        centre_x = x;
        centre_y = y;
        x_position_left = centre_x - (width/2);
        x_position_right = centre_x + (width/2);
        y_position_top = centre_y - (height/2);
        y_position_bottom = centre_y + (height/2);


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

    Cursor cursor(128,144);

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

    Platform floor(128,192,256,10);

    Platform platform1(30,160,30,10);

    Player player(128,172);

    Wall left_wall(-6,96,10,192);

    Wall right_wall(262,96,10,192);

    bool running = true;
    
	while(running) {


        //debug text
        iprintf("%i",player.get_y_position_bottom());
        iprintf("\n");
        iprintf("%i",floor.get_y_position_bottom());
        iprintf("\n");
        iprintf("%i",player.get_x_position_left());
        
        iprintf("\n");
        iprintf("%i",player.get_falling());
        

        scanKeys();

        //display sprites

        floor.display_position();

        player.display_position();

        platform1.display_position();

        left_wall.display_position();

        right_wall.display_position();

        //checking what keys have been pressed

        if(keysDown() & KEY_A){
            player.update_jump_action();
        }

        if(keysDown() & KEY_LEFT){
            player.update_move_left_action(true);
        }

        if(keysUp() & KEY_LEFT){
            player.update_move_left_action(false);
        }

        if(keysDown() & KEY_RIGHT){
            player.update_move_right_action(true);
        }

        if(keysUp() & KEY_RIGHT){
            player.update_move_right_action(false);
        }

        if(keysDown() & KEY_START){
            running = false;
        }

        player.character_movement();

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

        //if not on platform

        //if player's bottom is not inside platform and player is not jumping, set falling to true
        if ((floor.detect_collision_player(player) == false)&(platform1.detect_collision_player(player) == false)){
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
