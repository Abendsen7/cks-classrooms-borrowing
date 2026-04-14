#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl/lvgl.h"
//#define IMG_PATH2        "0:/show1.bin" 
lv_obj_t *kb = NULL;
lv_obj_t *ID;
lv_obj_t *key;
lv_obj_t *text_area ;
lv_obj_t *text_area1 ;
lv_obj_t *load_in = NULL;
lv_obj_t *yourID;
lv_obj_t *yourkey;
//lv_obj_t *display2 = NULL;
//extern lv_obj_t *exp_img;
//static int32_t dir_x = 10;      
//static int32_t dir_y = 1;      
//static int32_t angle_step = 10; 
void show_keyboard(void);
//void btn_event1_cb(lv_event_t * e)
//{
//    lv_event_code_t code = lv_event_get_code(e);
//    lv_obj_t * btn = lv_event_get_target(e);
//    lv_timer_handler();
//	  if(code == LV_EVENT_PRESSED) {
//     gpio_bit_write(GPIOA, GPIO_PIN_1 , RESET);
//			
//			

//     
//		} 
//    else
//    {	gpio_bit_write(GPIOA, GPIO_PIN_1 , SET);
//     			
//		}		
//		lv_obj_t * label = lv_obj_get_child(btn, 0);
//    lv_label_set_text(label, "Blue");  
//    }
//void btn_event2_cb(lv_event_t * e)
// {
//    lv_event_code_t code = lv_event_get_code(e);
//    lv_obj_t * btn = lv_event_get_target(e);
//    if(code == LV_EVENT_CLICKED)
//			{
//        static uint8_t cnt = 0;
//        cnt++;

//        /*Get the first child of the button which is the label and change its text*/
//        lv_obj_t * label = lv_obj_get_child(btn, 0);
//        lv_label_set_text_fmt(label, "Button: %d", cnt);
//        gpio_bit_write(GPIOA, GPIO_PIN_2 , RESET);
//			}
//	}
void textarea_click_cb(lv_event_t *e)
{
   
    lv_keyboard_set_textarea(kb, text_area);

 
	  

    show_keyboard();
}
void textarea1_click_cb(lv_event_t *e)
{
   
    lv_keyboard_set_textarea(kb, text_area1);

 
	  

    show_keyboard();
}
//void exp_timer_cb(lv_timer_t *timer)
//{
//    


//   
//    lv_coord_t x = lv_obj_get_x(exp_img);
//    lv_coord_t y = lv_obj_get_y(exp_img);

//    x += dir_x;
//    y += dir_y;

//   
////    lv_coord_t screen_w = lv_obj_get_width(display2);
////    lv_coord_t screen_h = lv_obj_get_height(display2);
//    lv_coord_t img_w = lv_obj_get_width(exp_img);
//    lv_coord_t img_h = lv_obj_get_height(exp_img);

//   
////    if (x + img_w > screen_w) {
////        x = screen_w - img_w;
////        dir_x = -dir_x;
////    } else if (x < 0) {
////        x = 0;
////        dir_x = -dir_x;
////    }

////    if (y + img_h > screen_h) {
////        y = screen_h - img_h;
////        dir_y = -dir_y;
////    } else if (y < 0) {
////        y = 0;
////        dir_y = -dir_y;
////    }

//    lv_obj_set_pos(exp_img, x, y);

//  lv_point_t *pivot;
// lv_img_set_pivot(exp_img,0,0);
//	lv_img_get_pivot(exp_img,pivot);
//    int32_t angle = lv_img_get_angle(exp_img);
//    angle += angle_step;
//    if (angle >= 3600) angle -= 3600;
//    lv_img_set_angle(exp_img, angle);
//}
void keyBoard_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);    
    lv_obj_t *target = lv_event_get_target(e);       
    uint32_t id = 0;
    const char *text;

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        id = lv_btnmatrix_get_selected_btn(target); 
        text = lv_keyboard_get_btn_text(target, id);
        if (strcmp(text, LV_SYMBOL_OK) == 0)
				{
					 lv_label_set_text(	yourID	,lv_textarea_get_text(text_area));	
					lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
				;
          lv_keyboard_set_textarea(kb, NULL);		
				}
			
			}
		}
void keyBoard_event_cb1(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);    
    lv_obj_t *target1 = lv_event_get_target(e);       
    uint32_t id = 0;
    const char *text;

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        id = lv_btnmatrix_get_selected_btn(target1); 
        text = lv_keyboard_get_btn_text(target1, id);
        if (strcmp(text, LV_SYMBOL_OK) == 0)
				{
					 lv_label_set_text(	yourkey,lv_textarea_get_text(text_area1));	
					lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	
  
          lv_keyboard_set_textarea(kb, NULL);		
				}
			
			}
		}
