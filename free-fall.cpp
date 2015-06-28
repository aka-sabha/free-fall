#include<iostream.h>
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<process.h>
#include<dos.h>
#include<math.h>

#define LEFT -1
#define RIGHT 1
#define DOWN 2

#define CLASSIC 1
#define SURVIVAL 2

#define NORMAL 1
#define HAPPY 2
#define SAD 3

char CaptStr[]="Points : ";
char TempString[20]={0};

int Lengths[8]={3,4,4,5,5,4,4,3};
int YPosArr[8]={272,188,104,20,20,104,188,272};
int DirArr[8]={RIGHT,RIGHT,RIGHT,RIGHT,LEFT,LEFT,LEFT,LEFT};

int Lives=5,Points,Diff=2,DelayTime=400,Mode=CLASSIC,DelayCountLimit,PointsLimit;
int DelayModulo=5,Time=0;

class Egg
{

private:
	int XRadius,YRadius,XCentre,YCentre;
	int Color;
	int Dir;
	int Broke;
	
public:

	void InitEgg(int,int,int);

	void DrawEgg();
	void DrawEgg(int);

	void MoveEgg();
	void MoveLeft();
	void MoveRight();
	void MoveDown();

	void ChangeDir(int);
	int GetXCentre();
	int GetYCentre();
	int GetColor();

	void SetBroke();
	friend void InitializeTheGame();
	friend void ResetGame();

	friend void MainMenu();

}*Life[5];

	void Egg :: InitEgg(int X,int Y,int D)
	{
		XRadius=5;
		YRadius=7;

		randomize();
		Color=(rand()%2) + 11;

		XCentre=X;
		YCentre=Y;
		Dir=D;
		Broke=0;
		//Text=0;
	}
	void Egg :: DrawEgg()
	{
		setfillstyle(1,Color);
		fillellipse(XCentre,YCentre,XRadius,YRadius);

		setcolor(Color);
		ellipse(XCentre,YCentre,0,360,XRadius,YRadius);

		if(Broke)
		{
			setcolor(RED);
			line(XCentre-XRadius,YCentre-YRadius,XCentre+XRadius,YCentre+YRadius);
			line(XCentre+XRadius,YCentre-YRadius,XCentre-XRadius,YCentre+YRadius);
		}
	}

	void Egg :: DrawEgg(int CopyColor)
	{
		setfillstyle(1,CopyColor);
		fillellipse(XCentre,YCentre,XRadius,YRadius);

		setcolor(CopyColor);
		ellipse(XCentre,YCentre,0,360,XRadius,YRadius);

		setcolor(CopyColor);
		line(XCentre-XRadius,YCentre-YRadius,XCentre+XRadius,YCentre+YRadius);
		line(XCentre+XRadius,YCentre-YRadius,XCentre-XRadius,YCentre+YRadius);
	}

	void Egg :: SetBroke()
	{
		Broke=1;

		DrawEgg();
	}

	void Egg :: MoveLeft()
	{
		DrawEgg(0);
		XCentre-=20;
		DrawEgg();
	}

	void Egg :: MoveRight()
	{
		DrawEgg(0);
		XCentre+=20;
		DrawEgg();
	}

	void Egg :: MoveDown()
	{
		DrawEgg(0);
		YCentre+=28;
		DrawEgg();
	}

	void Egg :: MoveEgg()
	{
		if(Dir==LEFT)
			MoveLeft();
		else if(Dir==RIGHT)
			MoveRight();
		else if(Dir==DOWN)
			MoveDown();
	}

	void Egg :: ChangeDir(int D)
	{
		Dir=D;
	}

	int Egg :: GetXCentre()
	{
		return XCentre;
	}

	int Egg :: GetYCentre()
	{
		return YCentre;
	}

	int Egg :: GetColor()
	{
		return Color;
	}

class Nest
{
	Egg *E[5];
	int Front,Rear;
	int Length;

	int XPos,YPos,Dir;

public:

	void EnQueue(Egg*);
	void DeQueue();

	void AddEgg();
	void InitNest(int,int,int);
	void MoveEggs();
	void DrawNest();
	friend void DetectCapture();

	friend void ResetGame();

