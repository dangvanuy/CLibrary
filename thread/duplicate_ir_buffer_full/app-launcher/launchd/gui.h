
#ifndef GUI_H_
#define GUI_H_


#define MAX_TEXT_LINE		20
#define TEXT_MENU_COUNT		22
#define MAX_TEXT_LENGTH			100
#define SPACE_BETWEEN_LINES			10 //Spaces between two lines

typedef enum
{
	MODE_NORMAL = 1,
	MODE_FAILED,
	MODE_AMP,
	MODE_DFB,
	MODE_APP_AMP_SELECT,
	MODE_APP_VOUT_SELECT,
	MODE_APP_EXIT_SELECT,
	MODE_APP_ZOOM_SELECT,
	MODE_APP_SETUP,
	MODE_APP_ZOOM,
	MODE_APP_PAN,
	MODE_APP_BENCHMARK,
	MODE_APP_DVD_PASSWORD,
	MODE_APP_MANAGE_STORAGE,
	MODE_APP_BAD_DISC_SELECT,
}eInputMode;

void init_gui(void);
void uninit_gui(void);
void uninit_osd_layer(void);



void show_menu(void);
void show_menu_red(void);
void show_menu_blue(void);
void show_menu_front_display(void);
void show_menu_config_front_display(void);
void show_menu_front_key(RMuint8 key);
void show_menu_top_front_key(void);
void show_menu_items_top_front_key(void);
void show_menu_tunner_and_output(void);
void show_menu_start_playing_channel(void);
void show_menu_start_playing_next_channel(void);
void show_menu_press_up_channel(void);
void show_menu_press_up_confirm(void);
void show_menu_update_production_firmware(void);
void show_menu_not_find_production_firmware(void);

void show_test_items(void);

void show_window(void);
void hide_window(void);

void show_message(int message_id);
void show_message_with_key_return(int message_id);
void show_confirm_message(int message_id);
void show_confirm_message_update_fw(int message_id);

char *get_script_file_name(int item_id);
void set_test_pass_status(int item);
void set_test_failed_status(int item);
void set_menu_top_front_key_pass_status(int item);
void set_menu_top_front_key_failed_status(int item);
int check_test_items_status(void);
void show_channel_playing(int chan);
void show_menu_get_key(void);
void show_message_starting_get_key(void);
void show_message_get_key_successful(void);
void show_message_get_key_failed(void);
void change_get_key_text(void);

#endif /*GUI_H_*/
