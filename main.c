#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl/lvgl.h"

#define WIDTH 1024
#define HEIGHT 600
#define CWIDTH 150
#define DWIDTH (WIDTH - CWIDTH)

#define MAXWEEK 10
#define MAXPERIOD 12

#define MAXROOM 80

extern const lv_font_t font0;

void testfont();

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

typedef struct
{
	int id;						  // 教室编号
	bool available;				  // 是否可用
	int capacity;				  // 容纳人数
	bool projector;				  // 是否有投影仪
	bool occupied[MAXPERIOD + 1]; // 教室课程表,1表示被占用
} Classroom;

typedef struct
{
	Classroom rooms[100]; // 教室数组
	char name[5];		  // 教学楼名称
	int floor;			  // 几层楼
	int room;			  // 一层楼多少教室 (默认每层教室数量相同)
} Building;

Building buildings[10];
Building *nbuilding;

/**
 * 初始化building
 * @note 只给下面这个函数用，别直接用
 */
void init_building(Building *build, char name[], int floor, int room, int cap)
{
	build->floor = floor;
	build->room = room;
	strcpy(build->name, name);

	int n = 0;
	for (int i = 1; i <= floor; i++)
	{
		for (int j = 1; j <= room; j++)
		{
			build->rooms[n].id = i * 100 + j;
			build->rooms[n].available = 1; // 默认可用
			build->rooms[n].capacity = cap;
			build->rooms[n].projector = 1; // 默认有投影仪
			n++;
			if (n >= 99)
			{
				break;
			}
		}
	}
}

/**
 * 初始化所有building
 * @note 直接用这个
 */
void init_all_buildings(Building *builds)
{
	init_building(&builds[0], "e5", 1, 25, 30);
	init_building(&builds[1], "e9A", 5, 10, 36);
	init_building(&builds[2], "e9B", 5, 3, 150);
	init_building(&builds[3], "e9C", 5, 3, 150);
	init_building(&builds[4], "e9D", 5, 15, 80);
	init_building(&builds[5], "e12", 4, 15, 150);
	init_building(&builds[6], "e12F", 3, 3, 340);
	init_building(&builds[7], "w5", 3, 15, 180);
	init_building(&builds[8], "w12S", 5, 10, 230);
	init_building(&builds[9], "w12N", 5, 10, 230);
}

/**
 * 清空教学楼的课程表
 * @param build  教学楼
 */
void clear_building(Building *build)
{
	for (int i = 0; i < build->floor * build->room; i++)
	{
		for (int j = 0; j <= MAXPERIOD; j++)
		{
			build->rooms[i].occupied[j] = 0;
		}
	}
}

/**
 * 修改课程表中教室状态
 * @param room 	教室
 * @param fp 	起始节
 * @param fp 	结束节
 */
void room_reserve(Classroom *room, int fp, int lp)
{
	for (int i = fp; i <= lp; i++)
	{
		room->occupied[i] = 1;
	}
}

void build_showpage(lv_obj_t *display, int page);

typedef struct
{
	int *page;
	lv_obj_t *display;
} btn_next_cb_data;

void btn_next_event_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	btn_next_cb_data *data = lv_event_get_user_data(e);
	switch (code)
	{
	case LV_EVENT_PRESSED:
		build_showpage(data->display, ++*(data->page));
		break;
	default:
		break;
	}
}

void btn_prev_event_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	btn_next_cb_data *data = lv_event_get_user_data(e);
	switch (code)
	{
	case LV_EVENT_PRESSED:
		build_showpage(data->display, --*(data->page));
		break;
	default:
		break;
	}
}

/**
 * 显示空闲教室
 * @param display 	显示空闲教室的界面，即display_classrooms_room
 * @param page 		当前显示第几页
 */
