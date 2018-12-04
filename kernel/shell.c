#include <kernel.h>




void start_shell()
{
     // initialize the keyboard
    int window_id = wm_create(10,3,50,17);
    wm_print(window_id, "TOS Shell \n");
    wm_print(window_id, "------------------------------");
    
    while(1){

        char ch = keyb_get_keystroke(window_id, TRUE);
        wm_print(window_id, "Got key: %c\n",ch);
    }

}