	friend void MainMenu();

}N[8];

	void Nest :: InitNest(int L,int Y,int D)
	{
		Length=L;
		Front=0;
		Rear=-1;
		for(int i=0;i<5;i++)
			E[i]=NULL;
		YPos=Y;
		Dir=D;
		if(Dir==RIGHT)
			XPos=getmaxx()/2 - 130;
		else
			XPos=getmaxx()/2 + 130;
	}

	void Nest :: AddEgg()
	{
		Egg* NewEgg=new Egg;
		NewEgg->InitEgg(XPos+(20*Dir),YPos,Dir);
		EnQueue(NewEgg);
	}

	void Nest :: MoveEggs()
	{
		for(int i=0;i<5;i++)
		{
			if(E[i]!=NULL)
			{
				if(E[i]->GetYCentre() >= 384)
					DeQueue();

				E[i]->MoveEgg();

				if(Dir==RIGHT && (E[i]->GetXCentre() > (XPos+5)+(Length*20)))
					E[i]->ChangeDir(DOWN);
				else if(Dir==LEFT && (E[i]->GetXCentre() < (XPos-5- Length*20)))
					E[i]->ChangeDir(DOWN);

			}

		}
	}
	void Nest :: DrawNest()
	{
		setfillstyle(2,LIGHTGREEN);
		bar(XPos,YPos-10,XPos+(5*Dir),YPos+10);

		setfillstyle(5,GREEN);
		bar(XPos,YPos+10,(XPos+(5*Dir))+(Length*20*Dir),YPos+15);
	}

	void Nest :: EnQueue(Egg* TempEgg)
	{
		Rear=(Rear+1)%5;
		E[Rear]=TempEgg;
	}

	void Nest :: DeQueue()
	{
		Egg* TempEgg=E[Front];
		TempEgg->DrawEgg(0);
		delete E[Front];
		E[Front]=NULL;
		Front=(Front+1)%5;

	}

class Basket
{
	int Radius;
	int XPos,YPos;
	int BreakLimit;
	int Emotion;

public:

	Basket();
	void DrawBasket();
	void DrawBasket(int);

	void MoveLeft();
	void MoveRight();

	int GetXPos();
	int GetYPos();

	void SetEmotion(int);

