#if 0
/*
=======================================================================================================================
Name:		 counter.c
Author:		 star_cluster
Description: 利用数组方式进行大数的运算(正整数范围)
=======================================================================================================================
*/
#pragma warning(disable:4996)//关掉getch()的安全检查
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define RADIX 1000000000
typedef long long DataType;

typedef struct OPERATION//定义一个结构体,存放运算数据
{
	char sign;//运算符号
	DataType *first;//第一个数
	DataType *second;//第二个数
	DataType *result;//运算结果
	int flen;//第一个数的长度
	int slen;//第二个数的长度
	int len;//运算结果的长度
}OPERATION;


void getnum(OPERATION *num);//输入函数,用于输入需要计算的式子,并返回式子的符号
void control(OPERATION *num);//控制函数,用于判断运算类型并进行处理
void sort(DataType *num, int len);//排序函数,将给定的数组按照逆序存放
int compare(DataType *first, int flen, DataType *second, int slen);//比较函数,比较输入的两个数的大小
void addition(OPERATION *num);//加法运算
void subtraction(OPERATION *num);//减法运算
void difference(OPERATION *num);//计算第一个数减去第二个数的结果
void multiplication(OPERATION *num);//乘法运算
void division(OPERATION *num);//除法运算
void factorial(OPERATION *num);//阶乘运算
void power(OPERATION *num);//幂运算
int putnum(OPERATION *num);//输出函数,用于输出等式
void keep(OPERATION *num,int len);//保存函数,将运算结果保存为文本文件,两个参数分别为需要保存的运算式和运算式结果的长度

static clock_t Start = 0, Finish = 0, Time_open = 0, Time_put = 0;//记录时间

int main(void)
{
	for (char ch = '\0'; ch != 27; ch = getch())
	{
		OPERATION num = { '\0', NULL, NULL, NULL, 0, 0, 0 };
		char choose = '\0';//选择
		int len = 0;//运算结果长度
		system("cls");
		printf("\n\t\t大数运算计算器1.2版");
		printf("\n\t\tMade in star cluster\n");
		printf("温馨提示:本计算器仅支持正整数的加减乘除、阶乘、幂运算,数据可以用逗号隔开.\n");
		printf("样例输入:12345+54321                 错误输入:12345*-12345\n");
		printf("\n\n请输入正整数范围内的运算式(必须指定运算符号):\n");
		for (getnum(&num); num.sign != '!' && (num.first == NULL || num.second == NULL); free(num.first), free(num.second), getnum(&num))
			printf("运算式输入错误!请重新输入!输入标准请参考温馨提示.\n");
		Start = clock();//记录程序运算时间
		control(&num);//通过控制函数进行计算
		Finish = clock();//记录程序运算时间
		Time_open = Finish - Start;//程序运算所耗费时间
		Start = clock();//记录程序输出时间
		printf("运算结果为:\n");
		len = putnum(&num);
		Finish = clock();//记录程序输出时间
		printf("运算结果的长度为:%d位数.\n", len);
		printf("The program running time of %ld milliseconds, %.3lf second.\n", Time_open, (double)(Time_open / 1000.0));
		Time_put = Finish - Start;//程序输出所耗费时间
		printf("The program output takes %ld milliseconds, %.3lf second.\n", Time_put, (double)(Time_put / 1000.0));
		for (choose = '\0'; 'Y' != choose && 'N' != choose && 'y' != choose && 'n' != choose; fflush(stdin), choose = getchar())
			printf("是否保存运算结果?(Y/N)");
		if ('Y' == choose || 'y' == choose)
			keep(&num,len);
		printf("按ESC键退出,按任意键继续...\n");
		fflush(stdin);
		//释放动态分配的内存
		free(num.first);
		free(num.second);
		free(num.result);
	}
	printf("感谢您的使用,再见!\n");
	Sleep(2500);
	return 0;
}


