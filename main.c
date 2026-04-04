#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"

extern const lv_font_t font0;
extern const lv_font_t test;

void testfont();


int main()
{
		sys_init();
		
		rcu_periph_clock_enable(RCU_GPIOA);
		gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
		gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_60MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
		gpio_bit_reset(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
		
		lv_init();
		lv_port_disp_init();
		lv_port_indev_init();
	
		testfont();
		


		while(1){
			
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

