#include "Elevator.h"

//ȷ���ն��й��λ��
void GotOxy(short x, short y)
{
	COORD coord = { x, y };  // ����һ�� COORD �ṹ�������������λ��
	/*COORD��Windows API�ж����һ�ֽṹ����ʾһ���ַ��ڿ���̨��Ļ�ϵ����ꡣ�䶨��Ϊ��

	typedef struct _COORD {

	SHORT X; // horizontal coordinate

	SHORT Y; // vertical coordinate
	} COORD;*/

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);// ���ÿ���̨�Ĺ��λ��
}

//��ʾ������Ϣ
void ShowOthersInfo()
{
	// ��ʾ����0�͵���1�ı�ʶ
	GotOxy(14, 8);
	printf("����0");
	GotOxy(42, 8);
	printf("����1");

	// ��ʾ¥����Ϣ
	for (int i = 0; i <= Maxfloor; i++)
	{
		GotOxy(LeftElevatorX - 2, LeftElevatorY - 1 + (4 - i) * FloorHight);
		printf("%d", i);
	}

	// ��ʾ����0�͵���1��״̬��Ϣ
	GotOxy(3, 4);
	printf("%s", ElevatorChar[0]);
	GotOxy(33, 4);
	printf("%s", ElevatorChar[1]);

	// ��ʾ�˿�0�ͳ˿�1��״̬��Ϣ
	GotOxy(3, 6);
	printf("%s", PeopleChar6[0]);
	GotOxy(33, 6);
	printf("%s", PeopleChar6[1]);

	// ��ʾ�˿��뿪���ݵ�״̬��Ϣ
	GotOxy(3, 7);
	printf("%s", PeopleChar7[0]);
	GotOxy(33, 7);
	printf("%s", PeopleChar7[0]);
}

//������ݵ���������
void PrintElevator()
{
	int j, i, k;
	for (i = 0; i <= 1; i++)// ��������0�͵���1
	{
		for (j = Maxfloor + 1; j >= 0; j--) // ��¥�� Maxfloor �� 0 ���л��Ʊ߿�
		{
			if (j == Maxfloor) // �������ƶ����߽�
			{
				GotOxy(6 + i * 27, 9); // ���ù��λ��
				printf("--------------------------"); // ���ƶ��߿�
			}
			GotOxy(6 + i * 27, 9 + j * FloorHight); // ����ÿһ��¥�ĺ���߿�λ��
			printf("--------------------------"); // ����ÿ��¥�ı߿�

		}

		j = Maxfloor + 1;
		for (k = 9; k <= 9 + j * FloorHight; k++) // �Ӷ�������ײ㣬�������������
		{
			GotOxy(5 + i * 27, k); // ���ù��λ�ã������������
			printf("|");
			GotOxy(5 + i * 27 + 26, k); // ���ù��λ�ã������ұ�����
			printf("|");
		}
	}
}

