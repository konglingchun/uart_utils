#include <stdio.h>

/*
可以选择的编码如下所示:
	编码    颜色/动作 
	0       重新设置属性到缺省设置 
	1       设置粗体 
	2       设置一半亮度（模拟彩色显示器的颜色） 
	4       设置下划线（模拟彩色显示器的颜色） 
	5       设置闪烁 
	7       设置反向图象
	8	隐形
	22      设置一般密度 
	24      关闭下划线 
	25      关闭闪烁 
	27      关闭反向图象 
	30      设置黑色前景 
	31      设置红色前景 
	32      设置绿色前景 
	33      设置棕色前景 
	34      设置蓝色前景 
	35      设置紫色前景 
	36      设置青色前景 
	37      设置白色前景 
	38      在缺省的前景颜色上设置下划线 
	39      在缺省的前景颜色上关闭下划线 
	40      设置黑色背景 
	41      设置红色背景 
	42      设置绿色背景 
	43      设置棕色背景 
	44      设置蓝色背景 
	45      设置紫色背景 
	46      设置青色背景 
	47      设置白色背景 
	49	设置缺省黑色背景

其他一些有趣的编码(“\033”也可写成“\e”):
	\033[2J		清屏 
	\033[K		清除从光标到行尾的内容 
	\033[?25l		隐藏光标 
	\033[?25h	显示光标
	\033[y;xH	设置光标位置（x代表行，y代表列）
	\033[s		保存光标位置 
	\033[u		恢复光标位置
	\033[nA		光标上移n行 
	\033[nB		光标下移n行 
	\033[nC		光标右移n行 
	\033[nD		光标左移n行
	\033[0q		关闭所有的键盘指示灯 
	\033[1q		设置“滚动锁定”指示灯 (Scroll Lock) 
	\033[2q		设置“数值锁定”指示灯 (Num Lock) 
	\033[3q		设置“大写锁定”指示灯 (Caps Lock) 
*/

void print_buffer(char *pre, char *buffer, int size)
{
	printf("\e[31;1m");
	fflush(stdout);
	printf("%s[%d]{%s}\n", pre, size, buffer);
	printf("\e[0m");
	fflush(stdout);
}

void print_buffer_char(char *pre, char *buffer, int size)
{
	int i;

	printf("\e[32;1m");
	fflush(stdout);
	printf("%s[%d]{", pre, size);
	for(i=0;i<size;i++) {
		if(buffer[i] == '\r'){
			printf("'\\r',");
		}else if(buffer[i] == '\n'){
			printf("'\\n',");
		}else{
			printf("%c,", (unsigned char)buffer[i]);
		}
	}
	printf("}\n");
	printf("\e[0m");
	fflush(stdout);
}

void print_buffer_char_index(char *pre, char *buffer, int size)
{
	int i;

	printf("\e[32;1m");
	fflush(stdout);
	printf("%s[%d]{", pre, size);
	for(i=0;i<size;i++) {
		if(buffer[i] == '\r'){
			printf("'\\r',");
		}else if(buffer[i] == '\n'){
			printf("'\\n',");
		}else{
			printf("[%d]=%c,", i, (unsigned char)buffer[i]);
		}
	}
	printf("}\n");
	printf("\e[0m");
	fflush(stdout);
}

void print_buffer_hex(char *pre, char *buffer, int size)
{
	int i;

	printf("\e[34;1m");
	fflush(stdout);
	printf("%s[%d]{", pre, size);
	for(i=0;i<size;i++) {
		printf("%#02hhx,", (unsigned char)buffer[i]);
	}
	printf("}\n");
	printf("\e[0m");
	fflush(stdout);
}

void print_buffer_hex_index(char *pre, char *buffer, int size)
{
	int i;

	printf("\e[34;1m");
	fflush(stdout);
	printf("%s[%d]{", pre, size);
	for(i=0;i<size;i++) {
		printf("[%d]=%#02hhx,", i, (unsigned char)buffer[i]);
	}
	printf("}\n");
	printf("\e[0m");
	fflush(stdout);
}

