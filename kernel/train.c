
#include <kernel.h>


// **************************
// run the train application
// **************************
int poll_track(char);
void init_switches(int);
void clear_serial_buffer(void);
int check_for_zamboni(void);

void train_process(PROCESS self, PARAM param)
{
   int window_id_train = wm_create(10,3,50,17);
   //wm_print(window_id_train,"this is the train");
   init_switches(window_id_train);
   int zam = check_for_zamboni();
   if(zam == 0)
   {
       wm_print(window_id_train, "No Zamboni\n");
   }

   if(zam == 1)
   {
       wm_print(window_id_train, "clockwise Zamboni\n");
   }
   
   if(zam == 2)
   {
       wm_print(window_id_train, "Anticlockwise Zamboni\n");
   }

   //determine_config();
//    int result;
//    wm_print(window_id_train, "Polling track 3\n");
//    result = poll_track('3');
//    wm_print(window_id_train, "%d", result );
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
@info: This function checks for zamboni
returns 0 if no zamboni, 1 for clockwise and 2 for anti clockwise
*/
int check_for_zamboni(void)
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




