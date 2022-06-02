#include <graphics.h>
#include<iostream>
#include<stdio.h>
//大部分函数调用中变量l表示格子列数（总长度），h表示格子行数（总高度），n表示地雷数量，s表示游戏进行时间,f表示插旗数
//*s1表示一局开始的现实时间
#include <stdlib.h> // Header file needed to use srand and rand
#include <ctime> // Header file needed to use time
#include <time.h>
#pragma comment(lib,"Winmm.lib")//音乐播放需要
#include<mmsystem.h>//音乐播放需要
#include<fstream>
#include <list>
#include <string>
using namespace std;
int mysize ;//格子尺寸
//记录胜局信息的类
class Record{
	public:
	Record(int l, int h, int n, int s);
	Record();
	~Record(){};
	virtual void recording();//与派生类函数同名虚函数，用于文件写入
	int length;
	int high;
	int number;
	int second;
	
	char date[30];//对局日期记录
	
};
//类的默认构造
Record::Record() {
	length = 10;
	high = 10;
	number = 10;
	second = 10;
	time_t c;
	time(&c);
	ctime_s(date, 30, &c);
}


//类的构造
Record::Record(int l, int h, int n, int s) {
	if (s >= 999)
		s = 999;
	length = l;
	high = h;
	number = n;
	second = s;
	time_t c;
	time(&c);
	ctime_s(date,30,&c);//调取当前日期
	 
}


//写入文件
void Record::recording() {
	ofstream fout;
	fout.open("Record.txt", ios::out | ios::app);//打开文本文件
	if (!fout.is_open()) {
		cout << "no";
		exit(EXIT_FAILURE);
	} //打开成功与否的判断
	 
	fout << this->length << "          " << this->high << "                " << this->number <<
		"            " <<"自定义" << "              " << this->second << "              " << this->date << "           " <<
		endl;//类元素的存入
	fout.close();
}

//派生类
class specialRecord : public Record {
private:
	string str;//储存难度信息的字符串
public:
	specialRecord(string s, const Record& a);//派生类的构造函数
	virtual	void recording();

};

//构造派生类
specialRecord::specialRecord(string s, const Record& a) : Record(a) {
	str = s;
}

//派生类写入文件
void specialRecord::recording() {

	ofstream fout;
	fout.open("Record.txt", ios::out | ios::app);
	if (!fout.is_open()) {
		cout << "no";
		exit(EXIT_FAILURE);
	}
	fout << this->length << "          " << this->high << "                " << this->number <<
		"               " << str << "              " << this->second << "              " << this->date << "           " <<
		endl;//与基类不同的写入方式
	fout.close();
}

//部分函数的声明
void space(int(*b)[50], int l, int h);
void num(int(*b)[50],int l,int h);
void background(int(*b)[50], int l,int h,int n,int *s1);
void record();
void music();
void save(int(*b)[50], int l, int h, int n, int* s1);
void load(int(*b)[50], int& l, int& h, int& n);
void bomb(int(*b)[50], int l, int h, int n, int y, int x);


//记录读取与显示
void record() {
	string s[100], S;
	ifstream fin;
	Record r;
	char changeline = '\n';//换行字符
	fin.open("Record.txt", ios::in );
	S= "列数      行数            雷数            难度          通关时间                       日期            \n";
	list<string>all;
	list<string>::iterator iter;//动态链表all的迭代器
	int i=0,ii=0;
	while (getline(fin, s[i])) {//整行读取进字符串
		
		all.push_back(s[i]);//将字符串插入链表结尾
		i++;
	}
		for (iter =--all.end(); iter !=all.begin(); iter--)
	{    S = S + *iter+changeline;
		ii++;
		if (ii == 10)
			break;
	}//将最新十条记录展示
	const char* a=S.data();//字符串转字符数组地址
	MessageBox(NULL, TEXT(a), TEXT("记录"), MB_OK | MB_SYSTEMMODAL);//将字符数组a的内容写入标题为记录的有确定按钮的显示在窗口前面的对话框
	all.clear();//释放链表
	fin.close();//结束流
}