	friend void ResetGame();
	friend void MainMenu();

}*B;

	Basket :: Basket()
	{
		Radius=12;
		XPos=getmaxx()/2 -10;
		YPos=373;
		BreakLimit=1;
		Emotion=NORMAL;
	}

	void Basket :: DrawBasket()
	{

		if(BreakLimit)
		{
			for(int i=getmaxx()/2 -130;i<getmaxx()/2 +130 ; i+=8)
			{
				setcolor(BLUE);
				line(i,YPos+27 -7,i+5,YPos+27 -7);
			}
		}

		setcolor(LIGHTMAGENTA);
		rectangle(XPos-Radius+2,YPos,XPos+Radius-2,YPos+20);

		setfillstyle(1,YELLOW);
		fillellipse(XPos,YPos,Radius,2);

		setcolor(LIGHTMAGENTA);
		ellipse(XPos,YPos,0,360,Radius,2);

		setfillstyle(1,WHITE);
		fillellipse(XPos,YPos+27,5,5);

		setcolor(WHITE);
		ellipse(XPos,YPos+27,0,360,5,5);

		setfillstyle(1,LIGHTGRAY);
		bar(XPos-13,YPos+27 +5,XPos+13,YPos+27 +10);
		bar(XPos-13,YPos+27 -5,XPos-9,YPos+27 +10);
		bar(XPos+9,YPos+27 -5,XPos+13,YPos+27 +10);
		bar(XPos-13,YPos+27 -7,XPos-7,YPos+27 -5);
		bar(XPos+7,YPos+27 -7,XPos+13,YPos+27 -5);
		bar(XPos-7,YPos+27 +25,XPos-1,YPos+27 +27);
		bar(XPos+1,YPos+27 +25,XPos+7,YPos+27 +27);

		setfillstyle(1,LIGHTBLUE);
		bar(XPos-5,YPos+27 +5,XPos+5,YPos+27 +15);
		bar(XPos-7,YPos+27 +5,XPos+7,YPos+27 +10);

		setfillstyle(1,LIGHTMAGENTA);
		bar(XPos-5,YPos+27 +15,XPos+5,YPos+27 +18);
		bar(XPos-5,YPos+27 +18,XPos-1,YPos+27 +25);
		bar(XPos+5,YPos+27 +18,XPos+1,YPos+27 +25);

		setfillstyle(1,0);
		fillellipse(XPos-2,YPos+27 -2,1,1);
		fillellipse(XPos+2,YPos+27 -2,1,1);

		setcolor(0);
		ellipse(XPos-2,YPos+27 -2,0,360,0,0);
		ellipse(XPos+2,YPos+27 -2,0,360,0,0);

		setcolor(0);

		if(Emotion == NORMAL)
			ellipse(XPos,YPos+27 +1,180,0,3,1);
		else if(Emotion == HAPPY)
		{
			line(XPos-3,YPos+27 +1,XPos+3,YPos+27 +1);
			ellipse(XPos,YPos+27 +1,180,0,3,3);
		}
		else if(Emotion == SAD)
			ellipse(XPos,YPos+27 +2,0,180,3,1);

	}

	void Basket :: DrawBasket(int CopyColor)
	{
		setcolor(CopyColor);
		rectangle(XPos-Radius+2,YPos,XPos+Radius-2,YPos+20);

		setfillstyle(1,CopyColor);
		fillellipse(XPos,YPos,Radius,2);

		setcolor(CopyColor);
		ellipse(XPos,YPos,0,360,Radius,2);

		setfillstyle(1,CopyColor);
		fillellipse(XPos,YPos+27,5,5);

		setcolor(CopyColor);
		ellipse(XPos,YPos+27,0,360,5,5);

		setfillstyle(1,CopyColor);
		bar(XPos-13,YPos+27 +5,XPos+13,YPos+27 +10);
		bar(XPos-13,YPos+27 -5,XPos-9,YPos+27 +10);
		bar(XPos+9,YPos+27 -5,XPos+13,YPos+27 +10);
		bar(XPos-13,YPos+27 -7,XPos-7,YPos+27 -5);
		bar(XPos+7,YPos+27 -7,XPos+13,YPos+27 -5);
		bar(XPos-7,YPos+27 +25,XPos-1,YPos+27 +27);
		bar(XPos+1,YPos+27 +25,XPos+7,YPos+27 +27);

		setfillstyle(1,CopyColor);
		bar(XPos-5,YPos+27 +5,XPos+5,YPos+27 +15);
		bar(XPos-7,YPos+27 +5,XPos+7,YPos+27 +10);

		setfillstyle(1,CopyColor);
		bar(XPos-5,YPos+27 +15,XPos+5,YPos+27 +18);
		bar(XPos-5,YPos+27 +18,XPos-1,YPos+27 +25);
		bar(XPos+5,YPos+27 +18,XPos+1,YPos+27 +25);
	}

	void Basket :: MoveLeft()
	{
		DrawBasket(0);
		if(XPos > (getmaxx()/2 -50))
			XPos-=20;
		DrawBasket();
	}

	void Basket :: MoveRight()
	{
		DrawBasket(0);
		if(XPos < (getmaxx()/2 +50))
			XPos+=20;
		DrawBasket();
	}

	int Basket :: GetXPos()
	{
		return XPos;
	}

	int Basket :: GetYPos()
	{
		return YPos;
	}


	void Basket :: SetEmotion(int Emo)
	{
		DrawBasket(0);
		Emotion=Emo;
		DrawBasket();
	}

void PrintPoints()
{
	setfillstyle(1,DARKGRAY);
	bar(30,getmaxy()-25,80,getmaxy()-5);

	setcolor(GREEN);
	rectangle(30,getmaxy()-25,80,getmaxy()-5);

	strcpy(TempString,"");
	char TempIntStr[3]={0};
	itoa(Points,TempIntStr,10);
	strcat(TempString,TempIntStr);

	setcolor(15);
	settextstyle(0,0,1);
	outtextxy(45,getmaxy()-15,TempString);

}

void DetectCapture()
{
	int EggColor=0;

	for(int i=0;i<8;i++)
	{
		if(B->GetYPos() ==  N[i].E[N[i].Front]->GetYCentre() -11)
		{
			if(B->GetXPos() == N[i].E[N[i].Front]->GetXCentre())
			{
				EggColor=N[i].E[N[i].Front]->GetColor();

				if(EggColor==LIGHTRED)
					Points+=2;
				else
					Points++;

				PrintPoints();

				B->SetEmotion(HAPPY);

			}

			else if(B->GetXPos() != N[i].E[N[i].Front]->GetXCentre())
			{
				N[i].E[N[i].Front]->SetBroke();
				Lives--;
				B->SetEmotion(SAD);
				Life[Lives]->SetBroke();

				setbkcolor(LIGHTRED);
				delay(50);
				setbkcolor(BLACK);
			}


		}

	}

}



void MainMenu();

