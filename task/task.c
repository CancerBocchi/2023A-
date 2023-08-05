#include "inc.h"
#include "Contrarian_1ph.h"
/*variable define*/

/*task init*/
void Task_Init()
{
    Contrarian_Init();
   
}

/*task do*/
void Task_Do()
{
    keyboard_execute(Keyboard_Input_Data,3);
    Keyboard_OLED(Keyboard_Input_Data);
}
/*IT Function*/
