#include <kernel.h>
# define BUFFER_LENGTH 50   // max length of buffer
char command_buffer[BUFFER_LENGTH]; // this is for the command buffer 
int i = 0; // count for the buffer
int window_id_1;
void shell_process(PROCESS self, PARAM param);



//store_command(){
//    malloc(c);
//    copuy command_buffer into c
//}




/*----------------------------------------------------------------*/

void Help(){ // Help Comamnd
 	wm_print(window_id_1,"TOS 2018 Help\n");
 	wm_print(window_id_1,"These shell commands are defined internally:\n");
 	wm_print(window_id_1,"help                  - for displaying help information\n");
 	wm_print(window_id_1,"cls                   - clear window\n");
    wm_print(window_id_1,"shell                 - opens a new shell\n");
    wm_print(window_id_1,"pong                  - starts the pong game\n");
    wm_print(window_id_1,"echo {string}         - Echo the string passed\n");
 	wm_print(window_id_1,"ps                    - list all processes\n");
 	wm_print(window_id_1,"history               - Prints command history\n");
    wm_print(window_id_1,"!<number>             - Executes the command from the history\n");
    wm_print(window_id_1,"about                 - Maker Information\n");	
   }

/*----------------------------------------------------------------*/

void clearShellWindow(){// cls command
    wm_clear(window_id_1);
}

/*----------------------------------------------------------------*/

void new_shell(){ // shell command
    
    create_process(shell_process,3,0,"shell_2");
 	resign();
}


/*----------------------------------------------------------------*/

void pong(){ // pong command

}

/*----------------------------------------------------------------*/

void echo(){ // echo command
	char* toEcho = command_buffer+5;
	wm_print(window_id_1,toEcho);
	wm_print(window_id_1,"\n");
}

/*-----------------------------------------------------------------*/

// Printing Processes copied over from process.c and modifined to use wm++

void wm_print_processes(int window_id){ // ps command
    int             i;
    PCB            *p = pcb;

    // print the heading
    wm_print(window_id, "State           Active Prio Name\n");
    wm_print(window_id, "------------------------------------------------\n");

    for (i = 0; i < MAX_PROCS; i++, p++) 
    {
        if (!p->used)
            continue;
        static const char *state[] = 
        {
            "READY          ",
            "ZOMBIE         ",
            "SEND_BLOCKED   ",
            "REPLY_BLOCKED  ",
            "RECEIVE_BLOCKED",
            "MESSAGE_BLOCKED",
            "INTR_BLOCKED   "
        };

        if (!p->used) 
        {
            wm_print(window_id, "PCB slot unused!\n");
            return;
        }
    
        /* State */
        wm_print(window_id, state[p->state]);
        /* Check for active_proc */
        if (p == active_proc)
        wm_print(window_id, " *      ");
        else
        wm_print(window_id, "        ");
        /* Priority */
        wm_print(window_id, "  %2d", p->priority);
        /* Name */
        wm_print(window_id, " %s\n", p->name); 
        
    }
}

/*----------------------------------------------------------------*/
// function for printing all the processes
void printAllProcesses(){
	wm_print_processes(window_id_1);
}

/*----------------------------------------------------------------*/

void history(){
    wm_print(window_id_1,"You didnt implement me yet! You moron\n");
    }

/*----------------------------------------------------------------*/

void number(){
    wm_print(window_id_1,"You didnt implement me yet! You moron\n");
}

/*----------------------------------------------------------------*/

 void About()
 // About command
 {
 	wm_print(window_id_1,"Implemented by Rohan Sagar\n");
 }

/*----------------------------------------------------------------*/



void clear_command_buffer(){ // function to clear buffer
	int i;
	for ( i= 0; i < BUFFER_LENGTH; ++i)
	{
		command_buffer[i] = '\0';
	}

}



 BOOL string_compare(char* cmd1, char* cmd2){// this function compares the buffer and the command

 	while (*cmd1 == *cmd2 && *cmd2) {
         cmd1++;
         cmd2++;
     }
     return (*cmd1-*cmd2 == *cmd1);
   
 }




void run_command(){
	char* cmd = &command_buffer[0];
	
	if(string_compare(cmd,"help"))
    {
		Help();
	}
    else if(string_compare(cmd,"cls"))
    {
		clearShellWindow();
	}
     else if(string_compare(cmd,"shell"))
    {
		new_shell();
	}
      else if(string_compare(cmd,"pong"))
    {
		pong();
	}
    else if(string_compare(cmd,"echo "))
    {
		echo();
	}
    else if(string_compare(cmd,"echo"))
    {
		wm_print(window_id_1,"Please enter something after echo\n");
	}
   
    else if(string_compare(cmd,"ps"))
    {
		printAllProcesses();
	}
    else if(string_compare(cmd,"history"))
    {
		history();
	}
    else if(string_compare(cmd,"!"))
    {
		number();
	}
    
    else if(string_compare(cmd,"about"))
    {
		About();
	}
    
}










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
    window_id_1 = wm_create(10,3,50,17);
    
    wm_print(window_id_1, "TOS Shell\n");
    wm_print(window_id_1, "------------------------------------------- \n");

    
    while(1)
    {
        wm_print(window_id_1,"#");
        while(1)
        {
            ch = keyb_get_keystroke(window_id_1, TRUE);
            switch(ch)
            {
                case '\b':
                {
                    if (i == 0)
                        continue;
                    i--;
                    wm_print(window_id_1, "%c", ch);
                    break;
                }

                case 13:
                {
					command_buffer[i]='\0';
					wm_print(window_id_1, "\n");
					run_command();
				    //store_command();
                    i=0;
					clear_command_buffer();

					wm_print(window_id_1,"#");
					break;
                }
                
                default:
				{
                	command_buffer[i] = ch;
					
					// buffer length check
					if(i<= BUFFER_LENGTH -1)
                    {
						i++;
						wm_print(window_id_1, "%c", ch);
					}

					
					break;
                }
            }
        }
     }  





}