#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl/lvgl.h"

#define WIDTH 1024
#define HEIGHT 600
#define CWIDTH 150

extern const lv_font_t font0;

void testfont();
void weekdays(lv_obj_t *display_classrooms_title, lv_obj_t **btn_weekdays);
void btn_weekdays_event_cb(lv_event_t *e);
void btn_classrooms_back_event_cb(lv_event_t *e);

static lv_obj_t *load_in = NULL;
extern lv_obj_t *text_area;
extern lv_obj_t *text_area1;
void create_load_in(void);
void create_input_ID(void);
void create_input_key(void);
extern lv_obj_t *kb;
void create_keyboard(void);
void textarea_click_cb(lv_event_t *e);
void textarea1_click_cb(lv_event_t *e);
void keyBoard_event_cb(lv_event_t *e);
void keyBoard_event_cb1(lv_event_t *e);
void input_ID(void);
void input_key(void);
extern lv_obj_t *yourID;
extern lv_obj_t *yourkey;
void create_enter(void);

int main()
{

	/*模板自带的系统初始化*/
	sys_init();

	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_60MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	gpio_bit_reset(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();

	// testfont();

	create_load_in();
	lv_scr_load(load_in);
	create_keyboard();
	create_input_ID();
	create_enter();
	create_input_key();
	input_ID();
	input_key();

	/*创建空闲教室查询主页面*/
	lv_obj_t *display_classrooms_root = lv_obj_create(NULL);
	lv_obj_t *display_classrooms_choices = lv_obj_create(display_classrooms_root);
	lv_obj_t *display_classrooms_title = lv_obj_create(display_classrooms_root);
	lv_obj_t *display_classrooms_room = lv_obj_create(display_classrooms_root);
	lv_obj_set_width(display_classrooms_title, CWIDTH);
	lv_obj_set_height(display_classrooms_title, HEIGHT);
	lv_obj_set_pos(display_classrooms_title, CWIDTH, 0);

	lv_obj_set_width(display_classrooms_title, WIDTH - CWIDTH);
	lv_obj_set_height(display_classrooms_title, HEIGHT / 3); // 标题总高200，顶多放五行字
	lv_obj_set_pos(display_classrooms_title, CWIDTH, 0);

	lv_obj_set_width(display_classrooms_room, WIDTH - CWIDTH);
	lv_obj_set_height(display_classrooms_room, HEIGHT * 2 / 3); // 下半部分总高400，可滑动，同时顶多放十行字
	lv_obj_set_pos(display_classrooms_room, CWIDTH, HEIGHT / 3);
	lv_scr_load(display_classrooms_root);

	lv_obj_t *label_classrooms_title = lv_label_create(display_classrooms_title);
	lv_label_set_text(label_classrooms_title, "空闲教室查询");
	lv_obj_set_width(label_classrooms_title, 6 * 32);
	lv_obj_set_height(label_classrooms_title, 32);
	lv_obj_set_style_text_font(label_classrooms_title, &font0, 0);
	lv_obj_set_pos(label_classrooms_title, (WIDTH - CWIDTH) / 2 - 3 * 32, 0);

	lv_obj_t *btn_weekdays[7];
	weekdays(display_classrooms_title, btn_weekdays);

	lv_obj_t *label_classrooms_query = lv_label_create(display_classrooms_title);
	lv_label_set_text(label_classrooms_query, "----查询----");
	lv_obj_set_width(label_classrooms_query, 640);
	lv_obj_set_height(label_classrooms_query, 32);
	lv_obj_set_style_text_font(label_classrooms_query, &font0, 0);
	lv_obj_set_pos(label_classrooms_query, 0, 3 * 40);

	lv_obj_t *btn_classrooms_back = lv_btn_create(display_classrooms_choices);
	lv_obj_set_width(btn_classrooms_back, 80);
	lv_obj_set_height(btn_classrooms_back, 60);
	lv_obj_add_event_cb(btn_classrooms_back, btn_classrooms_back_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_t *label_btn_classrooms_back = lv_label_create(btn_classrooms_back);
	lv_obj_center(label_btn_classrooms_back);
	lv_obj_set_height(label_btn_classrooms_back, 32);
	lv_obj_set_style_text_font(label_btn_classrooms_back, &font0, 0);
	lv_label_set_text(label_btn_classrooms_back, "取消");

	for (int i = 0; i < 12; i++)
	{
		lv_obj_t *label_classrooms_period = lv_label_create(display_classrooms_room);
		lv_label_set_text(label_classrooms_period, "第1节课");
		lv_obj_set_height(label_classrooms_period, 32);
		lv_obj_set_style_text_font(label_classrooms_period, &font0, 0);
		lv_obj_set_pos(label_classrooms_period, 0, i * 40 * 6);
		for (int j = 0; j < 5; j++)
		{
			lv_obj_t *label_classrooms_class = lv_label_create(display_classrooms_room);
			lv_label_set_text(label_classrooms_class, "教室1");
			lv_obj_set_height(label_classrooms_class, 32);
			lv_obj_set_style_text_font(label_classrooms_class, &font0, 0);
			lv_obj_set_pos(label_classrooms_class, 0, i * 40 * 6 + (j + 1) * 40);
		}
	}

	while (1)
	{

		delay_us(2000);
		lv_timer_handler();
	}
}

/*我只是一个用来测试字体的函数*/
void testfont()
{
	lv_obj_t *testlabel = lv_label_create(lv_scr_act());
	lv_obj_set_width(testlabel, 300);
	lv_obj_set_height(testlabel, 150);
	lv_obj_set_style_text_font(testlabel, &font0, 0);
	lv_label_set_text(testlabel, "华科C课设");
}

/**
 * 创建一行七个按钮，用于切换周一至周日
 * @param display_classrooms_title 标题界面，作为按钮的父对象
 * @param btn_weekdays 存储七个按钮的数组
 */
void weekdays(lv_obj_t *display_classrooms_title, lv_obj_t **btn_weekdays)
{
	for (int i = 0; i < 7; i++)
	{
		btn_weekdays[i] = lv_btn_create(display_classrooms_title);
		lv_obj_add_flag(btn_weekdays[i], LV_OBJ_FLAG_CHECKABLE);
		lv_obj_set_width(btn_weekdays[i], (WIDTH - CWIDTH - 50) / 7 - 5); // 预留50px防止按钮会溢出屏幕,两个按钮之间间隔5px
		lv_obj_set_height(btn_weekdays[i], 32);
		lv_obj_set_pos(btn_weekdays[i], i * (WIDTH - CWIDTH - 50) / 7, 2 * 40);
		lv_obj_t *label_btn_weekdays = lv_label_create(btn_weekdays[i]);
		lv_obj_center(label_btn_weekdays);
		lv_obj_set_height(label_btn_weekdays, 32);
		lv_obj_set_style_text_font(label_btn_weekdays, &font0, 0);
		lv_obj_add_event_cb(btn_weekdays[i], btn_weekdays_event_cb, LV_EVENT_ALL, btn_weekdays);
		switch (i)
		{
		case 0:
			lv_label_set_text(label_btn_weekdays, "周一");
			break;
		case 1:
			lv_label_set_text(label_btn_weekdays, "周二");
			break;
		case 2:
			lv_label_set_text(label_btn_weekdays, "周三");
			break;
		case 3:
			lv_label_set_text(label_btn_weekdays, "周四");
			break;
		case 4:
			lv_label_set_text(label_btn_weekdays, "周五");
			break;
		case 5:
			lv_label_set_text(label_btn_weekdays, "周六");
			break;
		case 6:
			lv_label_set_text(label_btn_weekdays, "周日");
			break;
		}
	}
}

/**
 * 按下按钮后，选择这一天，并将其他按钮取消选择
 * @note 需将btn_weekdays作为user_data传入
 */
void btn_weekdays_event_cb(lv_event_t *e)
{
	lv_obj_t **weekdays = lv_event_get_user_data(e);
	lv_obj_t *btn = lv_event_get_target(e);
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_VALUE_CHANGED:
		// 再次点击按钮无法取消选择
		if (!lv_obj_has_state(btn, LV_STATE_CHECKED))
		{
			lv_obj_add_state(btn, LV_STATE_CHECKED);
			break;
		}

		// 取消其他按钮的选择
		for (int i = 0; i < 7; i++)
		{
			if (weekdays[i] != btn && lv_obj_has_state(weekdays[i], LV_STATE_CHECKED))
			{
				lv_obj_clear_state(weekdays[i], LV_STATE_CHECKED);
			}
		}

		// 根据按下的按钮修改显示的room界面(界面都还没做呢)
		// lv_scr_load(display_classrooms_rooms[i]);
		break;
	default:
		break;
	}
}

/**
 * 按下按钮后，回到登录界面
 */
void btn_classrooms_back_event_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_PRESSED:
		// 回到登录界面
		lv_scr_load(load_in);
		break;
	default:
		break;
	}
}