//输入函数,用于输入需要计算的式子(按照逆序存放)
void getnum(OPERATION *num)
{
	char *temp = NULL;//temp是输入时中间变量
	int i = 0, j = 0, length = 0, right = 0, power = 0;//i为临时变量temp的下标号,j为循环变量,length是长度,right判断输入的字符是否含有运算符,power为10的length-1次幂
	num->sign = '\0', num->first = NULL, num->second = NULL, num->result = NULL, num->flen = 0, num->slen = 0;
	for (; right == 0;)
	{
		for (i = 0, temp = (char *)malloc(sizeof(char)); scanf("%c", temp + i), temp[i] != '\n'; temp = (char *)realloc(temp, (++i + 1)*sizeof(char)))//输入运算式并存放于temp中
		{
			if (temp[i] == '+' || temp[i] == '-' || temp[i] == '*' || temp[i] == '/' || temp[i] == '!' || temp[i] == '^')//检查是否输入了运算符
			{
				++right;
				continue;
			}
			if (temp[i] != ',' && (temp[i]<'0' || temp[i]>'9'))
			{
				right = 0;
				fflush(stdin);
				break;
			}
		}
		if (right == 0||right>1)
		{
			printf("运算式输入错误!请重新输入!输入标准请参考温馨提示.\n");
			free(temp); temp = NULL; right = 0;
		}
	}
	for (length = 1; temp[i] != '+' && temp[i] != '-' && temp[i] != '*' && temp[i] != '/' && temp[i] != '!'&&temp[i] != '^'; --i)//从最后一个字符开始读取并放入second中
	{
		if ('0' <= temp[i] && temp[i] <= '9')//只有当输入的数据是阿拉伯数字时才进行储存
		{
			if (1 == length)
			{
				num->second = (DataType *)realloc(num->second, ++num->slen*sizeof(DataType));//length为1说明是10亿进制中的"个位数",需要新申请一个内存空间存放
				num->second[num->slen - 1] = 0;
			}
			for (j = 1, power = 1; j < length; ++j)
				power *= 10;
			num->second[num->slen - 1] += (temp[i] - '0') * power;
			if (9 == length)//length达到9时是10亿进制中的"最高位",需要把length赋值为0
				length = 1;
			else//未到9则继续计数
				++length;
		}
	}
	for (num->sign = temp[i--], length = 1; i >= 0; --i)//记录运算符号并将剩余数据放入first中
	{
		if ('0' <= temp[i] && temp[i] <= '9')
		{
			if (1 == length)//length为1说明是10亿进制中的"个位数",需要新申请一个内存空间存放
			{
				num->first = (DataType *)realloc(num->first, ++num->flen*sizeof(DataType));
				num->first[num->flen - 1] = 0;
			}
			for (j = 1, power = 1; j < length; ++j)
				power *= 10;
			num->first[num->flen - 1] += (temp[i] - '0') * power;
			if (9 == length)//length达到9时是10亿进制中的"最高位",需要把length赋值为0
				length = 1;
			else//未到9则继续计数
				++length;
		}
	}
	free(temp);//释放temp
	fflush(stdin);//清空缓存区
}


//控制函数,用于判断运算类型并进行处理
void control(OPERATION *num)
{
	switch (num->sign)
	{
	case '+':
		addition(num);//加法
		break;
	case '-':
		subtraction(num);//减法
		break;
	case '*':
		multiplication(num);//乘法
		break;
	case '/':
		division(num);//除法
		break;
	case '!':
		if (num->first[0] < 0)
		{
			printf("负数没有阶乘!");
		}
		else
			factorial(num);//阶乘运算
		break;
	case '^':
		power(num);
		break;
	default:
		printf("不支持的运算符!\n");
		break;
	}
	//进行排序
	sort(num->first, num->flen);
	sort(num->second, num->slen);
	sort(num->result, num->len);
}


//排序函数,用于对运算结果进行排序,使之按照正常顺序存放
void sort(DataType *num, int len)
{
	int i = 0, j = 0; DataType temp = 0;
	for (i = 0, j = len - 1; i < j; ++i, --j)
	{
		temp = num[i];
		num[i] = num[j];
		num[j] = temp;
	}
}


//比较函数,比较输入的两个数的大小,返回1表示第一个数大于第二个数,-1则相反,0表示两数相等
int compare(DataType *first,int flen,DataType *second,int slen)
{
	int i = 0, j = 0, big = 0;//i为下标号,big记录大小信息
	if (flen > slen)//检查第一个数与第二个数的大小关系,1表示第一个数大于第二个数,0则相反,-1表示两数相等
		big = 1;
	else
	{
		if (flen < slen)
			big = -1;
		else
		{
			for (i = flen - 1, j = slen - 1; i >= 0; --i, --j)//如果被减数与减数长度相等,则从高位数(数组的最后一位)开始逐位判断
			{
				if (first[i] > second[j])
				{
					big = 1;
					break;
				}
				else
				{
					if (first[i] < second[j])
					{
						big = -1;
						break;
					}
				}
			}
		}
	}
	return big;
}