//音乐播放
void music() {
	mciSendString("open BGM.mp3", 0, 0, 0);//加载音乐
	
	int V;
	char v[20];

	
	mciSendString("status BGM.mp3 volume", v, sizeof(v), 0); // 获取当前音量至 volume 字符串中
	V = 50; // 字符串转化为整型
	 
	mciSendString("setaudio BGM.mp3 volume to 200",NULL, 0, NULL); // 执行媒体命令




	mciSendString("play  BGM.mp3 repeat", 0, 0, 0);//循环播放
}


//存档
void save(int(*b)[50], int l, int h, int n, int* s1) {
	ofstream fout;
	fout.open("save.txt");
	fout << l << ' ';
	fout << h << ' ';
	for (int i = 1; i<= h; i++) 
		for (int j = 1; j<= l; j++) 
	fout << b[i][j]<<' ';//写入每一个格子信息
	
	fout << n << ' ';
	fout << mysize << ' ';
	fout.close();
}

//读档
void load(int(*b)[50], int &l, int &h, int &n) {
	ifstream fin;
	fin.open("save.txt");
	fin>> l;
	fin>>h;
	for (int i = 1; i <= h; i++)
		for (int j = 1; j <= l; j++)
			fin >> b[i][j];

	fin >> n;
		fin >> mysize;
		fin.close();
}

//地雷生成
void bomb(int(*b)[50], int l, int h, int n,int y,int x) {//x和y是第一次点击的格子位置
	srand(time(0));//地雷的随机属性
	int i, j, m, ii, jj;//i，j分别为格子的行位置和列位置
	for (m = 1; m <= n; m++) {
		i = rand() % h + 1;
		j = rand() % l + 1;
		if (b[i][j] != 29)
		{
			if (i != y || j != x)
				b[i][j] = 29;
			else m--;//排除第一次点击触雷的情况
		}
		else m--;//排除类重合的情况
	}
	for (j = 1; j <= l ; j++) {
		for (i = 1; i <= h ; i++) {
			if (b[i][j] != 29) {
				for (ii = i - 1; ii <= i + 1; ii++)
					for (jj = j - 1; jj <= j + 1; jj++)
						if (b[ii][jj] == 29)
							b[i][j]++;
			}//数字生成
		}
	}

}

//扫清空格子周围一片
void nullspace(int(*b)[50], int y, int x){
	int yy, xx;
	
		for (yy = y - 1; yy <= y + 1; yy++) {
			for (xx = x - 1; xx <= x + 1; xx++) {
				if(b[yy][xx]>=20&&yy>=1&&xx>=1&& b[yy][xx] < 35) {
				b[yy][xx] = b[yy][xx] - 20;
					if (b[yy][xx] == 0)
					nullspace(b, yy, xx);//连锁反应效果
				}
			}
		}
}

//扫清周围插完旗的数字的格子
void nullflag(int(*b)[50], int y, int x) {
	int yy, xx, f = 0;//f为周围旗子数
	for (yy = y - 1; yy <= y + 1; yy++) {
		for (xx = x - 1; xx <= x + 1; xx++) {
			if (b[yy][xx] >= 40 && yy >= 1 && xx >= 1)
				f++;
		}
	}
	if (f == b[y][x])//判断周围旗子数是否和该格子相同
		for (yy = y - 1; yy <= y + 1; yy++) {
			for (xx = x - 1; xx <= x + 1; xx++) {
				if (b[yy][xx] >= 20 && b[yy][xx] < 35 && yy >= 1 && xx >= 1)
					b[yy][xx] = b[yy][xx] - 20;
				if (b[yy][xx] == 0)
					nullspace(b, yy, xx);//扫清空格子周围一片
			}
			
		}
}

