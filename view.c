#include "black_box.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "main.h"
#include "uart.h"
#include "external_eeprom.h"
#include "ds1307.h"

unsigned long delay = 0;
/*view log*/
unsigned char log_val;
unsigned int log = 0;
/*download log*/
unsigned char sl_no;
unsigned int ev;
/*event reader*/
unsigned char add = 0;
extern unsigned int event_count;
unsigned int e;
unsigned char store_event[10][17];
/*set time*/
unsigned char time1[9];
unsigned char set_time1[6];
unsigned int hr = 0, min = 0, sec = 0;
unsigned int field_flag = 1;
int s_flag;

void event_reader(void) { //to read the event from EEPROM and to store it to 2D array

    add = 0; //address starting from 0
    /*event count more than 10 then we should discard the oldest event to store new event*/
    /*here the data events are pushed up by changing the address of 1st event to 0th event address, 2nd event address with 1st event address*/
    if (event_count > 10) {
        add = (event_count % 10)*12; //changing the address when event_count > 10 which help to replace the address 
    }
    e = event_count > 10 ? 10 : event_count; //storing the event count in another integer
    for (int j = 0; j < e; j++) { //no of event times
        for (int i = 0; i < 14; i++) {
            if ((i == 8) || (i == 11)) { //for space between time to event and event to speed 
                store_event[j][i] = ' ';
            } else {
                store_event[j][i] = read_external_eeprom(add++); //read from eeprom and store in 2D array
            }
        }
        store_event[j][14] = '\0';
        /*if address more than address */
        if (add >= 120)
            add = 0;
    }
}

/* function for viewing the logs*/
void view_log(void) {
    log_val = (log % 10) + 48; //for sl_no as we cannot display integer in the CLCD we have to convert it into char
    event_reader(); //calling event reading
    if (e == 0) { //if no event found it should print no logs found in clcd
        clcd_print("    NO  LOGS    ", LINE1(0));
        clcd_print("      FOUND     ", LINE2(0));
        if (delay++ == 1000) {
            delay = 0;

            CLEAR_DISP_SCREEN;
            /*reseting the flags*/
            point_flag = 0;
            log = 0;
            log_flag = 0;
            state = e_main_menu;
        }
    } else {
        clcd_print("L TIME     EV SP", LINE1(0)); //display this in first line in clcd
        clcd_putch(log_val, LINE2(0)); //to display the sl no
        clcd_print(store_event[log], LINE2(2)); //to display events which are stored in 2D array

        if (key == 1) { //scroll up mechanism
            if (log > 0 && log <= e) {
                log--;
            }
        }
        if (key == 2) { //scroll down mechanism
            if (log >= 0 && log < e - 1) {
                log++;
            }
        }
        if (key == 12) {
            CLEAR_DISP_SCREEN;
            point_flag = 0;
            log = 0;
            log_flag = 0;
            state = e_main_menu;
        }
    }

}
/*functions for downloading the logs*/


