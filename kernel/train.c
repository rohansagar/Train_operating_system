
#include <kernel.h>


// **************************
// run the train application
// **************************
void init_switches(int);
void clear_serial_buffer(void);
int check_zamboni(void);
int check_for_zamboni_direction(void);
int determine_config(void);
int poll_track(char);
int poll_track_2(char, char);

void train_process(PROCESS self, PARAM param)
{
    int window_id_train = wm_create(10,3,50,17);
    //wm_print(window_id_train,"this is the train");
    //initializing the switches
    init_switches(window_id_train);
    // checking if zamboni is present
    int zam = check_zamboni();
    int zam_dir;
    
    // if zamboni is present check for zamboni direction
    if(zam == 1)
    {
        zam_dir = check_for_zamboni_direction();

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
    int config = determine_config();

    wm_print(window_id_train, "configuration %d\n", config);


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
    wm_print(window_id_train, "Initialized Switches\n");
 
}

/*
This function polls all outer tracks to see if zamboni is present
returns 0 if no zamboni is absent and returns 1 if zamboni is present 
*/
int check_zamboni()
{
    // poll all the outer tracks and store the results in variables.
    int poll_3 =0, poll_4 = 0, poll_6 = 0, poll_7 = 0, poll_10 = 0, poll_13 = 0, poll_14 = 0, poll_15 = 0;
    poll_3 = poll_track('3');
    poll_4 = poll_track('4');
    poll_6 = poll_track('6');
    poll_7 = poll_track('7');
    poll_10 = poll_track_2('1','0');
    poll_13 = poll_track_2('1','3');
    poll_14 = poll_track_2('1','4');
    poll_15 = poll_track_2('1','5');

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
int check_for_zamboni_direction(void)
{
    int poll_3 = 0;
    int poll_4 = 0; 
    int poll_6 = 0;
while(poll_4 == 0){
    poll_4 = poll_track('4');
}

while( !(poll_3==1  || poll_6 == 1))
{
    poll_3 = poll_track('3');
    poll_6 = poll_track('6');
}

if(poll_3==1){
    return 2;
}

else if(poll_6==1)
    return 1;
}

/*
@info: This function determines the configuration of the train simulation
@return: the config as integer
*/
int determine_config()
{
    // poll all the relavent tracks and store the results in variables
    int poll_2 = 0, poll_5 = 0, poll_8 =0, poll_9 =0, poll_11 =0, poll_12 =0 ;
    poll_2  = poll_track('2');
    poll_5  = poll_track('5');
    poll_8  = poll_track('8');
    poll_11 = poll_track_2('1','1');
    poll_12 = poll_track_2('1','2');

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
void start_train()
{


}



/*
@info: this fuction is used to poll the track sections 
@param[in]: track variable is used to probe a specific stack
@return : occupancy info 
*/
int poll_track (char track)
{

    
        char poll_command[4]; 
        poll_command[0]= 'C';
        poll_command[1] = track;
        poll_command[2] = '\015';
        poll_command[3] = '\0';
        // before we send the poll command we clear the buffer
        clear_serial_buffer();
        // After clearing the buffer we send the polling command
        
        int len = 3;
        char* rec;
        COM_Message msg;
        msg.output_buffer = poll_command;
        msg.len_input_buffer = len;
        msg.input_buffer = rec;
        send(com_port,&msg); 
        //sleep(10);
        // wm_print(window_id_train, "Polling track #%c\n",track);
        
        if(msg.input_buffer[1]=='0')
        {
            return 0;
        }
        else if (msg.input_buffer[1]=='1')
        {
            return 1;
        }
        
  //  wm_print(window_id_train, "\n");

}

/*
This function is to poll tracks that are numbered grater than 9
*/
int poll_track_2(char a, char b)
{

        char poll_command[5]; 
        poll_command[0]= 'C';
        poll_command[1] = a;
        poll_command[2] = b;
        poll_command[3] = '\015';
        poll_command[4] = '\0';
        // before we send the poll command we clear the buffer
        clear_serial_buffer();
        // After clearing the buffer we send the polling command
        
        int len = 3;
        char* rec;
        COM_Message msg;
        msg.output_buffer = poll_command;
        msg.len_input_buffer = len;
        msg.input_buffer = rec;
        send(com_port,&msg); 
        //sleep(10);
        // wm_print(window_id_train, "Polling track #%c\n",track);
        
        if(msg.input_buffer[1]=='0')
        {
            return 0;
        }
        else if (msg.input_buffer[1]=='1')
        {
            return 1;
        }
        


}



void clear_serial_buffer(){
    char clear_command[] = "R\015\0" ;
    int len;
    char* rec;
    COM_Message msg;
    msg.output_buffer = clear_command;
   	msg.len_input_buffer = len;
    msg.input_buffer = rec;

    send(com_port,&msg); 
    //sleep(10);
}