//������x��ĵ���
void DisappearEvelatorBody(int x, Elevatorcanshu*& Elevator, int e)
{
	int i, j;
	//����������Ϊ������ɫ(��ɫ)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);

	// ͨ��ѭ���ڵ��ݿ�ܵĶ�Ӧλ�ô�ӡ�ո񣬽�����"����"
	for (i = LeftElevatorX + Elevator[e].k * 27; i <= LeftElevatorX + Elevator[e].k * 26 + 24; i++)
	{
		// ���ù��λ�ã��ڵ��ݿ���ڴ�ӡ�ո������������
		GotOxy(i, LeftElevatorY - 1 + (4 - x) * FloorHight);
		printf(" ");  // ��ӡ�ո��������������
	}
	// �ָ�������ɫΪĬ�ϵİ�ɫ
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

//��x����ʾ����
void ShowElevator(int x, Elevatorcanshu*& Elevator, int e)
{
	int i, j;
	// ���ù��λ�õ����ݵ�����ܣ���ʾ���ݣ�λ��
	GotOxy(LeftElevatorX + Elevator[e].k * 27, LeftElevatorY - 1 + (4 - x) * FloorHight);
	// ��ӡ���ݵĿ�ͷ����
	printf("[");

	// ������ǰ�������ڵ�¥��ջ��Stack�����г����г˿͵�ID
	for (i = 0; i <= Maxfloor; i++)
	{
		Dianti ap = Elevator[e].Stack[i].next;
		while (ap) {
			// ���������ÿ���˿͵�ID
			printf(" %d", ap->Id);
			ap = ap->next;  // �ƶ�����һ���˿�
		}
	}

	// ���ù��λ�õ����ݵ��Ҳ��ܣ���ʾ���ݣ�λ��
	GotOxy(LeftElevatorX + Elevator[e].k * 26 + 24, LeftElevatorY - 1 + (4 - x) * FloorHight);
	
	// ��ӡ���ݵĽ�β����
	printf("]");
}

//��ʾ�ȴ����еĳ˿�
void ShowWaitingQueue(int i)
{
	People p;
	p = upqueue[i].next;  // ��ȡ�ȴ���¥�ĳ˿Ͷ���
	int j = 0;

	// �����ȴ���¥�ĳ˿�
	while (p != NULL)
	{
		// ���ù��λ��
		GotOxy(RightElevatorX + 26 + j * 6, RightElevatorY - 1 + (4 - p->InFloor) * FloorHight);
		
		// ��ӡ�˿�ID
		printf(" %d", p->Id);

		// ��ӡ��ͷ��Ŀ��¥��
		printf("�� ");
		printf("%d", p->OutFloor);

		// �ƶ�����һ���˿�
		p = p->next;
		j++;  // �ۼ�����ʾ��ͬ�˿͵�λ��
	}

	// �ۼ�����ʾ��ͬ�˿͵�λ��
	p = downqueue[i].next;

	// �����ȴ���¥�ĳ˿�
	while (p != NULL)
	{
		// ���ù��λ��
		GotOxy(RightElevatorX + 26 + j * 6, RightElevatorY - 1 + (4 - p->InFloor) * FloorHight);
		
		// ��ӡ�˿�ID
		printf(" %d", p->Id);

		// ��ӡ��ͷ��Ŀ��¥��
		printf("�� ");
		printf("%d", p->OutFloor);

		// �ƶ�����һ���˿�
		p = p->next;
		j++;  // �ۼ�����ʾ��ͬ�˿͵�λ��
	}
}

//�������ڵȴ�״̬�ĳ˿�
void DisappearWaiting(int i)
{
	//�����������ɫ���ñ�����ɫ��дһ�������ȥ���� 0�Ǻ�ɫ 7�ǰ�ɫ
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);

	// ���� ShowWaitingQueue ����������ʾ�ȴ����еĳ˿ͣ���ʱ���ñ���ɫ���ǣ�
	ShowWaitingQueue(i);

	// ��������ٻָ�������ɫ����ɫ��
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

//��ʾ˵����Ϣ
void PrintHelp()
{
	GotOxy(70, 5);
	printf("ǰ������֣��˿�ID");
	
	GotOxy(70, 7);
	printf("��ͷ������ or ����");
	
	GotOxy(70, 9);
	printf("��������֣�Ŀ��¥��");
}

//��ʼ��������Ϣ 
void InitElevator(Elevatorcanshu*& Elevator, int e)
{
	int i;

	// ���õ��ݳ�ʼ¥��Ϊ����
	Elevator[e].Floor = Basefloor;								
	printf("%d\n", Elevator[e].Floor);						// ��ӡ��һ���������ڵ�¥�㣨���ڵ��ԣ�
	printf("%d\n", Elevator[1].Floor);						// ��ӡ�ڶ����������ڵ�¥�㣨���ڵ��ԣ�
	

	// ��ʼ������״̬��־����
	Elevator[e].d1 = Elevator[e].d2 = Elevator[e].d3 = 0;	// ������ر�־������ʼ��Ϊ0
	Elevator[e].state = Elevator[e].prestate = Idle;		// ���ݵ�ǰ״̬����һ״̬����Ϊ��
	Elevator[e].again = 0;									// ���õ����ٴ������־Ϊ0
	Elevator[e].drcl = 0;									// ��ʼ�������ŵĿ��Ʋ���Ϊ0

	// ��ʼ��ʱ��������ֶ�
	Elevator[e].sxjiankong.f = NULL;						// ʱ���صĻص�����ָ������Ϊ��
	Elevator[e].sxjiankong.next = NULL;						// ʱ���ص���һ���¼�ָ���ʼ��Ϊ��
	Elevator[e].sxjiankong.person = 0;						// ʱ���صĳ˿��ֶγ�ʼ��Ϊ0���޳˿�
	Elevator[e].sxjiankong.xianzhitime = 0;					// ʱ���ص�����ʱ���ʼ��Ϊ0
	
	// ��ʼ��ÿ��¥��İ�ť״̬�͵����ڳ˿Ͷ�ջ
	for (i = 0; i <= Maxfloor; i++) {
		Elevator[e].CallDianti[i] = 0;						// �����ڰ�ť��ʼ��Ϊδ���£�0��
		Elevator[e].CallDown[i] = 0;						// ¥�����а�ť��ʼ��Ϊδ���£�0��
		Elevator[e].CallUp[i] = 0;							// ¥�����а�ť��ʼ��Ϊδ���£�0��
		Elevator[e].Stack[i].next = NULL;					// ÿ������ڳ˿Ͷ�ջ��ʼ��Ϊ��
	}

	// ��ʼ�������ڵ�ǰ�˿�����Ϊ0
	Elevator[e].PeopleNum = 0;
}

//��ʾ����ģ�����ͷ
void ShowTitle()
{
	printf("\n\n\n");
	printf("\n\t\t*********************************************************");
	printf("\n\t\t_________________________________________________________");
	printf("\n\t\t��������������������ӭ�������ݹ���ϵͳ ������������������\n\n");
	printf("\t\t                ���ε���ģ���ʱ�䵥λΪ%.1f��\n",UnitTime);
	printf("\n\t\t*********************************************************\n");
}

//����������ģ������ʱ�� 
void InputMaxTime()
{
	printf("\n\t\t��������ݵ��������ʱ�䣬�ﵽ��ʱ�䣬����ϵͳ�����Զ�ֹͣ\n");
	printf("\t\t                   �������������ʱ��:");
	while (scanf("%d", &Maxtime) != 1 || Maxtime < MinTime || Maxtime > MaxTime)
	{
		printf("\t\t     �����������������루%d<=�������ʱ��<=%d��\n", MinTime, MaxTime);
		while (getchar() != '\n')
			continue;
		printf("\t\t                   �������������ʱ��:");
	}
}

//�����³˿� 
void AddNewPerson(People pt, Elevatorcanshu*& Elevator, int e)
{
	system("cls");		// ����
	int InFloor, OutFloor, GiveupTime, EnterTime, FlagElevator0 = 0, FlagElevator1 = 0;
	
	while (1) {
		// ��ʾ��ǰ��ʱ�����ʾ��Ϣ
		printf("\t\t*********************************************************\n");
		printf("\t\t                   ʱ��Ϊ��%d\n", Time);
		printf("\t\t            ��%dλ�˿��߽������Ⱥ���ݡ�\n", id);
		
		// ����˿�����¥��
		while (1) {
			printf("\t\t            �����Ĳ�¥���ݵ���ǰ:_____\b\b\b");
			scanf("%d", &InFloor);
			if (InFloor >= 0 && InFloor <= Maxfloor)break;
			printf("\t\t            �����¥�㳬�����ޣ����������롣\n");
		}

		// �����Ƿ���е���0�͵���1
		while (1) {
			char c;
			printf("\t\t            �Ƿ���е���0�������루Y/N����");
			getchar();
			c = getchar();
			while (c != 'Y' && c != 'N' && c != 'y' && c!= 'n') {
				printf("\t\t            �����ʽ��������������,�Ƿ���е���0��Y/N����");
				getchar();
				c = getchar();
			}
			if (c == 'Y' || c == 'y') {
				FlagElevator0 = 1;		// ���е���0
			}


			printf("\t\t            �Ƿ���е���1�������루Y/N����");
			getchar();
			scanf("%c", &c);
			while (c != 'Y' && c != 'N' && c != 'y' && c != 'n') {
				printf("\t\t            �����ʽ��������������,�Ƿ���е���1��Y/N����");
				getchar();
				scanf("%c", &c);
			}
			if (c == 'Y' || c == 'y') {
				FlagElevator1 = 1;		// ���е���1
			}

			if (FlagElevator0 || FlagElevator1)
			{
				break;		// ���ٺ���һ������
			}
			printf("\t\t            ����û�к��е��ݣ�����������Ҫ���еĵ��ݡ�\n");
		}

		// ����˿�Ŀ��¥��
		while (1) {
			printf("\t\t            ��Ҫȥ�Ĳ�¥:_____\b\b\b");
			scanf("%d", &OutFloor);
			if (OutFloor >= 0 && OutFloor <= Maxfloor)break;
			printf("\t\t            �����¥�㳬�����ޣ����������롣\n");
		}

		// ����˿����ȴ�ʱ�����һ���˿ͳ��ֵ�ʱ��
		printf("\t\t            �������̵ĵȺ�ʱ��:_____\b\b\b");
		scanf("%d", &GiveupTime);
		printf("\t\t            ��һ���˿ͳ��ֵ�ʱ����:_____\b\b\b");
		scanf("%d", &EnterTime);
		printf("\t\t*********************************************************\n\n");
		if (OutFloor != InFloor) {
			break;			//Ŀ��¥��͵�ǰ¥����ͬ����������
		}

		printf("\t\t*********************************************************\n");
		printf("\t\t������󣺳˿�Ҫȥ��¥�����䵱ǰ����¥����ͬ������������\n");
		printf("\t\t*********************************************************\n\n");
	}

	// �����µĳ˿���Ϣ
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
	Exist[id - 1] = 1;								// ��ǳ˿ʹ���
	
	// ����Ŀ��¥�㷽�򽫳˿ͼ��������ж���
	if (OutFloor > InFloor) {
		AddUpQueue(p, InFloor);						// �������ж���
		if (FlagElevator0) {						
			Elevator[0].CallUp[InFloor] = 1;		// ����0��Ӧ����
		}
		if (FlagElevator1) {
			Elevator[1].CallUp[InFloor] = 1;		// ����1��Ӧ����
		}
	}
	else {
		AddDownQueue(p, InFloor);					// �������ж���
		if (FlagElevator0) {
			Elevator[0].CallDown[InFloor] = 1;		// ����0��Ӧ����
		}
		if (FlagElevator1) {
			Elevator[1].CallDown[InFloor] = 1;		// ����1��Ӧ����
		}
	}

	// ���µ���״̬��ʱ���¼�
	if (FlagElevator0 && FlagElevator1) {
		flagelevator = 1;							// ��������ͬʱ������
	}
	if (!FlagElevator0 && FlagElevator1) {
		AddTimeSequence(GiveupTime, p, GiveupWait, Elevator, 1);// ��ӳ�ʱ�¼�������1
		AddTimeSequence(EnterTime, NULL, AddNewPerson, Elevator, 1);// �����һ�˿��¼�������1
	}
	else {
		AddTimeSequence(GiveupTime, p, GiveupWait, Elevator, 0); // ��ӳ�ʱ�¼�������0
		AddTimeSequence(EnterTime, NULL, AddNewPerson, Elevator, 0);// �����һ�˿��¼�������0
	}

	// ������ˢ����ʾ
	system("cls");
	ShowOthersInfo();										// ��ʾ������Ϣ
	PrintElevator();								//������ݿ��
	PrintHelp();										// ��ʾ������Ϣ
	ShowElevator(Elevator[0].Floor, Elevator, 0);	// ��ʾ����0״̬
	ShowElevator(Elevator[1].Floor, Elevator, 1);   // ��ʾ����1״̬              //�����������
	int i;

	// ��ʾÿ��ĵȴ�����
	for (i = 0; i <= Maxfloor; i++) {
		ShowWaitingQueue(i);
	}
}

// ������������
void AddUpQueue(People& p, int in)
{
	// ��ȡָ��¥������ж���ͷ�ڵ�
	People q = &upqueue[in];

	// �������У��ҵ����һ���ڵ�
	while (q->next != NULL) {
		q = q->next;
	}

	// �ڶ���ĩβ����³˿ͽڵ�
	q->next = p;
	p->pre = q; // �����³˿ͽڵ��ǰ��ָ��
}

// �����½�����
void AddDownQueue(People& p, int in)
{
	// ��ȡָ��¥������ж���ͷ�ڵ�
	People q = &downqueue[in];

	// �������У��ҵ����һ���ڵ�
	while (q->next != NULL) {
		q = q->next;
	}

	// �ڶ���ĩβ����³˿ͽڵ�
	q->next = p;
	p->pre = q; // �����³˿ͽڵ��ǰ��ָ��
}

// ��������������һ����ʱ��˳�����еĶ�����
void AddTimeSequence(int t, People p, void(*fn)(People per, Elevatorcanshu*& Elevatortemp, int temp), Elevatorcanshu*& Elevator, int e)
{
	// ���������ʵ�ʴ���ʱ��
	int time1 = Time + t;

	// ��ȡ���ݵ��������¼�����ͷ�ڵ�
	Shixu r = &Elevator[e].sxjiankong;

	// �������¼��ڵ�
	Shixu q;
	q = (Shixu)malloc(sizeof(struct TimeSequence));
	q->xianzhitime = time1;					// �����¼�����ʱ��

	// ��������˿ͽڵ㲻Ϊ�գ����Ƴ˿���Ϣ
	if (p != NULL) {
		q->person = (People)malloc(sizeof(struct people));
		q->person->pre = p->pre;
		q->person->Id = p->Id;
		q->person->InFloor = p->InFloor;	// û�й����˿�ʱ����Ϊ��
	}
	else {
		q->person = NULL;
	}

	// �����¼���Ӧ�Ļص�����
	q->f = fn;
	q->next = NULL;

	// �ҵ��¼��������ʺϲ����λ�ã���ʱ����������
	while (r->next != NULL) {
		if (r->next->xianzhitime > time1)break;  // �ҵ���һ��ʱ����ڵ�ǰ�¼�ʱ��Ľڵ�
		r = r->next;
	}

	// �����¼��ڵ�
	q->next = r->next;
	r->next = q;
}

//�˿ͷ����ȴ����� 
void GiveupWait(People per, Elevatorcanshu*& Elevator, int e)
{
	// ����˿��ѽ�����ݣ�ֱ�ӷ���
	if (Exist[per->Id] == 0) {
		return;
	}
	// ������ݿ������ڵ�ǰ¥�㣬���������ɳ˿ͣ������ȴ�����ȡ��
	else if (per->InFloor == Elevator[0].Floor && Elevator[0].state == Idle && per->FlagElevator0 && Elevator[0].PeopleNum < MaxPeopleNum)
		return;
	else if (per->InFloor == Elevator[1].Floor && Elevator[1].state == Idle && per->FlagElevator1 && Elevator[1].PeopleNum < MaxPeopleNum)
		return;

	// ���µȴ������е���ʾ���Ƴ��ó˿�
	DisappearWaiting(per->InFloor);

	int i, a, num;

	// ����˿ͺ����˵��� 0
	if (per->FlagElevator0) {
		GotOxy(3, 6);   // �����ʾ
		printf("                           ");
		GotOxy(3, 7);   // �����ʾ
		printf("                           ");
		GotOxy(3, 6);   // ���´�ӡ�����Ⱥ�ĳ˿���Ϣ
		printf("�˿�%d�����Ⱥ�,�뿪��", per->Id);

		a = per->Id;    // �˿�ID��ֵ��a
		num = 0;        // ��ʼ�����ֵ�λ��
		strcpy(PeopleChar6[0], "�˿�");  // ��ʼ���ַ�����
		for (i = 0; PeopleChar6[0][i] != '\0'; i++) {}  // �����ַ���ĩβ

		do {  // ����˿�ID��λ��
			num++;
			a = a / 10;
		} while (a);  // ֱ��aΪ0

		a = per->Id;    // ���¸�ֵaΪ�˿�ID
		num--;          // �˿�ID��λ����1

		while (num != 0) {  // ��λ��ȡ��ƴ�ӳ˿�ID
			PeopleChar6[0][i] = '0' + a / (int)(pow(10, num)); // ȡ�õ�ǰλ�����ֲ�ת��Ϊ�ַ�
			a = a % (int)(pow(10, num));  // ȡ���ȡʣ�ಿ��
			num--;  // λ����1
			i++;    // �ƶ�����һ���ַ�λ��
		}
		PeopleChar6[0][i] = '0' + a;  // ���һ������
		PeopleChar6[0][i + 1] = '\0';  // �ַ���������
		strcat(PeopleChar6[0], "�����Ⱥ�,�뿪��");  // ƴ���������뿪��Ϣ
		strcpy(PeopleChar7[0], " ");  // ��յڶ����ַ�����
	}

	// ����˿ͺ����˵��� 1
	if (per->FlagElevator1) {
		GotOxy(3 + 30, 6);  // �ƶ���굽�µ�λ��
		printf("                           ");  // ���ԭ��Ϣ
		GotOxy(3 + 30, 7);  // ���ԭ��Ϣ
		printf("                           ");  // ���ԭ��Ϣ
		GotOxy(3 + 30, 6);  // ���´�ӡ�����Ⱥ�ĳ˿���Ϣ
		printf("�˿�%d�����Ⱥ�,�뿪��", per->Id);

		a = per->Id;  // �˿�ID��ֵ��a
		num = 0;      // ��ʼ�����ֵ�λ��
		strcpy(PeopleChar6[1], "�˿�");  // ��ʼ���ַ�����
		for (i = 0; PeopleChar6[1][i] != '\0'; i++) {}  // �����ַ���ĩβ

		do {  // ����˿�ID��λ��
			num++;
			a = a / 10;
		} while (a);  // ֱ��aΪ0

		a = per->Id;    // ���¸�ֵaΪ�˿�ID
		num--;          // �˿�ID��λ����1

		while (num != 0) {  // ��λ��ȡ��ƴ�ӳ˿�ID
			PeopleChar6[1][i] = '0' + a / (int)(pow(10, num));  // ȡ�õ�ǰλ�����ֲ�ת��Ϊ�ַ�
			a = a % (int)(pow(10, num));  // ȡ���ȡʣ�ಿ��
			num--;  // λ����1
			i++;    // �ƶ�����һ���ַ�λ��
		}
		PeopleChar6[1][i] = '0' + a;  // ���һ������
		PeopleChar6[1][i + 1] = '\0';  // �ַ���������
		strcat(PeopleChar6[1], "�����Ⱥ�,�뿪��");  // ƴ���������뿪��Ϣ
		strcpy(PeopleChar7[1], " ");  // ��յڶ����ַ�����
	}

	// ���¶���״̬���Ƴ��ó˿�
	int infloor = per->InFloor;  // ��ȡ�˿͵ĵ�ǰ¥��
	int outfloor = per->OutFloor;  // ��ȡ�˿͵�Ŀ��¥��
	Exist[per->Id] = 0;  // ��ǳ˿�IDΪ�Ѳ�����

	People q = per->pre, ap = q->next;  // ��ȡ�˿͵�ǰһ���ڵ����һ���ڵ�
	q->next = ap->next;  // ɾ����ǰ�˿ͽڵ�
	if (ap->next != NULL) {
		ap->next->pre = q;  // �����һ���ڵ㲻Ϊ�գ�������ǰ���ڵ�
	}

	// ���µ�ǰ¥��ĵȴ�������ʾ
	ShowWaitingQueue(per->InFloor);

	// �ͷų˿ͽڵ���ڴ�
	free(ap);
}

//����ģ���һ����ʱ�� 
void Simulate()
{
	// ��̬�����ڴ棬������������
	Elevatorcanshu* Elevator;
	Elevator = (Elevatorcanshu*)malloc(sizeof(Elevatorcanshu) * 2);
	Elevator[0].k = 0; // ��ʼ����һ�����ݵı��
	Elevator[1].k = 1; // ��ʼ���ڶ������ݵı��

	// ��ʼ����������
	InitElevator(Elevator, 0);
	InitElevator(Elevator, 1);

	// ��ʼ����˿�
	AddNewPerson(NULL, Elevator, 0);

	// ��ʼģ�⣬ֱ��ʱ��ﵽ���ģ��ʱ��
	while (Time <= Maxtime) {

		// �����һ���������¼������������ݶ�������
		if (Elevator[0].sxjiankong.next || flagelevator) {
			do {
				Elevator[0].again = 0;				// �����ٴ������־
				ControlElevator(Elevator, 0);				// ���Ƶ�һ����������
			} while (Elevator[0].again == 1);		// ������ٴ������ظ�����
		}

		// ����ڶ����������¼������������ݶ�������
		if (Elevator[1].sxjiankong.next || flagelevator) {
			do {
				Elevator[1].again = 0;				// �����ٴ������־
				ControlElevator(Elevator, 1);				// ���Ƶڶ������ݵ�����
			} while (Elevator[1].again == 1);		// ������ٴ������ظ�����
		}

		// ��ȡ��һ�����ݺ͵ڶ������ݵ�ʱ���¼�
		Shixu p = Elevator[0].sxjiankong.next;
		Shixu q = Elevator[1].sxjiankong.next;

		// ����������ݶ�û��ʱ���¼��������ģ��
		if (p == NULL && q == NULL) {
			Time = Maxtime;							// ����ʱ��Ϊ���ģ��ʱ�䣬����ģ��
		}

		// �����һ�����ݵ�ʱ���¼�
		while (p && p->xianzhitime <= Time) {
			Elevator[0].sxjiankong.next = p->next;	 // ����ʱ���¼�����
			p->f(p->person, Elevator, 0);			 // ִ��ʱ���¼��Ļص�����
			free(p);								 // �ͷ��¼��ڵ�
			p = Elevator[0].sxjiankong.next;		 // ��ȡ��һ��ʱ���¼�
		}

		// ����ڶ������ݵ�ʱ���¼�
		while (q && q->xianzhitime <= Time) {
			Elevator[1].sxjiankong.next = q->next;	 // ����ʱ���¼�����
			q->f(q->person, Elevator, 1);			 // ִ��ʱ���¼��Ļص�����
			free(q);								 // �ͷ��¼��ڵ�
			q = Elevator[1].sxjiankong.next;		 // ��ȡ��һ��ʱ���¼�
		}

		Time++;										 // ����ʱ��
		GotOxy(20, 2);								 // ���ù��λ�ã���ʾ��ǰʱ��
		printf("ʱ��Ϊ�� %d", Time);				 // ��ӡ��ǰʱ��
		Sleep(1000 * UnitTime);						 // ��ͣһ��ʱ�䣬ģ�ⵥλʱ�������
	}

	// ģ���������ʾ�û�����������˳�
	GotOxy(60, 30);									 // ���ù��λ��
	printf("��ʾ����������������˳�");				 // ��ʾ��Ϣ
	getchar();										 // �ȴ��û����������
	getchar();										 // �ȴ��û�������������ڶ��Σ���ȷ�����յ����룩
}

// �Ե��ݽ��п���
void ControlElevator(Elevatorcanshu*& Elevator, int e)
{
	int flag, i;

	// ������ݴ��ڿ���״̬
	if (Elevator[e].state == Idle) {
		// ������ݵ���һ��״̬Ҳ�ǿ���
		if (Elevator[e].prestate == Idle) {
			Elevator[e].prestate = getOrder(Elevator, e);  // ��ȡ���ݵ�ǰλ��
		}

		// ������ݵ������б�־��Ϊ 0���ҵ��ݵ�ǰ¥���к���������û���˳���
		if (Elevator[e].d1 == 0 && Elevator[e].d3 == 0 && Elevator[e].CallDianti[Elevator[e].Floor] == 2) {
			Elevator[e].CallDianti[Elevator[e].Floor] = 0;  // ���ú�������
			flag = 0;
			// ���ϼ���Ƿ�����������
			for (i = Elevator[e].Floor + 1; i <= Maxfloor; i++) {
				if (Elevator[e].CallDown[i] || Elevator[e].CallDianti[i] || Elevator[e].CallUp[i]) {
					flag = 1;  // ��������󣬱��Ϊ 1
					break;
				}
			}

			// ���¼���Ƿ�����������
			for (i = Elevator[e].Floor - 1; i >= 0; i--) {
				if (Elevator[e].CallDown[i] || Elevator[e].CallDianti[i] || Elevator[e].CallUp[i]) {
					flag = 1;  // ��������󣬱��Ϊ 1
					break;
				}
			}

			// ���û�������������������ݵ���Ϣ
			if (flag) {
				return;
			}
			GotOxy(3 + 30 * Elevator[e].k, 4);  // ��λ���λ��
			printf("                        ");
			GotOxy(3 + 30 * Elevator[e].k, 4);  // ���������Ϣ
			printf("�����������%d", Elevator[e].k);  // �������������Ϣ

			// ���ݵ��ݱ�Ÿ�����ʾ����
			if (Elevator[e].k) {
				strcpy(ElevatorChar[1], "�����������1");
			}
			else {
				strcpy(ElevatorChar[0], "�����������0");
			}
			return;
		}

		// �������������״̬���ҵ�ǰ¥�������������ҵ�������δ��
		if (!Elevator[e].d1 && !Elevator[e].d3 && Elevator[e].prestate == Goingup && ((Elevator[e].CallUp[Elevator[e].Floor] && Elevator[e].PeopleNum < MaxPeopleNum) || Elevator[e].CallDianti[Elevator[e].Floor]))
		{
			Elevator[e].d1 = 1;  // ���õ���Ϊ������
			Elevator[e].d2 = 1;  // ���õ���Ϊ�����ж�

			// ������ݹ��ű���ϣ���ʾ�����Ϣ�����������õ���״̬
			if (Elevator[e].drcl) {
				Elevator[e].drcl = 0;  // ���ô�ϱ�־
				GotOxy(3 + 30 * Elevator[e].k, 4);  // ��λ���
				printf("                        ");
				GotOxy(3 + 30 * Elevator[e].k, 4);  // ���������Ϣ
				printf("����%d����Ϲ���", Elevator[e].k);  // ��������Ϣ

				// ���µ���״̬��ʾ
				if (Elevator[e].k) {
					strcpy(ElevatorChar[1], "����1����Ϲ���");
				}
				else {
					strcpy(ElevatorChar[0], "����0����Ϲ���");
				}

				// �����Ų�����ӵ����ȶ���
				AddTimeSequence(Time - Elevator[e].drcltime, NULL, OpenDoor, Elevator, e);
			}
			else
			{
				// ��������������
				AddTimeSequence(Doortime, NULL, OpenDoor, Elevator, e);
			}
			return;
		}

		// �������������״̬���ҵ�ǰ¥�������������ҵ�������δ��
		else if (!Elevator[e].d1 && !Elevator[e].d3 && Elevator[e].prestate == Goingdown && ((Elevator[e].CallDown[Elevator[e].Floor] && Elevator[e].PeopleNum < MaxPeopleNum) || Elevator[e].CallDianti[Elevator[e].Floor]))
		{
			Elevator[e].d1 = 1;  // ���õ���Ϊ������
			Elevator[e].d2 = 1;  // ���õ���Ϊ�����ж�

			// ������ݹ��ű���ϣ���ʾ�����Ϣ�����������õ���״̬
			if (Elevator[e].drcl) {
				Elevator[e].drcl = 0;  // ���ô�ϱ�־
				GotOxy(3 + 30 * Elevator[e].k, 4);  // ��λ���
				printf("                        ");
				GotOxy(3 + 30 * Elevator[e].k, 4);  // ���������Ϣ
				printf("����%d����Ϲ���", Elevator[e].k);  // ��������Ϣ

				// ���µ���״̬��ʾ
				if (Elevator[e].k) {
					strcpy(ElevatorChar[1], "����1����Ϲ���");
				}
				else {
					strcpy(ElevatorChar[0], "����0����Ϲ���");
				}

				// �����Ų�����ӵ����ȶ���
				AddTimeSequence(Time - Elevator[e].drcltime, NULL, OpenDoor, Elevator, e);
			}
			else
			{
				// ��������������
				AddTimeSequence(Doortime, NULL, OpenDoor, Elevator, e);
			}
			return;
		}

		// ��������������У�����δ�ر����г˿ͽ���
		else if (Elevator[e].d3 == 1 && Elevator[e].prestate == Goingup && (Elevator[e].CallUp[Elevator[e].Floor] && Elevator[e].PeopleNum < MaxPeopleNum)) {
			Elevator[e].d1 = 1;		// ���õ���Ϊ������
			Elevator[e].d3 = 0;		// ������δ�رձ�־
			AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);  // ��ӳ˿ͽ����ݵĲ��������ȶ���
		}

		// ��������������У�����δ�ر����г˿ͽ���
		else if (Elevator[e].d3 == 1 && Elevator[e].prestate == Goingdown && (Elevator[e].CallDown[Elevator[e].Floor] && Elevator[e].PeopleNum < MaxPeopleNum)) {
			Elevator[e].d1 = 1;		// ���õ���Ϊ������
			Elevator[e].d3 = 0;		// ������δ�رձ�־
			AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);  // ��ӳ˿ͽ����ݵĲ��������ȶ���
		}

		// ������ݴ��ڿ���״̬����û�к���������û���ڹ��Ų������ҵ�����������
		else if (Elevator[e].d1 == 0 && Elevator[e].d3 == 0 && Elevator[e].drcl == 0 && Elevator[e].prestate == Goingup && !((Elevator[e].CallUp[Elevator[e].Floor] && Elevator[e].PeopleNum < MaxPeopleNum) || Elevator[e].CallDianti[Elevator[e].Floor]))
		{
			int temp = GetUp(Elevator, e);  // ��ȡ����״̬
			if (temp == 1) {
				Elevator[e].state = Goingup;  // ���õ���Ϊ����״̬
				AddTimeSequence(Accelerate, NULL, UniformSpeed, Elevator, e);  // ��Ӽ��ٲ��������ȶ���
			}
			else {
				Elevator[e].prestate = Idle;  // ���õ���Ϊ����״̬
				Elevator[e].state = Idle;  // ���õ���Ϊ����״̬
				Elevator[e].again = 1;  // ���õ�����Ҫ�ٴ�ִ�в���
			}
		}

		// ������ݴ��ڿ���״̬����û�к���������û���ڹ��Ų������ҵ�����������
		else if (Elevator[e].d1 == 0 && Elevator[e].d3 == 0 && Elevator[e].drcl == 0 && Elevator[e].prestate == Goingdown && !((Elevator[e].CallDown[Elevator[e].Floor] && Elevator[e].PeopleNum < MaxPeopleNum) || Elevator[e].CallDianti[Elevator[e].Floor]))
		{
			int temp = GetDown(Elevator, e);  // ��ȡ����״̬
			if (temp == 1) {
				Elevator[e].state = Goingdown;  // ���õ���Ϊ����״̬
				AddTimeSequence(Accelerate, NULL, UniformSpeed, Elevator, e);  // ��Ӽ��ٲ��������ȶ���
			}
			else {
				Elevator[e].prestate = Idle;  // ���õ���Ϊ����״̬
				Elevator[e].state = Idle;  // ���õ���Ϊ����״̬
				Elevator[e].again = 1;  // ���õ�����Ҫ�ٴ�ִ�в���
			}
		}
		// �����������ִ�����������򷵻ص�ԭʼλ��
		else if (!Elevator[e].d1 && !Elevator[e].d3 && Elevator[e].drcl == 0 && Elevator[e].d2) {
			Elevator[e].d2 = 0;  // �ر�ִ�������־
			Elevator[e].backfirsttime = Time;  // ���÷���ʱ��
			AddTimeSequence(Backtime, NULL, GoFirstFloor, Elevator, e);  // ��ӷ��ز��������ȶ���
		}
	}
}

