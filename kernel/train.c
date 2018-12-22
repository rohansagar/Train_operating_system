
#include <kernel.h>


// **************************
// run the train application
// **************************
void init_switches(int);
void clear_serial_buffer(int);
int check_zamboni(int);
int check_for_zamboni_direction(int);
int determine_config(int);
int poll_track(int,char);
int poll_track_2(int,char, char);
void config1(int);
void config2(int);
void config3(int);
void config4(int);
void config1_zam(int);
void config2_zam(int);
void config3_zam(int);
void config4_zam(int);
void start_train(int,char);
void stop_train(int);
void change_direction(int);
void set_switch(int,char,char);


void train_process(PROCESS self, PARAM param)
{
    int window_id_train = wm_create(10,3,50,17);
    //wm_print(window_id_train,"this is the train");
    //initializing the switches
    init_switches(window_id_train);
    wm_print(window_id_train, "Initialized Switches\n");

    // checking if zamboni is present
    wm_print(window_id_train, "Checking for Zamboni \n");
    int zam = check_zamboni(window_id_train);
   
    if(zam == 1)
        wm_print(window_id_train, "Found Zamboni\n");
    
    
    // this part is for checking zamboni direction
    /*
    int zam_dir;
    
    // if zamboni is present check for zamboni direction
    if(zam == 1)
    {
        zam_dir = check_for_zamboni_direction(window_id_train);

        if(zam_dir == 0)
        {
            wm_print(window_id_train, "No Zamboni\n");
        }

        if(zam_dir == 1)
        {
            wm_print(window_id_train, "clockwise Zamboni\n");
        }
    
        if(zam_dir == 2)
        {
            wm_print(window_id_train, "Anticlockwise Zamboni\n");
        }
     }
    
    else{

        wm_print(window_id_train, "No zamboni \n");
    }
    */
    wm_print(window_id_train, "Determining Configuration\n");
    int config = determine_config(window_id_train);
    wm_print(window_id_train, "configuration %d\n", config);

    if(zam == 0 && config == 1 )
        config1( window_id_train);

    else if(zam == 0 && config ==2)
        config2(window_id_train);

    else if(zam == 0 && config ==3)
        config3(window_id_train);

    else if(zam == 0 && config ==4)
        config4(window_id_train);

    else if(zam == 1 && config ==1)
        config1_zam(window_id_train);

    else if(zam == 1 && config ==2)
        config2_zam(window_id_train);

    else if(zam == 1 && config ==3)
        config3_zam(window_id_train);

    else if(zam == 1 && config ==4)
        config4_zam(window_id_train);

    wm_print(window_id_train,"Finished the task");
   while(1)
   {    
   }
}



void init_train()
{
    create_process(train_process,7,0,"train");
    resign();
}

/*
@info: This function is used to set the switches in a position so that zamboni only goes through the outer loop
*/
void init_switches(int window_id_train)
{
    /* Switch configurations.
        5G, 8G, 9R, 1G, 4G */

    char commands[5][4] =  {"M5G\015", "M8G\015","M9R\015","M1G\015","M4G\015"};
    //char command2[] = "L20S4\015" ;
    char* rec;
    int len;
    COM_Message msg;
    msg.output_buffer = commands[0];
   	msg.len_input_buffer = len;
   	msg.input_buffer = rec;
    send(com_port,&msg); 
 
}


/*
this function sets switches
*/
void set_switch(int window_id_train, char sw_no, char col)
{

    char switch_command[5];
    switch_command[0] = 'M';
    switch_command[1] = sw_no;
    switch_command[2] = col;
    switch_command[3] = '\015';
    switch_command[4] = '\0';

    int len;
    char* rec;
    COM_Message msg;
    msg.output_buffer = switch_command;
    msg.len_input_buffer = len;
    msg.input_buffer = rec;
    send(com_port,&msg); 
    wm_print(window_id_train, "Setting switch #%c to %c\n",sw_no, col);
}

