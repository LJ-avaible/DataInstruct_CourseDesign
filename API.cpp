#include "Elevator.h"

//确定终端中光标位置
void GotOxy(short x, short y)
{
	COORD coord = { x, y };  // 创建一个 COORD 结构，保存光标的坐标位置
	/*COORD是Windows API中定义的一种结构，表示一个字符在控制台屏幕上的坐标。其定义为：

	typedef struct _COORD {

	SHORT X; // horizontal coordinate

	SHORT Y; // vertical coordinate
	} COORD;*/

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);// 设置控制台的光标位置
}

//显示其余信息
void ShowOthersInfo()
{
	// 显示电梯0和电梯1的标识
	GotOxy(14, 8);
	printf("电梯0");
	GotOxy(42, 8);
	printf("电梯1");

	// 显示楼层信息
	for (int i = 0; i <= Maxfloor; i++)
	{
		GotOxy(LeftElevatorX - 2, LeftElevatorY - 1 + (4 - i) * FloorHight);
		printf("%d", i);
	}

	// 显示电梯0和电梯1的状态信息
	GotOxy(3, 4);
	printf("%s", ElevatorChar[0]);
	GotOxy(33, 4);
	printf("%s", ElevatorChar[1]);

	// 显示乘客0和乘客1的状态信息
	GotOxy(3, 6);
	printf("%s", PeopleChar6[0]);
	GotOxy(33, 6);
	printf("%s", PeopleChar6[1]);

	// 显示乘客离开电梯的状态信息
	GotOxy(3, 7);
	printf("%s", PeopleChar7[0]);
	GotOxy(33, 7);
	printf("%s", PeopleChar7[0]);
}

//输出电梯的整体轮廓
void PrintElevator()
{
	int j, i, k;
	for (i = 0; i <= 1; i++)// 遍历电梯0和电梯1
	{
		for (j = Maxfloor + 1; j >= 0; j--) // 从楼层 Maxfloor 到 0 逐行绘制边框
		{
			if (j == Maxfloor) // 在最顶层绘制顶部边界
			{
				GotOxy(6 + i * 27, 9); // 设置光标位置
				printf("--------------------------"); // 绘制顶边框
			}
			GotOxy(6 + i * 27, 9 + j * FloorHight); // 设置每一层楼的横向边框位置
			printf("--------------------------"); // 绘制每层楼的边框

		}

		j = Maxfloor + 1;
		for (k = 9; k <= 9 + j * FloorHight; k++) // 从顶部到最底层，绘制两侧的竖线
		{
			GotOxy(5 + i * 27, k); // 设置光标位置，绘制左边竖线
			printf("|");
			GotOxy(5 + i * 27 + 26, k); // 设置光标位置，绘制右边竖线
			printf("|");
		}
	}
}

