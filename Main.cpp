#include "Elevator.h"
#include "API.cpp"

int main()
{
    ShowTitle(); // 显示标题界面
    InputMaxTime(); // 输入最大模拟运行时间
    system("cls"); // 清屏，准备进入主界面

    // 初始化各楼层的上升和下降队列
    for (int i = 0; i <= Maxfloor; i++) {
        upqueue[i].next = downqueue[i].next = NULL; // 将每层的队列指针初始化为NULL
    }

    id = 1; // 初始化乘客编号，从1开始计数
    Time = 0; // 初始化模拟时间，从0开始
    ElevatorChar[0][0] = '\0'; // 初始化电梯提示字符（左电梯）
    ElevatorChar[1][0] = '\0'; // 初始化电梯提示字符（右电梯）
    PeopleChar6[0][0] = '\0'; // 初始化第六行乘客提示字符（左电梯）
    PeopleChar6[1][0] = '\0'; // 初始化第六行乘客提示字符（右电梯）
    PeopleChar7[0][0] = '\0'; // 初始化第七行乘客提示字符（左电梯）
    PeopleChar7[1][0] = '\0'; // 初始化第七行乘客提示字符（右电梯）

    Simulate(); // 调用模拟函数，开始模拟电梯的运行
    return 0; // 程序运行结束，返回0
}