//���ݻ�ȡ�²�ָ�� 
int getOrder(Elevatorcanshu*& Elevator, int e)
{
	int timeUp = -1, timeDown = -1;

	// ��鵱ǰ¥���Ƿ�����������
	if (upqueue[Elevator[e].Floor].next != NULL && Elevator[e].CallUp[Elevator[e].Floor]) {
		timeUp = upqueue[Elevator[e].Floor].next->EnterTime;
	}

	// ��鵱ǰ¥���Ƿ�����������
	if (downqueue[Elevator[e].Floor].next != NULL && Elevator[e].CallDown[Elevator[e].Floor]) {
		timeDown = downqueue[Elevator[e].Floor].next->EnterTime;
	}

	// ����������л���������
	if (timeUp != -1 || timeDown != -1) {
		if (timeUp != -1 && timeDown != -1) {
			return timeUp < timeDown ? Goingup : Goingdown;  // ����ʱ����������
		}
		else if (timeUp != -1) {
			return Goingup;  // ֻ����������
		}
		else if (timeDown != -1) {
			return Goingdown;  // ֻ����������
		}
	}
	else {  // �����ǰ¥��û������
		timeUp = Maxtime + 1; 
		timeDown = Maxtime + 1;

		// ɨ�赱ǰ¥��֮�ϵ�¥�㣬���Ƿ��������������������
		for (int i = Elevator[e].Floor + 1; i <= Maxfloor; i++) {
			if (upqueue[i].next != NULL && Elevator[e].CallUp[i]) {
				timeUp = min(timeUp, upqueue[i].next->EnterTime);
			}
			if (downqueue[i].next != NULL && Elevator[e].CallDown[i]) {
				timeUp = min(timeUp, downqueue[i].next->EnterTime);
			}
		}

		// ɨ�赱ǰ¥��֮�µ�¥�㣬���Ƿ��������������������
		for (int i = Elevator[e].Floor - 1; i >= 0; i--) {
			if (upqueue[i].next != NULL && Elevator[e].CallUp[i]) {
				timeDown = min(timeDown, upqueue[i].next->EnterTime);
			}
			if (downqueue[i].next != NULL && Elevator[e].CallDown[i]) {
				timeDown = min(timeDown, downqueue[i].next->EnterTime);
			}
		}

		// ������κ�����ѡ��ʱ������ķ���
		if (timeUp != Maxtime + 1 || timeDown != Maxtime + 1) {
			if (timeUp != Maxtime + 1 && timeDown != Maxtime + 1) {
				return timeUp < timeDown ? Goingup : Goingdown;  // ����ʱ����������
			}
			else if (timeUp != Maxtime + 1) {
				return Goingup;  // ֻ����������
			}
			else return Goingdown;  // ֻ����������
		}
		else {
			// ���û���κ����󣬿������Ƿ���ȥ����¥�������
			if (Elevator[e].CallDianti[Basefloor] == 2 && Basefloor > Elevator[e].Floor)
			{
				return Goingup;  // ����¥���������ҵ���������
			}
			else if (Elevator[e].CallDianti[Basefloor] == 2 && Basefloor < Elevator[e].Floor)
			{
				return Goingdown;  // ����¥���������ҵ������½�
			}
			return Idle;  // ���û���κ����󣬵���ͣ��
		}
	}
}