//隐藏在x层的电梯
void DisappearEvelatorBody(int x, Elevatorcanshu*& Elevator, int e)
{
	int i, j;
	//先设置字体为背景颜色(黑色)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);

	// 通过循环在电梯框架的对应位置打印空格，将电梯"隐藏"
	for (i = LeftElevatorX + Elevator[e].k * 27; i <= LeftElevatorX + Elevator[e].k * 26 + 24; i++)
	{
		// 设置光标位置，在电梯框架内打印空格，清除电梯内容
		GotOxy(i, LeftElevatorY - 1 + (4 - x) * FloorHight);
		printf(" ");  // 打印空格来清除电梯内容
	}
	// 恢复字体颜色为默认的白色
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

//在x层显示电梯
void ShowElevator(int x, Elevatorcanshu*& Elevator, int e)
{
	int i, j;
	// 设置光标位置到电梯的左侧框架（显示电梯）位置
	GotOxy(LeftElevatorX + Elevator[e].k * 27, LeftElevatorY - 1 + (4 - x) * FloorHight);
	// 打印电梯的开头符号
	printf("[");

	// 遍历当前电梯所在的楼层栈（Stack），列出所有乘客的ID
	for (i = 0; i <= Maxfloor; i++)
	{
		Dianti ap = Elevator[e].Stack[i].next;
		while (ap) {
			// 输出电梯内每个乘客的ID
			printf(" %d", ap->Id);
			ap = ap->next;  // 移动到下一个乘客
		}
	}

	// 设置光标位置到电梯的右侧框架（显示电梯）位置
	GotOxy(LeftElevatorX + Elevator[e].k * 26 + 24, LeftElevatorY - 1 + (4 - x) * FloorHight);
	
	// 打印电梯的结尾符号
	printf("]");
}

//显示等待队列的乘客
void ShowWaitingQueue(int i)
{
	People p;
	p = upqueue[i].next;  // 获取等待上楼的乘客队列
	int j = 0;

	// 遍历等待上楼的乘客
	while (p != NULL)
	{
		// 设置光标位置
		GotOxy(RightElevatorX + 26 + j * 6, RightElevatorY - 1 + (4 - p->InFloor) * FloorHight);
		
		// 打印乘客ID
		printf(" %d", p->Id);

		// 打印箭头和目标楼层
		printf("↑ ");
		printf("%d", p->OutFloor);

		// 移动到下一个乘客
		p = p->next;
		j++;  // 累加以显示不同乘客的位置
	}

	// 累加以显示不同乘客的位置
	p = downqueue[i].next;

	// 遍历等待下楼的乘客
	while (p != NULL)
	{
		// 设置光标位置
		GotOxy(RightElevatorX + 26 + j * 6, RightElevatorY - 1 + (4 - p->InFloor) * FloorHight);
		
		// 打印乘客ID
		printf(" %d", p->Id);

		// 打印箭头和目标楼层
		printf("↓ ");
		printf("%d", p->OutFloor);

		// 移动到下一个乘客
		p = p->next;
		j++;  // 累加以显示不同乘客的位置
	}
}

//消隐处于等待状态的乘客
void DisappearWaiting(int i)
{
	//设置字体的颜色，用背景颜色再写一遍就能消去字体 0是黑色 7是白色
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);

	// 调用 ShowWaitingQueue 函数重新显示等待队列的乘客（此时会用背景色覆盖）
	ShowWaitingQueue(i);

	// 消隐完后再恢复字体颜色（白色）
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

//显示说明信息
void PrintHelp()
{
	GotOxy(70, 5);
	printf("前面的数字：乘客ID");
	
	GotOxy(70, 7);
	printf("箭头：上行 or 下行");
	
	GotOxy(70, 9);
	printf("后面的数字：目标楼层");
}

//初始化电梯信息 
void InitElevator(Elevatorcanshu*& Elevator, int e)
{
	int i;

	// 设置电梯初始楼层为基层
	Elevator[e].Floor = Basefloor;								
	printf("%d\n", Elevator[e].Floor);						// 打印第一个电梯所在的楼层（用于调试）
	printf("%d\n", Elevator[1].Floor);						// 打印第二个电梯所在的楼层（用于调试）
	

	// 初始化电梯状态标志变量
	Elevator[e].d1 = Elevator[e].d2 = Elevator[e].d3 = 0;	// 电梯相关标志变量初始化为0
	Elevator[e].state = Elevator[e].prestate = Idle;		// 电梯当前状态和下一状态设置为空
	Elevator[e].again = 0;									// 重置电梯再次请求标志为0
	Elevator[e].drcl = 0;									// 初始化电梯门的控制参数为0

	// 初始化时序监控相关字段
	Elevator[e].sxjiankong.f = NULL;						// 时序监控的回调函数指针设置为空
	Elevator[e].sxjiankong.next = NULL;						// 时序监控的下一个事件指针初始化为空
	Elevator[e].sxjiankong.person = 0;						// 时序监控的乘客字段初始化为0（无乘客
	Elevator[e].sxjiankong.xianzhitime = 0;					// 时序监控的限制时间初始化为0
	
	// 初始化每个楼层的按钮状态和电梯内乘客堆栈
	for (i = 0; i <= Maxfloor; i++) {
		Elevator[e].CallDianti[i] = 0;						// 电梯内按钮初始化为未按下（0）
		Elevator[e].CallDown[i] = 0;						// 楼层下行按钮初始化为未按下（0）
		Elevator[e].CallUp[i] = 0;							// 楼层上行按钮初始化为未按下（0）
		Elevator[e].Stack[i].next = NULL;					// 每层电梯内乘客堆栈初始化为空
	}

	// 初始化电梯内当前乘客数量为0
	Elevator[e].PeopleNum = 0;
}

//显示电梯模拟界面头
void ShowTitle()
{
	printf("\n\n\n");
	printf("\n\t\t*********************************************************");
	printf("\n\t\t_________________________________________________________");
	printf("\n\t\t〓〓〓〓〓〓〓〓〓欢迎来到电梯管理系统 〓〓〓〓〓〓〓〓〓\n\n");
	printf("\t\t                本次电梯模拟的时间单位为%.1f秒\n",UnitTime);
	printf("\n\t\t*********************************************************\n");
}

//输入最大电梯模拟运行时间 
void InputMaxTime()
{
	printf("\n\t\t请输入电梯的最大运行时间，达到该时间，电梯系统将会自动停止\n");
	printf("\t\t                   请输入最大运行时间:");
	while (scanf("%d", &Maxtime) != 1 || Maxtime < MinTime || Maxtime > MaxTime)
	{
		printf("\t\t     输入有误，需重新输入（%d<=最大运行时间<=%d）\n", MinTime, MaxTime);
		while (getchar() != '\n')
			continue;
		printf("\t\t                   请输入最大运行时间:");
	}
}

//加入新乘客 
void AddNewPerson(People pt, Elevatorcanshu*& Elevator, int e)
{
	system("cls");		// 清屏
	int InFloor, OutFloor, GiveupTime, EnterTime, FlagElevator0 = 0, FlagElevator1 = 0;
	
	while (1) {
		// 显示当前的时间和提示信息
		printf("\t\t*********************************************************\n");
		printf("\t\t                   时间为：%d\n", Time);
		printf("\t\t            第%d位乘客走进来，等候电梯。\n", id);
		
		// 输入乘客所在楼层
		while (1) {
			printf("\t\t            他在哪层楼电梯的门前:_____\b\b\b");
			scanf("%d", &InFloor);
			if (InFloor >= 0 && InFloor <= Maxfloor)break;
			printf("\t\t            输入的楼层超出界限，请重新输入。\n");
		}

		// 输入是否呼叫电梯0和电梯1
		while (1) {
			char c;
			printf("\t\t            是否呼叫电梯0，请输入（Y/N）：");
			getchar();
			c = getchar();
			while (c != 'Y' && c != 'N' && c != 'y' && c!= 'n') {
				printf("\t\t            输入格式错误，请重新输入,是否呼叫电梯0（Y/N）：");
				getchar();
				c = getchar();
			}
			if (c == 'Y' || c == 'y') {
				FlagElevator0 = 1;		// 呼叫电梯0
			}


			printf("\t\t            是否呼叫电梯1，请输入（Y/N）：");
			getchar();
			scanf("%c", &c);
			while (c != 'Y' && c != 'N' && c != 'y' && c != 'n') {
				printf("\t\t            输入格式错误，请重新输入,是否呼叫电梯1（Y/N）：");
				getchar();
				scanf("%c", &c);
			}
			if (c == 'Y' || c == 'y') {
				FlagElevator1 = 1;		// 呼叫电梯1
			}

			if (FlagElevator0 || FlagElevator1)
			{
				break;		// 至少呼叫一个电梯
			}
			printf("\t\t            您都没有呼叫电梯，请重新输入要呼叫的电梯。\n");
		}

		// 输入乘客目标楼层
		while (1) {
			printf("\t\t            他要去哪层楼:_____\b\b\b");
			scanf("%d", &OutFloor);
			if (OutFloor >= 0 && OutFloor <= Maxfloor)break;
			printf("\t\t            输入的楼层超出界限，请重新输入。\n");
		}

		// 输入乘客最大等待时间和下一个乘客出现的时间
		printf("\t\t            他能容忍的等候时间:_____\b\b\b");
		scanf("%d", &GiveupTime);
		printf("\t\t            下一个乘客出现的时间间隔:_____\b\b\b");
		scanf("%d", &EnterTime);
		printf("\t\t*********************************************************\n\n");
		if (OutFloor != InFloor) {
			break;			//目的楼层和当前楼层相同，重新输入
		}

		printf("\t\t*********************************************************\n");
		printf("\t\t输入错误：乘客要去的楼层与其当前所在楼层相同，请重新输入\n");
		printf("\t\t*********************************************************\n\n");
	}

	// 创建新的乘客信息
	People p;
	p = (People)malloc(sizeof(struct people));
	p->EnterTime = Time;
	p->Id = id++;
	p->InFloor = InFloor;
	p->OutFloor = OutFloor;
	p->GiveupTime = GiveupTime;
	p->FlagElevator0 = FlagElevator0;
	p->FlagElevator1 = FlagElevator1;
	p->next = NULL;
	p->pre = NULL;
	Exist[id - 1] = 1;								// 标记乘客存在
	
	// 根据目标楼层方向将乘客加入上下行队列
	if (OutFloor > InFloor) {
		AddUpQueue(p, InFloor);						// 加入上行队列
		if (FlagElevator0) {						
			Elevator[0].CallUp[InFloor] = 1;		// 电梯0相应上行
		}
		if (FlagElevator1) {
			Elevator[1].CallUp[InFloor] = 1;		// 电梯1相应上行
		}
	}
	else {
		AddDownQueue(p, InFloor);					// 加入下行队列
		if (FlagElevator0) {
			Elevator[0].CallDown[InFloor] = 1;		// 电梯0响应下行
		}
		if (FlagElevator1) {
			Elevator[1].CallDown[InFloor] = 1;		// 电梯1响应下行
		}
	}

	// 更新电梯状态和时序事件
	if (FlagElevator0 && FlagElevator1) {
		flagelevator = 1;							// 两个电梯同时被呼叫
	}
	if (!FlagElevator0 && FlagElevator1) {
		AddTimeSequence(GiveupTime, p, GiveupWait, Elevator, 1);// 添加超时事件到电梯1
		AddTimeSequence(EnterTime, NULL, AddNewPerson, Elevator, 1);// 添加下一乘客事件到电梯1
	}
	else {
		AddTimeSequence(GiveupTime, p, GiveupWait, Elevator, 0); // 添加超时事件到电梯0
		AddTimeSequence(EnterTime, NULL, AddNewPerson, Elevator, 0);// 添加下一乘客事件到电梯0
	}

	// 清屏并刷新显示
	system("cls");
	ShowOthersInfo();										// 显示其他信息
	PrintElevator();								//输出电梯框架
	PrintHelp();										// 显示其他信息
	ShowElevator(Elevator[0].Floor, Elevator, 0);	// 显示电梯0状态
	ShowElevator(Elevator[1].Floor, Elevator, 1);   // 显示电梯1状态              //输出电梯主体
	int i;

	// 显示每层的等待队列
	for (i = 0; i <= Maxfloor; i++) {
		ShowWaitingQueue(i);
	}
}

// 加入上升队列
void AddUpQueue(People& p, int in)
{
	// 获取指定楼层的上行队列头节点
	People q = &upqueue[in];

	// 遍历队列，找到最后一个节点
	while (q->next != NULL) {
		q = q->next;
	}

	// 在队列末尾添加新乘客节点
	q->next = p;
	p->pre = q; // 更新新乘客节点的前驱指针
}

// 加入下降队列
void AddDownQueue(People& p, int in)
{
	// 获取指定楼层的下行队列头节点
	People q = &downqueue[in];

	// 遍历队列，找到最后一个节点
	while (q->next != NULL) {
		q = q->next;
	}

	// 在队列末尾添加新乘客节点
	q->next = p;
	p->pre = q; // 更新新乘客节点的前驱指针
}

// 将各个操作加入一个按时间顺序排列的队列中
void AddTimeSequence(int t, People p, void(*fn)(People per, Elevatorcanshu*& Elevatortemp, int temp), Elevatorcanshu*& Elevator, int e)
{
	// 计算操作的实际触发时间
	int time1 = Time + t;

	// 获取电梯调度器的事件链表头节点
	Shixu r = &Elevator[e].sxjiankong;

	// 创建新事件节点
	Shixu q;
	q = (Shixu)malloc(sizeof(struct TimeSequence));
	q->xianzhitime = time1;					// 设置事件触发时间

	// 如果关联乘客节点不为空，则复制乘客信息
	if (p != NULL) {
		q->person = (People)malloc(sizeof(struct people));
		q->person->pre = p->pre;
		q->person->Id = p->Id;
		q->person->InFloor = p->InFloor;	// 没有关联乘客时设置为空
	}
	else {
		q->person = NULL;
	}

	// 设置事件对应的回调函数
	q->f = fn;
	q->next = NULL;

	// 找到事件链表中适合插入的位置，按时间升序排列
	while (r->next != NULL) {
		if (r->next->xianzhitime > time1)break;  // 找到第一个时间大于当前事件时间的节点
		r = r->next;
	}

	// 插入事件节点
	q->next = r->next;
	r->next = q;
}

//乘客放弃等待电梯 
void GiveupWait(People per, Elevatorcanshu*& Elevator, int e)
{
	// 如果乘客已进入电梯，直接返回
	if (Exist[per->Id] == 0) {
		return;
	}
	// 如果电梯空闲且在当前楼层，并且能容纳乘客，放弃等待操作取消
	else if (per->InFloor == Elevator[0].Floor && Elevator[0].state == Idle && per->FlagElevator0 && Elevator[0].PeopleNum < MaxPeopleNum)
		return;
	else if (per->InFloor == Elevator[1].Floor && Elevator[1].state == Idle && per->FlagElevator1 && Elevator[1].PeopleNum < MaxPeopleNum)
		return;

	// 更新等待队列中的显示，移除该乘客
	DisappearWaiting(per->InFloor);

	int i, a, num;

	// 如果乘客呼叫了电梯 0
	if (per->FlagElevator0) {
		GotOxy(3, 6);   // 清除显示
		printf("                           ");
		GotOxy(3, 7);   // 清除显示
		printf("                           ");
		GotOxy(3, 6);   // 重新打印放弃等候的乘客信息
		printf("乘客%d放弃等候,离开了", per->Id);

		a = per->Id;    // 乘客ID赋值给a
		num = 0;        // 初始化数字的位数
		strcpy(PeopleChar6[0], "乘客");  // 初始化字符数组
		for (i = 0; PeopleChar6[0][i] != '\0'; i++) {}  // 查找字符串末尾

		do {  // 计算乘客ID的位数
			num++;
			a = a / 10;
		} while (a);  // 直到a为0

		a = per->Id;    // 重新赋值a为乘客ID
		num--;          // 乘客ID的位数减1

		while (num != 0) {  // 按位提取并拼接乘客ID
			PeopleChar6[0][i] = '0' + a / (int)(pow(10, num)); // 取得当前位的数字并转换为字符
			a = a % (int)(pow(10, num));  // 取余获取剩余部分
			num--;  // 位数减1
			i++;    // 移动到下一个字符位置
		}
		PeopleChar6[0][i] = '0' + a;  // 最后一个数字
		PeopleChar6[0][i + 1] = '\0';  // 字符串结束符
		strcat(PeopleChar6[0], "放弃等候,离开了");  // 拼接完整的离开信息
		strcpy(PeopleChar7[0], " ");  // 清空第二个字符数组
	}

	// 如果乘客呼叫了电梯 1
	if (per->FlagElevator1) {
		GotOxy(3 + 30, 6);  // 移动光标到新的位置
		printf("                           ");  // 清除原信息
		GotOxy(3 + 30, 7);  // 清除原信息
		printf("                           ");  // 清除原信息
		GotOxy(3 + 30, 6);  // 重新打印放弃等候的乘客信息
		printf("乘客%d放弃等候,离开了", per->Id);

		a = per->Id;  // 乘客ID赋值给a
		num = 0;      // 初始化数字的位数
		strcpy(PeopleChar6[1], "乘客");  // 初始化字符数组
		for (i = 0; PeopleChar6[1][i] != '\0'; i++) {}  // 查找字符串末尾

		do {  // 计算乘客ID的位数
			num++;
			a = a / 10;
		} while (a);  // 直到a为0

		a = per->Id;    // 重新赋值a为乘客ID
		num--;          // 乘客ID的位数减1

		while (num != 0) {  // 按位提取并拼接乘客ID
			PeopleChar6[1][i] = '0' + a / (int)(pow(10, num));  // 取得当前位的数字并转换为字符
			a = a % (int)(pow(10, num));  // 取余获取剩余部分
			num--;  // 位数减1
			i++;    // 移动到下一个字符位置
		}
		PeopleChar6[1][i] = '0' + a;  // 最后一个数字
		PeopleChar6[1][i + 1] = '\0';  // 字符串结束符
		strcat(PeopleChar6[1], "放弃等候,离开了");  // 拼接完整的离开信息
		strcpy(PeopleChar7[1], " ");  // 清空第二个字符数组
	}

	// 更新队列状态，移除该乘客
	int infloor = per->InFloor;  // 获取乘客的当前楼层
	int outfloor = per->OutFloor;  // 获取乘客的目标楼层
	Exist[per->Id] = 0;  // 标记乘客ID为已不存在

	People q = per->pre, ap = q->next;  // 获取乘客的前一个节点和下一个节点
	q->next = ap->next;  // 删除当前乘客节点
	if (ap->next != NULL) {
		ap->next->pre = q;  // 如果下一个节点不为空，更新其前驱节点
	}

	// 更新当前楼层的等待队列显示
	ShowWaitingQueue(per->InFloor);

	// 释放乘客节点的内存
	free(ap);
}

//电梯模拟的一个计时器 
void Simulate()
{
	// 动态分配内存，创建两个电梯
	Elevatorcanshu* Elevator;
	Elevator = (Elevatorcanshu*)malloc(sizeof(Elevatorcanshu) * 2);
	Elevator[0].k = 0; // 初始化第一个电梯的编号
	Elevator[1].k = 1; // 初始化第二个电梯的编号

	// 初始化两个电梯
	InitElevator(Elevator, 0);
	InitElevator(Elevator, 1);

	// 开始加入乘客
	AddNewPerson(NULL, Elevator, 0);

	// 开始模拟，直到时间达到最大模拟时间
	while (Time <= Maxtime) {

		// 如果第一个电梯有事件或者两个电梯都被呼叫
		if (Elevator[0].sxjiankong.next || flagelevator) {
			do {
				Elevator[0].again = 0;				// 重置再次请求标志
				ControlElevator(Elevator, 0);				// 控制第一个电梯运行
			} while (Elevator[0].again == 1);		// 如果有再次请求，重复控制
		}

		// 如果第二个电梯有事件或者两个电梯都被呼叫
		if (Elevator[1].sxjiankong.next || flagelevator) {
			do {
				Elevator[1].again = 0;				// 重置再次请求标志
				ControlElevator(Elevator, 1);				// 控制第二个电梯的运行
			} while (Elevator[1].again == 1);		// 如果有再次请求，重复控制
		}

		// 获取第一个电梯和第二个电梯的时序事件
		Shixu p = Elevator[0].sxjiankong.next;
		Shixu q = Elevator[1].sxjiankong.next;

		// 如果两个电梯都没有时序事件，则结束模拟
		if (p == NULL && q == NULL) {
			Time = Maxtime;							// 设置时间为最大模拟时间，结束模拟
		}

		// 处理第一个电梯的时序事件
		while (p && p->xianzhitime <= Time) {
			Elevator[0].sxjiankong.next = p->next;	 // 更新时序事件链表
			p->f(p->person, Elevator, 0);			 // 执行时序事件的回调函数
			free(p);								 // 释放事件节点
			p = Elevator[0].sxjiankong.next;		 // 获取下一个时序事件
		}

		// 处理第二个电梯的时序事件
		while (q && q->xianzhitime <= Time) {
			Elevator[1].sxjiankong.next = q->next;	 // 更新时序事件链表
			q->f(q->person, Elevator, 1);			 // 执行时序事件的回调函数
			free(q);								 // 释放事件节点
			q = Elevator[1].sxjiankong.next;		 // 获取下一个时序事件
		}

		Time++;										 // 增加时间
		GotOxy(20, 2);								 // 设置光标位置，显示当前时间
		printf("时间为： %d", Time);				 // 打印当前时间
		Sleep(1000 * UnitTime);						 // 暂停一段时间，模拟单位时间的流逝
	}

	// 模拟结束后，提示用户按下任意键退出
	GotOxy(60, 30);									 // 设置光标位置
	printf("演示结束，按下任意键退出");				 // 提示信息
	getchar();										 // 等待用户按下任意键
	getchar();										 // 等待用户按下任意键（第二次，以确保接收到输入）
}

// 对电梯进行控制
void ControlElevator(Elevatorcanshu*& Elevator, int e)
{
	int flag, i;

	// 如果电梯处于空闲状态
	if (Elevator[e].state == Idle) {
		// 如果电梯的上一个状态也是空闲
		if (Elevator[e].prestate == Idle) {
			Elevator[e].prestate = getOrder(Elevator, e);  // 获取电梯当前位置
		}

		// 如果电梯的上下行标志都为 0，且电梯当前楼层有呼叫请求，且没有人乘坐
		if (Elevator[e].d1 == 0 && Elevator[e].d3 == 0 && Elevator[e].CallDianti[Elevator[e].Floor] == 2) {
			Elevator[e].CallDianti[Elevator[e].Floor] = 0;  // 重置呼叫请求
			flag = 0;
			// 向上检查是否有其他请求
			for (i = Elevator[e].Floor + 1; i <= Maxfloor; i++) {
				if (Elevator[e].CallDown[i] || Elevator[e].CallDianti[i] || Elevator[e].CallUp[i]) {
					flag = 1;  // 如果有请求，标记为 1
					break;
				}
			}

			// 向下检查是否有其他请求
			for (i = Elevator[e].Floor - 1; i >= 0; i--) {
				if (Elevator[e].CallDown[i] || Elevator[e].CallDianti[i] || Elevator[e].CallUp[i]) {
					flag = 1;  // 如果有请求，标记为 1
					break;
				}
			}

			// 如果没有请求，输出无人请求电梯的信息
			if (flag) {
				return;
			}
			GotOxy(3 + 30 * Elevator[e].k, 4);  // 定位光标位置
			printf("                        ");
			GotOxy(3 + 30 * Elevator[e].k, 4);  // 重新输出信息
			printf("无人请求电梯%d", Elevator[e].k);  // 输出无人请求信息

			// 根据电梯编号更新显示内容
			if (Elevator[e].k) {
				strcpy(ElevatorChar[1], "无人请求电梯1");
			}
			else {
				strcpy(ElevatorChar[0], "无人请求电梯0");
			}
			return;
		}

		// 如果电梯在上行状态，且当前楼层有上行请求，且电梯人数未满
		if (!Elevator[e].d1 && !Elevator[e].d3 && Elevator[e].prestate == Goingup && ((Elevator[e].CallUp[Elevator[e].Floor] && Elevator[e].PeopleNum < MaxPeopleNum) || Elevator[e].CallDianti[Elevator[e].Floor]))
		{
			Elevator[e].d1 = 1;  // 设置电梯为已启动
			Elevator[e].d2 = 1;  // 设置电梯为正在行动

			// 如果电梯关门被打断，显示打断信息，并重新设置电梯状态
			if (Elevator[e].drcl) {
				Elevator[e].drcl = 0;  // 重置打断标志
				GotOxy(3 + 30 * Elevator[e].k, 4);  // 定位光标
				printf("                        ");
				GotOxy(3 + 30 * Elevator[e].k, 4);  // 重新输出信息
				printf("电梯%d被打断关门", Elevator[e].k);  // 输出打断信息

				// 更新电梯状态显示
				if (Elevator[e].k) {
					strcpy(ElevatorChar[1], "电梯1被打断关门");
				}
				else {
					strcpy(ElevatorChar[0], "电梯0被打断关门");
				}

				// 将关门操作添加到调度队列
				AddTimeSequence(Time - Elevator[e].drcltime, NULL, OpenDoor, Elevator, e);
			}
			else
			{
				// 否则正常开启门
				AddTimeSequence(Doortime, NULL, OpenDoor, Elevator, e);
			}
			return;
		}

		// 如果电梯在下行状态，且当前楼层有下行请求，且电梯人数未满
		else if (!Elevator[e].d1 && !Elevator[e].d3 && Elevator[e].prestate == Goingdown && ((Elevator[e].CallDown[Elevator[e].Floor] && Elevator[e].PeopleNum < MaxPeopleNum) || Elevator[e].CallDianti[Elevator[e].Floor]))
		{
			Elevator[e].d1 = 1;  // 设置电梯为已启动
			Elevator[e].d2 = 1;  // 设置电梯为正在行动

			// 如果电梯关门被打断，显示打断信息，并重新设置电梯状态
			if (Elevator[e].drcl) {
				Elevator[e].drcl = 0;  // 重置打断标志
				GotOxy(3 + 30 * Elevator[e].k, 4);  // 定位光标
				printf("                        ");
				GotOxy(3 + 30 * Elevator[e].k, 4);  // 重新输出信息
				printf("电梯%d被打断关门", Elevator[e].k);  // 输出打断信息

				// 更新电梯状态显示
				if (Elevator[e].k) {
					strcpy(ElevatorChar[1], "电梯1被打断关门");
				}
				else {
					strcpy(ElevatorChar[0], "电梯0被打断关门");
				}

				// 将关门操作添加到调度队列
				AddTimeSequence(Time - Elevator[e].drcltime, NULL, OpenDoor, Elevator, e);
			}
			else
			{
				// 否则正常开启门
				AddTimeSequence(Doortime, NULL, OpenDoor, Elevator, e);
			}
			return;
		}

		// 如果电梯正在上行，且门未关闭且有乘客进来
		else if (Elevator[e].d3 == 1 && Elevator[e].prestate == Goingup && (Elevator[e].CallUp[Elevator[e].Floor] && Elevator[e].PeopleNum < MaxPeopleNum)) {
			Elevator[e].d1 = 1;		// 设置电梯为已启动
			Elevator[e].d3 = 0;		// 重置门未关闭标志
			AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);  // 添加乘客进电梯的操作到调度队列
		}

		// 如果电梯正在下行，且门未关闭且有乘客进来
		else if (Elevator[e].d3 == 1 && Elevator[e].prestate == Goingdown && (Elevator[e].CallDown[Elevator[e].Floor] && Elevator[e].PeopleNum < MaxPeopleNum)) {
			Elevator[e].d1 = 1;		// 设置电梯为已启动
			Elevator[e].d3 = 0;		// 重置门未关闭标志
			AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);  // 添加乘客进电梯的操作到调度队列
		}

		// 如果电梯处于空闲状态，且没有呼叫请求且没有在关门操作，且电梯正在上行
		else if (Elevator[e].d1 == 0 && Elevator[e].d3 == 0 && Elevator[e].drcl == 0 && Elevator[e].prestate == Goingup && !((Elevator[e].CallUp[Elevator[e].Floor] && Elevator[e].PeopleNum < MaxPeopleNum) || Elevator[e].CallDianti[Elevator[e].Floor]))
		{
			int temp = GetUp(Elevator, e);  // 获取上行状态
			if (temp == 1) {
				Elevator[e].state = Goingup;  // 设置电梯为上行状态
				AddTimeSequence(Accelerate, NULL, UniformSpeed, Elevator, e);  // 添加加速操作到调度队列
			}
			else {
				Elevator[e].prestate = Idle;  // 设置电梯为空闲状态
				Elevator[e].state = Idle;  // 设置电梯为空闲状态
				Elevator[e].again = 1;  // 设置电梯需要再次执行操作
			}
		}

		// 如果电梯处于空闲状态，且没有呼叫请求且没有在关门操作，且电梯正在下行
		else if (Elevator[e].d1 == 0 && Elevator[e].d3 == 0 && Elevator[e].drcl == 0 && Elevator[e].prestate == Goingdown && !((Elevator[e].CallDown[Elevator[e].Floor] && Elevator[e].PeopleNum < MaxPeopleNum) || Elevator[e].CallDianti[Elevator[e].Floor]))
		{
			int temp = GetDown(Elevator, e);  // 获取下行状态
			if (temp == 1) {
				Elevator[e].state = Goingdown;  // 设置电梯为下行状态
				AddTimeSequence(Accelerate, NULL, UniformSpeed, Elevator, e);  // 添加加速操作到调度队列
			}
			else {
				Elevator[e].prestate = Idle;  // 设置电梯为空闲状态
				Elevator[e].state = Idle;  // 设置电梯为空闲状态
				Elevator[e].again = 1;  // 设置电梯需要再次执行操作
			}
		}
		// 如果电梯正在执行其他任务，则返回到原始位置
		else if (!Elevator[e].d1 && !Elevator[e].d3 && Elevator[e].drcl == 0 && Elevator[e].d2) {
			Elevator[e].d2 = 0;  // 关闭执行任务标志
			Elevator[e].backfirsttime = Time;  // 设置返回时间
			AddTimeSequence(Backtime, NULL, GoFirstFloor, Elevator, e);  // 添加返回操作到调度队列
		}
	}
}

