#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <iostream>
#include <sstream>

#define CHESSBOARDSIZE 40
#define Size 15

typedef enum GAMEOVERSTATE {
	GNO,            //游戏未结束
	ncWIN,            //游戏胜利        //在人人时，代表黑棋胜
	ncLOSE            //游戏失败        //在人人时，代表白棋胜
};

//枚举定义网格状态
typedef enum CHESSBOARDSTATE {
	ncNULL,            //空地
	ncWHITE,
	ncBLACK
};

class _class {
public:
	int mState;                //网格的状态
	bool isPress;            //网格是否被点击
};

//用于判断胜负
class dir {
public:
	int dy;
	int dx;
};

class Position {
public:
	int y;
	int x;
};

class Game {
public:
	sf::RenderWindow window;
	Game();
	~Game();

	/*画一个就能理解了OTL*/
	const dir d1 = { 0, 1 };//横
	const dir d2 = { 1, 0 };//竖
	const dir d3 = { 1, -1 };//副对角线
	const dir d4 = { 1, 1 };//主对角线

	Position point;
	Position AIpoint;

	sf::Vector2i nowPoint;

	int gameMode;//0人人 1人机 2机人
	int computer, player;//在人机模式中，保存人和机的棋子颜色
	bool gameOver, gameQuit;
	bool myTurn;//在我的回合不做逻辑判断
	int sequence;//人人用

	int Window_height, Window_width, stageWidth, stageHeight;
	int mMineNum, mFlagCalc;
	int mTime;        //游戏时间
	_class mGameData[Size][Size];        //数组取最高难度的舞台尺寸
	bool isGameBegin;        //游戏是否开始
	int isGameOverState;    //游戏结束的状态
	sf::Vector2i mCornerPoint;        //舞台左顶点坐标
	int chessboardSize;                    //块的大小

	int imgBGNo, imgSkinNo;

	sf::Texture tBackground, tTiles, tButtons, tNum, tCounter, tGameOver, tGameStart, tMap1, tMap2, tMap3, tMap4, tMap5, tMap6, tMap7, tMap8, tMap9;            //创建纹理对象
	sf::Texture tBlack, tWhite, tLast, tAnalog;
	sf::Sprite sBackground, sTiles, sButtons, sNum, sCounter, sGameOver, sGameStart, sMap, sChess, sLast, sAnalog;        //创建精灵对象
	sf::IntRect ButtonRectPVP, ButtonRectPVE, ButtonRectEVP, ButtonRectBG, ButtonRectRestart, ButtonRectQuit;


	//Clock类在对象实例化时即开始计时
	sf::Clock gameClock, mouseClickTimer;

	void Initial();//游戏初始化
	void IniData();//游戏数据初始化
	void LoadMediaData();//加载媒体素材

	void Logic();
	void LogicPVP();
	void LogicPVE();

	bool isWin();//游戏胜负判定
	Position newPoint(Position p, dir d, int lenth);//计算和p点距离为lenth的点的位置
	bool isInside(Position p);

	void turnComputer();//执行一个电脑回合
	int Score(Position p, int who);//对棋形的打分，用来决定AI落子位置的优先级

	void Input();//输入函数
	void LButtonDownPVP(sf::Vector2i mPoint);//鼠标左击(人人用)
	void LButtonDownAI(sf::Vector2i mPoint);//鼠标左击(人机用)

	void Draw();//游戏绘制主函数
	void DrawChessboard();//绘制舞台栅格纹理
	void DrawButton();//绘制交互按钮
	void DrawTimer();//绘制已用时间
	void DrawGameEnd();//绘制游戏结束界面
	void DrawGameStart();

	void Run();
};



