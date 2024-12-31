#include <windows.h>
#include<bits/stdc++.h>
#include<string.h> 
#include<stdio.h> 
#include<stdlib.h>
using  namespace std;

#define UnitTime 0.1        //��λʱ�� 
#define Maxpeople 1000000   //ģ��ʱ���˿����� 
#define MaxTime 10000       //�������ʱ������� 
#define MinTime 500         //�������ʱ������� 
#define Maxfloor 4          //��ߵ�¥�� 
#define MaxPeopleNum 8      //�����ڳ�����Ա��������� 
#define MaxWaitNumInFloor 7	//ÿһ������ȴ�����
#define Basefloor 1         //¥�Ļ��� 

#define FloorHight 3		//¥��߶�
#define LeftElevatorX 6		//��ߵ��ݵ�x��ʼ����
#define LeftElevatorY 12	//��ߵ��ݵ�y��ʼ����
#define RightElevatorX 34	//�ұߵ��ݵ�x��ʼ����
#define RightElevatorY 12	//�ұߵ��ݵ�y��ʼ����
#define Uptime 51           //��������ʱ�� 
#define Downtime 61         //�����½�ʱ�� 
#define UpSlowdown 14       //������������ʱ�� 
#define DownSlowdown 23     //�����½�����ʱ�� 
#define Accelerate 15       //���ݼ���ʱ��  
#define Doortime 20         //���ݹ���ʱ�� 
#define Peopletime 25       //�˽������ʱ�� 
#define Diantitime 40       //���ʱ�����ʱ�� 
#define Backtime 300        //����Ƿ�ص����� 

#define Goingup 1           //����״̬���� 
#define Goingdown -1        //����״̬�½� 
#define Idle 0              //����״̬�Ⱥ� 

int Time,id,Maxtime;
int flagelevator=0;         //���˿��Ƿ�ͬʱ����ȫ������ 
int Exist[Maxpeople]={0};   //�û��Ƿ���ڶ�����
char ElevatorChar[2][40];   //������ʾ 
char PeopleChar6[2][40];    //�����г˿���ʾ     
char PeopleChar7[2][40];    //�����г˿���ʾ 
typedef struct ElevatorArgument Elevatorcanshu;
typedef struct people{
    int Id;                //���û��ı��
    int EnterTime;         //����ʱ��
    int InFloor;           //�����ڵ�¥��
    int OutFloor;          //��Ҫȥ�Ĳ�¥
    int GiveupTime;        //�������̵ĵȺ�ʱ��
    int FlagElevator0;     //ֵΪ0���������������0 
    int FlagElevator1;     //ֵΪ0���������������1 
    struct people* pre;
    struct people* next;
}*People;

typedef struct Elevator{
	int Id;
	struct Elevator *next; 
}*Elevator;

typedef struct TimeSequence{
	int xianzhitime;
	People person;
	void(*f)(People per,Elevatorcanshu *&Elevatortemp,int temp);
	struct TimeSequence *next;
}*TimeSequence;

struct ElevatorArgument{
	int PeopleNum;                   //���������� 
	int CallUp[5];                   //���������ϰ�ť
	int CallDown[5];                 //���������°�ť
	int CallDianti[5];               //�����ڰ�ť
	int again;
	int drcl,drcltime,testdoortime; //�����Ų���
	int state;                      //���ݵĵ�ǰ״̬(GoingUp��GoingDOWEl��Idle)
	int prestate;                   //�����²�״̬(GoingUp��GoingDOWEl��Idle)
	int Floor;                      //���ݵĵ�ǰλ��(¥��)
	int d1;                         //ֵΪ0�������������ڽ�����뿪���ݣ�
	int d2;                         //ֵΪ0��������� �Ѿ���ĳ��ͣ��30Ot���ϣ�
	int d3;                         //ֵΪ0�����ǵ��� �������������˱ų����ݣ�
	struct Elevator Stack[Maxfloor+1];//�����ڳ˿�
	struct TimeSequence sxjiankong;        //����ʱ����
	int k;
	int backfirsttime; 
};

struct people upqueue[Maxfloor+1];  //�������� 
struct people downqueue[Maxfloor+1];//�½����� 

void InputMaxTime(); 
void ShowTitle();
void InitElevator(Elevatorcanshu *&Elevator,int e);
void AddNewPerson(People per,Elevatorcanshu *&Elevator,int e);             //����˿� 
void AddUpQueue(People &p,int in);  //������������ 
void AddDownQueue(People &p,int in);//�����½����� 
// ���ʱ�����У�ģ��ĳһʱ�̵ĵ������й���
void AddTimeSequence(int time, People p, void(*fn)(People per, Elevatorcanshu *&Elevatortemp, int temp), Elevatorcanshu *&Elevator, int e);

// �����ȴ�����ʾĳ���˿Ͳ��ٵȴ�����
void GiveupWait(People per, Elevatorcanshu *&Elevator, int e);

// ���е���ģ��
void Simulate();

// ���Ƶ��ݵ����У����ܸ������ȼ���ĳЩ�������ȣ�
void ControlElevator(Elevatorcanshu *&Elevator, int e);

// �򿪵�����
void OpenDoor(People p, Elevatorcanshu *&Elevator, int e);

// �˿ͽ������
void PeopleIn(People p, Elevatorcanshu *&Elevator, int e);

// �˿��뿪����
void PeopleOut(People p, Elevatorcanshu *&Elevator, int e);

// �˿ͽ������ʱ����������������
void UniformSpeed(People ap, Elevatorcanshu *&Elevator, int e);

// �˿�Ҫ��ȥһ¥
void GoFirstFloorFloorFloorFloorFloorFloor(People ap, Elevatorcanshu *&Elevator, int e);

// ����˿͵����µ���
void InOut(People p, Elevatorcanshu *&Elevator, int e);

// ���������ƶ�
void ElevatorDown(People ap, Elevatorcanshu *&Elevator, int e);

// ���������ƶ�
void ElevatorUp(People ap, Elevatorcanshu *&Elevator, int e);

// �رյ�����
void CloseElevator(People ap, Elevatorcanshu *&Elevator, int e);

// ����ͣ��
void ElevatorStop(People ap, Elevatorcanshu *&Elevator, int e);

// ��ȡ���ݵĵ�ǰ���󣨿��������л���������
int getOrder(Elevatorcanshu *&Elevator, int e);

// ��ȡ���ݵ���������
int GetUp(Elevatorcanshu *&Elevator, int e);

// ��ȡ���ݵ���������
int GetDown(Elevatorcanshu *&Elevator, int e);

// ��ȡ�����Ϣ������������ʾĳЩ״̬�����ĳ�ַ���
void GotOxy(short x, short y); 

// ��ʾ������Ϣ�������ǵ���ϵͳ��״̬�������Ϣ
void ShowOthersInfo();

// ��ӡ����״̬��Ϣ
void PrintElevator();

// ������ʧ��Ч���������ǵ����뿪�������ʧ�Ķ���Ч����
void DisappearEvelatorBody(int x, Elevatorcanshu *&Elevator, int e);

// ��ʾ���ݵ�״̬��λ��
void ShowElevator(int x, Elevatorcanshu *&Elevator, int e);

// ��ʾ�ȴ������еĳ˿���Ϣ
void ShowWaitingQueue(int i);

// ��ĳ���˿���ʧ�������ǳ˿��Ѿ��뿪���ٵȴ���
void DisappearWaiting(int i);

// ��ӡ������Ϣ�������û��˽����ʹ�õ���ϵͳ
void PrintHelp();