//电梯获取下步指令 
int getOrder(Elevatorcanshu*& Elevator, int e)
{
	int timeUp = -1, timeDown = -1;

	// 检查当前楼层是否有上行请求
	if (upqueue[Elevator[e].Floor].next != NULL && Elevator[e].CallUp[Elevator[e].Floor]) {
		timeUp = upqueue[Elevator[e].Floor].next->EnterTime;
	}

	// 检查当前楼层是否有下行请求
	if (downqueue[Elevator[e].Floor].next != NULL && Elevator[e].CallDown[Elevator[e].Floor]) {
		timeDown = downqueue[Elevator[e].Floor].next->EnterTime;
	}

	// 如果存在上行或下行请求
	if (timeUp != -1 || timeDown != -1) {
		if (timeUp != -1 && timeDown != -1) {
			return timeUp < timeDown ? Goingup : Goingdown;  // 返回时间较早的请求
		}
		else if (timeUp != -1) {
			return Goingup;  // 只有上行请求
		}
		else if (timeDown != -1) {
			return Goingdown;  // 只有下行请求
		}
	}
	else {  // 如果当前楼层没有请求
		timeUp = Maxtime + 1; 
		timeDown = Maxtime + 1;

		// 扫描当前楼层之上的楼层，看是否有上行请求或下行请求
		for (int i = Elevator[e].Floor + 1; i <= Maxfloor; i++) {
			if (upqueue[i].next != NULL && Elevator[e].CallUp[i]) {
				timeUp = min(timeUp, upqueue[i].next->EnterTime);
			}
			if (downqueue[i].next != NULL && Elevator[e].CallDown[i]) {
				timeUp = min(timeUp, downqueue[i].next->EnterTime);
			}
		}

		// 扫描当前楼层之下的楼层，看是否有上行请求或下行请求
		for (int i = Elevator[e].Floor - 1; i >= 0; i--) {
			if (upqueue[i].next != NULL && Elevator[e].CallUp[i]) {
				timeDown = min(timeDown, upqueue[i].next->EnterTime);
			}
			if (downqueue[i].next != NULL && Elevator[e].CallDown[i]) {
				timeDown = min(timeDown, downqueue[i].next->EnterTime);
			}
		}

		// 如果有任何请求，选择时间最早的方向
		if (timeUp != Maxtime + 1 || timeDown != Maxtime + 1) {
			if (timeUp != Maxtime + 1 && timeDown != Maxtime + 1) {
				return timeUp < timeDown ? Goingup : Goingdown;  // 返回时间较早的请求
			}
			else if (timeUp != Maxtime + 1) {
				return Goingup;  // 只有上行请求
			}
			else return Goingdown;  // 只有下行请求
		}
		else {
			// 如果没有任何请求，看电梯是否有去基础楼层的请求
			if (Elevator[e].CallDianti[Basefloor] == 2 && Basefloor > Elevator[e].Floor)
			{
				return Goingup;  // 基础楼层有请求且电梯需上升
			}
			else if (Elevator[e].CallDianti[Basefloor] == 2 && Basefloor < Elevator[e].Floor)
			{
				return Goingdown;  // 基础楼层有请求且电梯需下降
			}
			return Idle;  // 如果没有任何请求，电梯停留
		}
	}
}

