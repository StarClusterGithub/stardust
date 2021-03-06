/*
==================================================================================================================
Name        :	2048.c
Author      :	Star cluster
Description :	2048游戏,开局出现2个数(只能是2或4),之后玩家通过方向键控制数字移动方向,相同数字相撞会合并,
每一次操作都会在空白处随机出现2或4,当方格放满时游戏失败,经典模式下数字累加到2048则成功
==================================================================================================================
*/
#pragma warning(disable:4996)//关掉getch()的安全检查
#include<stdio.h>/*printf,scanf,NULL*/
#include<stdlib.h>/*srand,rand*/
#include<time.h>/*time*/
#include<conio.h>/*getch*/
#include<windows.h>/*system,Sleep*/


int start(void);//开始界面
void initialise(int num[4][4], int *mark, char *ch);//初始化
void newnum(int num[4][4]);//生成新数字
int control(int num[4][4]);//控制功能
int upward(int num[4][4]);//向上移动数字
int downward(int num[4][4]);//向下移动数字
int aleft(int num[4][4]);//向左移动数字
int dextrad(int num[4][4]);//向右移动数字
void grade(int num[4][4]);//计算等级
void output(int num[4][4]);//输出界面
int status(int num[4][4]);//判断游戏是否结束
int win(int num[4][4]);//胜利庆贺界面
void color(int num);//改变字体颜色


static int score = 0, rank = 0;//记录分数和等级

int main(void)
{
	int num[4][4];
	int flag = 1, *mark = &flag, pass = 2048;//用于标记游戏状态,flag是判断是否正确按键,*mark是初始化函数中改变flag的值,pass是无尽模式的判断标准
	char choose, *ch = &choose;//判断是否退出,*ch是初始化时改变choose的值
	srand((unsigned)time(NULL));//为随机数播种子
	system("color F0");
	while (1)
	{
		pass = 2048;//初始化
		initialise(num, mark, ch);
		switch (start())//调用开始函数输出提示信息并返回一个值供switch使用
		{
		case 1:
			while (flag != 2)//当无法移动方块或者玩家按下ESC(此时flag=2)时停止游戏
			{
				if (rank == 2048)//分数达到胜利标准
				{
					output(num);//输出胜利时的成绩
					printf("\r  恭喜你获得了最终胜利!按1重新开始本次游戏,按任意键回到主菜单,按Esc退出游戏");
					if (win(num) == 1)//胜利之后在win函数判断用户的选择并决定是否退出
						initialise(num, mark, ch);
					else
						break;
				}
				if (flag == 1)//判断是否生成新数
				{
					newnum(num);
					output(num);
				}
				flag = control(num);//调用控制函数并返回flag以便下一次循环时判断是否生成新数
				if (status(num) == 0)//调用status函数判断游戏是否结束(game over)
				{
					printf("\r\t   大侠,您挂了!重来请按1,回到主菜单请按任意键.");
					if ((choose = getch()) == 49)
						initialise(num, mark, ch);
					else
						break;
				}
			}
			break;
		case 2:
			while (flag != 2)//无尽模式其实是经典模式的无尽版
			{
				if (rank == pass&&rank != 0)//分数达到胜利标准
				{
					pass *= 2;//下一个等级
					flag = 0;//保证newnum函数不会对玩家的选择有反应
					output(num);//输出此时成绩
					printf("\r  恭喜你合成了数字%d!按1回到主菜单,按任意键继续,按Esc退出游戏", rank);
					if (win(num) == 1)//进阶后之后在win函数判断用户的选择并决定是否退出
						break;
				}
				if (flag == 1)//判断是否生成新数
				{
					newnum(num);
					output(num);
				}
				flag = control(num);//调用控制函数并返回flag以便下一次循环时判断是否生成新数
				if (status(num) == 0)//调用status函数判断游戏是否结束(game over)
				{
					printf("\r\t   大侠,您挂了!重来请按1,回到主菜单请按任意键.");
					if ((choose = getch()) == 49)
					{
						pass = 2048;
						initialise(num, mark, ch);
					}
					else
						break;
				}
			}
			break;
		case 3:
			printf("\t\t\t确定要退出吗?(Y/N)");
			choose = getch();
			if (choose == 'y' || choose == 'Y')
				exit(0);
			else
				break;
		default:
			printf("输入错误!没有这个选项!");
		}
	}
	return 0;
}


//开始界面
int start(void)
{
	int choose;
	system("cls");
	printf("\n\t\t\t欢迎来到2048游戏!");
	printf("\n\n\t\t\t   1.经典模式");
	printf("\n\t\t\t   2.无尽模式");
	printf("\n\t\t\t   3.退出");
	printf("\n\n\n\n\t\t\t   请选择:");
	scanf("%d", &choose);
	return choose;
}


