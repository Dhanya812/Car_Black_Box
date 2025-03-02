#include "black_box.h"
#include "clcd.h"
#include "matrix_keypad.h"

unsigned char *menu[] = {"VIEW LOG","DOWNLOAD LOG","CLEAR LOG","SET TIME"};  
/*point_flag = 0 ,-> on top line*/
/*point_flag = 1 ,-> on bottom line*/
void display_main_menu(void)
{
    if(point_flag == 0)  //point flag indicates arrow pointing the logs in main menu
    {
        clcd_print("->",LINE1(0));
    }
    else if(point_flag == 1)
    {
        clcd_print("->",LINE2(0));
    }
    
    clcd_print(menu[log],LINE1(2));  //printing the logs
    clcd_print(menu[log+1],LINE2(2));
    
    if(key == 1)  //scrolling up
    {
        if(point_flag == 1)
            point_flag = 0;
        else
        {
            if(log>0 && log <=3 )
                log--;
        }
            CLEAR_DISP_SCREEN;
        
    }
    else if(key == 2) //scrolling down
    {
        if(point_flag == 0)
            point_flag = 1;
        else
        {
            if(log >= 0 && log <2)
                    log++;
        }
        CLEAR_DISP_SCREEN;
    }
    else if(key == 11)  //to enter into the log
    {
       log_flag = 1; 
    }
    else if(key == 12)  //exit from menu
    {
        CLEAR_DISP_SCREEN;
        log_flag = 0;
        point_flag = 0;
        log = 0;
        state = e_dashboard;
    }
    /*when entering the logs if the arrow pointing on line1, check for the log index and according to log index change the state*/
   if(log_flag == 1)
    {
        if(point_flag == 0)
        {
            if(log == 0)
            {
                state = e_view_log;
            }
            else if(log == 1)
            {
                state = e_download_log;
            }
            else if(log == 2)
            {
                state = e_clear_log;
            }
           else if(log == 3)
            {
               state = e_set_time;
            }
        }
        /*when entering the logs if the arrow pointing on line2, check for the log index and according to log index change the state*/
        else if(point_flag == 1)
        {
            if(log == 0)
            {
                state = e_download_log;
            }
            else if(log == 1)
            {
                state = e_clear_log;
            }
            else if(log == 2)
            {
                state = e_set_time;
            }
        }
    }
}
