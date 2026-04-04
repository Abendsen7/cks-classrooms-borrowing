#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"

#define WIDTH 1024
#define HEIGHT 600

extern const lv_font_t font0;



void testfont();


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


	//testfont();
	
	/*创建空闲教室查询主页面*/
	lv_obj_t *display_classrooms_root = lv_obj_create(NULL);
	lv_obj_t *display_classrooms_title = lv_obj_create(display_classrooms_root);
	lv_obj_t *display_classrooms_room = lv_obj_create(display_classrooms_root);
	lv_obj_set_width(display_classrooms_title, WIDTH);
	lv_obj_set_height(display_classrooms_title, HEIGHT/3);  					 //标题总高200，顶多放五行字
	lv_obj_set_pos(display_classrooms_title, 0, 0);
	lv_obj_set_width(display_classrooms_room, WIDTH);
	lv_obj_set_height(display_classrooms_room, HEIGHT*2/3);						 //下半部分总高400，可滑动，同时顶多放十行字
	lv_obj_set_pos(display_classrooms_room, 0, HEIGHT/3);
	lv_scr_load(display_classrooms_root);

	lv_obj_t *label_classrooms_title = lv_label_create(display_classrooms_title);
	lv_label_set_text(label_classrooms_title, "空闲教室查询");
	lv_obj_set_width(label_classrooms_title, 6*32);
	lv_obj_set_height(label_classrooms_title, 32);
	lv_obj_set_style_text_font(label_classrooms_title, &font0, 0);
	lv_obj_set_pos(label_classrooms_title, WIDTH/2-3*32, 0);

	lv_obj_t *label_classrooms_weeks = lv_label_create(display_classrooms_title);
	lv_label_set_text(label_classrooms_weeks, "----一周----");
	lv_obj_set_width(label_classrooms_weeks, 640);
	lv_obj_set_height(label_classrooms_weeks, 32);
	lv_obj_set_style_text_font(label_classrooms_weeks, &font0, 0);
	lv_obj_set_pos(label_classrooms_weeks, 0, 40);

	lv_obj_t *label_classrooms_query = lv_label_create(display_classrooms_title);
	lv_label_set_text(label_classrooms_query, "----查询----");
	lv_obj_set_width(label_classrooms_query, 640);
	lv_obj_set_height(label_classrooms_query, 32);
	lv_obj_set_style_text_font(label_classrooms_query, &font0, 0);
	lv_obj_set_pos(label_classrooms_query, 0, 80);

	
	for (int i = 0; i < 12; i++)
	{
		lv_obj_t *label_classrooms_period = lv_label_create(display_classrooms_room);
	  lv_label_set_text(label_classrooms_period, "第1节课");
	  lv_obj_set_height(label_classrooms_period, 32);
	  lv_obj_set_style_text_font(label_classrooms_period, &font0, 0);
	  lv_obj_set_pos(label_classrooms_period, 0, i*40*6);
		for (int j = 0; j < 5; j++)
		{
			lv_obj_t *label_classrooms_class = lv_label_create(display_classrooms_room);
		  lv_label_set_text(label_classrooms_class, "教室1");
		  lv_obj_set_height(label_classrooms_class, 32);
		  lv_obj_set_style_text_font(label_classrooms_class, &font0, 0);
		  lv_obj_set_pos(label_classrooms_class, 0, i*40*6+(j+1)*40);
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