//加法运算
void addition(OPERATION *num)
{
	int i = 0;//i为两个运算数及和数的下标
	int carry = 0, bigger = (num->flen > num->slen ? num->flen : num->slen), minor = (num->flen < num->slen ? num->flen : num->slen);//carry是进位数,bigger是加数和被加数中长度较大的,minor则是较小的
	for (i = 0; i < bigger; ++i)
	{
		num->result = (DataType *)realloc(num->result, ++num->len *sizeof(DataType));//改变num->result的大小,使之能够容纳运算结果
		if (i >= minor)//如果两个数中的一个数已经加完了,那么加完的那个数不继续参与运算
		{
			if (i >= num->flen)
			{
				num->result[i] = (num->second[i] + carry) % RADIX;//将未加完的数与进位数继续进行运算
				carry = (num->second[i] + carry) / RADIX;
			}
			else
			{
				num->result[i] = (num->first[i] + carry) % RADIX;//同上
				carry = (num->first[i] + carry) / RADIX;
			}
		}
		else
		{
			num->result[i] = (num->first[i] + num->second[i] + carry) % RADIX;//求出相加后的数
			carry = (num->first[i] + num->second[i] + carry) / RADIX;//记录相加后的进位数
		}
		if ((i + 1 == bigger) && carry != 0)//如果输入的数中长的那位数已经加到了最前一位,但还需要进位,则执行此语句
		{
			num->result = (DataType *)realloc(num->result, ++num->len*sizeof(DataType));//改变num->result的大小,使之能够容纳运算结果
			num->result[num->len - 1] = carry;
		}
	}
}


//减法运算
void subtraction(OPERATION *num)
{
	DataType *temporary = NULL; int temp = 0; //temp为调整num->result顺序和计算小数减大数的整型中间变量,temporary是计算小数减大数的指针中间变量
	switch (compare(num->first,num->flen,num->second,num->slen))//根据减数与被减数的大小关系进行运算
	{
	case 0://被减数等于减数
		num->result = (DataType *)malloc(++num->len*sizeof(DataType));//运算结果为0,只需要一个int型内存单位
		num->result[0] = 0;
		break;
	case 1://被减数大于减数
		difference(num);//直接计算差值
		break;
	case -1://被减数小于减数

		//将减数和被减数互换
		temporary = num->first;
		num->first = num->second;
		num->second = temporary;
		temp = num->flen;
		num->flen = num->slen;
		num->slen = temp;

		difference(num);//调用函数计算差值

		//将减数和被减数换回来
		temporary = num->first;
		num->first = num->second;
		num->second = temporary;
		temp = num->flen;
		num->flen = num->slen;
		num->slen = temp;

		//小数减大数,结果为负,最前一位数加上负号
		num->result[num->len - 1] *= -1;
	}
}


//计算第一个数减去第二个数的差值
void difference(OPERATION *num)
{
	int i = 0;//i为下标号变量
	int carry = 0, bigger = (num->flen > num->slen ? num->flen : num->slen), minor = (num->flen < num->slen ? num->flen : num->slen);// carry为进位数,bigger是两个数中长度较大的数的长度,minor则是较小的
	for (i = 0; i < bigger; ++i)
	{
		num->result = (DataType *)realloc(num->result, ++num->len*sizeof(DataType));//使num->result能放运算结果
		if (i >= minor)//当减数已经减完了则只需要算出实际的被减数(即num->first[i]+carry)
		{
			if (carry + num->first[i] < 0)//被减数实际值小于0时
			{
				num->result[i] = (carry + num->first[i] + RADIX) % RADIX;//加上RADIX(10亿)以保证num->first[i]为正
				carry = -1;
			}
			else//被减数实际值不小于0
			{
				num->result[i] = (carry + num->first[i]) % RADIX;
				carry = 0;
			}
		}
		else//减数未减完
		{
			if (num->first[i] + carry < num->second[i])//如果该位数的被减数小于减数
			{
				num->result[i] = ((num->first[i] + carry + RADIX) - num->second[i]) % RADIX;//计算差,+RADIX和%RADIX保证结果大于0小于10亿
				carry = -1;//此时需要向高位数借1
			}
			else//该位数的被减数大于减数则直接相减
			{
				num->result[i] = (num->first[i] + carry) - num->second[i];
				carry = 0;
			}
		}
	}

	//将减完之后的运算结果中多余的0去掉
	for (i = num->len - 1; i >= 0; --i)
	{
		if (num->result[i] == 0)
		{
			num->result = (DataType *)realloc(num->result, --num->len*sizeof(DataType));
		}
		else
			break;//只要碰到一个不为0的数则说明剩下的都是有效数字了,则退出
	}
}