//生成扫雷盘面
void space(int(*b)[50], int l, int h) {
	BeginBatchDraw();//清除闪烁效果
	int x;
	int y;
	IMAGE img1;
	loadimage(&img1, "logo8.png",  false);
	putimage(0, -30, &img1);//加载棋盘后面背景图
	
   //显示每个格子对应图片
	for(x=0;x<= mysize *l- mysize;x=x+ mysize)
	for(y=0;y<= mysize *h-mysize;y=y+mysize)
	{ 
	switch (b[y/ mysize +1][x/ mysize +1])
	{
	case 0://空格
		loadimage(&img1, "0.jpg", mysize, mysize, true);
		putimage(x, y, &img1);
		break;
	
	case 1://周围雷数
		loadimage(&img1, "1.jpg", mysize, mysize, true);
		putimage(x, y, &img1);
		break;
	case 2:
		loadimage(&img1, "2.jpg", mysize, mysize, true);
		putimage(x, y, &img1);
		break;
	case 3:
		loadimage(&img1, "3.jpg", mysize, mysize, true);
		putimage(x, y, &img1);
		break;
	case 4:
		loadimage(&img1, "4.jpg", mysize, mysize, true);
		putimage(x, y, &img1);
		break;
	case 5:
		loadimage(&img1, "5.jpg", mysize, mysize, true);
		putimage(x, y, &img1);
		break;
	case 6:
		loadimage(&img1, "6.png", mysize, mysize, true);
		putimage(x, y, &img1);
		break;
	case 7:
		loadimage(&img1, "7.jpg", mysize, mysize, true);
		putimage(x, y, &img1);
		break;
	case 8:
		loadimage(&img1, "8.jpg", mysize, mysize, true);
		putimage(x, y, &img1);
		break;
	case 9://踩到的地雷
		loadimage(&img1, "9.jpg", mysize, mysize, true);
		putimage(x, y, &img1);
		break;
	case 19://踩雷后显示没有爆炸的雷
		loadimage(&img1, "19.jpg", mysize, mysize, true);
		putimage(x, y, &img1);
		break;
	default:
		if(b[y / mysize + 1][x / mysize + 1]<40){
		loadimage(&img1, "20.jpg", mysize, mysize, true);
		putimage(x, y, &img1);break;}//没有点击的格子
				
				if (b[y / mysize + 1][x / mysize + 1] >= 40){
					loadimage(&img1, "40.jpg", mysize, mysize, true);
				putimage(x, y, &img1);break;}//插旗的格子
				
	}

	
}
	FlushBatchDraw();
EndBatchDraw();
}

//再来一局
void newgame(int(*b)[50],int l, int h,int n,int *s1){
	int i, j;
	
	for (i = 0; i < 50; i++)
		for (j = 0; j < 50; j++)
			b[i][j] = -1;
	for (i = 1; i <= h; i++)
		for (j = 1; j <= l; j++)
			b[i][j] = 20;//重置格子
	
	

	space(b, l, h);
	clearrectangle(0, -30, l * mysize, 0);//清理时间和雷数背景
	
	*s1 = time(NULL);//重置时间
	
}

//点击操作
void click(int(*b)[50],int l,int h,int n,int *s1) {
	ExMessage m;
	int x, y,i,j,k=0;//k判断是否为第一次点击
	 peekmessage(&m);//持续读取鼠标信息
	if(m.message!=0){
	m.y = m.y - 30;//显示原始y轴坐标
	if (m.message == WM_LBUTTONDOWN)//左键
	{

		if (m.y > 0) {//点击位置在棋盘上
			x = m.x - m.x % mysize;
			y = m.y - m.y % mysize;//取整数
			x = x / mysize + 1;
			y = y / mysize + 1;//转换为格子编号
			for (i = 1; i <= h; i++)
				for (j = 1; j <= l; j++)
					if (b[i][j] < 20)
						k = 1;
			if (k == 0) {
				bomb(b, l, h, n, y, x);
			}//k为第一次点击则读取点击位置后生成地雷

			if (b[y][x] >= 20)
				b[y][x] = b[y][x] - 20;//点击后即可显示为原始雷数
			if (b[y][x] == 0)
				nullspace(b, y, x);//若为0则清空周围
			space(b, l, h);//显示点击后的棋盘
		}
		else if (m.x >(mysize * l - 30) * 0.5 && m.x <(mysize * l + 30) * 0.5)
			newgame(b, l, h, n, s1);//刷新游戏
		else 	if (m.x >= (mysize * l - 30) * 0.5-30 && m.x < (mysize * l - 30) * 0.5)
			b[49][48] = 99;//返回
		else if (m.x <= (mysize * l + 30) * 0.5+30 && m.x >= (mysize * l + 30) * 0.5)
			save(b,  l,  h,  n,  s1);//保存
	}
	
	if (m.message == WM_RBUTTONDOWN) {//标旗或快速排雷
		if(m.y>0){
		x = m.x - m.x % mysize;
		y = m.y - m.y % mysize;
		x = x / mysize + 1;
		y = y / mysize + 1;//与上一函数批注原理相同
		
		if (b[y][x] >= 20 && b[y][x] <= 35)
			b[y][x] = b[y][x] + 20;//插旗
		else if (b[y][x] >= 40)
			b[y][x] = b[y][x] - 20;//撤销插旗
		else if (b[y][x] <= 15)
			nullflag(b,y,x);//清空周围
		//num(b, l, h);
		space(b, l, h);//显示盘面
	}}
}
}

