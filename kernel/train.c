
#include <kernel.h>


// **************************
// run the train application
// **************************

void train_process(PROCESS self, PARAM param)
{
   int window_id_train = wm_create(10,3,50,17);
   wm_print(window_id_train,"this is the train");
   while(1){
       
   }
}


void init_train()
{
    create_process(train_process,4,0,"train");
    resign();

}