//�򿪵����� 
void OpenDoor(People p, Elevatorcanshu*& Elevator, int e)
{
	// �ڵ��ݵ���ʾλ�ô�ӡ"�����ſ���"
	GotOxy(3 + 30 * Elevator[e].k, 4);  // ��λ��굽������ʾ����
	printf("                        ");  // ���ԭ�е���ʾ
	GotOxy(3 + 30 * Elevator[e].k, 4);  // ��λ���
	printf("����%d�ſ���", Elevator[e].k);  // ��ӡ�����ſ��˵���Ϣ

	// ���µ���״̬��ʾ�ַ�����
	if (Elevator[e].k) {
		strcpy(ElevatorChar[1], "����1�ſ���");  // ����ǵ���1��������ʾ
	}
	else {
		strcpy(ElevatorChar[0], "����0�ſ���");  // ����ǵ���0��������ʾ
	}

	// �����ǰ¥���еȺ�ĳ˿ͣ�ִ����Ӧ�ĳ˿ͽ�������
	if (Elevator[e].Stack[Elevator[e].Floor].next) {  // �����ǰ¥���г˿���Ҫ�µ���
		AddTimeSequence(Peopletime, NULL, PeopleOut, Elevator, e);  // �����³�����
	}
	else if (Elevator[e].prestate == Goingup && upqueue[Elevator[e].Floor].next && Elevator[e].PeopleNum < MaxPeopleNum) {
		// ��������������У��Ҹ�¥�������������ҵ������п�λ
		AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);  // �����ϳ�����
	}
	else if (Elevator[e].prestate == Goingdown && downqueue[Elevator[e].Floor].next && Elevator[e].PeopleNum < MaxPeopleNum) {
		// ��������������У��Ҹ�¥�������������ҵ������п�λ
		AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);  // �����³�����
	}
	else {
		// ���������û���˽������ҵ�ǰ¥��û������������
		if (Elevator[e].PeopleNum < MaxPeopleNum) {
			Elevator[e].CallUp[Elevator[e].Floor] = 0;  // �����¥�����������
		}
		Elevator[e].CallDown[Elevator[e].Floor] = 0;  // �����¥�����������
		Elevator[e].CallDianti[Elevator[e].Floor] = 0;  // �����¥��ĵ���������

		// ���õ���״̬���ſ��ŵ�û���˽���
		Elevator[e].d1 = 0;
		Elevator[e].d3 = 1;   //û�˽����������ǿ��ŵ�
		Elevator[e].testdoortime = Time; // ��¼�Ŵ�ʱ��
		AddTimeSequence(Diantitime, NULL, InOut, Elevator, e);  // ���ò��Խ����������ȴ�һ��ʱ��
	}
}