void download_log(void) {

    event_reader();
    if (event_count == 0) //if event count is zero then no logs found
    {
        clcd_print(" NO LOGS PRESENT ", LINE1(0));
        clcd_print("  TO DOWNLOAD!!  ", LINE2(0));
        if (delay++ == 1000) //delay used to keep the above data in clcd for sometimes and come back to main menu
        {
            delay = 0;
            puts("NO LOGS FOUND!!"); //printing this in tera term
            CLEAR_DISP_SCREEN;
            log_flag = 0;   //disabling entry flag
            state = e_main_menu;  //coming back to main menu
        }
    } else {  //if event count is present then print the logs stored in 2d array in tera term
        clcd_print(" DOWNLOAD LOGS  ", LINE1(0));
        clcd_print("   SUCCESSFUL   ", LINE2(0));
        if (delay++ == 100) {
            delay = 0;
            ev = event_count > 10 ? 10 : event_count;  //checking whether the event count is more than 10 ,if so making it as 10 
            puts("no   time   ev sp");
            puts("\n\r");
            for (int i = 0; i < ev; i++) {  //to print the events one after the another in tera term 
                sl_no = i + 48;   //to print log no in character
                puts(" ");
                putch(sl_no);
                puts(" ");
                puts(store_event[i]);   //printing events
                puts("\n\r");
            }
            CLEAR_DISP_SCREEN;
            log_flag = 0;
            state = e_main_menu;
        }
    }
}
/* to clear the log*/
void clear_log(void) {
    event_count = 0;  //to clear the log simply reset the event count as 0
    clcd_print("   CLEAR  LOG   ", LINE1(0));
    clcd_print("  SUCCESSFULLY  ", LINE2(0));
    if (delay++ == 1000) {
        delay = 0;
        CLEAR_DISP_SCREEN;
        log_flag = 0;
        state = e_main_menu;
    }
}
/*set time function*/
void set_time(void) {  

    clcd_print("    HH:MM:SS    ", LINE1(0));

    if (s_flag == 0) {   //s_flag is zero because only once the time value should be converted as integer and stored
        hr = time[0] - 48;  //time[0] is tense place and time[1]in units place
        hr = (hr * 10) + (time[1] - 48);  //converting the hour value in char to int
        min = time[3] - 48;
        min = (min * 10) + (time[4] - 48);//converting the min value in char to int
        sec = time[6] - 48;
        sec = (sec * 10) + (time[7] - 48);//converting the sec value in char to int
        s_flag = 1;
    }
    /*after converting it to integer shore thst value of time in a string by again converting from int to char*/
    time1[0] = hr / 10 + 48;
    time1[1] = hr % 10 + 48;
    time1[2] = ':';
    time1[3] = min / 10 + 48;
    time1[4] = min % 10 + 48;
    time1[5] = ':';
    time1[6] = sec / 10 + 48;
    time1[7] = sec % 10 + 48;
    time1[8] = '\0';

    clcd_print("    ", LINE2(0));
    /*for incrementing the value*/
    if (key == 1) {   
        if (field_flag == 1) {  //if field flag is 1 increment hour
            if (hr < 23)
                hr++;
            else
                hr = 0;
        } else if (field_flag == 2) {   //if field flag is 2 increment min
            if (min < 59)
                min++;
            else
                min = 0;
        } else if (field_flag == 3) {   //if field flag is 3 increment sec
            if (sec < 59)
                sec++;
            else
                sec = 0;
        }
    }
    /*to change the field*/
    if (key == 2) {  
        if (field_flag >= 1 && field_flag < 3) {
            field_flag++;
        } else if (field_flag == 3) {
            field_flag = 1;
        }
    }
    //to blink the hour field and change the hour value
    if (field_flag == 1) {  

        if (delay++ < 500) {

            time1[0] = (hr / 10) + 48;
            time1[1] = (hr % 10) + 48;

        } else if (delay++ < 1000) {

            time1[0] = 0xff;
            time1[1] = 0xff;
        } else
            delay = 0;

    } 
    //to blink the minute field and change the minute value
    else if (field_flag == 2) {

        if (delay++ < 500) {

            time1[3] = (min / 10) + 48;
            time1[4] = (min % 10) + 48;

        } else if (delay++ < 1000) {

            time1[3] = 0xff;
            time1[4] = 0xff;
        } else
            delay = 0;

    }
    //to blink the sec field and change the sec value
    else if (field_flag == 3) {

        if (delay++ < 500) {

            time1[6] = (sec / 10) + 48;
            time1[7] = (sec % 10) + 48;

        } else if (delay++ < 1000) {

            time1[6] = 0xff;
            time1[7] = 0xff;
        } else
            delay = 0;
    }
    clcd_print(time1, LINE2(4));
  /*to save the changes and exit*/
    if (key == 11) {
        write_ds1307(HOUR_ADDR, ((hr / 10) << 4) | (hr % 10));
        write_ds1307(MIN_ADDR, ((min / 10) << 4) | (min % 10));
        write_ds1307(SEC_ADDR, ((sec / 10) << 4) | (sec % 10));
        s_flag = 0;
        CLEAR_DISP_SCREEN;

        log_flag = 0;
        state = e_main_menu;
    }
/*to exit without saving*/
    if (key == 12) {
        CLEAR_DISP_SCREEN;
        
        log_flag = 0;
        state = e_main_menu;
    }
}

