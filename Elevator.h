#include <windows.h>
#include<bits/stdc++.h>
#include<string.h> 
#include<stdio.h> 
#include<stdlib.h>
using  namespace std;

#define UnitTime 0.1        //单位时间 
#define Maxpeople 1000000   //模拟时最大乘客数量 
#define MaxTime 10000       //最大运行时间的上限 
#define MinTime 500         //最大运行时间的下限 
#define Maxfloor 4          //最高的楼层 
#define MaxPeopleNum 8      //电梯内乘坐人员的最大数量 
#define MaxWaitNumInFloor 7	//每一层的最大等待人数
#define Basefloor 1         //楼的基层 

#define FloorHight 3		//楼层高度
#define LeftElevatorX 6		//左边电梯的x起始坐标
#define LeftElevatorY 12	//左边电梯的y起始坐标
#define RightElevatorX 34	//右边电梯的x起始坐标
#define RightElevatorY 12	//右边电梯的y起始坐标
#define Uptime 51           //电梯上升时间 
#define Downtime 61         //电梯下降时间 
#define UpSlowdown 14       //电梯上升减速时间 
#define DownSlowdown 23     //电梯下降减速时间 
#define Accelerate 15       //电梯加速时间  
#define Doortime 20         //电梯关门时间 
#define Peopletime 25       //人进入电梯时间 
#define Diantitime 40       //检查时否关门时间 
#define Backtime 300        //检查是否回到基层 

#define Goingup 1           //电梯状态上升 
#define Goingdown -1        //电梯状态下降 
#define Idle 0              //电梯状态等候 

int Time,id,Maxtime;
int flagelevator=0;         //检查乘客是否同时呼叫全部电梯 
int Exist[Maxpeople]={0};   //用户是否存在队列中
char ElevatorChar[2][40];   //电梯提示 
char PeopleChar6[2][40];    //第六行乘客提示     
char PeopleChar7[2][40];    //第七行乘客提示 
typedef struct ElevatorArgument Elevatorcanshu;
typedef struct people{
    int Id;                //该用户的编号
    int EnterTime;         //进入时间
    int InFloor;           //他所在的楼层
    int OutFloor;          //他要去哪层楼
    int GiveupTime;        //他能容忍的等候时间
    int FlagElevator0;     //值为0，除非有请求电梯0 
    int FlagElevator1;     //值为0，除非有请求电梯1 
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
	int PeopleNum;                   //电梯内人数 
	int CallUp[5];                   //电梯外向上按钮
	int CallDown[5];                 //电梯外向下按钮
	int CallDianti[5];               //电梯内按钮
	int again;
	int drcl,drcltime,testdoortime; //电梯门参量
	int state;                      //电梯的当前状态(GoingUp，GoingDOWEl，Idle)
	int prestate;                   //电梯下步状态(GoingUp，GoingDOWEl，Idle)
	int Floor;                      //电梯的当前位置(楼层)
	int d1;                         //值为0，除非人们正在进入和离开电梯；
	int d2;                         //值为0，如果电梯 已经在某层停候30Ot以上；
	int d3;                         //值为0，除非电梯 门正开着又无人迸出电梯；
	struct Elevator Stack[Maxfloor+1];//电梯内乘客
	struct TimeSequence sxjiankong;        //电梯时序监控
	int k;
	int backfirsttime; 
};

struct people upqueue[Maxfloor+1];  //上升队列 
struct people downqueue[Maxfloor+1];//下降队列 

void InputMaxTime(); 
void ShowTitle();
void InitElevator(Elevatorcanshu *&Elevator,int e);
void AddNewPerson(People per,Elevatorcanshu *&Elevator,int e);             //加入乘客 
void AddUpQueue(People &p,int in);  //加入上升队列 
void AddDownQueue(People &p,int in);//加入下降队列 
// 添加时间序列，模拟某一时刻的电梯运行过程
void AddTimeSequence(int time, People p, void(*fn)(People per, Elevatorcanshu *&Elevatortemp, int temp), Elevatorcanshu *&Elevator, int e);

// 放弃等待，表示某个乘客不再等待电梯
void GiveupWait(People per, Elevatorcanshu *&Elevator, int e);

// 运行电梯模拟
void Simulate();

// 控制电梯的运行（可能根据优先级或某些条件调度）
void ControlElevator(Elevatorcanshu *&Elevator, int e);

// 打开电梯门
void OpenDoor(People p, Elevatorcanshu *&Elevator, int e);

// 乘客进入电梯
void PeopleIn(People p, Elevatorcanshu *&Elevator, int e);

// 乘客离开电梯
void PeopleOut(People p, Elevatorcanshu *&Elevator, int e);

// 乘客进入电梯时，电梯以匀速运行
void UniformSpeed(People ap, Elevatorcanshu *&Elevator, int e);

// 乘客要求去一楼
void GoFirstFloorFloorFloorFloorFloorFloor(People ap, Elevatorcanshu *&Elevator, int e);

// 处理乘客的上下电梯
void InOut(People p, Elevatorcanshu *&Elevator, int e);

// 电梯向下移动
void ElevatorDown(People ap, Elevatorcanshu *&Elevator, int e);

// 电梯向上移动
void ElevatorUp(People ap, Elevatorcanshu *&Elevator, int e);

// 关闭电梯门
void CloseElevator(People ap, Elevatorcanshu *&Elevator, int e);

// 电梯停运
void ElevatorStop(People ap, Elevatorcanshu *&Elevator, int e);

// 获取电梯的当前请求（可能是上行或下行请求）
int getOrder(Elevatorcanshu *&Elevator, int e);

// 获取电梯的上行请求
int GetUp(Elevatorcanshu *&Elevator, int e);

// 获取电梯的下行请求
int GetDown(Elevatorcanshu *&Elevator, int e);

// 获取光标信息，可能用于显示某些状态或进行某种反馈
void GotOxy(short x, short y); 

// 显示其他信息，可能是电梯系统的状态或调度信息
void ShowOthersInfo();

// 打印电梯状态信息
void PrintElevator();

// 电梯消失的效果（可能是电梯离开或电梯消失的动画效果）
void DisappearEvelatorBody(int x, Elevatorcanshu *&Elevator, int e);

// 显示电梯的状态或位置
void ShowElevator(int x, Elevatorcanshu *&Elevator, int e);

// 显示等待队列中的乘客信息
void ShowWaitingQueue(int i);

// 让某个乘客消失（可能是乘客已经离开或不再等待）
void DisappearWaiting(int i);

// 打印帮助信息，帮助用户了解如何使用电梯系统
void PrintHelp();