void DrawBoard()
{
	cleardevice();

	setcolor(GREEN);
	line(getmaxx()/2 - 130,0,getmaxx()/2 -130,430);
	line(getmaxx()/2 + 130,0,getmaxx()/2 +130,430);
	line(getmaxx()/2 -130,430,getmaxx()/2 +130,430);

	for(int i=0;i<8;i++)
		N[i].DrawNest();

	B->DrawBasket();

	setcolor(GREEN);
	settextstyle(0,0,1);
	outtextxy(30,getmaxy()-35,"Points");


	setfillstyle(1,DARKGRAY);
	bar(30,getmaxy()-25,80,getmaxy()-5);

	bar(22,5,38,110);
	bar(getmaxx()-135,getmaxy()-20,getmaxx()-5,getmaxy()-5);
	bar(getmaxx()-80,3,getmaxx()-3,15);

	setcolor(GREEN);
	rectangle(30,getmaxy()-25,80,getmaxy()-5);
	rectangle(22,5,38,110);
	rectangle(getmaxx()-135,getmaxy()-20,getmaxx()-5,getmaxy()-5);
	rectangle(getmaxx()-80,3,getmaxx()-3,15);

	for(i=0;i<5;i++)
	{
		Life[i]->DrawEgg();
	}

	strcpy(TempString,"");
	char TempIntStr[3]={0};
	itoa(Points,TempIntStr,10);
	strcat(TempString,TempIntStr);

	setcolor(GREEN);
	settextstyle(0,1,1);
	outtextxy(15,35,"Lives");

	setcolor(15);
	settextstyle(0,0,1);
	outtextxy(45,getmaxy()-15,TempString);

	outtextxy(getmaxx()-130,getmaxy()-15,"Ctrl + C : Menu");
	outtextxy(getmaxx()-75,5,"P : Pause");

}

void InitializeTheGame()
{
	Lives=5;
	Points=0;
	DelayTime=500 - (Diff-1)*100;

	DelayCountLimit=(30000)/DelayTime;

	PointsLimit=30 +(Diff-1)*10;

	DelayModulo=pow(Diff,2)+1;
	Time=0;

	for(int i=0;i<8;i++)
	{
		N[i].InitNest(Lengths[i],YPosArr[i],DirArr[i]);
	}
	B=new Basket;

	for(i=0;i<5;i++)
	{
		Life[i]=new Egg;
		Life[i]->InitEgg(30,18+(20*i),LEFT);
		Life[i]->Color=WHITE;
	}
}

void ResetGame()
{
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<5;j++)
		{
			delete N[i].E[j];
			N[i].E[j]=NULL;
		}
	}

	for(i=0;i<5;i++)
	{
		delete Life[i];
		Life[i]=NULL;
	}

	Lives=5;
	Points=0;
	Diff=2;
	DelayTime=400;
	DelayCountLimit=75;
	PointsLimit=40;

	DelayModulo=5;
	Time=0;

	Mode=CLASSIC;

	delete B;
	B=NULL;
}

void Game()
{
	int i=0;
	char ch,EndChar;
	int DelayCount=0;

	InitializeTheGame();
	DrawBoard();

	do
	{

		if(kbhit())
		{
			ch=getch();
			if(ch==(char)77)
				{
					B->MoveRight();
				}
			else if(ch==(char)75)
				{
					B->MoveLeft();
				}
			else if(ch==(char)3)
			{
				goto BackToMenu;
			}
			else if(ch=='p')
			{
					int c2;
					do
					{
					c2=getch();
					}while(c2!='p');
				}

			B->SetEmotion(NORMAL);
		}
		else
		{

			if(DelayCount%DelayModulo == 0)
			{
				Time+=Diff;
				setcolor(LIGHTGREEN);
				outtextxy(40,getmaxy()/2 -15,"Time");
				rectangle(30,getmaxy()/2 -20,70,getmaxy()/2 +10);
				setfillstyle(1,DARKGRAY);
				bar(30,getmaxy()/2 -20,70,getmaxy()/2 +10);
				gotoxy(5,15);
				cout<<Time;
			}

			if((Mode==CLASSIC || Mode==SURVIVAL) && DelayCount%5==0 && ((DelayCount< DelayCountLimit) || Points<PointsLimit))
			{
				randomize();
				N[rand()%8].AddEgg();
			}

			else if(Mode==CLASSIC && Points>=PointsLimit)
			{
				break;
			}

			else if(Mode==SURVIVAL && (DelayCount>=DelayCountLimit))
			{
				break;
			}


			for(int i=0;i<8;i++)
				N[i].MoveEggs();

			DetectCapture();

			delay(DelayTime);
			DelayCount++;

		}


	}while(Lives>0);

	if(Lives)
	{
		do
		{
			setcolor(i%15+1);
			settextstyle(3,1,4);
			outtextxy(getmaxx()-75,150,"You Won !!");
			i++;
			delay(250);
			if(kbhit())
			{
				EndChar=getch();
				if(EndChar==(char)13)
					break;
			}
		}while(1);

	}
	else
	{
		do
		{
			setcolor(i%3+10);
			settextstyle(3,1,4);
			outtextxy(75,220,"GAME");
			outtextxy(getmaxx()-75,220,"OVER");
			i++;

			delay(500);
			if(kbhit())
			{
				EndChar=getch();
				if(EndChar==(char)13)
					break;
			}

		}while(1);

	}
	
BackToMenu:
	cleardevice();
	ResetGame();

}

