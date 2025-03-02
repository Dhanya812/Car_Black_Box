/*
 * File:   main.c
 * Author: DHANYA
 * project : Car Black Box
 * Created on 3 February, 2025, 8:18 PM
 */

#include <xc.h>
#include "clcd.h"
#include "main.h"
#include "matrix_keypad.h"
#include "black_box.h"
#include "adc.h"
#include "uart.h"
#include "clcd.h"
#include "i2c.h"
#include "ds1307.h"
#include "external_eeprom.h"

State_t state;
unsigned char clock_reg[3];
unsigned int i;
unsigned short reg_val;
unsigned int event_count = 0;
unsigned int index = 0;
unsigned char address = 0;
unsigned int memory = 0;
unsigned char speed[3];
unsigned int flag;
char event[9][3] = {"ON", "GN", "G1", "G2", "G3", "G4", "G5", "GR", "C_"}; //events

void init_config() {

    init_adc();
    init_clcd();
    init_matrix_keypad();
    state = e_dashboard;
    init_uart();
    init_i2c();
    init_ds1307();
}

static void get_time(void) { //get time from real time clock
    clock_reg[0] = read_ds1307(HOUR_ADDR);
    clock_reg[1] = read_ds1307(MIN_ADDR);
    clock_reg[2] = read_ds1307(SEC_ADDR);

    if (clock_reg[0] & 0x40) {
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
        time[1] = '0' + (clock_reg[0] & 0x0F);
    } else {
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
        time[1] = '0' + (clock_reg[0] & 0x0F);
    }
    time[2] = ':';
    time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
    time[4] = '0' + (clock_reg[1] & 0x0F);
    time[5] = ':';
    time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
    time[7] = '0' + (clock_reg[2] & 0x0F);
    time[8] = '\0';
}

void view_dashboard() {
    get_time(); //to get time from RTC
    reg_val = (read_adc(CHANNEL4) / 11); //to read the value from ADC(potentiometer) and convert it to integer value
    /*converting value of adc which is speed from int to char to display on clcd*/
    speed[0] = (reg_val / 10) + 48; //tense 's place   
    speed[1] = (reg_val % 10) + 48; //unit's place
    speed[2] = '\0';

    if (index == 0 && event_count == 0) { //Storing ON Condition
        event_store();
    }
    if (key == 1) { //for collision event
        index = 8; //collision event detected
        event_store(); //storing the event
    }
    if (key == 2) { //to increment the gear events from GN to GR
        if (index >= 0 && index < 7) {
            index++;
        } else if (index == 8) { //if collision change it to GN
            index = 1;
        }
        event_store();
    }
    if (key == 3) { //to decrement the gear event from GR to GN
        if (index <= 7 && index > 1) {
            index--;
        } else if (index == 8) { //if collision change it to GN
            index = 1;
        }
        event_store();
    }
    /*CLCD displaying*/
    clcd_print("TIME", LINE1(0));
    clcd_print("EV", LINE1(9));
    clcd_print("SPEED", LINE1(14));
    clcd_print(time, LINE2(0));
    clcd_print(event[index], LINE2(9));
    clcd_putch(speed[0], LINE2(14));
    clcd_putch(speed[1], LINE2(15));

    if (key == 11) { //to enter into main menu
        CLEAR_DISP_SCREEN;
        state = e_main_menu;
    }
}

void event_store(void) { //function to store events when gear event changes
    /*to store the time (HH:MM:SSS) pattern*/
    for (int i = 0; i < 8; i++) {
        write_external_eeprom(address, time[i]);
        address++;
    }
    /*to store the events*/
    write_external_eeprom(address++, event[index][0]);
    write_external_eeprom(address++, event[index][1]);

    /*to store the speed*/
    write_external_eeprom(address++, speed[0]);
    write_external_eeprom(address++, speed[1]);

    /*as we are storing 10 events, 12 bytes for each event , once address reaches 120 change the address to 0, so that the new 
     events will be replaced by the oldest events*/
    if (address >= 120) {
        address = 0;
    }
    event_count++; //increment the event count for each event change
}

void main(void) {
    init_config();

    while (1) {

        key = read_switches(STATE_CHANGE);
        //Detect key press
        switch (state) {
            case e_dashboard:
                // Display dashboard
                view_dashboard();
                break;

            case e_main_menu:
                // Display dashboard
                display_main_menu();
                break;

            case e_view_log:
                // Display dashboard
                view_log();
                break;

            case e_download_log:
                download_log();
                break;

            case e_clear_log:
                clear_log();
                break;

            case e_set_time:
                set_time();
                break;
        }

    }

}