//乘法运算
void multiplication(OPERATION *num)
{
	int i = 0, j = 0, k = 0;//i,j,k为下标号变量
	int carry = 0;// carry为进位数
	DataType temp = 0;//temp为累加时的临时变量

	for (j = 0; j < num->slen; ++j)//按照乘法规则从最后一位算起
	{
		for (i = 0, k = j, carry = 0, temp = 0; i < num->flen; ++i, ++k)//i为第一位数的下标号,k为结果的下标号
		{
			num->result = (DataType *)realloc(num->result, (num->len > (k + 1) ? num->len : (k + 1))*sizeof(DataType));//将result长度调整为合适长度,由于每次执行内循环时有k=j,所以需要将k+1和len比较,将较大的作为长度
			if (num->result[k] < 0 || num->result[k] > RADIX - 1)//判断是否有效数字,没有的话则置0
			{
				num->result[k] = 0;
			}
			temp = num->result[k] + num->second[j] * num->first[i] + carry;
			num->result[k] = temp % RADIX;//取出个位数
			carry = temp / RADIX;//取出进位数
			if (i == num->flen - 1 && carry != 0)//如果已经算完但是还需进位
			{
				num->result = (DataType *)realloc(num->result, (++k + 1)*sizeof(DataType));
				num->result[k] = carry;
			}
		}
		num->len = k;//令num->len正确记录result的长度
	}
}


//除法运算
void division(OPERATION *num)
{
	OPERATION temp = { '-', NULL, NULL, NULL, 0, 0, 0 };//运算减法的临时变量
	int i = 0, k = 0;//循环变量i,num->result下标号变量k
	int end = 1;//结束符号

	num->result = (DataType *)malloc(++num->len*sizeof(DataType));//为运算结果申请内存
	num->result[0] = 0;//初始化

	//复制num的值
	temp.first = (DataType *)malloc(num->flen*sizeof(DataType));
	for (temp.flen = num->flen, i = 0; i < num->flen; i++)
		temp.first[i] = num->first[i];
	temp.second = (DataType *)malloc(num->slen*sizeof(DataType));
	for (temp.slen = num->slen, i = 0; i < num->slen; i++)
		temp.second[i] = num->second[i];

	for (; end;)
	{
		switch (compare(temp.first, temp.flen, temp.second, temp.slen))//判断两数大小情况并执行相应操作,-1为小于,0为相等,1为大于
		{
		case -1:
			end = 0;
			break;
		case 0:
			++num->result[0];//商加一
			if (num->result[0] > RADIX - 1)//纠正商
			{
				num->result[0] = 0;
				for (k = 1;; k++)
				{
					if (k > num->len - 1)//数组不够长时
					{
						num->result = (DataType *)realloc(num->result, ++num->len*sizeof(DataType));
						num->result[num->len - 1] = 0;
					}
					++num->result[k];
					if (num->result[k] <= RADIX - 1)
						break;
					else
						num->result[k] = 0;
				}
			}
			end = 0;
			break;
		case 1:
			difference(&temp);//相减
			//相减之后将结果作为被减数
			temp.flen = temp.len;
			temp.len = 0;
			free(temp.first);
			temp.first = temp.result;
			temp.result = NULL;
			++num->result[0];//商加一
			if (num->result[0] > RADIX - 1)//纠正商
			{
				num->result[0] = 0;
				for (k = 1;; k++)
				{
					if (k > num->len - 1)//数组不够长时
					{
						num->result = (DataType *)realloc(num->result, ++num->len*sizeof(DataType));
						num->result[num->len - 1] = 0;
					}
					++num->result[k];
					if (num->result[k] <= RADIX - 1)
						break;
					else
						num->result[k] = 0;
				}
			}
			break;
		}
	}
	free(temp.first);
	free(temp.second);
	if (temp.result != NULL)
		free(temp.result);
}