void MainMenu()
{
	//  GUI Menu

InitMenu :

	setfillstyle(1,0);
	bar(getmaxx()-140,3,getmaxx()-3,15);
	setcolor(0);
	rectangle(getmaxx()-140,3,getmaxx()-3,15);

	int ManPos=1,TempManPos=1,ModePos=1,TempModePos=1,LevelPos=2,TempLevelPos=2;

	char ch;
	int i=0;

	Egg *Play,*Level,*Help,*Exit;
	Play=new Egg;
	Level=new Egg;
	Help=new Egg;
	Exit=new Egg;

	Egg *Classic,*Survival;
	Classic=new Egg;
	Survival=new Egg;

	Egg *Easy,*Medium,*Difficult;
	Easy=new Egg;
	Medium=new Egg;
	Difficult=new Egg;

	Egg *ModeEggs[2]={Classic,Survival};
	Egg *LevelEggs[3]={Easy,Medium,Difficult};

	Basket Selector;

	Egg *MenuEggs[4]={Play,Level,Help,Exit};

	char *MenuEggsText[4]={"Play","Level","Help","Exit"};
	char *ModeEggsText[2]={"Classic","Survival"};
	char *LevelEggsText[3]={"Easy","Medium","Hard"};

	Nest VideoNest[4];
	Egg VideoEgg[3];
	Basket VideoBasket;

	int NestYPos[4]={204,120,120,204};
	int NestLengths[4]={3,5,5,3};
	int NestDirs[4]={RIGHT,RIGHT,LEFT,LEFT};

	ManPos=TempManPos;
	ModePos=TempModePos;
	LevelPos=TempLevelPos;

Video :
	for(i=0;i<4;i++)
	{
		VideoNest[i].InitNest(NestLengths[i],NestYPos[i],NestDirs[i]);
	}

	VideoBasket.XPos=getmaxx()/2 -10;
	VideoBasket.YPos=277;

	VideoEgg[0].InitEgg(getmaxx()/2 -130 +80,NestYPos[0]+56,DOWN);
	VideoEgg[1].InitEgg(getmaxx()/2 -130 +120,NestYPos[0]-56,DOWN);
	VideoEgg[2].InitEgg(VideoNest[3].XPos -20,VideoNest[3].YPos,LEFT);

	VideoEgg[0].Color=LIGHTCYAN;
	VideoEgg[1].Color=LIGHTRED;
	VideoEgg[2].Color=LIGHTCYAN;

	delay(500);

	setcolor(GREEN);
	rectangle(getmaxx()/2 -130,105,getmaxx()/2+130,333);

	for(i=0;i<4;i++)
	{
		VideoNest[i].DrawNest();
		if(i<3)
			VideoEgg[i].DrawEgg();
	}
	VideoBasket.DrawBasket();

	delay(1000);

	for(i=0;i<3;i++)
		VideoEgg[i].MoveEgg();

	VideoBasket.MoveLeft();
	VideoEgg[0].SetBroke();
	VideoBasket.SetEmotion(SAD);

	delay(1000);

	VideoEgg[0].DrawEgg(0);
	VideoBasket.MoveLeft();
	VideoBasket.SetEmotion(NORMAL);
	for(i=1;i<3;i++)
	{
		VideoEgg[i].MoveEgg();
	}
	delay(1000);

	VideoBasket.MoveRight();

	VideoEgg[0].InitEgg(VideoNest[1].XPos+20,VideoNest[1].YPos,RIGHT);

	for(i=0;i<3;i++)
	{
		VideoEgg[i].MoveEgg();
	}

	delay(1000);

	VideoEgg[2].ChangeDir(DOWN);
	VideoBasket.MoveRight();

	for(i=0;i<6;i++)
	{
		VideoEgg[i%3].MoveEgg();
		if(i%3 == 2)
			delay(1000);
	}

	VideoBasket.SetEmotion(HAPPY);


	setbkcolor(YELLOW);
	delay(50);
	setbkcolor(BLACK);

	setcolor(CYAN);
	settextstyle(3,0,4);
	outtextxy(getmaxx()/2 -120,50,"EGG CATCHER !!");

	delay(500);

MainMenu :

	ModePos=Mode;
	LevelPos=Diff;

	setfillstyle(1,0);
	bar(getmaxx()-140,3,getmaxx()-3,15);
	setcolor(0);
	rectangle(getmaxx()-140,3,getmaxx()-3,15);

	Selector.Radius=15;
	Selector.XPos=getmaxx()*ManPos /5;
	Selector.YPos=getmaxy()-60;
	Selector.BreakLimit=0;

	Play->InitEgg(getmaxx()/5 - 6*32,getmaxy()-90,0);
	Play->XRadius=15;
	Play->YRadius=20;
	Play->Color=LIGHTBLUE;
	Play->Dir=RIGHT;

	Level->InitEgg((getmaxx()*2)/5 - 6*32,getmaxy()-90,0);
	Level->XRadius=15;
	Level->YRadius=20;
	Level->Color=LIGHTBLUE;
	Level->Dir=RIGHT;

	Help->InitEgg((getmaxx()*3)/5 + 6*32,getmaxy()-90,0);
	Help->XRadius=15;
	Help->YRadius=20;
	Help->Color=LIGHTBLUE;
	Help->Dir=LEFT;

	Exit->InitEgg((getmaxx()*4)/5 + 6*32,getmaxy()-90,0);
	Exit->XRadius=15;
	Exit->YRadius=20;
	Exit->Color=LIGHTBLUE;
	Exit->Dir=LEFT;

	setfillstyle(1,0);
	bar(0,335,getmaxx(),getmaxy());

	for(i=1;i<=24;i++)
	{
		for(int j=0;j<4;j++)
		{
			MenuEggs[j]->DrawEgg(0);
			MenuEggs[j]->XCentre+=(8*MenuEggs[j]->Dir);
			MenuEggs[j]->DrawEgg();
		}
		delay(50);
	}

	for(i=0;i<4;i++)
	{
		setfillstyle(1,YELLOW);
		bar(MenuEggs[i]->XCentre-20,MenuEggs[i]->YCentre-10,MenuEggs[i]->XCentre+25,MenuEggs[i]->YCentre+10);

		setcolor(LIGHTRED);
		settextstyle(0,0,1);
		outtextxy(MenuEggs[i]->XCentre -MenuEggs[i]->XRadius ,MenuEggs[i]->YCentre -5,MenuEggsText[i]);
	}

	Selector.Emotion=HAPPY;
	Selector.DrawBasket();

	setfillstyle(1,DARKGRAY);
	bar(getmaxx()-140,3,getmaxx()-3,15);
	setcolor(GREEN);
	rectangle(getmaxx()-140,3,getmaxx()-3,15);
	settextstyle(0,0,1);
	outtextxy(getmaxx()-130,5,"   Esc : Exit");

	do
	{
		Selector.DrawBasket();

		ch=getch();

		Selector.DrawBasket(0);

		if(ch==(char)75)
		{
			ManPos=(ManPos+2)%4 +1;
		}
		else if(ch==(char)77)
		{
			ManPos=(ManPos+4)%4 +1;
		}
		else if(ch==(char)27)
		{
			goto End;
		}

		Selector.XPos=(getmaxx()*ManPos)/5;

	}while(ch!=(char)13);

	setfillstyle(1,0);
	bar(0,335,getmaxx(),getmaxy());
	delay(500);

	Classic->InitEgg(210 - 6*40,getmaxy()-90,0);
	Classic->XRadius=15;
	Classic->YRadius=20;
	Classic->Color=LIGHTMAGENTA;
	Classic->Dir=RIGHT;

	Survival->InitEgg(420 +  6*40,getmaxy()-90,0);
	Survival->XRadius=15;
	Survival->YRadius=20;
	Survival->Color=LIGHTMAGENTA;
	Survival->Dir=LEFT;

	Easy->InitEgg(getmaxx()/4 - 8*25,getmaxy()-90,0);
	Easy->XRadius=15;
	Easy->YRadius=20;
	Easy->Color=YELLOW;
	Easy->Dir=RIGHT;

	Medium->InitEgg(getmaxx()/2,getmaxy()-90,0);
	Medium->XRadius=15;
	Medium->YRadius=20;
	Medium->Color=YELLOW;
	Medium->Dir=0;

	Difficult->InitEgg((getmaxx()*3)/4  + 8*25,getmaxy()-90,0);
	Difficult->XRadius=15;
	Difficult->YRadius=20;
	Difficult->Color=YELLOW;
	Difficult->Dir=LEFT;

	setfillstyle(1,DARKGRAY);
	bar(getmaxx()-140,3,getmaxx()-3,15);
	setcolor(GREEN);
	rectangle(getmaxx()-140,3,getmaxx()-3,15);
	settextstyle(0,0,1);
	outtextxy(getmaxx()-130,5,"Esc : Prev Menu");

	if(ManPos == 1)
	{
		for(i=1;i<=40;i++)
		{
			for(int j=0;j<2;j++)
			{
					ModeEggs[j]->DrawEgg(0);
					ModeEggs[j]->XCentre+=(6*ModeEggs[j]->Dir);
					ModeEggs[j]->DrawEgg();
			}
		}
		delay(100);

		for(i=0;i<2;i++)
		{
			setfillstyle(1,YELLOW);
			bar(ModeEggs[i]->XCentre-35,ModeEggs[i]->YCentre-10,ModeEggs[i]->XCentre+40,ModeEggs[i]->YCentre+10);

			setcolor(BLUE);
			settextstyle(0,0,1);
			outtextxy(ModeEggs[i]->XCentre-25,ModeEggs[i]->YCentre-5,ModeEggsText[i]);
		}
		delay(50);

		Selector.XPos=(getmaxx()*ModePos)/3;

		do
		{
			Selector.DrawBasket();
			ch=getch();

			Selector.DrawBasket(0);

			if(ch==(char)75)
			{
				ModePos=ModePos%2 +1;
			}
			else if(ch==(char)77)
			{
				ModePos=(ModePos+2)%2 +1;
			}
			else if(ch==(char)27)
			{
				goto MainMenu;
			}

			Selector.XPos=(getmaxx()*ModePos)/3;

		}while(ch!=(char)13);

		Mode=ModePos;

		Game();
		cleardevice();

		setcolor(GREEN);
		rectangle(getmaxx()/2 -130,105,getmaxx()/2+130,333);

		for(i=0;i<4;i++)
		{
			VideoNest[i].DrawNest();
			if(i!=3)
				VideoEgg[i].DrawEgg();
		}
		VideoBasket.DrawBasket();

		setcolor(CYAN);
		settextstyle(3,0,4);
		outtextxy(getmaxx()/2 -120,50,"EGG CATCHER !!");

		delay(250);

		goto MainMenu;


	}
	else if(ManPos == 2)
	{
		LevelEggs[1]->DrawEgg();

		for(i=1;i<=25;i++)
		{
			for(int j=0;j<3;j++)
			{
				if(j!=1)
				{
					LevelEggs[j]->DrawEgg(0);
					LevelEggs[j]->XCentre+=(8*LevelEggs[j]->Dir);
					LevelEggs[j]->DrawEgg();
				}
			}
		}
		delay(100);

		for(i=0;i<3;i++)
		{
			setfillstyle(1,LIGHTGREEN);
			bar(LevelEggs[i]->XCentre-30,LevelEggs[i]->YCentre-10,LevelEggs[i]->XCentre+30,LevelEggs[i]->YCentre+10);

			setcolor(BLACK);
			settextstyle(0,0,1);
			outtextxy(LevelEggs[i]->XCentre-20,LevelEggs[i]->YCentre-5,LevelEggsText[i]);
		}
		delay(50);

		Selector.XPos=(getmaxx()*LevelPos)/4;

		do
		{
			Selector.DrawBasket();
			ch=getch();

			Selector.DrawBasket(0);

			if(ch==(char)75)
			{
				LevelPos=(LevelPos+1)%3 +1;
			}
			else if(ch==(char)77)
			{
				LevelPos=(LevelPos+3)%3 +1;
			}
			else if(ch==(char)27)
			{
				goto MainMenu;
			}

			Selector.XPos=(getmaxx()*LevelPos)/4;

		}while(ch!=(char)13);

		Diff=LevelPos;

		goto MainMenu;

	}
	else if(ManPos == 3)
	{
		cleardevice();

		setfillstyle(1,DARKGRAY);
		bar(getmaxx()-140,3,getmaxx()-3,15);
		setcolor(GREEN);
		rectangle(getmaxx()-140,3,getmaxx()-3,15);
		settextstyle(0,0,1);
		outtextxy(getmaxx()-130,5,"Esc : Prev Menu");

		setcolor(YELLOW);
		settextstyle(4,0,4);
		outtextxy(265,40,"Help");

		setcolor(LIGHTBLUE);
		line(265,80,320,80);
		setcolor(LIGHTRED);
		line(265,83,320,83);

		Page1:

		setfillstyle(1,0);
		bar(0,100,getmaxx(),getmaxy());

		setcolor(GREEN);
		settextstyle(0,1,1);
		outtextxy(20,110,"Page 1");

		setcolor(CYAN);
		settextstyle(3,0,1);
		outtextxy(100,100,"Egg Catcher is an Arcade game..");

		setcolor(LIGHTBLUE);
		outtextxy(100,130,"The game is to Save the falling eggs");
		outtextxy(100,160,"In Each game, You'd have 5 lives");

		setcolor(LIGHTMAGENTA);
		outtextxy(100,200,"The game can be played in  THREE difficulty levels..");
		outtextxy(100,230,"Namely ");

		setcolor(YELLOW);
		outtextxy(180,230,"Easy, Medium(Default), Hard");

		setcolor(LIGHTRED);
		outtextxy(100,270,"and in Two Game Modes..");
		outtextxy(100,300,"Namely ");

		setcolor(YELLOW);
		outtextxy(180,300,"Classic(Default) and Survival");

		setcolor(LIGHTGREEN);
		outtextxy(100,340,"Controls : ");

		setcolor(LIGHTMAGENTA);
		outtextxy(150,370,"Left Arrow - To Move The Basket Left");
		outtextxy(150,400,"Right Arrow - To Move The Basket Right");

		setcolor(LIGHTGREEN);
		settextstyle(2,0,0);
		outtextxy(5,getmaxy()-15,"* Default - Starting the Game without Setting any modes or difficulty level");


		setfillstyle(1,DARKGRAY);
		bar(getmaxx()-140,getmaxy()-30,getmaxx()-3,getmaxy()-15);
		setcolor(GREEN);
		rectangle(getmaxx()-140,getmaxy()-30,getmaxx()-3,getmaxy()-15);
		settextstyle(0,0,1);
		outtextxy(getmaxx()-130,getmaxy()-25,"Down Arr: Page 2");

		goto Navigate;

		Page2:

		setfillstyle(1,0);
		bar(0,100,getmaxx(),getmaxy());

		setcolor(GREEN);
		settextstyle(0,1,1);
		outtextxy(20,110,"Page 2");

		setcolor(LIGHTRED);
		settextstyle(3,0,1);
		outtextxy(150,120,"Target Table .. ");

		setfillstyle(5,YELLOW);
		bar(150,150,220,200);

		setfillstyle(1,LIGHTCYAN);
		bar(150,200,220,250);
		setcolor(LIGHTMAGENTA);
		outtextxy(165,215,"Easy");

		setfillstyle(1,LIGHTBLUE);
		bar(150,250,220,300);
		setcolor(LIGHTRED);
		outtextxy(155,265,"Medium");

		setfillstyle(1,LIGHTCYAN);
		bar(150,300,220,350);


		setcolor(LIGHTGREEN);
		settextstyle(2,0,0);
		outtextxy(5,getmaxy()-15,"* Default - Starting the Game without Setting any modes or difficulty level");

		setfillstyle(1,DARKGRAY);
		bar(getmaxx()-140,getmaxy()-15,getmaxx()-3,getmaxy()-2);
		setcolor(GREEN);
		rectangle(getmaxx()-140,getmaxy()-15,getmaxx()-3,getmaxy()-2);
		settextstyle(0,0,1);
		outtextxy(getmaxx()-130,getmaxy()-10,"Up Arr : Page 1");

		Navigate :
		do
		{
			ch=getch();

			if(ch==(char)72)
				goto Page1;
			else if(ch==(char)80)
				goto Page2;

		}while(ch!=(char)27);

		cleardevice();

		setcolor(GREEN);
		rectangle(getmaxx()/2 -130,105,getmaxx()/2+130,333);

		for(i=0;i<4;i++)
		{
			VideoNest[i].DrawNest();
			if(i!=3)
				VideoEgg[i].DrawEgg();
		}
		VideoBasket.DrawBasket();

		setcolor(CYAN);
		settextstyle(3,0,4);
		outtextxy(getmaxx()/2 -120,50,"EGG CATCHER !!");

		delay(250);

		goto MainMenu;



	}

End :

	cleardevice();
	ResetGame();

}

void main()
{
	int gdriver=DETECT,gmode;
	initgraph(&gdriver,&gmode,"C:\\TC\\BGI\\");

	MainMenu();
}