//打开电梯门 
void OpenDoor(People p, Elevatorcanshu*& Elevator, int e)
{
	// 在电梯的显示位置打印"电梯门开了"
	GotOxy(3 + 30 * Elevator[e].k, 4);  // 定位光标到电梯显示区域
	printf("                        ");  // 清除原有的显示
	GotOxy(3 + 30 * Elevator[e].k, 4);  // 定位光标
	printf("电梯%d门开了", Elevator[e].k);  // 打印电梯门开了的信息

	// 更新电梯状态显示字符数组
	if (Elevator[e].k) {
		strcpy(ElevatorChar[1], "电梯1门开了");  // 如果是电梯1，更新显示
	}
	else {
		strcpy(ElevatorChar[0], "电梯0门开了");  // 如果是电梯0，更新显示
	}

	// 如果当前楼层有等候的乘客，执行相应的乘客进出操作
	if (Elevator[e].Stack[Elevator[e].Floor].next) {  // 如果当前楼层有乘客需要下电梯
		AddTimeSequence(Peopletime, NULL, PeopleOut, Elevator, e);  // 调用下车操作
	}
	else if (Elevator[e].prestate == Goingup && upqueue[Elevator[e].Floor].next && Elevator[e].PeopleNum < MaxPeopleNum) {
		// 如果电梯正在上行，且该楼层有上行请求并且电梯内有空位
		AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);  // 调用上车操作
	}
	else if (Elevator[e].prestate == Goingdown && downqueue[Elevator[e].Floor].next && Elevator[e].PeopleNum < MaxPeopleNum) {
		// 如果电梯正在下行，且该楼层有下行请求并且电梯内有空位
		AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);  // 调用下车操作
	}
	else {
		// 如果电梯内没有人进出，且当前楼层没有上下行请求
		if (Elevator[e].PeopleNum < MaxPeopleNum) {
			Elevator[e].CallUp[Elevator[e].Floor] = 0;  // 清除该楼层的上行请求
		}
		Elevator[e].CallDown[Elevator[e].Floor] = 0;  // 清除该楼层的下行请求
		Elevator[e].CallDianti[Elevator[e].Floor] = 0;  // 清除该楼层的电梯内请求

		// 设置电梯状态：门开着但没有人进出
		Elevator[e].d1 = 0;
		Elevator[e].d3 = 1;   //没人进出，但门是开着的
		Elevator[e].testdoortime = Time; // 记录门打开时间
		AddTimeSequence(Diantitime, NULL, InOut, Elevator, e);  // 调用测试进出操作，等待一定时间
	}
}