//阶乘函数
void factorial(OPERATION *num)
{
	if (num->flen == 1 && (num->first[0] == 0 || num->first[0] == 1))
	{
		num->result = (DataType *)malloc(++num->len*sizeof(DataType));//给result申请一个长度的内存
		num->result[0] = 1;//0和1的阶乘都为1
	}
	else
	{
		OPERATION temp = { '*', NULL, NULL, NULL, 0, 0, 0 };//临时变量,second代表因数,result是结果,first是前一个因数的阶乘
		int i = 0;//i为因数(second)的下标号

		//因数从3开始计算,而结果从2开始,因为若输入的是2,则2的阶乘仍为2
		temp.result = (DataType *)malloc(++temp.len*sizeof(DataType)); temp.result[0] = 2;
		temp.second = (DataType *)malloc(++temp.slen*sizeof(DataType)); temp.second[0] = 3;

		for (; compare(num->first, num->flen, temp.second, temp.slen) != -1;)//循环条件为因数小于等于输入的数
		{
			free(temp.first);
			temp.first = temp.result; temp.flen = temp.len;//将结果(前一个数的阶乘)作为被乘数
			temp.result = NULL; temp.len = 0;//使result不指向任何内存
			multiplication(&temp);//乘法计算
			for (++temp.second[0], i = 0; temp.second[i] == RADIX; ++i)//使因数自增1
			{
				temp.second[i] = 0;
				if (i == temp.slen - 1)//如果长度不足则改变temp.second的大小
				{
					temp.second = (DataType *)realloc(temp.second, ++temp.slen*sizeof(DataType));
					temp.second[++i] = 1;
				}
				else
					++temp.second[i + 1];
			}
		}
		num->result = temp.result; num->len = temp.len;//最后结果即所求结果
		//释放内存
		free(temp.first);
		free(temp.second);
	}
}


//幂运算函数
void power(OPERATION *num)
{
	if (num->slen == 1 && num->second[0] == 0)//检查指数是否为0,是则结果为1
	{
		num->result = (DataType *)malloc(++num->len*sizeof(DataType));
		num->result[0] = 1;
	}
	else
	{
		if (num->slen == 1 && num->second[0] == 1)//检查指数是否为1,是则结果为底数本身
		{
			int i = 0;
			num->len = num->flen;
			num->result = (DataType *)malloc(num->len*sizeof(DataType));
			for (i = 0; i < num->len; ++i)
				num->result[i] = num->first[i];
		}
		else//若指数不为1或0,则用二进制求幂法计算结果
		{
			OPERATION baseTemp = { '*', NULL, NULL, NULL, 0, 0, 0 }, resultTemp = { '*', NULL, NULL, NULL, 0, 0, 0 };//baseTemp是底数临时变量,用于计算底数的平方,resultTemp是结果临时变量,用于计算新的结果
			DataType *temp = NULL;int tlen = 0;//temp和tlen是计算二进制时的临时变量
			int *binary = NULL, blen = 0, carry = 0;//binary和blen用于储存指数的二进制的数组及数组的长度,carry是进位数
			int i = 0, j = 0;//下标号变量
			//复制底数
			baseTemp.first = baseTemp.second = (DataType *)malloc(num->flen*sizeof(DataType)), baseTemp.flen = baseTemp.slen = num->flen;
			for (i = 0; i < num->flen; ++i)
				baseTemp.first[i] = num->first[i];
			//初始化结果临时变量
			resultTemp.result = (DataType *)malloc(sizeof(DataType)), resultTemp.len = 1; resultTemp.result[0] = 1;
			//复制指数
			temp = (DataType *)malloc(num->slen*sizeof(DataType)); tlen = num->slen;
			for (i = 0; i < num->slen; ++i)
				temp[i] = num->second[i];
			//计算指数的二进制并存放于binary
			for (; tlen > 0;)
			{
				binary = (int *)realloc(binary, ++blen*sizeof(int));
				binary[blen - 1] = temp[0] % 2;
				for (carry = 0, i = tlen - 1; i >= 0; --i)
				{
					if (temp[i] % 2 == 1)
					{
						temp[i] = (temp[i] + carry * RADIX) / 2;
						carry = 1;
					}
					else
					{
						temp[i] = (temp[i] + carry * RADIX) / 2;
						carry = 0;
					}

				}
				for (i = tlen - 1; i >= 0 && temp[i] == 0; --i)
					temp = (DataType *)realloc(temp, --tlen*sizeof(DataType));
			}
			for (i = 0; i <= blen; ++i)
			{
				if (binary[i] == 1 || i == blen)//求值
				{
					free(resultTemp.second);
					resultTemp.first = baseTemp.first, resultTemp.flen = baseTemp.flen;
					resultTemp.second = resultTemp.result; resultTemp.slen = resultTemp.len;
					resultTemp.result = NULL, resultTemp.len = 0;
					multiplication(&resultTemp);
				}
				if (i != blen)//底数自乘
				{
					multiplication(&baseTemp);
					free(baseTemp.first);
					baseTemp.first = baseTemp.second = baseTemp.result; baseTemp.flen = baseTemp.slen = baseTemp.len;
					baseTemp.result = NULL; baseTemp.len = 0;
				}
			}
			num->result = resultTemp.second; num->len = resultTemp.slen;
			free(baseTemp.first);
		}//指数不为0或1时的else的花括号
	}
}


