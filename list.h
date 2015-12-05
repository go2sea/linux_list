# define POISON_POINTER_DELTA 0

#define LIST_POISON1  ((void *) 0x00100100 + POISON_POINTER_DELTA)
#define LIST_POISON2  ((void *) 0x00200200 + POISON_POINTER_DELTA)

//计算member在type中的位置
#define offsetof(type, member)  (size_t)(&((type*)0)->member)
//根据member的地址获取type的起始地址
//比注掉写法好在：如果传入的ptr指针类型并不是和指向结构体成员member指针一样，那么编译器会给出一个警告，防止意外发生。
#define container_of(ptr, type, member) ({          \
        const typeof(((type *)0)->member)*__mptr = (ptr);    \
    (type *)((char *)__mptr - offsetof(type, member)); })
//#define container_of(ptr, type, member) ({          \
//    (type *)((char *)ptr - offsetof(type, member)); })


//链表结构
struct list_head {
	struct list_head *prev;
	struct list_head *next;
};

//初始化链表头
static inline void init_list_head(struct list_head *list) {
	list->prev = list;
	list->next = list;
}

//prev和next之间添加new
static inline void __list_add(struct list_head *new, struct list_head *prev, struct list_head *next) {
	prev->next = new;
	new->prev = prev;
	new->next = next;
	next->prev = new;
}

//从头部添加
static inline void list_add(struct list_head *new, struct list_head *head) {
	__list_add(new, head, head->next);
}
//从尾部添加
static inline void list_add_tail(struct list_head *new, struct list_head *head) {
	__list_add(new, head->prev, head);
}

//删除prev和next之间的节点但不释放
static inline  void __list_del(struct list_head *prev, struct list_head *next) {
	prev->next = next;
	next->prev = prev;
}

//删除某节点
static inline void list_del(struct list_head *entry) {
	__list_del(entry->prev, entry->next);
	entry->next = LIST_POISON1;
	entry->prev = LIST_POISON2;
}

//将某节点移至头部（head之后）
static inline void list_move(struct list_head *list, struct list_head *head) {
	__list_del(list->prev, list->next);
	list_add(list, head);
}

//将某节点移至尾部（head之前）
static inline void list_move_tail(struct list_head *list, struct list_head *head) {
	__list_del(list->prev, list->next);
	list_add_tail(list, head);
}

#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)