void build_showpage(lv_obj_t *display, int page)
{
	lv_obj_clean(display);
	static int p;
	p = page;
	int num = 0;								  // 用来存每个时间段内空闲的教室数量
	int l = 0;									  // 用来存已经用了几行
	int per = nbuilding->floor * nbuilding->room; // 某一节课最多有多少空闲教室
	int fp = MAXROOM / per * (p - 1) + 1;		  // 这一面从第几节开始
	int period = fp;
	for (; (period < fp + MAXROOM / per) && (period <= MAXPERIOD); period++)
	{
		lv_obj_t *label_classrooms_period = lv_label_create(display);
		char temp1[20];
		snprintf(temp1, sizeof(temp1), "第%d节课", period);
		lv_label_set_text(label_classrooms_period, temp1);
		lv_obj_set_height(label_classrooms_period, 32);
		lv_obj_set_style_text_font(label_classrooms_period, &font0, 0);
		lv_obj_set_pos(label_classrooms_period, 0, (period - fp) * 40 * (per / 8 + 2));

		l++;
		num = 0;
		for (int n = 0; n < per; n++)
		{
			if (nbuilding->rooms[n].occupied[period] == 0)
			{

				lv_obj_t *btn_classrooms_room = lv_btn_create(display);
				lv_obj_set_width(btn_classrooms_room, 100);
				lv_obj_set_height(btn_classrooms_room, 32);
				lv_obj_set_pos(btn_classrooms_room, num % 8 * 104, (period - fp) * 40 * (per / 8 + 2) + (num / 8 + 1) * 40);
				// lv_obj_add_event_cb(btn_classrooms_class, btn_classrooms_room_event_cb, LV_EVENT_ALL, NULL);
				lv_obj_t *label_btn_classrooms_room = lv_label_create(btn_classrooms_room);
				lv_obj_center(label_btn_classrooms_room);
				lv_obj_set_height(label_btn_classrooms_room, 32);
				lv_obj_set_style_text_font(label_btn_classrooms_room, &font0, 0);
				char temp2[20];
				snprintf(temp2, sizeof(temp2), "%d", nbuilding->rooms[n].id);
				lv_label_set_text(label_btn_classrooms_room, temp2);

				num++;
			}
		}
		l += num / 8;
		if (l % 8)
		{
			l++;
		}
	}

	int maxpage = MAXPERIOD / (MAXROOM / per) + ((MAXPERIOD % (MAXROOM / per)) ? 1 : 0);
	lv_obj_t *label_page = lv_label_create(display);
	char temp3[15];
	snprintf(temp3, sizeof(temp3), "page:%d/%d", p, maxpage);
	lv_label_set_text(label_page, temp3);
	lv_obj_set_width(label_page, 50);
	lv_obj_set_height(label_page, 32);
	lv_obj_set_pos(label_page, 104 * 5 + 50, (period - fp) * 40 * (per / 8 + 2) + 60);

	static btn_next_cb_data data;
	data.page = &p;
	data.display = display;
	if (p < maxpage)
	{
		lv_obj_t *btn_next = lv_btn_create(display);
		lv_obj_set_width(btn_next, 80);
		lv_obj_set_height(btn_next, 32);
		lv_obj_set_pos(btn_next, 104 * 7, (period - fp) * 40 * (per / 8 + 2) + 60);
		lv_obj_add_event_cb(btn_next, btn_next_event_cb, LV_EVENT_ALL, &data);
		lv_obj_t *label_btn_next = lv_label_create(btn_next);
		lv_obj_center(label_btn_next);
		lv_obj_set_height(label_btn_next, 32);
		lv_obj_set_style_text_font(label_btn_next, &font0, 0);
		lv_label_set_text(label_btn_next, "2");
	}
	if (p > 1)
	{
		lv_obj_t *btn_prev = lv_btn_create(display);
		lv_obj_set_width(btn_prev, 80);
		lv_obj_set_height(btn_prev, 32);
		lv_obj_set_pos(btn_prev, 104 * 6, (period - fp) * 40 * (per / 8 + 2) + 60);
		lv_obj_add_event_cb(btn_prev, btn_prev_event_cb, LV_EVENT_ALL, &data);
		lv_obj_t *label_btn_prev = lv_label_create(btn_prev);
		lv_obj_center(label_btn_prev);
		lv_obj_set_height(label_btn_prev, 32);
		lv_obj_set_style_text_font(label_btn_prev, &font0, 0);
		lv_label_set_text(label_btn_prev, "1");
	}
}