/*调试时查看值
void num(int(*b)[50],int l,int h) {
	int i, j;
	for (i = 0; i <=l+1; i++)
	{
		for (j = 0; j <= h+1; j++)
			cout << b[i][j] << "  ";
		cout << endl;
	}
}*/

//输赢判断
void winorlose(int(*b)[50], int l, int h,int n,int*s1) {
	int i, j,t=0,ii,jj;//t为已打开格子的数目
	
	IMAGE img1;
	IMAGE img2;
	IMAGE img3;
	loadimage(&img1, "刷新.jpg", 30, 30, true);
	loadimage(&img2, "炸弹.png",5*mysize, 5 * mysize , true);
	for(i=1;i<=h;i++)
		for (j = 1; j <= l; j++)
		{
			if (b[i][j] == 9){//踩雷输
				b[49][49] = 99;//中止
				
				ii = i;
				jj = j;
				while (jj + 2 > l)
					jj--;
				while (ii+ 2 > h)
					ii--;
				while (ii - 3 < 0)
					ii++;
				while (jj - 3<0)
					jj++;//爆炸特效位置调节
				putimage((jj-3)*mysize ,( ii - 3)* mysize, &img2);
				Sleep(600);//爆炸特效


				for (ii = 1; ii <= h; ii++)
					for (jj = 1; jj <= l; jj++)
						if (b[ii][jj] == 29|| b[ii][jj] == 49)
							b[ii][jj] = 19;//显示所有雷

				space(b, l, h);
				background(b, l, h, n, s1);//显示棋盘和背景
				break;
			}
			if (b[i][j] >= 0 && b[i][j] <= 8)
				t++;}
	if (t == l * h - n) {//除雷外的格子全部打开
		int  s2 = time(NULL);
		b[49][49] = 99;
		Record a0(l, h, n, s2 - *s1);//记录胜利对局的信息的类
		if(l==10&&h==10&&n==10){
			specialRecord a("简单", a0);
		a.recording();}
		else if (l == 16 && h == 16 && n == 40){
			specialRecord a("中等",a0);
		a.recording();}
		else if (l == 30 && h == 16 && n == 99){
			specialRecord a("困难",a0);//判断难度选择是基类还是派生类
		a.recording();}
		else
		a0.recording();//储存于文件中
		
		
		IMAGE img4;
		loadimage(&img4, "logo8.png");
		putimage(0, 0, &img4);
		setbkmode(TRANSPARENT);
		settextstyle(100, 0, _T("隶书"));
		settextcolor(RED);
		outtextxy(mysize*l*0.4, mysize * h *0.4 , _T("win!"));
		//胜利画面
	}
		

}

//上方功能长条
void background(int(*b)[50] ,int l,int h,int n,int* s1 ) {
	BeginBatchDraw();
clearrectangle(l * mysize-75, -30, l * mysize, 0);//刷新时间
	int f=0,i,j;
	char str[10];//未发现的雷数
	
	 for (i = 1; i <= h; i++)
		for (j = 1; j <= l; j++)
			if (b[i][j] >=40)
				f++;
	 sprintf_s(str, " %d", n-f);//类型转换
	

	 //图片文字加载
	IMAGE img1;
	loadimage(&img1, "刷新.jpg", 30, 30, true);
	putimage((mysize *l-30)*0.5, -30, &img1);
	IMAGE img2;
	loadimage(&img2, "返回.jpg", 30, 30, true);
	putimage((mysize * l - 30) * 0.5-30, -30, &img2);
	IMAGE img3;
	loadimage(&img3, "保存.jpg", 30, 30, true);
	putimage((mysize * l + 30) * 0.5, -30, &img3);
	setbkmode(TRANSPARENT);//设置背景色防止文字重叠
	settextstyle(25, 25,"宋体");
	settextcolor(WHITE);//文字样式
	if (n - f >= 1000)
		outtextxy(0, -30, " 999");
	else if(n-f>=100)
	outtextxy(0, -30,str);
	else if (n - f >= 10)
		outtextxy(0, -30, str);
	else if (n - f >= 0)
		outtextxy(0, -30, str);
	int s2 = time(NULL);
	sprintf_s(str, " %d", s2-*s1);
	if (s2 - *s1 >= 1000)
		outtextxy(l * mysize - 4 * 25, -30, " 999");
	else if(s2 - *s1>=100)
	outtextxy(mysize *l-4*25, -30, str);
	else if (s2 - *s1 >= 10)
		outtextxy(mysize * l - 3 * 25, -30, str);
	else if (s2 - *s1 >= 0)
		outtextxy(mysize * l - 2 * 25, -30, str);
	FlushBatchDraw();
	EndBatchDraw();
}