//乘客进入电梯 
void PeopleIn(People p, Elevatorcanshu*& Elevator, int e)
{
	int a, num, i;

	// 如果电梯正在上行
	if (Elevator[e].prestate == Goingup) {
		People ap = upqueue[Elevator[e].Floor].next;  // 获取上行请求队列中的第一个乘客
		if (ap && Elevator[e].PeopleNum < MaxPeopleNum) {  // 如果有乘客且电梯未满
			// 显示乘客进入电梯的信息
			DisappearEvelatorBody(Elevator[e].Floor, Elevator, e);
			DisappearWaiting(Elevator[e].Floor);
			GotOxy(3 + 30 * Elevator[e].k, 6);
			printf("                           ");
			GotOxy(3 + 30 * Elevator[e].k, 7);
			printf("                           ");
			GotOxy(3 + 30 * Elevator[e].k, 6);
			printf("乘客%d成功进入电梯%d\n", ap->Id, Elevator[e].k);
			a = ap->Id;

			// 处理乘客ID显示格式
			num = 0;
			strcpy(PeopleChar6[Elevator[e].k], "乘客");
			for (i = 0; PeopleChar6[Elevator[e].k][i] != '\0'; i++) {
			}
			do {
				num++;
				a = a / 10;
			} while (a);
			a = ap->Id;
			num--;
			while (num != 0) {
				PeopleChar6[Elevator[e].k][i] = '0' + a / (int)(pow(10, num));
				a = a % (int)(pow(10, num));
				num--;
				i++;
			}
			PeopleChar6[Elevator[e].k][i] = '0' + a;
			PeopleChar6[Elevator[e].k][i + 1] = '\0';
			if (Elevator[e].k) {
				strcat(PeopleChar6[Elevator[e].k], "成功进入电梯1");
			}
			else {
				strcat(PeopleChar6[Elevator[e].k], "成功进入电梯0");
			}
			strcpy(PeopleChar7[Elevator[e].k], " ");

			// 更新电梯乘客数量，移除该乘客并将其添加到目标楼层的电梯栈中
			Elevator[e].PeopleNum++;
			Exist[ap->Id] = 0;
			upqueue[Elevator[e].Floor].next = ap->next;

			// 将乘客加入目标楼层的电梯栈中
			Dianti pr = (Dianti)malloc(sizeof(struct ElevatorStop));
			int i = ap->OutFloor;
			Elevator[e].CallDianti[i] = 1;
			pr->next = Elevator[e].Stack[i].next;
			Elevator[e].Stack[i].next = pr;
			pr->Id = ap->Id;

			// 更新电梯和等待队列显示
			ShowElevator(Elevator[e].Floor, Elevator, e);
			ShowWaitingQueue(Elevator[e].Floor);

			// 释放已处理的乘客
			free(ap);
		}

		// 如果有上行请求并且电梯内有空位，继续处理下一个乘客进入
		if (upqueue[Elevator[e].Floor].next && Elevator[e].PeopleNum < MaxPeopleNum)
			AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);
		else {
			// 如果上行队列中有等待乘客且电梯已满载
			if (upqueue[Elevator[e].Floor].next) {
				People t = upqueue[Elevator[e].Floor].next;
				GotOxy(3 + 30 * Elevator[e].k, 4);
				printf("                  ");
				GotOxy(3 + 30 * Elevator[e].k, 4);
				printf("电梯%d已满载", Elevator[e].k);
				if (Elevator[e].k) {
					strcpy(ElevatorChar[1], "电梯1已满载");
				}
				else {
					strcpy(ElevatorChar[0], "电梯0已满载");
				}
				GotOxy(3 + 30 * Elevator[e].k, 6);
				printf("                           ");
				GotOxy(3 + 30 * Elevator[e].k, 7);
				printf("                           ");
				GotOxy(3 + 30 * Elevator[e].k, 6);

				// 显示无法进入电梯的乘客信息
				while (t) {
					printf("乘客%d，", t->Id);
					a = t->Id;
					num = 0;
					strcpy(PeopleChar6[Elevator[e].k], "乘客");
					for (i = 0; PeopleChar6[Elevator[e].k][i] != '\0'; i++) {
					}
					do {
						num++;
						a = a / 10;
					} while (a);
					a = t->Id;
					num--;
					while (num != 0) {
						PeopleChar6[Elevator[e].k][i] = '0' + a / (int)(pow(10, num));
						a = a % (int)(pow(10, num));
						num--;
						i++;
					}
					PeopleChar6[Elevator[e].k][i] = ',';
					PeopleChar6[Elevator[e].k][i + 1] = '\0';
					t = t->next;
				}
				GotOxy(3 + 30 * Elevator[e].k, 7);
				printf("无法继续进入电梯%d", Elevator[e].k);
				if (Elevator[e].k) {
					strcpy(PeopleChar7[Elevator[e].k], "无法继续进入电梯1");
				}
				else {
					strcpy(PeopleChar7[Elevator[e].k], "无法继续进入电梯0");
				}
			}

			// 如果该楼层没有上行请求，清除上行请求
			if (!upqueue[Elevator[e].Floor].next) {
				Elevator[e].CallUp[Elevator[e].Floor] = 0;
			}
			Elevator[e].CallDianti[Elevator[e].Floor] = 0; // 清除电梯内请求
			Elevator[e].d1 = 0;   // 设定门为打开状态
			Elevator[e].d3 = 1;  // 门仍然开着
			Elevator[e].testdoortime = Time;  // 记录门开时间
			AddTimeSequence(Diantitime, NULL, InOut, Elevator, e);  // 调用门操作等待时间
		}
	}

	// 如果电梯正在下行，进行类似操作
	else if (Elevator[e].prestate == Goingdown && Elevator[e].PeopleNum < MaxPeopleNum) {
		People ap = downqueue[Elevator[e].Floor].next;
		if (ap && Elevator[e].PeopleNum < MaxPeopleNum) { // 如果有乘客且电梯未满
			// 显示乘客进入电梯的信息
			DisappearEvelatorBody(Elevator[e].Floor, Elevator, e);
			DisappearWaiting(Elevator[e].Floor);
			GotOxy(3 + 30 * Elevator[e].k, 6);
			printf("                           ");
			GotOxy(3 + 30 * Elevator[e].k, 7);
			printf("                           ");
			GotOxy(3 + 30 * Elevator[e].k, 6);
			printf("乘客%d成功进入电梯%d\n", ap->Id, Elevator[e].k);

			// 处理乘客ID显示格式
			a = ap->Id;
			num = 0;
			strcpy(PeopleChar6[Elevator[e].k], "乘客");
			for (i = 0; PeopleChar6[Elevator[e].k][i] != '\0'; i++) {
			}
			do {
				num++;
				a = a / 10;
			} while (a);
			a = ap->Id;
			num--;
			while (num != 0) {
				PeopleChar6[Elevator[e].k][i] = '0' + a / (int)(pow(10, num));
				a = a % (int)(pow(10, num));
				num--;
				i++;
			}
			PeopleChar6[Elevator[e].k][i] = '0' + a;
			PeopleChar6[Elevator[e].k][i + 1] = '\0';
			if (Elevator[e].k) {
				strcat(PeopleChar6[Elevator[e].k], "成功进入电梯1");
			}
			else {
				strcat(PeopleChar6[Elevator[e].k], "成功进入电梯0");
			}
			strcpy(PeopleChar7[Elevator[e].k], " ");

			// 更新电梯乘客数量，移除该乘客并将其添加到目标楼层的电梯栈中
			Elevator[e].PeopleNum++;
			Exist[ap->Id] = 0;
			downqueue[Elevator[e].Floor].next = ap->next;

			// 将乘客加入目标楼层的电梯栈中
			Dianti pr = (Dianti)malloc(sizeof(struct ElevatorStop));
			int i = ap->OutFloor;
			Elevator[e].CallDianti[i] = 1;
			pr->next = Elevator[e].Stack[i].next;
			Elevator[e].Stack[i].next = pr;
			pr->Id = ap->Id;

			// 更新电梯和等待队列显示
			ShowElevator(Elevator[e].Floor, Elevator, e);
			ShowWaitingQueue(Elevator[e].Floor);

			// 释放已处理的乘客
			free(ap);
		}

		// 如果有下行请求并且电梯内有空位，继续处理下一个乘客进入
		if (downqueue[Elevator[e].Floor].next && Elevator[e].PeopleNum < MaxPeopleNum)
			AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);
		else {
			// 如果下行队列中有等待乘客且电梯已满载
			if (downqueue[Elevator[e].Floor].next) {
				People t = downqueue[Elevator[e].Floor].next;
				GotOxy(3 + 30 * Elevator[e].k, 4);
				printf("                        ");
				GotOxy(3 + 30 * Elevator[e].k, 4);
				printf("电梯%d已满载", Elevator[e].k);
				if (Elevator[e].k) {
					strcpy(ElevatorChar[1], "电梯1已满载");
				}
				else {
					strcpy(ElevatorChar[0], "电梯0已满载");
				}
				GotOxy(3 + 30 * Elevator[e].k, 6);
				printf("                           ");
				GotOxy(3 + 30 * Elevator[e].k, 7);
				printf("                           ");
				GotOxy(3 + 30 * Elevator[e].k, 6);

				// 显示无法进入电梯的乘客信息
				while (t) {
					printf("乘客%d，", t->Id);
					a = t->Id;
					num = 0;
					strcpy(PeopleChar6[Elevator[e].k], "乘客");
					for (i = 0; PeopleChar6[Elevator[e].k][i] != '\0'; i++) {
					}
					do {
						num++;
						a = a / 10;
					} while (a);
					a = t->Id;
					num--;
					while (num != 0) {
						PeopleChar6[Elevator[e].k][i] = '0' + a / (int)(pow(10, num));
						a = a % (int)(pow(10, num));
						num--;
						i++;
					}
					PeopleChar6[Elevator[e].k][i] = ',';
					PeopleChar6[Elevator[e].k][i + 1] = '\0';
					t = t->next;
				}
				printf("无法继续进入电梯%d", Elevator[e].k);
				if (Elevator[e].k) {
					strcpy(PeopleChar7[Elevator[e].k], "无法继续进入电梯1");
				}
				else {
					strcpy(PeopleChar7[Elevator[e].k], "无法继续进入电梯0");
				}
			}
			// 如果该楼层没有下行请求，清除下行请求
			if (!downqueue[Elevator[e].Floor].next) {
				Elevator[e].CallDown[Elevator[e].Floor] = 0;
			}
			Elevator[e].CallDianti[Elevator[e].Floor] = 0;  // 清除电梯内请求
			Elevator[e].d1 = 0;   // 设定门为打开状态
			Elevator[e].d3 = 1;  // 门仍然开着
			Elevator[e].testdoortime = Time;  // 记录门开时间
			AddTimeSequence(Diantitime, NULL, InOut, Elevator, e);  // 调用门操作等待时间
		}
	}
}

