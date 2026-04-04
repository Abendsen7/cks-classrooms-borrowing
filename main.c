#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"

extern const lv_font_t font0;

void Hang2Hang();
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

	lv_style_t style;
	lv_style_init(&style);
	lv_style_set_text_font(&style, &font0);
	lv_obj_add_style(testlabel,&style,0);

	lv_label_set_text(testlabel, "一二三四五六七八九十年月日周0123456789第SNABCD楼层东西取消确认查询重
							置详情至空闲占用教室节次借用记录条起始结束用途日期参与人数联系电话容量
							设备要求下步选择不能为空错误用户名密码登录请输入情况预览华中科技大学课设！。：-");
}