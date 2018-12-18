// Name: Rohan Sagar K.S.
// Student id: 917929160.





#include <kernel.h>
# define BUFFER_LENGTH 50   // max length of buffer
#define HISTORY_BUFFER_SIZE 10 // number of commands stored in history



char command_buffer[BUFFER_LENGTH]; // this is for the command buffer 
void shell_process(PROCESS self, PARAM param); // declaration of shell_process function
// type defining structure and pointer to structure
typedef struct _command_hist HISTORY;
typedef HISTORY* HISTORY_PTR;

// defining data structure for commands in history
struct _command_hist {
    short num; // the number of the command
    char command[BUFFER_LENGTH]; //the actual command
};

// this is where we store the pointers to the structure HISTORY
HISTORY_PTR command_ptr[HISTORY_BUFFER_SIZE] ;

short current_buffer_pointer = 0; // this variable keeps track of the current command history structure address in the array command_pt[]
int global_command_count = 1; // this variable for numbering the commands
BOOL overflow = FALSE; // indecates teh overflow of the command history

/*----------------------------------------------------------------*/

// this command is used to store the command_buffer into the command variable of the respective HISTORY structure
void store_command(int window_id_1)
{
    // check if the buffer has overflown atleast once
    if(overflow)
    {
        free(command_ptr[current_buffer_pointer]); // this is only done if the buffer has already overflown
    }
    
    if(current_buffer_pointer < HISTORY_BUFFER_SIZE)
    {
        // allocate memory for the command
        command_ptr[current_buffer_pointer] = (HISTORY_PTR) malloc(sizeof(HISTORY));
        
        // store the number
        command_ptr[current_buffer_pointer] -> num = global_command_count++;
        
        // store the command into the history structure
        for(int i = 0; (command_buffer[i]) != '\0' ; i++)
        {
            command_ptr[current_buffer_pointer] -> command[i] = command_buffer[i];
        }
        current_buffer_pointer++;
    }

    
    else if(current_buffer_pointer % HISTORY_BUFFER_SIZE == 0) // sets buffer overflow indicators
    {
        current_buffer_pointer = 0;
        overflow = TRUE;
        store_command(window_id_1);
    }    
    
}

/*----------------------------------------------------------------*/

// this command is used to print the history
void history(int window_id_1)
{
    int i;
    if(!overflow) // check if the buffer was overflown
    {
        // printing the history
        for(i=0;i<current_buffer_pointer;i++)
        {
            wm_print(window_id_1, "%d", (command_ptr[i]->num) );
            wm_print(window_id_1, "          " );
            
            // printing the command inside the HISTORY structure
            for(int j =0; command_ptr[i]-> command[j]!= '\0' /*j< BUFFER_LENGTH*/ ;j++ )
            {
                wm_print(window_id_1, "%c", (command_ptr[i]->command[j]) );
            }
            wm_print(window_id_1, "\n");    
        }
    }
    
    
    else
    {
        // printing history in case of no overflow
        int k = current_buffer_pointer;
        for(int i=0;i< HISTORY_BUFFER_SIZE;i++)
        {
            wm_print(window_id_1, "%d", (command_ptr[k]->num) );
            wm_print(window_id_1, "          " );
            
            // printing the command inside the HISTORY structure
            for(int j =0; j< BUFFER_LENGTH ;j++ )
            {
                wm_print(window_id_1, "%c", (command_ptr[k]->command[j]) );
            }
            wm_print(window_id_1, "\n");    
            k++;
            if(k == HISTORY_BUFFER_SIZE){
                k = 0;
            }
        }
    }    

}



/*----------------------------------------------------------------*/