/*
This function polls all outer tracks to see if zamboni is present
returns 0 if no zamboni is absent and returns 1 if zamboni is present 
*/
int check_zamboni(int window_id_train)
{
    // poll all the outer tracks and store the results in variables.
    int poll_3 =0, poll_4 = 0, poll_6 = 0, poll_7 = 0, poll_10 = 0, poll_13 = 0, poll_14 = 0, poll_15 = 0;
    poll_3 = poll_track( window_id_train ,'3');
    poll_4 = poll_track( window_id_train ,'4');
    poll_6 = poll_track( window_id_train ,'6');
    poll_7 = poll_track( window_id_train ,'7');
    poll_10 = poll_track_2( window_id_train ,'1','0');
    poll_13 = poll_track_2( window_id_train ,'1','3');
    poll_14 = poll_track_2( window_id_train ,'1','4');
    poll_15 = poll_track_2( window_id_train ,'1','5');

    if(poll_3 + poll_4 + poll_6 + poll_7 + poll_10 + poll_13 + poll_14 + poll_15 < 1)
    {
        return 0;
    }

    else{
        return 1;
    }

}


/*
@info: This function checks for zamboni
returns 0 if no zamboni, 1 for clockwise and 2 for anti clockwise
*/
int check_for_zamboni_direction(int window_id_train)
{
    int poll_3 = 0;
    int poll_4 = 0; 
    int poll_6 = 0;
    while(poll_4 == 0){
        poll_4 = poll_track( window_id_train ,'4');
    }

    while( !(poll_3==1  || poll_6 == 1))
    {
        poll_3 = poll_track( window_id_train ,'3');
        poll_6 = poll_track( window_id_train ,'6');
    }

    if(poll_3==1){
        return 2;
    }

    else if(poll_6==1)
        return 1;
}    

/*
This function determines the configuration
*/
int determine_config(int window_id_train)
{
    // poll all the relavent tracks and store the results in variables
    int poll_2 = 0, poll_5 = 0, poll_8 =0, poll_9 =0, poll_11 =0, poll_12 =0 ;
    poll_2  = poll_track( window_id_train ,'2');
    poll_5  = poll_track( window_id_train ,'5');
    poll_8  = poll_track( window_id_train ,'8');
    poll_11 = poll_track_2( window_id_train ,'1','1');
    poll_12 = poll_track_2( window_id_train ,'1','2');

    if(poll_8 == 1 && poll_11 == 1)
    return 1; // config 1

    if(poll_12 == 1 && poll_2 == 1)
    return 2; // config 2

    if(poll_2 == 1 && poll_11 == 1)
    return 3; // config 3

    if(poll_5 == 1 && poll_12 == 1)
    return 4; // config 4


}


/*
@info: this function start the train
*/
void start_train(int window_id_train,char speed)
{
    char start_command[7];
    start_command[0] = 'L';
    start_command[1] = '2';
    start_command[2] = '0';
    start_command[3] = 'S';
    start_command[4] = speed;
    start_command[5] = '\015';
    start_command[6] = '\0';
    
    
    
    int len ;
    char* rec;
    COM_Message msg;
    msg.output_buffer = start_command;
    msg.len_input_buffer = len;
    msg.input_buffer = rec;
    send(com_port,&msg); 
    wm_print(window_id_train, "Started Train\n");

}
/*
This function stops the train by setting its speed to 0
*/

void stop_train(int window_id_train)
{
    char stop_command[] = "L20S0\015\0";

    int len ;
    char* rec;
    COM_Message msg;
    msg.output_buffer = stop_command;
    msg.len_input_buffer = len;
    msg.input_buffer = rec;
    send(com_port,&msg); 
    wm_print(window_id_train, "Stopped Train\n");
    
}

/*
This function changes the direction of the train
*/
void change_direction(int window_id_train)
{
    char direction_command[] = "L20D\015\0";

    int len ;
    char* rec;
    COM_Message msg;
    msg.output_buffer = direction_command;
    msg.len_input_buffer = len;
    msg.input_buffer = rec;
    send(com_port,&msg); 
    wm_print(window_id_train, "Train Direction Changed\n");
}