//�˿ͽ������ 
void PeopleIn(People p, Elevatorcanshu*& Elevator, int e)
{
	int a, num, i;

	// ���������������
	if (Elevator[e].prestate == Goingup) {
		People ap = upqueue[Elevator[e].Floor].next;  // ��ȡ������������еĵ�һ���˿�
		if (ap && Elevator[e].PeopleNum < MaxPeopleNum) {  // ����г˿��ҵ���δ��
			// ��ʾ�˿ͽ�����ݵ���Ϣ
			DisappearEvelatorBody(Elevator[e].Floor, Elevator, e);
			DisappearWaiting(Elevator[e].Floor);
			GotOxy(3 + 30 * Elevator[e].k, 6);
			printf("                           ");
			GotOxy(3 + 30 * Elevator[e].k, 7);
			printf("                           ");
			GotOxy(3 + 30 * Elevator[e].k, 6);
			printf("�˿�%d�ɹ��������%d\n", ap->Id, Elevator[e].k);
			a = ap->Id;

			// ����˿�ID��ʾ��ʽ
			num = 0;
			strcpy(PeopleChar6[Elevator[e].k], "�˿�");
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
				strcat(PeopleChar6[Elevator[e].k], "�ɹ��������1");
			}
			else {
				strcat(PeopleChar6[Elevator[e].k], "�ɹ��������0");
			}
			strcpy(PeopleChar7[Elevator[e].k], " ");

			// ���µ��ݳ˿��������Ƴ��ó˿Ͳ�������ӵ�Ŀ��¥��ĵ���ջ��
			Elevator[e].PeopleNum++;
			Exist[ap->Id] = 0;
			upqueue[Elevator[e].Floor].next = ap->next;

			// ���˿ͼ���Ŀ��¥��ĵ���ջ��
			Dianti pr = (Dianti)malloc(sizeof(struct ElevatorStop));
			int i = ap->OutFloor;
			Elevator[e].CallDianti[i] = 1;
			pr->next = Elevator[e].Stack[i].next;
			Elevator[e].Stack[i].next = pr;
			pr->Id = ap->Id;

			// ���µ��ݺ͵ȴ�������ʾ
			ShowElevator(Elevator[e].Floor, Elevator, e);
			ShowWaitingQueue(Elevator[e].Floor);

			// �ͷ��Ѵ���ĳ˿�
			free(ap);
		}

		// ��������������ҵ������п�λ������������һ���˿ͽ���
		if (upqueue[Elevator[e].Floor].next && Elevator[e].PeopleNum < MaxPeopleNum)
			AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);
		else {
			// ������ж������еȴ��˿��ҵ���������
			if (upqueue[Elevator[e].Floor].next) {
				People t = upqueue[Elevator[e].Floor].next;
				GotOxy(3 + 30 * Elevator[e].k, 4);
				printf("                  ");
				GotOxy(3 + 30 * Elevator[e].k, 4);
				printf("����%d������", Elevator[e].k);
				if (Elevator[e].k) {
					strcpy(ElevatorChar[1], "����1������");
				}
				else {
					strcpy(ElevatorChar[0], "����0������");
				}
				GotOxy(3 + 30 * Elevator[e].k, 6);
				printf("                           ");
				GotOxy(3 + 30 * Elevator[e].k, 7);
				printf("                           ");
				GotOxy(3 + 30 * Elevator[e].k, 6);

				// ��ʾ�޷�������ݵĳ˿���Ϣ
				while (t) {
					printf("�˿�%d��", t->Id);
					a = t->Id;
					num = 0;
					strcpy(PeopleChar6[Elevator[e].k], "�˿�");
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
				printf("�޷������������%d", Elevator[e].k);
				if (Elevator[e].k) {
					strcpy(PeopleChar7[Elevator[e].k], "�޷������������1");
				}
				else {
					strcpy(PeopleChar7[Elevator[e].k], "�޷������������0");
				}
			}

			// �����¥��û���������������������
			if (!upqueue[Elevator[e].Floor].next) {
				Elevator[e].CallUp[Elevator[e].Floor] = 0;
			}
			Elevator[e].CallDianti[Elevator[e].Floor] = 0; // �������������
			Elevator[e].d1 = 0;   // �趨��Ϊ��״̬
			Elevator[e].d3 = 1;  // ����Ȼ����
			Elevator[e].testdoortime = Time;  // ��¼�ſ�ʱ��
			AddTimeSequence(Diantitime, NULL, InOut, Elevator, e);  // �����Ų����ȴ�ʱ��
		}
	}

	// ��������������У��������Ʋ���
	else if (Elevator[e].prestate == Goingdown && Elevator[e].PeopleNum < MaxPeopleNum) {
		People ap = downqueue[Elevator[e].Floor].next;
		if (ap && Elevator[e].PeopleNum < MaxPeopleNum) { // ����г˿��ҵ���δ��
			// ��ʾ�˿ͽ�����ݵ���Ϣ
			DisappearEvelatorBody(Elevator[e].Floor, Elevator, e);
			DisappearWaiting(Elevator[e].Floor);
			GotOxy(3 + 30 * Elevator[e].k, 6);
			printf("                           ");
			GotOxy(3 + 30 * Elevator[e].k, 7);
			printf("                           ");
			GotOxy(3 + 30 * Elevator[e].k, 6);
			printf("�˿�%d�ɹ��������%d\n", ap->Id, Elevator[e].k);

			// ����˿�ID��ʾ��ʽ
			a = ap->Id;
			num = 0;
			strcpy(PeopleChar6[Elevator[e].k], "�˿�");
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
				strcat(PeopleChar6[Elevator[e].k], "�ɹ��������1");
			}
			else {
				strcat(PeopleChar6[Elevator[e].k], "�ɹ��������0");
			}
			strcpy(PeopleChar7[Elevator[e].k], " ");

			// ���µ��ݳ˿��������Ƴ��ó˿Ͳ�������ӵ�Ŀ��¥��ĵ���ջ��
			Elevator[e].PeopleNum++;
			Exist[ap->Id] = 0;
			downqueue[Elevator[e].Floor].next = ap->next;

			// ���˿ͼ���Ŀ��¥��ĵ���ջ��
			Dianti pr = (Dianti)malloc(sizeof(struct ElevatorStop));
			int i = ap->OutFloor;
			Elevator[e].CallDianti[i] = 1;
			pr->next = Elevator[e].Stack[i].next;
			Elevator[e].Stack[i].next = pr;
			pr->Id = ap->Id;

			// ���µ��ݺ͵ȴ�������ʾ
			ShowElevator(Elevator[e].Floor, Elevator, e);
			ShowWaitingQueue(Elevator[e].Floor);

			// �ͷ��Ѵ���ĳ˿�
			free(ap);
		}

		// ��������������ҵ������п�λ������������һ���˿ͽ���
		if (downqueue[Elevator[e].Floor].next && Elevator[e].PeopleNum < MaxPeopleNum)
			AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);
		else {
			// ������ж������еȴ��˿��ҵ���������
			if (downqueue[Elevator[e].Floor].next) {
				People t = downqueue[Elevator[e].Floor].next;
				GotOxy(3 + 30 * Elevator[e].k, 4);
				printf("                        ");
				GotOxy(3 + 30 * Elevator[e].k, 4);
				printf("����%d������", Elevator[e].k);
				if (Elevator[e].k) {
					strcpy(ElevatorChar[1], "����1������");
				}
				else {
					strcpy(ElevatorChar[0], "����0������");
				}
				GotOxy(3 + 30 * Elevator[e].k, 6);
				printf("                           ");
				GotOxy(3 + 30 * Elevator[e].k, 7);
				printf("                           ");
				GotOxy(3 + 30 * Elevator[e].k, 6);

				// ��ʾ�޷�������ݵĳ˿���Ϣ
				while (t) {
					printf("�˿�%d��", t->Id);
					a = t->Id;
					num = 0;
					strcpy(PeopleChar6[Elevator[e].k], "�˿�");
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
				printf("�޷������������%d", Elevator[e].k);
				if (Elevator[e].k) {
					strcpy(PeopleChar7[Elevator[e].k], "�޷������������1");
				}
				else {
					strcpy(PeopleChar7[Elevator[e].k], "�޷������������0");
				}
			}
			// �����¥��û���������������������
			if (!downqueue[Elevator[e].Floor].next) {
				Elevator[e].CallDown[Elevator[e].Floor] = 0;
			}
			Elevator[e].CallDianti[Elevator[e].Floor] = 0;  // �������������
			Elevator[e].d1 = 0;   // �趨��Ϊ��״̬
			Elevator[e].d3 = 1;  // ����Ȼ����
			Elevator[e].testdoortime = Time;  // ��¼�ſ�ʱ��
			AddTimeSequence(Diantitime, NULL, InOut, Elevator, e);  // �����Ų����ȴ�ʱ��
		}
	}
}