//乘客走出电梯 
void PeopleOut(People p, Elevatorcanshu*& Elevator, int e)
{
	int a, num, i;

	// 如果电梯栈中有乘客（即当前楼层有等待下车的乘客）
	if (Elevator[e].Stack[Elevator[e].Floor].next) {
		// 移除电梯内的乘客
		DisappearEvelatorBody(Elevator[e].Floor, Elevator, e);// 消失电梯内体积
		Dianti ap = Elevator[e].Stack[Elevator[e].Floor].next; // 获取栈中的第一个乘客
		Elevator[e].Stack[Elevator[e].Floor].next = ap->next; // 将该乘客从栈中移除
		Elevator[e].PeopleNum--;  // 电梯内乘客数量减1

		// 更新电梯显示
		ShowElevator(Elevator[e].Floor, Elevator, e);
		GotOxy(3 + 30 * Elevator[e].k, 6);
		printf("                           ");
		GotOxy(3 + 30 * Elevator[e].k, 7);
		printf("                           ");
		GotOxy(3 + 30 * Elevator[e].k, 6);

		// 显示乘客走出电梯的信息
		printf("乘客%d走出电梯%d", ap->Id, Elevator[e].k);
		a = ap->Id;
		num = 0;
		strcpy(PeopleChar6[Elevator[e].k], "乘客");
		for (i = 0; PeopleChar6[Elevator[e].k][i] != '\0'; i++) {} // 计算ID的字符长度

		// 处理ID的显示
		do {
			num++;
			a = a / 10;
		} while (a);
		a = ap->Id;
		num--;
		while (num != 0) {
			PeopleChar6[Elevator[e].k][i] = '0' + a / (int)(pow(10, num));
			a = a % (int)(pow(10, num));
			num--;
			i++;
		}
		PeopleChar6[Elevator[e].k][i] = '0' + a;
		PeopleChar6[Elevator[e].k][i + 1] = '\0';
		if (Elevator[e].k) {
			strcat(PeopleChar6[Elevator[e].k], "走出电梯1");
		}
		else {
			strcat(PeopleChar6[Elevator[e].k], "走出电梯0");
		}
		strcpy(PeopleChar7[Elevator[e].k], " ");  // 清除与该乘客相关的消息

		// 释放已处理的乘客
		free(ap);
	}

	// 如果该楼层还有等待的乘客，继续处理
	if (Elevator[e].Stack[Elevator[e].Floor].next) {
		AddTimeSequence(Peopletime, NULL, PeopleOut, Elevator, e);
	}

	// 如果电梯正在上行并且该楼层有上行请求的乘客
	else if (Elevator[e].prestate == Goingup && upqueue[Elevator[e].Floor].next) {
		AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);
	}

	// 如果电梯正在下行并且该楼层有下行请求的乘客
	else if (Elevator[e].prestate == Goingdown && downqueue[Elevator[e].Floor].next) {
		AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);
	}
	else {
		// 如果电梯正在上行，且该楼层没有上行请求，检查更高楼层是否有请求
		if (Elevator[e].prestate == Goingup) {
			Elevator[e].CallUp[Elevator[e].Floor] = 0;
			int flag = 0;
			for (int i = Elevator[e].Floor + 1; i <= Maxfloor; i++) {
				if (Elevator[e].CallDianti[i] || Elevator[e].CallUp[i] || Elevator[e].CallDown[i]) {
					flag = 1;  // 发现更高楼层有请求
					break;
				}
			}
			// 如果没有请求，且下楼队列有乘客，则开始下行
			if (flag == 0) {
				if (downqueue[Elevator[e].Floor].next != NULL) {
					Elevator[e].prestate = Goingdown;
					AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);
					return;
				}
			}
		}
		// 如果电梯正在下行，且该楼层没有下行请求，检查更低楼层是否有请求
		else if (Elevator[e].prestate == Goingdown) {
			Elevator[e].CallDown[Elevator[e].Floor] = 0;
			int flag = 0;
			for (int i = Elevator[e].Floor - 1; i >= 0; i--) {
				if (Elevator[e].CallDianti[i] || Elevator[e].CallUp[i] || Elevator[e].CallDown[i]) {
					flag = 1;  // 发现更低楼层有请求
					break;
				}
			}
			// 如果没有请求，且上楼队列有乘客，则开始上行
			if (flag == 0) {
				if (upqueue[Elevator[e].Floor].next != NULL) {
					Elevator[e].prestate = Goingup;
					AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);
					return;
				}
			}
		}
		// 如果没有任何乘客进出，且电梯的门已经打开
		Elevator[e].CallDianti[Elevator[e].Floor] = 0;
		Elevator[e].d1 = 0; // 设置电梯门关闭
		Elevator[e].d3 = 1; // 电梯门保持打开状态
		Elevator[e].testdoortime = Time;  // 记录电梯门打开的时间
		AddTimeSequence(Diantitime, NULL, InOut, Elevator, e);  // 执行等待时间，进行门操作
	}
}