/*
@info: this fuction is used to poll the track sections 
@param[in]: track variable is used to probe a specific stack
@return : occupancy info 
*/
int poll_track (int window_id_train,char track)
{

    
        char poll_command[4]; 
        poll_command[0]= 'C';
        poll_command[1] = track;
        poll_command[2] = '\015';
        poll_command[3] = '\0';
        // before we send the poll command we clear the buffer
        clear_serial_buffer( window_id_train);
        // After clearing the buffer we send the polling command
        
        int len = 3;
        char* rec;
        COM_Message msg;
        msg.output_buffer = poll_command;
        msg.len_input_buffer = len;
        msg.input_buffer = rec;
        send(com_port,&msg); 
        wm_print(window_id_train, "Polling track #%c\n",track);
        
        if(msg.input_buffer[1]=='0')
        {
            wm_print(window_id_train, "Received 0\n");
            return 0;
        }
        else if (msg.input_buffer[1]=='1')
        {
            wm_print(window_id_train, "Received 1\n");
            return 1;
        }
        

}

/*
This function is to poll tracks that are numbered grater than 9
*/
int poll_track_2(int window_id_train,char a, char b)
{

        char poll_command[5]; 
        poll_command[0]= 'C';
        poll_command[1] = a;
        poll_command[2] = b;
        poll_command[3] = '\015';
        poll_command[4] = '\0';
        // before we send the poll command we clear the buffer
        clear_serial_buffer( window_id_train);
        // After clearing the buffer we send the polling command
        
        int len = 3;
        char* rec;
        COM_Message msg;
        msg.output_buffer = poll_command;
        msg.len_input_buffer = len;
        msg.input_buffer = rec;
        send(com_port,&msg); 
        //sleep(10);
        wm_print(window_id_train, "Polling track #%c%c\n",a,b);
        
        if(msg.input_buffer[1]=='0')
        {
            wm_print(window_id_train, "Received 0\n");
            return 0;
        }
        else if (msg.input_buffer[1]=='1')
        {
            wm_print(window_id_train, "Received 1\n");
            return 1;
        }
        

}



/*
@info: this function clears the serial buffer
*/
void clear_serial_buffer(int window_id_train)
{
    char clear_command[] = "R\015\0" ;
    int len;
    char* rec;
    COM_Message msg;
    msg.output_buffer = clear_command;
   	msg.len_input_buffer = len;
    msg.input_buffer = rec;

    send(com_port,&msg); 
    wm_print(window_id_train, "Cleared serial buffer\n");
}


/*
Written below are all the configurations of the train with and without zamboni
*/
void config1(int window_id_train)
{
    // set switches 5 and 6  to red
    set_switch( window_id_train,'5','R');
    set_switch( window_id_train,'6','R');
    sleep(2);
    // start train 
    start_train( window_id_train,'4');
    sleep(2);
    // wait until it reaches 7
    while(poll_track( window_id_train ,'7')==0);
    //stop the train
    stop_train( window_id_train);
    // set 6 and 7 to green
    set_switch( window_id_train,'6','G');
    set_switch( window_id_train,'7','G');

    // start the train in reverse direction
    change_direction( window_id_train );
    start_train( window_id_train,'4');
    //wait until it reaches 12
    while(poll_track_2( window_id_train ,'1','2')==0);

    //stop the train
    stop_train( window_id_train);
    // switch 7 to red and 8 to red
    set_switch( window_id_train,'7','R');
    set_switch( window_id_train,'8','R');
    set_switch( window_id_train,'5','G');

    // start the train in reverse direction
    change_direction( window_id_train);
    start_train( window_id_train,'4');
    // wait until you hit 13
    while(poll_track_2( window_id_train ,'1','3')==0);
    stop_train( window_id_train);

    //change the direction of the train
    change_direction( window_id_train);
    start_train( window_id_train,'4');
    // wait until you reach 12
    while(poll_track_2( window_id_train ,'1','2')==0);

    //change 7 and 6 to green
    set_switch( window_id_train,'6','G');
    set_switch( window_id_train,'7','G');
    set_switch( window_id_train,'5','R');
    //stop the train
    stop_train( window_id_train);
    //reverse the direction
    change_direction( window_id_train);
    // start the train
    start_train( window_id_train,'4');
    //wait until 7
    while(poll_track( window_id_train ,'7')==0);
    // stop the train
    stop_train( window_id_train);
    // change direction
    change_direction( window_id_train);
    // start the train
    start_train( window_id_train,'4');
    //change 5 and 6 to red
    set_switch( window_id_train,'5','R');
    set_switch( window_id_train,'6','R');
    //go until 8 
    while(poll_track(window_id_train,'8')==0);

    //stop
    stop_train( window_id_train);
}