//输出函数,用于输出等式
int putnum(OPERATION *num)
{
	int i = 0;
	int len = 0, counting = 0;//len是运算结果的长度,counting是长度的计数变量
	printf("%lld", num->first[0]);//输出第一个数
	for (i = 1; i < num->flen; i++)//输出第一个数
	{
		printf("%09lld", num->first[i]);
	}
	printf(" %c ", num->sign);//输出运算符号
	if (num->second != NULL)//如果是阶乘运算则second为NULL,读取其值会发生异常
	{
		printf("%lld", num->second[0]);//输出第一个数
		for (i = 1; i < num->slen; i++)//输出第二个数
		{
			printf("%09lld", num->second[i]);
		}
	}
	printf("  =  ");//输出等号
	printf("%lld", num->result[0]);//输出第一个数
	for (i = 1; i < num->len; i++)//输出运算结果
	{
		printf(",%09lld", num->result[i]);
	}
	printf("\n");//换行
	for (len = (num->len - 1) * 9, counting = 1; counting <= (num->result[0] >= 0 ? num->result[0] : -1 * num->result[0]); counting *= 10, ++len);

	return len;
}


//保存函数,将运算结果保存为文本文件,两个参数分别为需要保存的运算式和运算式结果的长度
void keep(OPERATION *num, int len)
{
	FILE *target = NULL;//文件指针
	char catalogue[100], tabletop[] = "桌面", here[] = "当前文件夹", temp[100];//文件目录catalogue,桌面tabletop,当前文件夹here,临时变量temp
	int i = 0;//循环变量
	clock_t start = 0, finish = 0;//时间变量
	printf("请输入保存路径及文件名,如D:\\运算结果.txt\n");
	printf("若要保存于桌面或者当前文件夹则可以直接输入桌面或当前文件夹,如桌面\\运算结果.txt\n");
	fflush(stdin);
	gets(temp);
	start = clock();//获取保存时间
	for (i = 0; i < 10; ++i)//判断输入的路径
	{
		if (temp[i] != tabletop[i] && temp[i] != here[i])
			break;
	}
	switch (i)//将输入的路径放在catalogue中
	{
	case 4:strcpy(catalogue, "C:\\Users\\Administrator\\Desktop\\"); strcat(catalogue, temp + 5); break;
	case 10:strcpy(catalogue, temp + 11); break;
	default:strcpy(catalogue, temp); break;
	}
	if (NULL == (target = fopen(catalogue, "w")))//创建(打开)文件
	{
		perror("Cannot open this file!\nThe error message");
		system("pause"); exit(EXIT_FAILURE);
	}
	fprintf(target, "%lld", num->first[0]);//输出第一个数
	for (i = 1; i < num->flen; i++)//输出第一个数
	{
		fprintf(target, "%09lld", num->first[i]);
	}
	fprintf(target, " %c ", num->sign);//输出运算符号
	if (num->second != NULL)//如果是阶乘运算则second为NULL,读取其值会发生异常
	{
		fprintf(target, "%lld", num->second[0]);//输出第一个数
		for (i = 1; i < num->slen; i++)//输出第二个数
		{
			fprintf(target, "%09lld", num->second[i]);
		}
	}
	fprintf(target, "  =  ");//输出等号
	fprintf(target, "%lld", num->result[0]);//输出第一个数
	for (i = 1; i < num->len; i++)//输出运算结果
	{
		fprintf(target, ",%09lld", num->result[i]);
	}
	fprintf(target, "\n");//换行
	finish = clock();//获取保存时间
	fprintf(target,"运算结果的长度为:%d位数.\n", len);
	fprintf(target, "The program running time of %ld milliseconds, %.3lf second.\n", Time_open, (double)(Time_open / 1000.0));
	fprintf(target, "The program output takes %ld milliseconds, %.3lf second.\n", Time_put, (double)(Time_put / 1000.0));
	fprintf(target, "Save the results of time - consuming %ld milliseconds, %.3lf second.", finish - start, (double)((finish - start) / 1000.0));
	printf("保存成功!\n");
	fclose(target);//关闭文件
}
#endif