//电梯是否上升 
int GetUp(Elevatorcanshu*& Elevator, int e)
{
	// 从当前楼层的上方开始，检查是否有上行请求、下行请求或电梯召唤请求
	for (int i = Elevator[e].Floor + 1; i <= Maxfloor; i++) {
		// 如果某个楼层有上行、下行或电梯召唤请求
		if (Elevator[e].CallDianti[i] || Elevator[e].CallUp[i] || Elevator[e].CallDown[i])
			return 1; // 返回1，表示电梯需要上升
	}
	return 0;  // 如果没有任何请求，返回0，表示电梯不需要上升
}

//电梯是否下降 
int GetDown(Elevatorcanshu*& Elevator, int e)
{
	// 从当前楼层的下方开始，检查是否有上行请求、下行请求或电梯召唤请求
	for (int i = Elevator[e].Floor - 1; i >= 0; i--) {
		// 如果某个楼层有上行、下行或电梯召唤请求
		if (Elevator[e].CallDianti[i] || Elevator[e].CallUp[i] || Elevator[e].CallDown[i])
			return 1;  // 返回1，表示电梯需要下降
	}
	return 0;  // 如果没有任何请求，返回0，表示电梯不需要下降
}

//电梯加速完成，将进入正常速度 
void UniformSpeed(People ap, Elevatorcanshu*& Elevator, int e)
{
	// 判断电梯的状态
	if (Elevator[e].state == Goingup) {
		// 清除之前显示的内容
		GotOxy(3 + 30 * Elevator[e].k, 4);
		printf("                        ");

		// 输出电梯加速上升的信息
		GotOxy(3 + 30 * Elevator[e].k, 4);
		printf("电梯%d已加速上升！", Elevator[e].k);

		// 根据电梯的编号更新显示的信息
		if (Elevator[e].k) {
			strcpy(ElevatorChar[1], "电梯1已加速上升！");
		}
		else {
			strcpy(ElevatorChar[0], "电梯0已加速上升！");
		}

		// 记录加速上升状态的事件
		AddTimeSequence(Uptime, NULL, ElevatorUp, Elevator, e);
	}
	else {
		// 清除之前显示的内容
		GotOxy(3 + 30 * Elevator[e].k, 4);
		printf("                        ");

		// 输出电梯加速下降的信息
		GotOxy(3 + 30 * Elevator[e].k, 4);
		printf("电梯%d已加速下降！", Elevator[e].k);

		// 根据电梯的编号更新显示的信息
		if (Elevator[e].k) {
			strcpy(ElevatorChar[1], "电梯1已加速下降！");
		}
		else {
			strcpy(ElevatorChar[0], "电梯0已加速下降！");
		}

		// 记录加速下降状态的事件
		AddTimeSequence(Downtime, NULL, ElevatorDown, Elevator, e);
	}
}

//电梯去基层
void GoFirstFloor(People ap, Elevatorcanshu*& Elevator, int e)
{
	// 判断电梯是否处于空闲状态，且电梯返回基础层的时间是否已经过去
	if (Elevator[e].state != Idle || Elevator[e].backfirsttime + Backtime != Time)
		return;

	// 如果电梯已经在基础楼层，并且没有人在等待进入电梯
	if (Elevator[e].Floor == Basefloor && Elevator[e].d2 == 0) {
		Elevator[e].CallDianti[Basefloor] = 2; // 设置电梯到达基础层的标志
		return;
	}
	// 如果电梯已经在基础楼层，直接返回
	else if (Elevator[e].Floor == Basefloor)
		return;

	// 电梯当前不在基础楼层，显示提示信息，表示将自动返回基础楼层
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("                         ");
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("无人请求电梯%d!将进入%d层", Elevator[e].k, Basefloor);

	// 更新电梯信息字符串
	if (Elevator[e].k) {
		strcpy(ElevatorChar[1], "无人请求电梯1!将进入1层");
	}
	else {
		strcpy(ElevatorChar[0], "无人请求电梯0!将进入1层");
	}

	// 设置电梯到达基础楼层的标志
	Elevator[e].CallDianti[Basefloor] = 2;
}

// 结束乘客进出，正在关门
void InOut(People p, Elevatorcanshu*& Elevator, int e)
{
	// 上行电梯的处理
	if (Elevator[e].prestate == Goingup) {
		// 如果当前有乘客要进出电梯，或者电梯已经达到最大乘客数，或者电梯没有完成关门的时间要求
		if ((upqueue[Elevator[e].Floor].next && Elevator[e].PeopleNum < MaxPeopleNum) || Elevator[e].Stack[Elevator[e].Floor].next || Elevator[e].testdoortime + Diantitime != Time)
			return;

		// 如果上行队列没有请求，将该楼层的呼叫信号关闭
		if (!upqueue[Elevator[e].Floor].next) {
			Elevator[e].CallUp[Elevator[e].Floor] = 0;
		}

		// 关闭电梯的上下呼叫信号，并标记电梯状态
		Elevator[e].CallDianti[Elevator[e].Floor] = 0;
		Elevator[e].d1 = 0; Elevator[e].d3 = 0;
		Elevator[e].drcl = 1; Elevator[e].drcltime = Time;

		// 显示电梯关门提示信息
		GotOxy(3 + 30 * Elevator[e].k, 4);
		printf("                        ");
		GotOxy(3 + 30 * Elevator[e].k, 4);
		printf("电梯%d正在关门", Elevator[e].k);

		// 更新电梯状态描述
		if (Elevator[e].k) {
			strcpy(ElevatorChar[1], "电梯1正在关门");
		}
		else {
			strcpy(ElevatorChar[0], "电梯0正在关门");
		}

		// 添加关门任务
		AddTimeSequence(Doortime, NULL, CloseElevator, Elevator, e);
	}

	// 下行电梯的处理
	else if (Elevator[e].prestate == Goingdown) {
		// 如果当前有乘客要进出电梯，或者电梯已经达到最大乘客数，或者电梯没有完成关门的时间要求
		if ((downqueue[Elevator[e].Floor].next && Elevator[e].PeopleNum < MaxPeopleNum) || Elevator[e].Stack[Elevator[e].Floor].next || Elevator[e].testdoortime + Diantitime != Time)
			return;

		// 如果下行队列没有请求，将该楼层的呼叫信号关闭
		if (!downqueue[Elevator[e].Floor].next) {
			Elevator[e].CallDown[Elevator[e].Floor] = 0;
		}

		// 关闭电梯的上下呼叫信号，并标记电梯状态
		Elevator[e].CallDianti[Elevator[e].Floor] = 0;
		Elevator[e].d1 = 0; Elevator[e].d3 = 0;
		Elevator[e].drcl = 1; Elevator[e].drcltime = Time;
		GotOxy(3 + 30 * Elevator[e].k, 4);
		printf("                        ");

		// 显示电梯关门提示信息
		GotOxy(3 + 30 * Elevator[e].k, 4);
		printf("电梯%d正在关门", Elevator[e].k);

		// 更新电梯状态描述
		if (Elevator[e].k) {
			strcpy(ElevatorChar[1], "电梯1正在关门");
		}
		else {
			strcpy(ElevatorChar[0], "电梯0正在关门");
		}

		// 添加关门任务
		AddTimeSequence(Doortime, NULL, CloseElevator, Elevator, e);
	}
}