void config2(int window_id_train)
{
    // set 1 and 2 switches to red
    set_switch( window_id_train,'1','R');
    set_switch( window_id_train,'2','R');
    //start train
    start_train( window_id_train,'4');
    //wait until it reaches 15 or 14
    while(poll_track_2( window_id_train ,'1','4')==0 && poll_track_2( window_id_train ,'1','5') == 0);

    //stop train
    stop_train( window_id_train);
    // reverser the train
    change_direction( window_id_train);
    // put 2 and 3 in green
    set_switch( window_id_train,'2','G');
    set_switch( window_id_train,'3','G');

    // start the train
    start_train( window_id_train,'4');
    // wait until it reaches 6
    while(poll_track( window_id_train ,'6')==0 );

    //switch 4 to red
    set_switch( window_id_train,'4','R');
    // stop the train
    stop_train( window_id_train);
    // reverse the direction
    change_direction( window_id_train);
    // start the train
    start_train( window_id_train,'4');
    // wait until the track train leaves the track
    while(poll_track(window_id_train,'6')==1);
    // then switch 4 back
    set_switch( window_id_train,'4', 'G');
    // wait until 15 or 14
    while(poll_track_2( window_id_train ,'1','4')==0 && poll_track_2( window_id_train ,'1','5') == 0);

    // stop the train
    stop_train( window_id_train);
    //flip 2 to red
    set_switch( window_id_train,'2','R');

    // change the direction
    change_direction( window_id_train);
    // start the train
    start_train( window_id_train,'4');
    //wait intil it reaches 12
    while(poll_track_2( window_id_train ,'1','2')==0 );

    // stop
    stop_train( window_id_train);
}
void config3(int window_id_train)
{
    
    //set switches
    // 2 to green
    // 1 to red 
    // 8 to red
    // 7 to red
    set_switch( window_id_train,'2','G');
    set_switch( window_id_train,'1','R');
    set_switch( window_id_train,'8','R');
    set_switch( window_id_train,'7','R');
    // change direction
    change_direction( window_id_train);
    // start train
    start_train( window_id_train,'5');
    //while the train not is in 14 or 13 wait
    while(poll_track_2( window_id_train ,'1','4')==0 && poll_track_2( window_id_train ,'1','3') == 0 );
    //then switch 2 to red
    set_switch( window_id_train,'2','R');

    // while train is not on 14 wait
    while(poll_track_2( window_id_train ,'1','2')==0);
    // then switch 8 to green
    set_switch( window_id_train,'8','G');
    //4 t0 red
    set_switch( window_id_train,'4','R');

    //3 to green    
    set_switch( window_id_train,'3','G');

    // wait until it is in 2
    while(poll_track( window_id_train ,'2')==0);

    // stop 
    stop_train( window_id_train);

}