/*查询筛选功能*/

/*单条预约信息链表*/
typedef struct ReservationList ReservationList;
struct ReservationList
{
	int id; // 预约编号
	int week;
	int day;
	int fp;				   // 起始时间
	int lp;				   // 结束时间
	char building[5];	   // 所属教学楼
	int room;			   // 教室编号
	char use[100];		   // 预约用途
	int num;			   // 预约人数
	int status;			   // 预约状态，0表示待审批，1表示已批准，2表示已拒绝
	char reserver[30];	   // 预约人
	char admin[30];		   // 审批人
	ReservationList *next; // 这样就能当链表用
};

/*单条预约信息纯净版*/
typedef struct
{
	int id; // 预约编号
	int week;
	int day;
	int fp;			   // 起始时间
	int lp;			   // 结束时间
	char building[5];  // 所属教学楼
	int room;		   // 教室编号
	char use[100];	   // 预约用途
	int num;		   // 预约人数
	int status;		   // 预约状态，0表示待审批，1表示已批准，2表示已拒绝
	char reserver[30]; // 预约人
	char admin[30];	   // 审批人
} Reservation;

/**
 * 导入预约信息
 * @param head 	预约信息头指针
 * @return 		预约信息尾指针
 */
ReservationList *loadall_reservations(ReservationList *head)
{
	FIL fp;
	UINT bytes_read;
	Reservation buf;

	if (f_open(&fp, "0:/reservations.bin", FA_READ) != FR_OK)
	{
		f_mount(NULL, "", 0);
	}

	ReservationList *now = head;

	while (1)
	{
		f_read(&fp, &buf, sizeof(Reservation), &bytes_read);
		if (bytes_read != sizeof(Reservation))
		{
			// 读到文件末尾了
			free(now->next);
			now->next = NULL;
			break;
		}

		now->id = buf.id;
		now->week = buf.week;
		now->day = buf.day;
		now->fp = buf.fp;
		now->lp = buf.lp;
		strcpy(now->building, buf.building);
		now->room = buf.room;
		strcpy(now->use, buf.use);
		now->num = buf.num;
		now->status = buf.status;
		strcpy(now->reserver, buf.reserver);
		strcpy(now->admin, buf.admin);

		now->next = (ReservationList *)malloc(sizeof(ReservationList));
		now = now->next;
	}
	f_close(&fp);
	return now;
}

/**
 * 存入预约信息
 * @param head 	预约信息头指针
 */