void create_enter(void)
{

	lv_obj_t *btn1 = lv_btn_create(lv_scr_act()); /*Add a button the current screen*/
	lv_obj_set_pos(btn1, 500, 400);				  /*Set its position*/
	lv_obj_set_size(btn1, 120, 50);				  /*Set its size*/

	lv_obj_t *label = lv_label_create(btn1); /*Add a label to the button*/
	lv_label_set_text(label, "enter");
}

void create_load_in(void)
{
	load_in = lv_obj_create(NULL);
	lv_obj_set_size(load_in, LV_HOR_RES, LV_VER_RES);

	lv_obj_set_style_bg_color(load_in, lv_color_hex(0xEEEEEE), LV_PART_MAIN);
}
void create_input_ID(void)
{
	lv_obj_t *ID = lv_label_create(load_in);
	lv_label_set_text(ID, "ID");
	lv_obj_align(ID, LV_ALIGN_TOP_MID, -100, -20);
	lv_obj_set_style_text_font(ID, LV_FONT_MONTSERRAT_20, LV_PART_MAIN);
	text_area = lv_textarea_create(load_in);
	lv_textarea_set_placeholder_text(text_area, "please input your ID");

	lv_obj_set_size(text_area, 280, 80);
	lv_obj_align(text_area, LV_ALIGN_CENTER, 0, -30);
	yourID = lv_label_create(load_in);
	lv_label_set_text(yourID, "yourID");
	lv_obj_set_pos(yourID, 230, 250);
}
void create_input_key(void)
{
	lv_obj_t *key = lv_label_create(load_in);
	lv_label_set_text(key, "key");
	lv_obj_align(key, LV_ALIGN_TOP_MID, 100, -20);
	lv_obj_set_style_text_font(key, LV_FONT_MONTSERRAT_20, LV_PART_MAIN);
	text_area1 = lv_textarea_create(load_in);
	lv_textarea_set_placeholder_text(text_area1, "please input your key");

	lv_obj_set_size(text_area1, 280, 80);
	lv_obj_align(text_area1, LV_ALIGN_CENTER, 0, 30);
	yourkey = lv_label_create(load_in);
	lv_label_set_text(yourkey, "yourkey");
	lv_obj_set_pos(yourkey, 230, 350);
}
void create_keyboard(void)
{
	kb = lv_keyboard_create(load_in);
	lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
}
void input_ID(void)
{
	lv_obj_add_event_cb(text_area, textarea_click_cb, LV_EVENT_CLICKED, kb);
	lv_obj_add_event_cb(kb, keyBoard_event_cb, LV_EVENT_ALL, NULL);
}
void input_key(void)
{
	lv_obj_add_event_cb(text_area1, textarea1_click_cb, LV_EVENT_CLICKED, kb);
	lv_obj_add_event_cb(kb, keyBoard_event_cb1, LV_EVENT_ALL, NULL);
}

void hide_keyboard(void)
{
	if (kb != NULL && !lv_obj_has_flag(kb, LV_OBJ_FLAG_HIDDEN))
	{
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
}

void show_keyboard(void)
{
	lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
}