void config4(int window_id_train){
    //set 3 to red
    set_switch( window_id_train,'3','R');
    // 4 to red
    set_switch( window_id_train,'4','R'); 
    // 5 to green    
    set_switch( window_id_train,'5','G');
    // 8 to green
    set_switch( window_id_train,'8','G');
    // 1 to red
    set_switch( window_id_train,'1','R');
    // 2 to red
    set_switch( window_id_train,'2','R');
    // 7 to green
    set_switch( window_id_train,'7','G');
    // 6 to green 
    set_switch( window_id_train,'6','G');
    // start the train
    start_train( window_id_train,'5');
    // wait until the train is on 14
    while(poll_track_2( window_id_train ,'1','4')==0);
    // set 5 to red
    set_switch( window_id_train,'5','R');
    // wait until the train reaches 5
    while(poll_track( window_id_train ,'5') == 0);
    // stop the train
    stop_train( window_id_train);
}

void config1_zam(int window_id_train){
    // in this config zamboni is counter clock wise
    // wait until zamboni reaches 7
    while(poll_track( window_id_train ,'6') == 0);
    // set 6 to red
    set_switch( window_id_train,'6','R');
    // set 5 to red
    set_switch( window_id_train,'5','R');
    // start train
    start_train( window_id_train,'5');
    // wait until it reaches 7
    while(poll_track( window_id_train ,'7') == 0);
    // stop the train
    stop_train( window_id_train);
    // change the direction of the train
    change_direction( window_id_train);
    // set 6 to green
    set_switch( window_id_train,'6','G');
    set_switch( window_id_train,'7','G');
    // start the train
    start_train( window_id_train,'4');
    // wait until it reaches 12
    while(poll_track_2( window_id_train ,'1', '2') == 0);
    // stop the train
    stop_train( window_id_train);
    // change the direction
    change_direction( window_id_train);
    // reset 5 to green
    set_switch( window_id_train,'5','G');
    //change 7 to red
    set_switch( window_id_train,'7','R');
    // wait intil zamboni reaches 10
    while(poll_track_2( window_id_train ,'1','0') == 0);
    //set 8 to red
    set_switch( window_id_train,'8','R');
    // start the train
    start_train( window_id_train,'5');
    // wait until it reaches 13
    while(poll_track_2( window_id_train ,'1','3') == 0);
    //stop the train
    stop_train( window_id_train);
    // reverse the train
    change_direction( window_id_train);
    //start the train
    start_train( window_id_train,'4');
    // wait until it reaches 12
    while(poll_track_2( window_id_train ,'1','2') == 0);
    // reset 8 to green
    set_switch( window_id_train,'8','G');
    //stop the train
    stop_train( window_id_train);
    //change the direction 
    change_direction( window_id_train);
    // set 7 to green
    set_switch( window_id_train,'7','G');
    // wait until zamboni reaches 7
    while(poll_track( window_id_train ,'6') == 0);
    // set 5 to red
    set_switch( window_id_train,'5','R');
    //start the train
    start_train( window_id_train,'5');
    //wait until it reaches 7
    while(poll_track( window_id_train ,'7') == 0);
    //stop the train
    stop_train( window_id_train);
    // change the direction
    change_direction( window_id_train);
    //change 6 to red
    set_switch( window_id_train,'6','R');
    // start the train
    start_train( window_id_train,'5');
    //wait until it reaches 8
    while(poll_track( window_id_train ,'8') == 0);
    // stop the train
    stop_train( window_id_train);
    // set 5 to green
    set_switch( window_id_train,'5','G');
  
}
void config2_zam(int window_id_train){
    // in this configuration zamboni is clockwise
    //wait until zamboni reaches 3
    while(poll_track( window_id_train ,'3') == 0);
    // set 1 and 2 to red
    set_switch( window_id_train,'1','R');
    set_switch( window_id_train,'2','R');
    // start train a little slow 3
    start_train( window_id_train,'4');
    // wait until it gets to 15
    while(poll_track_2( window_id_train ,'1','5') == 0 && poll_track_2( window_id_train ,'1','4') == 0);
    // stop the train
    stop_train( window_id_train);
    // change the direction
    change_direction( window_id_train);
    // change 2 to green
    set_switch( window_id_train,'2','G');
    
    // start the train in 4
    start_train( window_id_train,'5');
    // wait until it reaches 1
    while(poll_track( window_id_train ,'1') == 0);
    // stop the train
    stop_train( window_id_train);
    //switch 1 to green
    set_switch( window_id_train,'1','G');
    // 3 to green 
    set_switch( window_id_train,'3','G');
    //wait until zamboni is in 6
    while(poll_track( window_id_train ,'7') == 0);
    // 4 to red
    set_switch( window_id_train,'4','R');
    // start the train in 4
    start_train( window_id_train,'5');
    // wait until it reaches 6
    while(poll_track( window_id_train ,'6') == 0);
    //stop the train
    stop_train( window_id_train);
    // change direction 
    change_direction( window_id_train);
    // start the train
    start_train( window_id_train,'5');
    // wait until it reaches 1 or 2
    while(poll_track( window_id_train ,'1') == 0 && poll_track( window_id_train ,'2') == 0);
    // switch 4 to green
    set_switch( window_id_train,'4','G');
    // stop the train
    stop_train( window_id_train);
    // wait until zamboni is in 3
    while(poll_track( window_id_train ,'3') == 0);
   
    //switch 1 to red
    set_switch( window_id_train,'1','R');
    // start the train in 4
    start_train( window_id_train,'5');
    // wait until it reaches 15 or 14
    while(poll_track_2( window_id_train ,'1','5') == 0 && poll_track_2( window_id_train ,'1','4') == 0);
    // stop the train
    stop_train( window_id_train);
    // switch 2 to red
    set_switch( window_id_train,'2','R');
    // reverse the direction
    change_direction( window_id_train);
    // start the train
    start_train( window_id_train,'4');
    // wait until it reaches 12
    while(poll_track_2( window_id_train ,'1','2') == 0 );
    //stop the train
    stop_train( window_id_train);
    // set 1 to green
    set_switch( window_id_train,'1','G');

}
void config3_zam(int window_id_train){
    // in this configuration zamboni is clockwise
    // reverse the direction of the train
    change_direction( window_id_train);
    //start the train in 3
    start_train( window_id_train,'4');
    //wait until it gets to 1
    while(poll_track( window_id_train ,'1') == 0);
    //stop the train
    stop_train( window_id_train);
    // wait until zamboni is in 3
    while(poll_track( window_id_train ,'3') == 0);
    // set 2 to green
    set_switch( window_id_train,'2','G');
    // set 1 to red
    set_switch( window_id_train,'1','R');
    //start the train in 5
    start_train( window_id_train,'5');
    // wait until it gets to 15 0r 14
    while(poll_track_2( window_id_train ,'1','5') == 0 && poll_track_2( window_id_train ,'1','4') == 0);

    //stop the train
    stop_train( window_id_train);
    //reverse the direction
    change_direction( window_id_train);
    //set 2 to red
    set_switch( window_id_train,'2','R');
    //start in 5
    start_train( window_id_train,'5');
    // wait until it reaches 12
    while(poll_track_2( window_id_train ,'1','2') == 0);
    //stop the train
    stop_train( window_id_train);
    //set 7 to red
    set_switch( window_id_train,'7','R');
    //set 1 to green
    set_switch( window_id_train,'1','G');
    // sleep
    sleep(2);
    // wait until zamboni is in 14
    while(poll_track_2( window_id_train ,'1','4') == 0);
    //set 8 to red
    set_switch( window_id_train,'8','R');
    //start in 5
    start_train( window_id_train,'4');
    // wait until the train reaches 13 or 14
    while(poll_track_2( window_id_train ,'1','3') == 0  &&  poll_track_2( window_id_train ,'1','4') == 0);
    // stop
    stop_train( window_id_train);
    // reverse the direction
    change_direction( window_id_train);
    //start in 3
    start_train( window_id_train,'3');
    // wait until the train is in 11 
    while(poll_track_2( window_id_train ,'1','1') == 0);
    // switch 8 to green
    set_switch( window_id_train,'8','G');
    //wait until the train is in 12
    while(poll_track_2( window_id_train ,'1','2') == 0);
    //stop the train
    stop_train( window_id_train);
    //wait until zamboni is in 3
    while(poll_track( window_id_train ,'3') == 0);
    //set 1 to red
    set_switch( window_id_train,'1','R');
    //start the train in 4
    start_train( window_id_train,'4');
    //wait until it is in 15 or 14
    while(poll_track_2( window_id_train ,'1','4') == 0  &&  poll_track_2( window_id_train ,'1','5') == 0);
    // stop
    stop_train( window_id_train);
    //switch 2 to green
    set_switch( window_id_train,'2','G');
    //reverse
    change_direction( window_id_train);
    //start
    start_train( window_id_train,'5');
    //wait until we the train gets back to 2
    while(poll_track( window_id_train ,'1') == 0);
    // set 1 to green
    set_switch( window_id_train,'1','G');
    //wait until the train reaches 2
    while(poll_track( window_id_train ,'2') == 0);
    //stop the train
    stop_train( window_id_train);
    
    
}
void config4_zam(int window_id_train)
{
    // zamboni is counter clockwise in this config
    //set 3 to red
    set_switch( window_id_train,'3','R');
    // wait until zamboni is in 4
    while(poll_track( window_id_train ,'4') == 0);
    // set 4 to red
    set_switch( window_id_train,'4','R');
    //start train in 5
    start_train( window_id_train,'4');
    // wait until it gets to 6
    while(poll_track( window_id_train ,'6') == 0 );
    // stop train
    stop_train( window_id_train);
    //set 3 to green
    set_switch( window_id_train,'3','G');
    
    // reverse direction
    change_direction( window_id_train);
    //start in 5
    start_train( window_id_train,'4');
    // wait until the train is in 2 
    while(poll_track( window_id_train ,'2') == 0);
    // set 4 to green
    set_switch( window_id_train,'4','G');
    //wait until the train is in 1 
    while(poll_track( window_id_train ,'1') == 0);
    // stop train
    stop_train( window_id_train);
    // set 2 to green
    set_switch( window_id_train,'2','G');
    // sleep
    sleep(2);
    // wait until zamboni is in 14 or 13
    while(poll_track_2( window_id_train ,'1','4') == 0 && poll_track_2( window_id_train ,'1','3') == 0);
    //switch 1 to red
    set_switch( window_id_train,'1','R');
    //start in 4
    start_train( window_id_train,'5');
        //wait until the train reaches 15 or 14
        while(poll_track_2( window_id_train ,'1','5') == 0 && poll_track_2( window_id_train ,'1','4') == 0);
        //set 2 to red
        set_switch( window_id_train,'2','R');
        //stop
        stop_train( window_id_train);
        //change direction
        change_direction( window_id_train);
        //start in 5
        start_train( window_id_train,'5');
        // set 7 to red
        set_switch( window_id_train,'7','R');
        //wait until the train reaches 12
    while(poll_track_2( window_id_train ,'1','1') == 0);
    //stop
    stop_train( window_id_train);
    //set 1 to green
    set_switch( window_id_train,'1','G');
    //change direction
    change_direction( window_id_train);
    // wait until zamboni reaches 14 or 13
    while(poll_track_2( window_id_train ,'1','4') == 0 /*&& poll_track_2( window_id_train ,'1','3') == 0*/);
    //set 1 to red
    set_switch( window_id_train,'1','R');
    // start in 5
    start_train( window_id_train,'5');
    // wait until the train reaches 15 or 14
    while(poll_track_2( window_id_train ,'1','5') == 0 && poll_track_2( window_id_train ,'1','4') == 0);
    //stop
    stop_train( window_id_train);
    //set 2 to green
    set_switch( window_id_train,'2','G');
    //change direction
    change_direction( window_id_train);
    //start in 5
    start_train( window_id_train,'5');
    //wait until train is in 1
    while(poll_track( window_id_train ,'1') == 0);
    //switch 1 to green
    set_switch( window_id_train,'1','G');
    //wait until the train is in 2
    while(poll_track( window_id_train ,'2') == 0);
    // stop the train
    stop_train( window_id_train);

}