//�˿��߳����� 
void PeopleOut(People p, Elevatorcanshu*& Elevator, int e)
{
	int a, num, i;

	// �������ջ���г˿ͣ�����ǰ¥���еȴ��³��ĳ˿ͣ�
	if (Elevator[e].Stack[Elevator[e].Floor].next) {
		// �Ƴ������ڵĳ˿�
		DisappearEvelatorBody(Elevator[e].Floor, Elevator, e);// ��ʧ���������
		Dianti ap = Elevator[e].Stack[Elevator[e].Floor].next; // ��ȡջ�еĵ�һ���˿�
		Elevator[e].Stack[Elevator[e].Floor].next = ap->next; // ���ó˿ʹ�ջ���Ƴ�
		Elevator[e].PeopleNum--;  // �����ڳ˿�������1

		// ���µ�����ʾ
		ShowElevator(Elevator[e].Floor, Elevator, e);
		GotOxy(3 + 30 * Elevator[e].k, 6);
		printf("                           ");
		GotOxy(3 + 30 * Elevator[e].k, 7);
		printf("                           ");
		GotOxy(3 + 30 * Elevator[e].k, 6);

		// ��ʾ�˿��߳����ݵ���Ϣ
		printf("�˿�%d�߳�����%d", ap->Id, Elevator[e].k);
		a = ap->Id;
		num = 0;
		strcpy(PeopleChar6[Elevator[e].k], "�˿�");
		for (i = 0; PeopleChar6[Elevator[e].k][i] != '\0'; i++) {} // ����ID���ַ�����

		// ����ID����ʾ
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
			strcat(PeopleChar6[Elevator[e].k], "�߳�����1");
		}
		else {
			strcat(PeopleChar6[Elevator[e].k], "�߳�����0");
		}
		strcpy(PeopleChar7[Elevator[e].k], " ");  // �����ó˿���ص���Ϣ

		// �ͷ��Ѵ���ĳ˿�
		free(ap);
	}

	// �����¥�㻹�еȴ��ĳ˿ͣ���������
	if (Elevator[e].Stack[Elevator[e].Floor].next) {
		AddTimeSequence(Peopletime, NULL, PeopleOut, Elevator, e);
	}

	// ��������������в��Ҹ�¥������������ĳ˿�
	else if (Elevator[e].prestate == Goingup && upqueue[Elevator[e].Floor].next) {
		AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);
	}

	// ��������������в��Ҹ�¥������������ĳ˿�
	else if (Elevator[e].prestate == Goingdown && downqueue[Elevator[e].Floor].next) {
		AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);
	}
	else {
		// ��������������У��Ҹ�¥��û���������󣬼�����¥���Ƿ�������
		if (Elevator[e].prestate == Goingup) {
			Elevator[e].CallUp[Elevator[e].Floor] = 0;
			int flag = 0;
			for (int i = Elevator[e].Floor + 1; i <= Maxfloor; i++) {
				if (Elevator[e].CallDianti[i] || Elevator[e].CallUp[i] || Elevator[e].CallDown[i]) {
					flag = 1;  // ���ָ���¥��������
					break;
				}
			}
			// ���û����������¥�����г˿ͣ���ʼ����
			if (flag == 0) {
				if (downqueue[Elevator[e].Floor].next != NULL) {
					Elevator[e].prestate = Goingdown;
					AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);
					return;
				}
			}
		}
		// ��������������У��Ҹ�¥��û���������󣬼�����¥���Ƿ�������
		else if (Elevator[e].prestate == Goingdown) {
			Elevator[e].CallDown[Elevator[e].Floor] = 0;
			int flag = 0;
			for (int i = Elevator[e].Floor - 1; i >= 0; i--) {
				if (Elevator[e].CallDianti[i] || Elevator[e].CallUp[i] || Elevator[e].CallDown[i]) {
					flag = 1;  // ���ָ���¥��������
					break;
				}
			}
			// ���û����������¥�����г˿ͣ���ʼ����
			if (flag == 0) {
				if (upqueue[Elevator[e].Floor].next != NULL) {
					Elevator[e].prestate = Goingup;
					AddTimeSequence(Peopletime, NULL, PeopleIn, Elevator, e);
					return;
				}
			}
		}
		// ���û���κγ˿ͽ������ҵ��ݵ����Ѿ���
		Elevator[e].CallDianti[Elevator[e].Floor] = 0;
		Elevator[e].d1 = 0; // ���õ����Źر�
		Elevator[e].d3 = 1; // �����ű��ִ�״̬
		Elevator[e].testdoortime = Time;  // ��¼�����Ŵ򿪵�ʱ��
		AddTimeSequence(Diantitime, NULL, InOut, Elevator, e);  // ִ�еȴ�ʱ�䣬�����Ų���
	}
}

