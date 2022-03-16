#include <nds.h>

#include <stdio.h>

#include<gl2d.h>

using namespace std;

//256x192 pixels

volatile int frame = 0;

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

    void displayPosition(){

        glBegin2D();/*opens gl for 2d creation*/

        glBoxFilled(x_position_left,y_position_top,x_position_right,y_position_bottom,RGB15(255, 0, 0));

        glEnd2D();/*ends gl for 2d creation*/

    }

};


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

        void displayPosition(){

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


void Vblank() {
	frame++;
}

bool detect_collision_platform(Player player, Platform platform){

    bool contact = false;
    // if player's bottom is inside platform
    if ((player.get_x_position_left()<platform.get_x_position_right())
        &(player.get_x_position_right()>platform.get_x_position_left())
        &(player.get_y_position_bottom()>platform.get_y_position_top())
        &(player.get_y_position_bottom()<platform.get_y_position_bottom())){
            contact = true;
        }
    
    return contact;
}

int main(void) {

    videoSetMode(MODE_5_3D);//set video mode 

    consoleDemoInit();//setup sub screen as text output

    glScreen2D();

    lcdMainOnTop(); //set main screen to top

    Platform floor(128,192,256,10);

    Platform platform1(30,160,30,10);

    Player player(128,160);
    
	while(1) {


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

        floor.displayPosition();

        player.displayPosition();

        platform1.displayPosition();

        //checking what keys have been pressed

        if(keysHeld() & KEY_A){
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

        player.character_movement();

        //collistion detection calling

        //if on platform

        //if player's bottom is inside platform and if not jumping, set falling to false
        if (detect_collision_platform(player,floor) == true){
            if(player.get_jumping() == false){
                player.set_falling(false);
                player.set_all_y(floor.get_y_position_top()-15);
                
            }
        }

        if (detect_collision_platform(player,platform1) == true){
            if(player.get_jumping() == false){
                player.set_falling(false);
                player.set_all_y(platform1.get_y_position_top()-15);
                
            }
        }

        //if not on platform

        //if player's bottom is not inside platform and player is not jumping, set falling to true
        if ((detect_collision_platform(player,floor) == false)&(detect_collision_platform(player,platform1) == false)){
            if(player.get_jumping() == false){
                player.set_falling(true);
            }
        }


        glFlush(0);

        swiWaitForVBlank();//wait for next frame

        consoleClear();//clear bottom screen of text

    }

	return 0;

}
