#include <xc.h>
#include "clcd.h"
#include "main.h"
#include "matrix_keypad.h"
/* Enum for maintaining the app state */
typedef enum {
    e_dashboard, e_main_menu, e_view_log, e_set_time, e_download_log, e_clear_log
} State_t;
unsigned int log;
unsigned int point_flag = 0;
unsigned int log_flag =0;
unsigned char time[9];
unsigned char key;
extern State_t state; // App state

//Function declarations

//Dashboard function declaration
void view_dashboard(void);

//Storing events function declaration
void event_store(void);

//Password function declaration
//void password(void);

//main menu function declaration
void display_main_menu(void);

//View log function declaration
void view_log(void);

//Reading events function declaration
void event_reader(void);

//Change password function declaration
//void change_password(void);

//Set time function declaration
void set_time(void);

//Download log function _decleration
void download_log(void);

//Clear log function declaration
void clear_log(void);