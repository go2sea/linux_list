#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <errno.h>
#include "list.h"

//typedef void (*Foo)(const struct app_info*);//定义一个函数指针

//定义app_info链表结构
typedef struct application_info
{
	uint32_t  app_id;
	uint32_t  up_flow;
	uint32_t  down_flow;
	struct    list_head app_info_node;//链表节点
} app_info;

//检查malloc是否失败
static int check_malloc(void* p) {
	if (p == NULL) {
		fprintf(stderr, "Failed to malloc memory, errno:%u, reason:%s\n",
			errno, strerror(errno));
		return 0;
	}
	return 1;
}

static void print_app_info(struct app_info* app) {
	app_info* temp = app;
	printf("app_id:%u\tup_flow:%u\tdown_flow:%u\n", temp->app_id, temp->up_flow, temp->down_flow);
}

//构造一个app_info
app_info* get_app_info(uint32_t app_id, uint32_t up_flow, uint32_t down_flow)
{
	app_info *app = (app_info*)malloc(sizeof(app_info));
	if (!check_malloc(app))
		return NULL;
	app->app_id = app_id;
	app->up_flow = up_flow;
	app->down_flow = down_flow;
	return app;
}

//遍历链表
static void for_each_app(const struct list_head* head, void(*foo)(const struct app_info*)) {
	struct list_head* pos;
	app_info *app;
	list_for_each(pos, head) {
		app = container_of(pos, app_info, app_info_node);
		foo(app);
	}
}

//释放链表
void destroy_app_list(struct list_head* head) {
	struct list_head* temp = NULL;
	struct list_head* pos;
	list_for_each(pos, head) {
		temp = pos->next;
		//摘掉节点
		list_del(pos);
		//释放节点
		free(container_of(pos, app_info, app_info_node));
		pos = temp;
	}
}

int main() {
	//头节点，不get_app_info， 直接malloc
	app_info* app_info_list = (app_info*)malloc(sizeof(app_info));
	if (!check_malloc(app_info_list))
		return -1;
	app_info* app;
	//链表头
	struct list_head* head = &app_info_list->app_info_node;
	init_list_head(head);
	app = get_app_info(1001, 100, 200);
	list_add_tail(&app->app_info_node, head);
	app = get_app_info(1002, 200, 300);
	list_add_tail(&app->app_info_node, head);
	app = get_app_info(1003, 300, 400);
	list_add_tail(&app->app_info_node, head);

	printf("At the begin:\n");
	for_each_app(head, print_app_info);
	printf("After moving the head to the tail:\n");
	list_move_tail(head->next, head);
	for_each_app(head, print_app_info);
	printf("After deleting the tail:\n");
	list_del(head->prev);
	for_each_app(head, print_app_info);
	//删除并释放
	destroy_app_list(head);
	free(app_info_list);


	app_info* appp = (app_info*)malloc(sizeof(app_info));
	(char*)appp;
	struct list_head* headd = &appp->app_info_node;

	return 0;
}