void saveall_reservations(ReservationList *head)
{
	FIL fp;
	UINT bytes_write;
	Reservation buf;

	if (f_open(&fp, "0:/reservations.bin", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
	{
		f_mount(NULL, "", 0);
	}

	ReservationList *now = head;

	while (1)
	{
		buf.id = now->id;
		buf.week = now->week;
		buf.day = now->day;
		buf.fp = now->fp;
		buf.lp = now->lp;
		strcpy(buf.building, now->building);
		buf.room = now->room;
		strcpy(buf.use, now->use);
		buf.num = now->num;
		buf.status = now->status;
		strcpy(buf.reserver, now->reserver);
		strcpy(buf.admin, now->admin);
		f_write(&fp, &buf, sizeof(Reservation), &bytes_write);
		if (now->next == NULL)
		{
			// 保存成功
			break;
		}
		else
		{
			now = now->next;
		}
	}

	f_close(&fp);
}

/**
 * 载入审批通过的预约信息至课程表
 * @param builds 	教学楼数组
 * @param head 		预约信息头指针
 * @param week 		第几周
 * @param day 		星期几
 */
void builds_reserveday(Building *build, ReservationList *head, int week, int day)
{
	clear_building(build);
	ReservationList *now = head;
	while (1)
	{
		if (now->status == 1 && now->week == week && now->day == day && !strcmp(build->name, now->building))
		{

			for (int j = 0; j < (build->floor * build->room); j++) // 找到教室
			{
				if (build->rooms[j].id == now->room)
				{
					room_reserve(&build->rooms[j], now->fp, now->lp);
					break;
				}
			}
		}
		if (now->next == NULL)
		{
			break;
		}
		else
		{
			now = now->next;
		}
	}
}

typedef struct
{
	lv_obj_t **btns;
	int *day;
	Building *building;
	lv_obj_t *screen;
	ReservationList *head;
} btn_weekdays_cb_data;

void weekdays(lv_obj_t *display, lv_obj_t **btn_weekdays, int *day, Building *build, lv_obj_t *scr, ReservationList *head);

int week;
int day;

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

	/*初始化教室信息*/
	init_all_buildings(buildings);
	nbuilding = &buildings[2];

	/*创建空闲教室查询主页面*/
	lv_obj_t *display_classrooms_root = lv_obj_create(NULL);
	lv_obj_t *display_classrooms_choices = lv_obj_create(display_classrooms_root);
	lv_obj_t *display_classrooms_title = lv_obj_create(display_classrooms_root);
	lv_obj_t *display_classrooms_room = lv_obj_create(display_classrooms_root);
	lv_obj_set_width(display_classrooms_title, CWIDTH);
	lv_obj_set_height(display_classrooms_title, HEIGHT);
	lv_obj_set_pos(display_classrooms_title, CWIDTH, 0);

	lv_obj_set_width(display_classrooms_title, DWIDTH);
	lv_obj_set_height(display_classrooms_title, HEIGHT / 3); // 标题总高200，顶多放五行字
	lv_obj_set_pos(display_classrooms_title, CWIDTH, 0);

	lv_obj_set_width(display_classrooms_room, DWIDTH);
	lv_obj_set_height(display_classrooms_room, HEIGHT * 2 / 3); // 下半部分总高400，可滑动，同时顶多放十行字
	lv_obj_set_pos(display_classrooms_room, CWIDTH, HEIGHT / 3);

	lv_scr_load(display_classrooms_root); // 这行能加载空闲教室查询界面

	lv_obj_t *label_classrooms_title = lv_label_create(display_classrooms_title);
	lv_label_set_text(label_classrooms_title, "空闲教室查询");
	lv_obj_set_width(label_classrooms_title, 6 * 32);
	lv_obj_set_height(label_classrooms_title, 32);
	lv_obj_set_style_text_font(label_classrooms_title, &font0, 0);
	lv_obj_set_pos(label_classrooms_title, DWIDTH / 2 - 3 * 32, 0);

	lv_obj_t *label_classrooms_query = lv_label_create(display_classrooms_title);
	lv_label_set_text(label_classrooms_query, "----查询----");
	lv_obj_set_width(label_classrooms_query, 640);
	lv_obj_set_height(label_classrooms_query, 32);
	lv_obj_set_style_text_font(label_classrooms_query, &font0, 0);
	lv_obj_set_pos(label_classrooms_query, 0, 3 * 40);

	/*
	lv_obj_t *btn_classrooms_back = lv_btn_create(display_classrooms_choices); // 按下回到登录界面
	lv_obj_set_width(btn_classrooms_back, 80);
	lv_obj_set_height(btn_classrooms_back, 60);
	lv_obj_add_event_cb(btn_classrooms_back, btn_classrooms_back_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_t *label_btn_classrooms_back = lv_label_create(btn_classrooms_back);
	lv_obj_center(label_btn_classrooms_back);
	lv_obj_set_height(label_btn_classrooms_back, 32);
	lv_obj_set_style_text_font(label_btn_classrooms_back, &font0, 0);
	lv_label_set_text(label_btn_classrooms_back, "取消");
	*/

	/*
			ReservationList *reservation_head = (ReservationList *)malloc(sizeof(ReservationList));
			reservation_head->id = 1;
			reservation_head->week = 1;
			reservation_head->day = 1;
			reservation_head->fp = 1;
			reservation_head->lp = 2;
			strcpy(reservation_head->building, "e9B");
			reservation_head->room = 101;
			strcpy(reservation_head->use, "use");
			reservation_head->num = 20;
			reservation_head->status = 1;
			strcpy(reservation_head->reserver, "reserver");
			strcpy(reservation_head->admin, "admin");
			reservation_head->next = (ReservationList *)malloc(sizeof(ReservationList));

			ReservationList *now = reservation_head->next;
			now->id = 2;
			now->week = 1;
			now->day = 1;
			now->fp = 1;
			now->lp = 4;
			strcpy(now->building,"e9B");
			now->room = 201;
			strcpy(now->use, "use");
			now->num = 20;
			now->status = 1;
			strcpy(now->reserver, "reserver");
			strcpy(now->admin, "admin");
			now->next = NULL;
		*/

	ReservationList *reservation_head = (ReservationList *)malloc(sizeof(ReservationList));
	ReservationList *reservation_tail = loadall_reservations(reservation_head);
	// saveall_reservations(reservation_head);

	lv_obj_t *btn_weekdays[7];
	week = 1;
	day = 1;
	weekdays(display_classrooms_title, btn_weekdays, &day, nbuilding, display_classrooms_room, reservation_head);

	lv_obj_add_state(btn_weekdays[0], LV_STATE_CHECKED);
	builds_reserveday(nbuilding, reservation_head, 1, 1);
	build_showpage(display_classrooms_room, 1);

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
void weekdays(lv_obj_t *display, lv_obj_t **btn_weekdays, int *day, Building *build, lv_obj_t *scr, ReservationList *head)
{
	for (int i = 0; i < 7; i++)
	{

		static btn_weekdays_cb_data data;
		data.btns = btn_weekdays;
		data.day = day;
		data.building = nbuilding;
		data.screen = scr;
		data.head = head;

		btn_weekdays[i] = lv_btn_create(display);
		lv_obj_add_flag(btn_weekdays[i], LV_OBJ_FLAG_CHECKABLE);
		lv_obj_set_width(btn_weekdays[i], (DWIDTH - 50) / 7 - 5); // 预留50px防止按钮会溢出屏幕,两个按钮之间间隔5px
		lv_obj_set_height(btn_weekdays[i], 32);
		lv_obj_set_pos(btn_weekdays[i], i * (DWIDTH - 50) / 7, 2 * 40);
		lv_obj_t *label_btn_weekdays = lv_label_create(btn_weekdays[i]);
		lv_obj_center(label_btn_weekdays);
		lv_obj_set_height(label_btn_weekdays, 32);
		lv_obj_set_style_text_font(label_btn_weekdays, &font0, 0);
		lv_obj_add_event_cb(btn_weekdays[i], btn_weekdays_event_cb, LV_EVENT_ALL, &data);
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
	btn_weekdays_cb_data *data = lv_event_get_user_data(e);
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

		// 取消其他按钮的选择,修改星期几变量
		for (int i = 0; i < 7; i++)
		{
			if (data->btns[i] != btn && lv_obj_has_state(data->btns[i], LV_STATE_CHECKED))
			{
				lv_obj_clear_state(data->btns[i], LV_STATE_CHECKED);
			}
			else if (data->btns[i] == btn)
			{
				*(data->day) = i + 1;
				builds_reserveday(data->building, data->head, week, *(data->day));
				build_showpage(data->screen, 1);
			}
		}
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