//电梯上升
void ElevatorUp(People ap, Elevatorcanshu*& Elevator, int e)
{
	// 电梯楼层增加（上升）
	Elevator[e].Floor++;

	// 更新显示电梯上升的状态
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("                        ");
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("电梯%d正在上升", Elevator[e].k);

	// 设置电梯上升的描述
	if (Elevator[e].k) {
		strcpy(ElevatorChar[1], "电梯1正在上升");
	}
	else {
		strcpy(ElevatorChar[0], "电梯0正在上升");
	}

	// 隐藏上一个楼层的电梯并显示当前楼层的电梯
	DisappearEvelatorBody(Elevator[e].Floor - 1, Elevator, e);
	ShowElevator(Elevator[e].Floor, Elevator, e);

	// 判断电梯是否需要停下来
	if (Elevator[e].prestate == Goingup && (Elevator[e].CallUp[Elevator[e].Floor] || Elevator[e].CallDianti[Elevator[e].Floor] || Elevator[e].CallDown[Elevator[e].Floor])) {
		// 如果上升过程中有请求，检查是否需要停下来
		if (Elevator[e].CallUp[Elevator[e].Floor] || Elevator[e].CallDianti[Elevator[e].Floor])
			AddTimeSequence(UpSlowdown, NULL, ElevatorStop, Elevator, e); // 停止上升
		else {
			int flag = 0;
			// 检查未来楼层是否有其他请求
			for (int i = Elevator[e].Floor + 1; i <= Maxfloor; i++) {
				if (Elevator[e].CallUp[i] || Elevator[e].CallDianti[i] || Elevator[e].CallDown[i]) {
					flag = 1;
					break;
				}
			}
			if (flag == 0) {
				AddTimeSequence(UpSlowdown, NULL, ElevatorStop, Elevator, e); // 如果没有请求，停止上升
			}
			else {
				AddTimeSequence(Uptime, NULL, ElevatorUp, Elevator, e); // 否则继续上升
			}
		}
	}
	else if (Elevator[e].prestate == Goingdown && (Elevator[e].CallUp[Elevator[e].Floor] || Elevator[e].CallDianti[Elevator[e].Floor] || Elevator[e].CallDown[Elevator[e].Floor])) {
		// 如果电梯是下行，且当前楼层有请求，检查是否停止上升
		if (Elevator[e].CallDown[Elevator[e].Floor] || Elevator[e].CallDianti[Elevator[e].Floor])
			AddTimeSequence(UpSlowdown, NULL, ElevatorStop, Elevator, e); // 停止上升
		else {
			int flag = 0;
			// 检查过去楼层是否有请求
			for (int i = Elevator[e].Floor - 1; i >= 0; i--) {
				if (Elevator[e].CallUp[i] || Elevator[e].CallDianti[i] || Elevator[e].CallDown[i]) {
					flag = 1;
					break;
				}
			}
			if (flag == 0)AddTimeSequence(UpSlowdown, NULL, ElevatorStop, Elevator, e); // 如果没有请求，停止上升
			else {
				AddTimeSequence(Uptime, NULL, ElevatorUp, Elevator, e); // 否则继续上升
			}
		}
	}
	else {
		// 如果没有请求并且电梯没有到达最高楼层，继续上升
		if (Elevator[e].Floor == Maxfloor) {
			AddTimeSequence(UpSlowdown, NULL, ElevatorStop, Elevator, e);// 如果没有请求并且电梯没有到达最高楼层，继续上升
		}
		else {
			AddTimeSequence(Uptime, NULL, ElevatorUp, Elevator, e); // 否则继续上升
		}
	}
}

//电梯下降 
void ElevatorDown(People ap, Elevatorcanshu*& Elevator, int e)
{
	// 电梯楼层减少（下降）
	Elevator[e].Floor--;

	// 更新显示电梯下降的状态
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("                        ");
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("电梯%d正在下降", Elevator[e].k);

	// 设置电梯下降的描述
	if (Elevator[e].k) {
		strcpy(ElevatorChar[1], "电梯1正在下降");
	}
	else {
		strcpy(ElevatorChar[0], "电梯0正在下降");
	}

	// 隐藏上一个楼层的电梯并显示当前楼层的电梯
	DisappearEvelatorBody(Elevator[e].Floor + 1, Elevator, e);
	ShowElevator(Elevator[e].Floor, Elevator, e);

	// 判断是否需要停止下降
	if (Elevator[e].prestate == Goingup && (Elevator[e].CallUp[Elevator[e].Floor] || Elevator[e].CallDianti[Elevator[e].Floor])) {
		// 如果电梯正在上升并且当前楼层有请求，停止下降
		AddTimeSequence(UpSlowdown, NULL, ElevatorStop, Elevator, e);
	}
	else if (Elevator[e].prestate == Goingdown && (Elevator[e].CallDown[Elevator[e].Floor] || Elevator[e].CallDianti[Elevator[e].Floor])) {
		// 如果电梯正在下降并且当前楼层有请求，停止下降
		AddTimeSequence(UpSlowdown, NULL, ElevatorStop, Elevator, e);
	}
	else {
		// 如果电梯没有请求且还没有到达底楼，则继续下降
		if (Elevator[e].Floor == 0) {
			AddTimeSequence(DownSlowdown, NULL, ElevatorStop, Elevator, e); // 到达底楼时停止下降
		}
		else {
			AddTimeSequence(Downtime, NULL, ElevatorDown, Elevator, e); // 否则继续下降
		}
	}
}

//电梯门关上 
void CloseElevator(People ap, Elevatorcanshu*& Elevator, int e)
{
	// 判断电梯是否已关闭门或者关闭门的时间是否达到
	if (Elevator[e].drcl == 0 || Time != Elevator[e].drcltime + Doortime)
		return;  // 如果门已经关闭或者时间未到，直接返回

	// 电梯门状态更新，表示门已经关闭
	Elevator[e].drcl = 0;

	// 清除当前显示的电梯门状态信息
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("                        ");

	// 更新电梯门关闭的信息
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("电梯%d门关了", Elevator[e].k);

	// 更新电梯状态描述
	if (Elevator[e].k) {
		strcpy(ElevatorChar[1], "电梯1门关了");
	}
	else {
		strcpy(ElevatorChar[0], "电梯0门关了");
	}
}

//电梯停下 
void ElevatorStop(People ap, Elevatorcanshu*& Elevator, int e)
{
	// 设置电梯状态为Idle，表示电梯停止
	Elevator[e].state = Idle;

	// 重置电梯的相关状态
	Elevator[e].d1 = 0;
	Elevator[e].d3 = 0;

	// 清除当前电梯的状态显示信息
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("                        ");

	// 更新显示电梯停下的状态
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("电梯%d停了，当前层是%d", Elevator[e].k, Elevator[e].Floor);

	// 电梯当前楼层赋值给变量a
	int a = Elevator[e].Floor, i;

	// 更新电梯0或电梯1的状态描述
	if (Elevator[e].k) {  // 如果是电梯1
		strcpy(ElevatorChar[1], "电梯1停了，当前层是");
		for (i = 0; ElevatorChar[1][i] != '\0'; i++) {  // 将当前楼层数字转换为字符
		}
		ElevatorChar[1][i] = '0' + Elevator[e].Floor;
		ElevatorChar[1][i + 1] = '\0';
	}
	else {  // 如果是电梯0
		strcpy(ElevatorChar[0], "电梯0停了，当前层是");
		for (i = 0; ElevatorChar[0][i] != '\0'; i++) {
		}
		ElevatorChar[0][i] = '0' + Elevator[e].Floor;  // 将当前楼层数字转换为字符
		ElevatorChar[0][i + 1] = '\0';
	}
}

