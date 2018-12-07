#include <kernel.h>



// this function is called from the main and it creates a shell process
void start_shell()
{
    create_process(shell_process,3,0,"shell");
 	resign();
}    

// the shell process
void shell_process(PROCESS self, PARAM param)
{
    char ch;
    Keyb_Message message;
    int window_id_1 = wm_create(10,3,50,17);
    wm_clear(window_id_1);
    
    wm_print("Hello world \n");

    wm_print_char(window_id_1,"#");
    
    while(1)
    {
        msg.key_buffer = &ch;
	    send(keyb_port, &message);
    
        }





}