//初始化
void initialise(int num[4][4], int *mark, char *ch)
{
	int i, j;
	for (i = 0; i<4; i++)
	for (j = 0; j<4; j++)
		num[i][j] = 0;
	score = 0, rank = 0;
	*mark = 1, *ch = '\0';
}


//生成新数
void newnum(int num[4][4])
{
	int i = 0, j = 0, k = 2, flag = 0;
	for (i = 0; i<4; i++)//对每一个数进行检查,如果有空位则表示应生成新数
	for (j = 0; j<4; j++)
	if (num[i][j] == 0)
	{
		flag = 1;//用1表示有空位
		i = 3;//将i改为3配合break退出循环
		break;
	}
	while (flag)//如果flag为0则表示方格已满,不生成新数
	{
		//利用随即函数在随机空位生成新数
		i = rand() % 4;
		j = rand() % 4;
		if (num[i][j] != 0)//判断位置是否为空位,不是空位的话重新开始
			continue;
		if (rand() % 2 == 1)//随机生成2或4
			num[i][j] = 2;
		else
			num[i][j] = 4;
		k--;
		//通过rank和k来判断是否是刚开始游戏,是则出现两个数字,否则新增一个数字
		if (rank != 0 || k == 0)
			break;
	}
}


//控制功能
int control(int num[4][4])
{
	int key, flag = 1;//用flag判断是否移动了方块
	if ((key = getch()) == 224)//判断玩家是否按下方向键,如果没有操作或没有正确操作则在else语句中返回0
	{
		switch (key = getch())//判断玩家按下的方向键是哪个,并执行相应操作
		{
			//4个case语句都用flag判断是否正确按键,没有则flag为0,则在主函数中不执行操作
		case 72:
			flag = upward(num);
			break;
		case 80:
			flag = downward(num);
			break;
		case 75:
			flag = aleft(num);
			break;
		case 77:
			flag = dextrad(num);
			break;
		default:
			flag = 0;
			break;
		}
	}
	else
	{
		if (key == 27)//如果玩家按下的是Esc键,则询问是否退出
		{
			printf("\n\t\t\t确定退出吗?(Y/N)");
			if ((key = getch()) == 89 || key == 121)
				flag = 2;
			else
			{
				flag = 0;
				output(num);//如果玩家取消了退出,则将此次选择所按下的键视为无效按键,并刷新输出页面
			}
		}
		else
			flag = 0;//如果既不是方向键又不是Esc键,则说明没有正确按键,返回0值
	}
	grade(num);//操作完成后获取当前等级
	return flag;
}


//向上移动数字
int upward(int num[4][4])
{
	int i = 0, j = 0, k = 0, flag = 0;//i,j,k用作下标号,flag判断变量,判断是否改变了方块位置
	for (j = 0; j<4; j++)//j控制列,i控制行,以列为基准,对同一列每行逐个检查,寻找符合条件的数
	{
		for (i = 0; i<3; i++)//寻找相等的数,由于是i行和i+1行对比,因此i<3
		{
			for (k = i + 1; num[i][j] != 0 && k<4; k++)//限制在num[i][j]不为0的情况下寻找相同的数
			{
				if (num[k][j] == 0)//遇到0则重新开始循环
					continue;
				if (num[k][j] != num[i][j])//如果后面一位不为0的数与num[i][j]不相同则退出循环判断下一行
					break;
				if (num[i][j] == num[k][j])//判断是否找到相同的数
				{
					num[i][j] *= 2;//相同的数相加可以用乘以2代替
					num[k][j] = 0;//将合并后的数置空,以达到合并效果
					score += num[i][j];//计算分数
					flag = 1;//将flag置为1表示方块位置发生改变
					break;//退出循环寻找下一位
				}
			}
		}
		for (i = 0; i<3; i++)//寻找空位,由于每一次均会填补完前面的空位,因此可以不必检查最后一行
		{
			if (num[i][j] == 0)//发现空位则执行下一步
			{
				for (k = i + 1; k<4; k++)//从发现的空位开始检查后面的数,找到不是空位的
				{
					if (num[k][j] != 0)//判断是否为空位
					{
						num[i][j] = num[k][j];//将找到的非空位的数填补前面的空位
						num[k][j] = 0;//将移动后的数字置空,以达到移动的效果
						flag = 1;//将flag置为1表示方块位置发生改变
						break;//退出循环寻找下一位
					}
				}
			}
		}
	}
	return flag;
}