//�����Ƿ����� 
int GetUp(Elevatorcanshu*& Elevator, int e)
{
	// �ӵ�ǰ¥����Ϸ���ʼ������Ƿ������������������������ٻ�����
	for (int i = Elevator[e].Floor + 1; i <= Maxfloor; i++) {
		// ���ĳ��¥�������С����л�����ٻ�����
		if (Elevator[e].CallDianti[i] || Elevator[e].CallUp[i] || Elevator[e].CallDown[i])
			return 1; // ����1����ʾ������Ҫ����
	}
	return 0;  // ���û���κ����󣬷���0����ʾ���ݲ���Ҫ����
}

//�����Ƿ��½� 
int GetDown(Elevatorcanshu*& Elevator, int e)
{
	// �ӵ�ǰ¥����·���ʼ������Ƿ������������������������ٻ�����
	for (int i = Elevator[e].Floor - 1; i >= 0; i--) {
		// ���ĳ��¥�������С����л�����ٻ�����
		if (Elevator[e].CallDianti[i] || Elevator[e].CallUp[i] || Elevator[e].CallDown[i])
			return 1;  // ����1����ʾ������Ҫ�½�
	}
	return 0;  // ���û���κ����󣬷���0����ʾ���ݲ���Ҫ�½�
}

//���ݼ�����ɣ������������ٶ� 
void UniformSpeed(People ap, Elevatorcanshu*& Elevator, int e)
{
	// �жϵ��ݵ�״̬
	if (Elevator[e].state == Goingup) {
		// ���֮ǰ��ʾ������
		GotOxy(3 + 30 * Elevator[e].k, 4);
		printf("                        ");

		// ������ݼ�����������Ϣ
		GotOxy(3 + 30 * Elevator[e].k, 4);
		printf("����%d�Ѽ���������", Elevator[e].k);

		// ���ݵ��ݵı�Ÿ�����ʾ����Ϣ
		if (Elevator[e].k) {
			strcpy(ElevatorChar[1], "����1�Ѽ���������");
		}
		else {
			strcpy(ElevatorChar[0], "����0�Ѽ���������");
		}

		// ��¼��������״̬���¼�
		AddTimeSequence(Uptime, NULL, ElevatorUp, Elevator, e);
	}
	else {
		// ���֮ǰ��ʾ������
		GotOxy(3 + 30 * Elevator[e].k, 4);
		printf("                        ");

		// ������ݼ����½�����Ϣ
		GotOxy(3 + 30 * Elevator[e].k, 4);
		printf("����%d�Ѽ����½���", Elevator[e].k);

		// ���ݵ��ݵı�Ÿ�����ʾ����Ϣ
		if (Elevator[e].k) {
			strcpy(ElevatorChar[1], "����1�Ѽ����½���");
		}
		else {
			strcpy(ElevatorChar[0], "����0�Ѽ����½���");
		}

		// ��¼�����½�״̬���¼�
		AddTimeSequence(Downtime, NULL, ElevatorDown, Elevator, e);
	}
}

//����ȥ����
void GoFirstFloor(People ap, Elevatorcanshu*& Elevator, int e)
{
	// �жϵ����Ƿ��ڿ���״̬���ҵ��ݷ��ػ������ʱ���Ƿ��Ѿ���ȥ
	if (Elevator[e].state != Idle || Elevator[e].backfirsttime + Backtime != Time)
		return;

	// ��������Ѿ��ڻ���¥�㣬����û�����ڵȴ��������
	if (Elevator[e].Floor == Basefloor && Elevator[e].d2 == 0) {
		Elevator[e].CallDianti[Basefloor] = 2; // ���õ��ݵ��������ı�־
		return;
	}
	// ��������Ѿ��ڻ���¥�㣬ֱ�ӷ���
	else if (Elevator[e].Floor == Basefloor)
		return;

	// ���ݵ�ǰ���ڻ���¥�㣬��ʾ��ʾ��Ϣ����ʾ���Զ����ػ���¥��
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("                         ");
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("�����������%d!������%d��", Elevator[e].k, Basefloor);

	// ���µ�����Ϣ�ַ���
	if (Elevator[e].k) {
		strcpy(ElevatorChar[1], "�����������1!������1��");
	}
	else {
		strcpy(ElevatorChar[0], "�����������0!������1��");
	}

	// ���õ��ݵ������¥��ı�־
	Elevator[e].CallDianti[Basefloor] = 2;
}

// �����˿ͽ��������ڹ���
void InOut(People p, Elevatorcanshu*& Elevator, int e)
{
	// ���е��ݵĴ���
	if (Elevator[e].prestate == Goingup) {
		// �����ǰ�г˿�Ҫ�������ݣ����ߵ����Ѿ��ﵽ���˿��������ߵ���û����ɹ��ŵ�ʱ��Ҫ��
		if ((upqueue[Elevator[e].Floor].next && Elevator[e].PeopleNum < MaxPeopleNum) || Elevator[e].Stack[Elevator[e].Floor].next || Elevator[e].testdoortime + Diantitime != Time)
			return;

		// ������ж���û�����󣬽���¥��ĺ����źŹر�
		if (!upqueue[Elevator[e].Floor].next) {
			Elevator[e].CallUp[Elevator[e].Floor] = 0;
		}

		// �رյ��ݵ����º����źţ�����ǵ���״̬
		Elevator[e].CallDianti[Elevator[e].Floor] = 0;
		Elevator[e].d1 = 0; Elevator[e].d3 = 0;
		Elevator[e].drcl = 1; Elevator[e].drcltime = Time;

		// ��ʾ���ݹ�����ʾ��Ϣ
		GotOxy(3 + 30 * Elevator[e].k, 4);
		printf("                        ");
		GotOxy(3 + 30 * Elevator[e].k, 4);
		printf("����%d���ڹ���", Elevator[e].k);

		// ���µ���״̬����
		if (Elevator[e].k) {
			strcpy(ElevatorChar[1], "����1���ڹ���");
		}
		else {
			strcpy(ElevatorChar[0], "����0���ڹ���");
		}

		// ��ӹ�������
		AddTimeSequence(Doortime, NULL, CloseElevator, Elevator, e);
	}

	// ���е��ݵĴ���
	else if (Elevator[e].prestate == Goingdown) {
		// �����ǰ�г˿�Ҫ�������ݣ����ߵ����Ѿ��ﵽ���˿��������ߵ���û����ɹ��ŵ�ʱ��Ҫ��
		if ((downqueue[Elevator[e].Floor].next && Elevator[e].PeopleNum < MaxPeopleNum) || Elevator[e].Stack[Elevator[e].Floor].next || Elevator[e].testdoortime + Diantitime != Time)
			return;

		// ������ж���û�����󣬽���¥��ĺ����źŹر�
		if (!downqueue[Elevator[e].Floor].next) {
			Elevator[e].CallDown[Elevator[e].Floor] = 0;
		}

		// �رյ��ݵ����º����źţ�����ǵ���״̬
		Elevator[e].CallDianti[Elevator[e].Floor] = 0;
		Elevator[e].d1 = 0; Elevator[e].d3 = 0;
		Elevator[e].drcl = 1; Elevator[e].drcltime = Time;
		GotOxy(3 + 30 * Elevator[e].k, 4);
		printf("                        ");

		// ��ʾ���ݹ�����ʾ��Ϣ
		GotOxy(3 + 30 * Elevator[e].k, 4);
		printf("����%d���ڹ���", Elevator[e].k);

		// ���µ���״̬����
		if (Elevator[e].k) {
			strcpy(ElevatorChar[1], "����1���ڹ���");
		}
		else {
			strcpy(ElevatorChar[0], "����0���ڹ���");
		}

		// ��ӹ�������
		AddTimeSequence(Doortime, NULL, CloseElevator, Elevator, e);
	}
}

