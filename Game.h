#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <iostream>
#include <sstream>

#define CHESSBOARDSIZE 40
#define Size 15

typedef enum GAMEOVERSTATE {
	GNO,            //��Ϸδ����
	ncWIN,            //��Ϸʤ��        //������ʱ���������ʤ
	ncLOSE            //��Ϸʧ��        //������ʱ���������ʤ
};

//ö�ٶ�������״̬
typedef enum CHESSBOARDSTATE {
	ncNULL,            //�յ�
	ncWHITE,
	ncBLACK
};

class _class {
public:
	int mState;                //�����״̬
	bool isPress;            //�����Ƿ񱻵��
};

//�����ж�ʤ��
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

	/*��һ�����������OTL*/
	const dir d1 = { 0, 1 };//��
	const dir d2 = { 1, 0 };//��
	const dir d3 = { 1, -1 };//���Խ���
	const dir d4 = { 1, 1 };//���Խ���

	Position point;
	Position AIpoint;

	sf::Vector2i nowPoint;

	int gameMode;//0���� 1�˻� 2����
	int computer, player;//���˻�ģʽ�У������˺ͻ���������ɫ
	bool gameOver, gameQuit;
	bool myTurn;//���ҵĻغϲ����߼��ж�
	int sequence;//������

	int Window_height, Window_width, stageWidth, stageHeight;
	int mMineNum, mFlagCalc;
	int mTime;        //��Ϸʱ��
	_class mGameData[Size][Size];        //����ȡ����Ѷȵ���̨�ߴ�
	bool isGameBegin;        //��Ϸ�Ƿ�ʼ
	int isGameOverState;    //��Ϸ������״̬
	sf::Vector2i mCornerPoint;        //��̨�󶥵�����
	int chessboardSize;                    //��Ĵ�С

	int imgBGNo, imgSkinNo;

	sf::Texture tBackground, tTiles, tButtons, tNum, tCounter, tGameOver, tGameStart, tMap1, tMap2, tMap3, tMap4, tMap5, tMap6, tMap7, tMap8, tMap9;            //�����������
	sf::Texture tBlack, tWhite, tLast, tAnalog;
	sf::Sprite sBackground, sTiles, sButtons, sNum, sCounter, sGameOver, sGameStart, sMap, sChess, sLast, sAnalog;        //�����������
	sf::IntRect ButtonRectPVP, ButtonRectPVE, ButtonRectEVP, ButtonRectBG, ButtonRectRestart, ButtonRectQuit;


	//Clock���ڶ���ʵ����ʱ����ʼ��ʱ
	sf::Clock gameClock, mouseClickTimer;

	void Initial();//��Ϸ��ʼ��
	void IniData();//��Ϸ���ݳ�ʼ��
	void LoadMediaData();//����ý���ز�

	void Logic();
	void LogicPVP();
	void LogicPVE();

	bool isWin();//��Ϸʤ���ж�
	Position newPoint(Position p, dir d, int lenth);//�����p�����Ϊlenth�ĵ��λ��
	bool isInside(Position p);

	void turnComputer();//ִ��һ�����Իغ�
	int Score(Position p, int who);//�����εĴ�֣���������AI����λ�õ����ȼ�

	void Input();//���뺯��
	void LButtonDownPVP(sf::Vector2i mPoint);//������(������)
	void LButtonDownAI(sf::Vector2i mPoint);//������(�˻���)

	void Draw();//��Ϸ����������
	void DrawChessboard();//������̨դ������
	void DrawButton();//���ƽ�����ť
	void DrawTimer();//��������ʱ��
	void DrawGameEnd();//������Ϸ��������
	void DrawGameStart();

	void Run();
};