//游戏进行
void game(int(*b)[50], int l, int h, int n, int* s1) {
	while (1) {
		
		
	click(b, l, h, n, s1);
	if (b[49][48] == 99)//返回主界面
		break;
	background(b, l, h, n, s1);
	winorlose(b, l, h, n,s1);	
	
	if (b[49][49] == 99)//已输或已赢
	{
		while (1) {
			ExMessage m;
			m = getmessage();
			m.y = m.y - 30;
			if (m.message == WM_LBUTTONDOWN) {
				if (m.x >(mysize * l - 30) * 0.5 && m.x <= (mysize * l + 30) * 0.5 && m.y <= 0) {
					newgame(b, l, h, n, s1);
					break;
					
				}if (m.x>=(mysize * l - 30) * 0.5 - 30&& m.x < (mysize * l - 30) * 0.5&&m.y<= 0 ) {
					b[49][48] = 99;//返回主界面
					break;
				}
				}
			}
		}
	if (b[49][48] == 99)
		break;
	}
}

//难度选择
void choose(int &l,int &h,int &n,int &end) {
	//美工排版
	IMAGE bb;
	initgraph(900, 500);
	HWND hWnd = GetHWnd();
	SetWindowText(hWnd, "扫雷");
	loadimage(&bb, "./images/choose.png", 900, 500, true);
	putimage(0, 0, &bb);
	setbkmode(TRANSPARENT);
	settextcolor(RGB(79,79,79));
	settextstyle(50, 0, _T("隶书"));	
	outtextxy(450, 50, _T("简单"));
	outtextxy(450, 100, _T("中等"));
	outtextxy(450, 150, _T("困难"));
	outtextxy(425, 200, _T("自定义"));
	outtextxy(450, 250, _T("返回"));


	char s1[10];
	char s2[10];
	char s3[10];
	int t= 0;//打破循环的变量，判断是否选择难度
	ExMessage m;
	while (1) {
		peekmessage(&m);
		if (m.message != 0) {
			if (m.message == WM_LBUTTONDOWN && m.x >= 425 && m.x <= 575&&m.y>=50&&m.y<=300) {
				switch (m.y - m.y % 50) {//点击效果
				
				case 50:
					l = 10;
					h = 10;
					n = 10;
					end = 0;
					mysize = 650 / h;
					break;
					//设置行列数和对应格子尺寸
				case 100:
					l = 16;
					h = 16;
					n = 40;
					end = 0;
					mysize = 650 / h;
					break;
				case 150:
					l = 30;
					h = 16;
					n = 99;
					end = 0;
					mysize = 1200 / l;
					break;
				case 200:
					//弹出对话框
				if(InputBox(s1, 3, "请输入格子列数(10～50)", "列数", "10", 250, 0, false)==true&&InputBox(s2, 3, "请输入格子行数（10～30）", "行数", "10", 250, 0, false) == true&& InputBox(s3, 5, "请输入地雷数（大于0且注意不要超过格子上限）", "地雷数", "10", 250, 0, false) == true)
				{
					l = atoi(s1);	//字符串转整型				
					h = atoi(s2);
					n= atoi(s3);

					//违规值转换
					if (l > 50)
						l = 50;
					if (l <10)
						l = 10;
					if (h < 10)
						h = 10;
					if (h> 30)
						h= 30;
					if (n <= 0)
						n = 1;
					if (n > l*h)
						n = l*h-1;

					//格子尺寸判断
					if (l > 1.5 * h)
						mysize = 1000 / l;
					else
						mysize = 650/ h;
					
					end = 0;
					

					break;}
				else t = 1;//未点击或取消自定义则保持界面
				break;
				case 250:
					closegraph();//关闭窗口，返回主页面
									
				}
				if (t == 0) {
                       break;//返回
				}
				t = 0;//重置t
			}
			}
		}
	}