void Help(int window_id_1){ // Help Comamnd
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

void clearShellWindow(int window_id_1){// cls command clears the window
    wm_clear(window_id_1);
}

/*----------------------------------------------------------------*/

void new_shell(){ // shell command- opens a new shell
    // create a new shell process with same priority as the current one
    create_process(shell_process,3,0,"shell_2");
 	resign();
}

/*----------------------------------------------------------------*/

void pong(int window_id_1){ // pong command: starts pong game
    start_pong();
}

/*----------------------------------------------------------------*/

void echo(int window_id_1){ // echo command: prints whatever is after the echo 
	char* echo_temp = command_buffer+5;
	wm_print(window_id_1, echo_temp);
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
void printAllProcesses(int window_id_1){
	wm_print_processes(window_id_1);
}

/*----------------------------------------------------------------*/
// this function tis used to retreive and execute a command from the command history
void number(int window_id_1)
{
   //storing the number that is inputted into a variable 
   short number = (short) (command_buffer[1]) - 48;
   BOOL found = FALSE;
   
   clear_command_buffer(window_id_1);   
   
   // checking for this number in the existing history 
   for(int i = 0; i < HISTORY_BUFFER_SIZE; i++)
   {
       if(command_ptr[i]-> num == number)
       {
            // print the command if found by looping through the elements of the command
            for(int j =0; j< BUFFER_LENGTH ;j++ )
            {
                wm_print(window_id_1, "%c", (command_ptr[i]->command[j]) );
                command_buffer[j] = command_ptr[i]->command[j]; // now store the found command into the command_buffer so that we can execute it later
            }
            wm_print(window_id_1, "\n");    
            
            found = TRUE; // setting the found variable
            run_command( window_id_1); //once the command is found and printing it is done run that command

       }


   }
    // print a message it not found
    if(!found)
    {
       wm_print(window_id_1, "Command Not Found\n");
    }
   }

/*----------------------------------------------------------------*/

 void About(int window_id_1)
 // About command
 {
 	wm_print(window_id_1,"Implemented by Rohan Sagar\n");
 }

/*----------------------------------------------------------------*/

void clear_command_buffer(int window_id_1){ // function to clear buffer
	// loopint throught the buffer and clearing it
    int i;
	for ( i= 0; i < BUFFER_LENGTH; ++i)
	{
		command_buffer[i] = '\0';
	}

}

/*----------------------------------------------------------------*/

 BOOL string_compare(char* cmd1, char* cmd2){// this function compares the buffer and the command
    // returns 1 if the initial charecters of the input string are equal to the command
 	while (*cmd1 == *cmd2 && *cmd2) {
         cmd1++;
         cmd2++;
     }
     return (*cmd1-*cmd2 == *cmd1);
   
 }


/*----------------------------------------------------------------*/
// this function compares the command buffer with the command and then calls the appropriate function
void run_command(int window_id_1) 
{
	char* cmd = &command_buffer[0];
	
	if(string_compare(cmd,"help"))
    {
		Help( window_id_1);
	}
    else if(string_compare(cmd,"cls"))
    {
		clearShellWindow( window_id_1);
	}
     else if(string_compare(cmd,"shell"))
    {
		new_shell( window_id_1);
	}
      else if(string_compare(cmd,"pong"))
    {
		pong( window_id_1);
	}
    else if(string_compare(cmd,"echo "))
    {
		echo(window_id_1);
	}
    
    else if(string_compare(cmd,"ps"))
    {
		printAllProcesses( window_id_1);
	}
    else if(string_compare(cmd,"history"))
    {
		history( window_id_1);
	}
    else if(string_compare(cmd,"!"))
    {
		number( window_id_1);
	}
    
    else if(string_compare(cmd,"about"))
    {
		About( window_id_1);
	}
    
}

/*----------------------------------------------------------------*/

// this function handles the execution of multiple commands in a single line seperated by ; 
void check_for_multiple_commands(int window_id_1)
{
    // this function checks if the given command has multiple commands seperated by 
    BOOL found = FALSE;
    int index; 
    //wm_print(window_id_1, "checking for multiple commands");

    // the function is called after the execution of the first command hence we search the buffer so see if it contains a ;
    for(int i=0; i<BUFFER_LENGTH ;i++)
    {
        if(command_buffer[i] == (char)';')
        {
            found = TRUE;
            index = i;
            break;
        }
    }

    if(found)
    {
        //slice the buffer to remove the first command
        for(int i=0; i< BUFFER_LENGTH ;i++)
        {
            command_buffer[i] = command_buffer[i+index+1];
            //  if(command_buffer[i] == '\0')
            //  {
            //      break;
            //  }
        }
    
    
    
            // remove white spaces before the next command
    
        // detect whitespaces
        for(int i = 0; i<BUFFER_LENGTH ; i++)
        {
            index = i;
            if(command_buffer[i]!= (char)' '){
                break;
            }
        }

        for(int i=0; i<BUFFER_LENGTH ; i++)
        {
            command_buffer[i] = command_buffer[i+index];
        
        
            //  if (command_buffer[i+index] == '\0')
            //  {
            //      break;
            //  }

        }
    

    
        run_command(window_id_1); // run the command
        check_for_multiple_commands(window_id_1); // check if there is another command
    }
}

/*----------------------------------------------------------------*/

// this function is called from the main and it creates a shell process
void start_shell()
{
    int i;
    create_process(shell_process,3,0,"shell");
 	resign();
}    

/*----------------------------------------------------------------*/

// the shell process
void shell_process(PROCESS self, PARAM param)
{
    char ch;
    
    int window_id_1 = wm_create(10,3,50,17); //creating a new window
    int i;

    wm_print(window_id_1, "TOS Shell\n"); // print some basic message
    wm_print(window_id_1, "------------------------------------------- \n");

    
    while(1) // constantly get the keyboard strokes and based on what they are perform different functions
    {
        wm_print(window_id_1,"$"); 
        while(1)
        {
            ch = keyb_get_keystroke(window_id_1, TRUE); // getting the key stroke
            switch(ch)
            {
                case '\b': 
                {// backspace
                    
                    if (i == 0)
                        continue;
                    i--;
                    wm_print(window_id_1, "%c", ch);
                    break;
                }

                case 13: // if return 
                {
					command_buffer[i]='\0';
					wm_print(window_id_1, "\n"); // print it
					
				    store_command(window_id_1); // store the command to history
                    run_command(window_id_1); // run the command
                    check_for_multiple_commands(window_id_1); // check if there are multiple commands
                    i=0;
					clear_command_buffer(window_id_1); // finally clear the buffer

					wm_print(window_id_1,"$");
					break;
                }
                
                default:
				{
                	command_buffer[i] = ch; // store the charecter in the command buffer
					
					// check the buffer length limit
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