//��������
void ElevatorUp(People ap, Elevatorcanshu*& Elevator, int e)
{
	// ����¥�����ӣ�������
	Elevator[e].Floor++;

	// ������ʾ����������״̬
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("                        ");
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("����%d��������", Elevator[e].k);

	// ���õ�������������
	if (Elevator[e].k) {
		strcpy(ElevatorChar[1], "����1��������");
	}
	else {
		strcpy(ElevatorChar[0], "����0��������");
	}

	// ������һ��¥��ĵ��ݲ���ʾ��ǰ¥��ĵ���
	DisappearEvelatorBody(Elevator[e].Floor - 1, Elevator, e);
	ShowElevator(Elevator[e].Floor, Elevator, e);

	// �жϵ����Ƿ���Ҫͣ����
	if (Elevator[e].prestate == Goingup && (Elevator[e].CallUp[Elevator[e].Floor] || Elevator[e].CallDianti[Elevator[e].Floor] || Elevator[e].CallDown[Elevator[e].Floor])) {
		// ������������������󣬼���Ƿ���Ҫͣ����
		if (Elevator[e].CallUp[Elevator[e].Floor] || Elevator[e].CallDianti[Elevator[e].Floor])
			AddTimeSequence(UpSlowdown, NULL, ElevatorStop, Elevator, e); // ֹͣ����
		else {
			int flag = 0;
			// ���δ��¥���Ƿ�����������
			for (int i = Elevator[e].Floor + 1; i <= Maxfloor; i++) {
				if (Elevator[e].CallUp[i] || Elevator[e].CallDianti[i] || Elevator[e].CallDown[i]) {
					flag = 1;
					break;
				}
			}
			if (flag == 0) {
				AddTimeSequence(UpSlowdown, NULL, ElevatorStop, Elevator, e); // ���û������ֹͣ����
			}
			else {
				AddTimeSequence(Uptime, NULL, ElevatorUp, Elevator, e); // �����������
			}
		}
	}
	else if (Elevator[e].prestate == Goingdown && (Elevator[e].CallUp[Elevator[e].Floor] || Elevator[e].CallDianti[Elevator[e].Floor] || Elevator[e].CallDown[Elevator[e].Floor])) {
		// ������������У��ҵ�ǰ¥�������󣬼���Ƿ�ֹͣ����
		if (Elevator[e].CallDown[Elevator[e].Floor] || Elevator[e].CallDianti[Elevator[e].Floor])
			AddTimeSequence(UpSlowdown, NULL, ElevatorStop, Elevator, e); // ֹͣ����
		else {
			int flag = 0;
			// ����ȥ¥���Ƿ�������
			for (int i = Elevator[e].Floor - 1; i >= 0; i--) {
				if (Elevator[e].CallUp[i] || Elevator[e].CallDianti[i] || Elevator[e].CallDown[i]) {
					flag = 1;
					break;
				}
			}
			if (flag == 0)AddTimeSequence(UpSlowdown, NULL, ElevatorStop, Elevator, e); // ���û������ֹͣ����
			else {
				AddTimeSequence(Uptime, NULL, ElevatorUp, Elevator, e); // �����������
			}
		}
	}
	else {
		// ���û�������ҵ���û�е������¥�㣬��������
		if (Elevator[e].Floor == Maxfloor) {
			AddTimeSequence(UpSlowdown, NULL, ElevatorStop, Elevator, e);// ���û�������ҵ���û�е������¥�㣬��������
		}
		else {
			AddTimeSequence(Uptime, NULL, ElevatorUp, Elevator, e); // �����������
		}
	}
}

//�����½� 
void ElevatorDown(People ap, Elevatorcanshu*& Elevator, int e)
{
	// ����¥����٣��½���
	Elevator[e].Floor--;

	// ������ʾ�����½���״̬
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("                        ");
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("����%d�����½�", Elevator[e].k);

	// ���õ����½�������
	if (Elevator[e].k) {
		strcpy(ElevatorChar[1], "����1�����½�");
	}
	else {
		strcpy(ElevatorChar[0], "����0�����½�");
	}

	// ������һ��¥��ĵ��ݲ���ʾ��ǰ¥��ĵ���
	DisappearEvelatorBody(Elevator[e].Floor + 1, Elevator, e);
	ShowElevator(Elevator[e].Floor, Elevator, e);

	// �ж��Ƿ���Ҫֹͣ�½�
	if (Elevator[e].prestate == Goingup && (Elevator[e].CallUp[Elevator[e].Floor] || Elevator[e].CallDianti[Elevator[e].Floor])) {
		// ������������������ҵ�ǰ¥��������ֹͣ�½�
		AddTimeSequence(UpSlowdown, NULL, ElevatorStop, Elevator, e);
	}
	else if (Elevator[e].prestate == Goingdown && (Elevator[e].CallDown[Elevator[e].Floor] || Elevator[e].CallDianti[Elevator[e].Floor])) {
		// ������������½����ҵ�ǰ¥��������ֹͣ�½�
		AddTimeSequence(UpSlowdown, NULL, ElevatorStop, Elevator, e);
	}
	else {
		// �������û�������һ�û�е����¥��������½�
		if (Elevator[e].Floor == 0) {
			AddTimeSequence(DownSlowdown, NULL, ElevatorStop, Elevator, e); // �����¥ʱֹͣ�½�
		}
		else {
			AddTimeSequence(Downtime, NULL, ElevatorDown, Elevator, e); // ��������½�
		}
	}
}

//�����Ź��� 
void CloseElevator(People ap, Elevatorcanshu*& Elevator, int e)
{
	// �жϵ����Ƿ��ѹر��Ż��߹ر��ŵ�ʱ���Ƿ�ﵽ
	if (Elevator[e].drcl == 0 || Time != Elevator[e].drcltime + Doortime)
		return;  // ������Ѿ��رջ���ʱ��δ����ֱ�ӷ���

	// ������״̬���£���ʾ���Ѿ��ر�
	Elevator[e].drcl = 0;

	// �����ǰ��ʾ�ĵ�����״̬��Ϣ
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("                        ");

	// ���µ����Źرյ���Ϣ
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("����%d�Ź���", Elevator[e].k);

	// ���µ���״̬����
	if (Elevator[e].k) {
		strcpy(ElevatorChar[1], "����1�Ź���");
	}
	else {
		strcpy(ElevatorChar[0], "����0�Ź���");
	}
}

//����ͣ�� 
void ElevatorStop(People ap, Elevatorcanshu*& Elevator, int e)
{
	// ���õ���״̬ΪIdle����ʾ����ֹͣ
	Elevator[e].state = Idle;

	// ���õ��ݵ����״̬
	Elevator[e].d1 = 0;
	Elevator[e].d3 = 0;

	// �����ǰ���ݵ�״̬��ʾ��Ϣ
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("                        ");

	// ������ʾ����ͣ�µ�״̬
	GotOxy(3 + 30 * Elevator[e].k, 4);
	printf("����%dͣ�ˣ���ǰ����%d", Elevator[e].k, Elevator[e].Floor);

	// ���ݵ�ǰ¥�㸳ֵ������a
	int a = Elevator[e].Floor, i;

	// ���µ���0�����1��״̬����
	if (Elevator[e].k) {  // ����ǵ���1
		strcpy(ElevatorChar[1], "����1ͣ�ˣ���ǰ����");
		for (i = 0; ElevatorChar[1][i] != '\0'; i++) {  // ����ǰ¥������ת��Ϊ�ַ�
		}
		ElevatorChar[1][i] = '0' + Elevator[e].Floor;
		ElevatorChar[1][i + 1] = '\0';
	}
	else {  // ����ǵ���0
		strcpy(ElevatorChar[0], "����0ͣ�ˣ���ǰ����");
		for (i = 0; ElevatorChar[0][i] != '\0'; i++) {
		}
		ElevatorChar[0][i] = '0' + Elevator[e].Floor;  // ����ǰ¥������ת��Ϊ�ַ�
		ElevatorChar[0][i + 1] = '\0';
	}
}