int downward(int num[4][4])//向下移动数字
{
	int i = 0, j = 0, k = 0, flag = 0;//i,j,k用作下标号,flag判断变量,判断是否改变了方块位置
	for (j = 0; j<4; j++)//j控制列,i控制行,以列为基准,对同一列每行逐个检查,寻找符合条件的数
	{
		for (i = 3; i>0; i--)//寻找相等的数,由于是i行和i-1行对比,因此i>0
		{
			for (k = i - 1; num[i][j] != 0 && k >= 0; k--)//限制在num[i][j]不为0的情况下寻找相同的数
			{
				if (num[k][j] == 0)//遇到0则重新开始循环
					continue;
				if (num[k][j] != num[i][j])//如果后面一位不为0的数与num[i][j]不相同则退出循环判断下一行
					break;
				if (num[i][j] == num[k][j])//判断是否找到相同的数
				{
					num[i][j] *= 2;//相同的数相加可以用乘以2代替
					num[k][j] = 0;//将合并后的数置空,以达到合并效果
					score += num[i][j];//计算分数
					flag = 1;//将flag置为1表示方块位置发生改变
					break;//退出循环寻找下一位
				}
			}
		}
		for (i = 3; i>0; i--)//寻找空位,由于每一次均会填补完下面的空位,因此可以不必检查第一行
		{
			if (num[i][j] == 0)//发现空位则执行下一步
			{
				for (k = i - 1; k >= 0; k--)//从发现的空位开始检查后面的数,找到不是空位的
				{
					if (num[k][j] != 0)//判断是否为空位
					{
						num[i][j] = num[k][j];//将找到的非空位的数填补前面的空位
						num[k][j] = 0;//将移动后的数字置空,以达到移动的效果
						flag = 1;//将flag置为1表示方块位置发生改变
						break;//退出循环寻找下一位
					}
				}
			}
		}
	}
	return flag;
}


int aleft(int num[4][4])//向左移动数字
{
	int i = 0, j = 0, k = 0, flag = 0;//i,j,k用作下标号,flag判断变量,判断是否改变了方块位置
	for (i = 0; i<4; i++)//i控制行,j控制列,以行为基准,对同一行每列逐个检查,寻找符合条件的数
	{
		for (j = 0; j<3; j++)//寻找相等的数,由于是j行和j+1行对比,因此i<3
		{
			for (k = j + 1; num[i][j] != 0 && k<4; k++)//限制在num[i][j]不为0的情况下寻找相同的数
			{
				if (num[i][k] == 0)//遇到0则重新开始循环
					continue;
				if (num[i][k] != num[i][j])//如果后面一位不为0的数与num[i][j]不相同则退出循环判断下一行
					break;
				if (num[i][j] == num[i][k])//判断是否找到相同的数
				{
					num[i][j] *= 2;//相同的数相加可以用乘以2代替
					num[i][k] = 0;//将合并后的数置空,以达到合并效果
					score += num[i][j];//计算分数
					flag = 1;//将flag置为1表示方块位置发生改变
					break;//退出循环寻找下一位
				}
			}
		}
		for (j = 0; j<3; j++)//寻找空位,由于每一次均会填补完前面的空位,因此可以不必检查最后一行
		{
			if (num[i][j] == 0)//发现空位则执行下一步
			{
				for (k = j + 1; k<4; k++)//从发现的空位开始检查后面的数,找到不是空位的
				{
					if (num[i][k] != 0)//判断是否为空位
					{
						num[i][j] = num[i][k];//将找到的非空位的数填补前面的空位
						num[i][k] = 0;//将移动后的数字置空,以达到移动的效果
						flag = 1;//将flag置为1表示方块位置发生改变
						break;//退出循环寻找下一位
					}
				}
			}
		}
	}
	return flag;
}


int dextrad(int num[4][4])//向右移动数字
{
	int i = 0, j = 0, k = 0, flag = 0;//i,j,k用作下标号,flag判断变量,判断是否改变了方块位置
	for (i = 0; i<4; i++)//j控制列,i控制行,以列为基准,对同一列每行逐个检查,寻找符合条件的数
	{
		for (j = 3; j>0; j--)//寻找相等的数,由于是j行和j-1行对比,因此i>0
		{
			for (k = j - 1; num[i][j] != 0 && k >= 0; k--)//限制在num[i][j]不为0的情况下寻找相同的数
			{
				if (num[i][k] == 0)//遇到0则重新开始循环
					continue;
				if (num[i][k] != num[i][j])//如果后面一位不为0的数与num[i][j]不相同则退出循环判断下一行
					break;
				if (num[i][j] == num[i][k])//判断是否找到相同的数
				{
					num[i][j] *= 2;//相同的数相加可以用乘以2代替
					num[i][k] = 0;//将合并后的数置空,以达到合并效果
					score += num[i][j];//计算分数
					flag = 1;//将flag置为1表示方块位置发生改变
					break;//退出循环寻找下一位
				}
			}
		}
		for (j = 3; j>0; j--)//寻找空位,由于每一次均会填补完前面的空位,因此可以不必检查第一行
		{
			if (num[i][j] == 0)//发现空位则执行下一步
			{
				for (k = j - 1; k >= 0; k--)//从发现的空位开始检查后面的数,找到不是空位的
				{
					if (num[i][k] != 0)//判断是否为空位
					{
						num[i][j] = num[i][k];//将找到的非空位的数填补前面的空位
						num[i][k] = 0;//将移动后的数字置空,以达到移动的效果
						flag = 1;//将flag置为1表示方块位置发生改变
						break;//退出循环寻找下一位
					}
				}
			}
		}
	}
	return flag;
}