//开始界面
void  start(int &l,int &h,int&n,int &end) {
	int start = 1;
	int x;
	char p[30];
	initgraph(900, 500);//生成窗口

	HWND hWnd = GetHWnd();
	SetWindowText(hWnd, "扫雷");//窗口标题设置

	music();//播放bgm
	
	//随机生成背景图片
	IMAGE logo;
	srand(time(0));
	x = rand()%8+1;
	sprintf_s(p,"./images/logo%d.png",x);
	loadimage(&logo, p, 900, 500, true);
	putimage(0, 0, &logo);

	//设置字体样式
	settextcolor(WHITE);
	settextstyle(50, 0, _T("隶书"));

	//规则的图片
	IMAGE img1;
	loadimage(&img1, "./images/规则.png", 750, 500, false);

	MOUSEMSG mouse;
	while (1)
	{
		HWND hWnd = GetHWnd();
		SetWindowText(hWnd, "扫雷");//窗口标题设置

		mouse = GetMouseMsg();//获得鼠标信息
		switch (mouse.uMsg)//鼠标信息判断
		{
		case WM_MOUSEMOVE://移动
			if (mouse.x >= 30 && mouse.x <= 130 && mouse.y >= 30 && mouse.y < 80)
			{

				putimage(150, 0, &img1);//放规则图片
			}
			
			else 
			{
				putimage(0, 0, &logo);//放背景图


				//文字按键
				settextstyle(50, 0, _T("隶书"));
				setbkmode(TRANSPARENT);
				outtextxy(30, 30, _T("规则"));
				outtextxy(30, 80, _T("新局"));
				outtextxy(30, 130, _T("继续"));
				outtextxy(30, 180, _T("记录"));
			}break;
		case WM_LBUTTONDOWN:
			if (mouse.x >= 30 && mouse.x <= 130 && mouse.y >80 && mouse.y < 130)//进入选择难度界面
			{
				choose(l,h,n,end);
				if (end == 0)//选择游戏
					break;

				//选择返回主界面
				initgraph(900, 500);
				putimage(0, 0, &logo);
			}
			if (mouse.x >= 30 && mouse.x <= 130 && mouse.y > 130 && mouse.y <180)
			{
				end = 2;//继续存档了的游戏
				break;
			}
			if (mouse.x >= 30 && mouse.x <= 130 && mouse.y >= 180 && mouse.y < 230) 
			record();//打开游戏记录
			

		}
		if (end == 0 || end == 2)
			break;//推出循环
	}
}

int main()
{
	 int s;
	int* s1 = &s;//时间

	int l, h, n,i,j,end=1;
    int b[50][50];//地雷信息

	IMAGE AA;
	loadimage(&AA, "./images/game1.png", 950, 600, false);//游戏进行时背景
	while (1) {
	start(l,h,n,end);


	for(i=0;i<50;i++)
	for (j = 0;  j < 50;j++)
	b[i][j] = -1;
	for (i = 1; i <= h; i++)
	for (j = 1; j <= l; j++)
	b[i][j] =20;
	//b的初始化


	s= time(NULL);//设置时间
	if (end == 2) {//继续保存的游戏
		load(b, l, h, n);//加载对局信息
		s = 1652000000;//实现最大时间
	}
	initgraph(l*mysize, h*mysize+30);
	HWND hWnd = GetHWnd();
	SetWindowText(hWnd, "扫雷");
	 setorigin(0,30 ); //重新定义原点，方便格子坐标计算
	putimage(0, -30, &AA);
	 background(b,l,h,n,s1);
	space(b, l, h);
	game(b, l, h, n, s1);
	end = 1;//重置end
	}

	
}