//计算等级
void grade(int num[4][4])
{
	int i, j, flag = 1;
	for (i = 0; i<4; i++)
	for (j = 0; j<4; j++)
	{
		if (num[i][j]>rank)
			rank = num[i][j];
	}
}


//输出功能
void output(int num[4][4])
{
	int i = 0, j = 0;
	system("cls");
	printf("\n\t\t 您目前的分数为:%d    您的等级为:%d", score, rank);
	printf("\n\t\t╔═══╦═══╦═══╦═══╗");
	printf("\n\t\t║      ║      ║      ║      ║");
	for (i = 0; i<4; i++)
	{
		printf("\n\t\t");
		for (j = 0; j<4; j++)
		{
			if (num[i][j] == 0)
				printf("║      ");
			else
			{
				printf("║  ");
				color(num[i][j]);
				printf("%-4d", num[i][j]);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
			}


		}
		printf("║");
		if (i == 3)
			break;
		printf("\n\t\t║      ║      ║      ║      ║");
		printf("\n\t\t╠═══╬═══╬═══╬═══╣");
		printf("\n\t\t║      ║      ║      ║      ║");
	}
	printf("\n\t\t║      ║      ║      ║      ║");
	printf("\n\t\t╚═══╩═══╩═══╩═══╝");
	printf("\n\t\t  ↑↓←→控制方块滑动,按Esc退出");
}


//判断游戏是否结束
int status(int num[4][4])
{
	int i, j;
	for (i = 0; i<4; i++)//对每一个数进行检查,如果有空位则表示游戏未结束,返回1
	{
		for (j = 0; j<4; j++)

		{
			if (num[i][j] == 0)
				return 1;
		}
	}
	for (i = 0; i<4; i++)//横向检查是否有相邻两个数相同的,有则表示游戏未结束,返回1
	{
		for (j = 0; j<3; j++)
		{
			if (num[i][j] == num[i][j + 1])
				return 1;
		}
	}
	for (i = 0; i<3; i++)//纵向检查是否有相邻两个数相同的,有则表示游戏未结束,返回1
	{
		for (j = 0; j<4; j++)
		{
			if (num[i][j] == num[i + 1][j])
				return 1;
		}
	}
	return 0;//如果即没有找到空位又没有相邻的两个数相同,则游戏结束,返回0
}


//胜利庆贺界面
int win(int num[4][4])
{
	int over = 0;//用over返回一个值判断玩家是否选择继续游戏
	char choose = '\0', c, color[9] = "color ";//color数组和变量c用于胜利后调用system("color");函数
	for (; kbhit() == 0;)//如果玩家没有按键,则一直随机变色
	{
		while ((c = rand() % 58)<'1' || c>'9');
		color[6] = c;
		while ((c = rand() % 71)<'A' || c>'F');
		color[7] = c;
		system(color);
		Sleep(1000);
	}
	if ((choose = getch()) == 224)//如果玩家按下方向键,则把输入内容抛弃掉
		choose = getch();
	else
	if (choose == 27)//判断玩家的选择
		exit(0);
	else
	if (choose == 49)//选择继续游戏
		over = 1;
	system("color F0");
	return over;
}
void color(int num)
{
	switch (num)
	{
	case 2:
	case 4:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf8);
		break;
	case 8:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xfb);
		break;
	case 16:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xfa);
		break;
	case 32:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xfe); 
		break;
	case 64:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf6); 
		break;
	case 128:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xfd);
		break;
	case 256:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf5);
		break;
	case 512:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xfc);
		break;
	case 1024:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf4);
		break;
	case 2048:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
		break;
	case 4096:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf9);
		break;
	case 8192:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf3);
		break;
	case 16384:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf1);
		break;
	default :
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
		break;
	}
}