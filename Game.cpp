#include "Game.h"

using namespace sf;

Game::Game() {
	Window_width = 1280;
	Window_height = 800;

	gameMode = 0;
	imgBGNo = 1;

	Uint32 WindowStyle = Style::Close | Style::Titlebar;
	window.create(sf::VideoMode(Window_width, Window_height), L"GoBang_myh", WindowStyle);
}

Game::~Game()
{
}

void Game::Initial() {
	window.setFramerateLimit(60);

	chessboardSize = CHESSBOARDSIZE;

	stageHeight = Size;
	stageWidth = Size;

	sequence = 0;

	gameOver = false;
	gameQuit = false;

	isGameOverState = GNO;        //初始化游戏的结束状态
	isGameBegin = false;          //初始化游戏是否开始
	mTime = 0;                    //初始化游戏进行的时间

	mCornerPoint.x = (Window_width - stageWidth * chessboardSize) / 2;
	mCornerPoint.y = (Window_height - stageHeight * chessboardSize) / 2;

	IniData();
	LoadMediaData();
}

void Game::LoadMediaData() {//加载媒体素材
	std::stringstream ss;
	ss << "images/BK0" << imgBGNo << ".png";
	if (!tBackground.loadFromFile(ss.str())) std::cout << "BK image 没有找到" << std::endl;

	//加载棋盘
	if (!tMap1.loadFromFile("images/1.png")) std::cout << "1.png没有找到" << std::endl;
	if (!tMap2.loadFromFile("images/2.png")) std::cout << "2.png没有找到" << std::endl;
	if (!tMap3.loadFromFile("images/3.png")) std::cout << "3.png没有找到" << std::endl;
	if (!tMap4.loadFromFile("images/4.png")) std::cout << "4.png没有找到" << std::endl;
	if (!tMap5.loadFromFile("images/5.png")) std::cout << "5.png没有找到" << std::endl;
	if (!tMap6.loadFromFile("images/6.png")) std::cout << "6.png没有找到" << std::endl;
	if (!tMap7.loadFromFile("images/7.png")) std::cout << "7.png没有找到" << std::endl;
	if (!tMap8.loadFromFile("images/8.png")) std::cout << "8.png没有找到" << std::endl;
	if (!tMap9.loadFromFile("images/9.png")) std::cout << "9.png没有找到" << std::endl;

	//加载棋子
	if (!tBlack.loadFromFile("images/black.png")) std::cout << "black.png没有找到" << std::endl;
	if (!tWhite.loadFromFile("images/white.png")) std::cout << "white.png没有找到" << std::endl;
	if (!tLast.loadFromFile("images/last.png")) std::cout << "last.png没有找到" << std::endl;
	if (!tAnalog.loadFromFile("images/analog.png")) std::cout << "analog.png没有找到" << std::endl;

	//加载其他
	if (!tButtons.loadFromFile("images/button.png")) std::cout << "button.png 没有找到" << std::endl;
	if (!tGameStart.loadFromFile("images/gamestart.png")) std::cout << "gamestart.png 没有找到" << std::endl;
	if (gameMode == 0) {
		if (!tGameOver.loadFromFile("images/gameoverpvp.png")) std::cout << "gameoverpvp.png 没有找到" << std::endl;
	}
	else {
		if (!tGameOver.loadFromFile("images/gameoverpve.png")) std::cout << "gameoverpve.png 没有找到" << std::endl;
	}

	sBackground.setTexture(tBackground);
	sTiles.setTexture(tTiles);
	sButtons.setTexture(tButtons);
	sGameOver.setTexture(tGameOver);
	sGameStart.setTexture(tGameStart);
	sLast.setTexture(tLast);
	sAnalog.setTexture(tAnalog);
}

void Game::IniData() {//初始化
	int i, j;
	for (j = 0; j < stageHeight; ++j)
	{
		for (i = 0; i < stageWidth; ++i)
		{
			mGameData[j][i].mState = ncNULL;
			mGameData[j][i].isPress = false;
		}
	}
}

void Game::Input() {
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
			gameQuit = true;
		}

		if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Escape)
		{
			window.close();
			gameQuit = true;
		}

		if (event.type == sf::Event::EventType::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		{
			if (isGameOverState == GNO)
			{
				if (gameMode == 0)
				{
					LButtonDownPVP(Mouse::getPosition(window));
				}
				else
				{
					LButtonDownAI(Mouse::getPosition(window));
				}
			}
		}

		if (event.type == sf::Event::EventType::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
		{
			if (isGameOverState == GNO)
			{
				if (isGameBegin == false)
				{
					if (ButtonRectPVP.contains(event.mouseButton.x, event.mouseButton.y))
					{
						gameMode = 0;
					}
					if (ButtonRectPVE.contains(event.mouseButton.x, event.mouseButton.y))
					{
						gameMode = 1;
						player = ncWHITE;
						computer = ncBLACK;
						myTurn = true;
					}
					if (ButtonRectEVP.contains(event.mouseButton.x, event.mouseButton.y))
					{
						gameMode = 2;
						player = ncBLACK;
						computer = ncWHITE;
						myTurn = false;
					}

					Initial();
				}
			}

			if (ButtonRectBG.contains(event.mouseButton.x, event.mouseButton.y))
			{
				imgBGNo++;
				if (imgBGNo > 4) imgBGNo = 1;
				LoadMediaData();
			}

			if (ButtonRectRestart.contains(event.mouseButton.x, event.mouseButton.y))
			{
				Initial();
				gameMode = 0;
			}

			if (ButtonRectQuit.contains(event.mouseButton.x, event.mouseButton.y))
			{
				window.close();
				gameQuit = true;
			}
		}

		nowPoint = Mouse::getPosition(window);
	}
}

void Game::LButtonDownPVP(Vector2i mPoint) {
	int i, j;
	i = (mPoint.x - mCornerPoint.x) / chessboardSize;
	j = (mPoint.y - mCornerPoint.y) / chessboardSize;

	if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight) {
		if (isGameBegin == false)
		{
			isGameBegin = true;
			gameClock.restart();
		}

		if (mGameData[j][i].isPress == false) {
			if (sequence == 0)
			{
				mGameData[j][i].mState = ncWHITE;
				sequence = 1;
			}
			else if (sequence == 1)
			{
				mGameData[j][i].mState = ncBLACK;
				sequence = 0;
			}

			mGameData[j][i].isPress = true;
			point.x = i;
			point.y = j;
		}
	}
}

void Game::LButtonDownAI(Vector2i mPoint) {
	int i, j;
	i = (mPoint.x - mCornerPoint.x) / chessboardSize;
	j = (mPoint.y - mCornerPoint.y) / chessboardSize;

	if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight) {
		if (isGameBegin == false)
		{
			isGameBegin = true;
			gameClock.restart();
		}

		if (mGameData[j][i].isPress == false)
		{
			mGameData[j][i].mState = player;
			mGameData[j][i].isPress = true;
			point.x = i;
			point.y = j;
			myTurn = false;
		}
	}
}

void Game::Logic() {
	if (gameMode == 0)
	{
		LogicPVP();
	}
	else
	{
		LogicPVE();
	}
}

void Game::LogicPVP() {
	bool gamestate;

	if (isGameBegin == true) gamestate = isWin();
	else gamestate = false;

	if (sequence == 0 && gamestate == true)
	{
		std::cout << "Game Over\n";
		isGameOverState = ncWIN;
		//        gameOver = true;
		isGameBegin = false;
	}
	else if (sequence == 1 && gamestate == true)
	{
		std::cout << "Game Over\n";
		isGameOverState = ncLOSE;
		//        gameOver = true;
		isGameBegin = false;
	}
}

void Game::LogicPVE() {
	bool gamestate;

	//根据上一次玩家下的棋判断输赢
	if (isGameBegin == true) gamestate = isWin();
	else gamestate = false;

	if (gamestate == true) {
		std::cout << "Game Over\n";
		isGameOverState = ncWIN;
		isGameBegin = false;
	}

	if (myTurn == false) {
		if (gameMode == 1) {
			if (isGameBegin == false) {
				return;
			}
		}
		turnComputer();
		point = AIpoint;
		myTurn = true;

		isGameBegin = true;

		if (isGameBegin == true) gamestate = isWin();
		else gamestate = false;

		if (gamestate == true) {
			std::cout << "Game Over\n";
			isGameOverState = ncLOSE;
			isGameBegin = false;
		}
	}
}

bool Game::isWin() {//胜负判定
	for (int i = 1; i <= 4; ++i)
	{
		dir d;
		if (i == 1) d = d1;
		else if (i == 2) d = d2;
		else if (i == 3) d = d3;
		else if (i == 4) d = d4;

		int count = 0;

		for (int j = -4; j <= 4; ++j)
		{
			Position p = newPoint(point, d, j);
			if (isInside(p) && (mGameData[p.y][p.x].mState == mGameData[point.y][point.x].mState)) {
				count++;
			}
			else {
				count = 0;
			}
			if (count == 5) return true;
		}
	}
	return false;
}

Position Game::newPoint(Position p, dir d, int lenth) {
	Position pt = { p.y + d.dy * lenth,p.x + d.dx * lenth };
	return pt;
}

bool Game::isInside(Position p) {
	if (p.x >= 0 && p.x < stageWidth && p.y >= 0 && p.y < stageHeight)
	{
		return true;
	}
	return false;
}

void Game::turnComputer() {
	Position best1, best2;
	best1 = best2 = { 7,7 };

	int a1, b1;
	if (mGameData[best1.x][best1.y].isPress == true) {
		a1 = 0;
		b1 = 0;
	}
	else
	{
		a1 = Score(best1, computer);
		b1 = Score(best1, player);
	}

	for (int i = 0; i < stageWidth; i++)
	{
		for (int j = 0; j < stageHeight; j++)
		{
			if (mGameData[i][j].mState != ncNULL)
			{
				continue;
			}

			Position current = { i, j };
			int m1 = Score(current, computer);
			int m2 = Score(current, player);

			if (m1 > a1)
			{
				best1 = current;
				a1 = m1;
				b1 = m2;
			}

			else if (m1 == a1)
			{
				if (m2 > b1)
				{
					best1 = current;
					b1 = m2;
				}
			}
		}
	}

	int a2, b2;
	if (mGameData[best1.x][best1.y].isPress == true)
	{
		a2 = 0;
		b2 = 0;
	}

	else
	{
		a2 = Score(best2, computer);
		b2 = Score(best2, player);
	}

	for (int i = 0; i < stageHeight; i++)
	{
		for (int j = 0; j < stageWidth; j++)
		{
			if (mGameData[i][j].mState != ncNULL)
			{
				continue;
			}

			Position current = { i, j };
			int m1 = Score(current, player);
			int m2 = Score(current, computer);

			if (m1 > a2)
			{
				best2 = current;
				a2 = m1;
				b2 = m2;
			}

			else if (m1 == a2)
			{
				if (m2 > b2)
				{
					best2 = current;
					b2 = m2;
				}
			}
		}
	}

	if (a1 >= a2)
	{
		AIpoint = best1;
	}
	else
	{
		AIpoint = best2;
	}

	mGameData[AIpoint.y][AIpoint.x].mState = computer;
	mGameData[AIpoint.y][AIpoint.x].isPress = true;
}

int Game::Score(Position p, int who) {
	int win5 = 0, live4 = 0, die4 = 0, ddie4 = 0, live3 = 0, dlive3 = 0, die3 = 0, live2 = 0, dlive2 = 0, die2 = 0, die = 0;
	int side;

	if (who == ncWHITE)
	{
		side = ncBLACK;
	}
	else
	{
		side = ncWHITE;
	}

	for (int i = 1; i <= 4; ++i)
	{
		dir d;
		if (i == 1) { d = d1; }
		else if (i == 2) { d = d2; }
		else if (i == 3) { d = d3; }
		else if (i == 4) { d = d4; }

		int l = 1;
		Position pl, pr;
		Position p1;

		int left[5], right[5];

		p1 = newPoint(p, d, -1);
		pl = p;
		while (isInside(p1) && mGameData[p1.y][p1.x].mState == who)
		{
			pl = p1;
			p1 = newPoint(p1, d, -1);
			l++;
		}

		p1 = newPoint(p, d, 1);
		pr = p;
		while (isInside(p1) && mGameData[p1.y][p1.x].mState == who)
		{
			pr = p1;
			p1 = newPoint(p1, d, 1);
			l++;
		}

		for (int j = 1; j <= 4; ++j)
		{
			p1 = newPoint(pl, d, -j);
			if (isInside(p1))
			{
				left[j] = mGameData[p1.y][p1.x].mState;
			}
			else {
				left[j] = side;
			}

			p1 = newPoint(pr, d, j);
			if (isInside(p1))
			{
				right[j] = mGameData[p1.y][p1.x].mState;
			}
			else
			{
				right[j] = side;
			}
		}
		//**********************************打分机制**********************************//
		if (l == 5) {
			win5++;
		}

		else if (l == 4) {
			if (left[1] == 0 && right[1] == 0) {//live4
				live4++;//011110
			}
			else if (left[1] == 0 || right[1] == 0) {//die4
				die4++;//x11110
			}
			else {//die
				die++;//x1111x
			}
		}

		else if (l == 3) {
			if ((left[1] == 0 && left[2] == who) || (right[1] == 0 && right[2] == who)) {//ddie4
				ddie4++;//10111
			}
			else if (left[1] == 0 && right[1] == 0 && (left[2] == 0 || right[2] == 0)) {//live3
				live3++;//001110
			}
			else if ((left[1] == 0 && left[2] == 0) || (right[1] == 0 && right[2] == 0)) {//die3
				die3++;//00111x
			}
			else if (left[1] == 0 && right[1] == 0) {//die3
				die3++;//0111x
			}
			else {//die
				die++;//x111x
			}
		}

		else if (l == 2) {
			if ((left[1] == 0 && left[2] == who && left[3] == who) && (right[1] == 0 && right[2] == who && right[3] == who)) {//die4
				ddie4++;//11011
			}
			else if (left[1] == 0 && right[1] == 0 && ((left[2] == who && left[3] == 0) || (right[2] == who && right[3] == 0))) {//dlive3
				dlive3++;//010110
			}
			else if ((left[1] == 0 && left[3] == 0 && left[2] == who) || (right[1] == 0 && right[3] == 0 && right[2] == who)) {//die3
				die3++;//01011x
			}
			else if ((left[1] == 0 && right[1] == 0) && (left[2] == who || right[2] == who)) {//die3
				die3++;//10110
			}
			else if ((left[1] == 0 && left[2] == 0 && left[3] == who) || (right[1] == 0 && right[2] == 0 && right[3] == who)) {//die3
				die3++;//10011
			}
			else if ((left[1] == 0 && right[1] == 0 && right[2] == 0 && right[3] == 0) || (left[1] == 0 && left[2] == 0 && right[1] == 0 && right[2] == 0) || (left[1] == 0 && left[2] == 0 && left[3] == 0 && right[1] == 0)) {//live2
				live2++;//011000//001100//000110
			}
			else if ((left[1] == 0 && left[2] == 0 && left[3] == 0) || (right[1] == 0 && right[2] == 0 && right[3] == 0)) {//die2
				die2++;//00011
			}
			else {//die
				die++;//x11x
			}
		}

		else if (l == 1) {
			if ((left[1] == 0 && left[2] == who && left[3] == who && left[4] == who) || (right[1] == 0 && right[2] == who && right[3] == who && right[4] == who)) {//ddie4
				ddie4++;//11101
			}
			else if ((left[1] == 0 && right[1] == 0) && ((left[2] == who && left[3] == who && left[4] == 0) || (right[2] == who && right[3] == who && right[4] == 0))) {//dlive3
				dlive3++;//011010
			}
			else if ((left[1] == 0 && right[1] == 0) && ((left[2] == who && left[3] == who) || (right[2] == who && right[3] == who))) {//die3
				die3++;//11010
			}
			else if ((left[1] == 0 && left[4] == 0 && left[2] == who && left[3] == who) || (right[1] == 0 && right[4] == 0 && right[2] == who && right[3] == who)) {//die3
				die3++;//01101
			}
			else if ((left[1] == 0 && left[2] == 0 && left[3] == who && left[4] == who) || (right[1] == 0 && right[2] == 0 && right[3] == who && right[4] == who)) {//die3
				die3++;//11001
			}
			else if ((left[1] == 0 && left[3] == 0 && left[2] == who && left[4] == who) || (right[1] == 0 && right[3] == 0 && right[2] == who && right[4] == who)) {//die3
				die3++;//10101
			}
			else if ((left[1] == 0 && right[1] == 0 && right[3] == 0 && right[2] == who) && (left[2] == 0 || right[4] == 0)) {//dlive2
				dlive2++;//001010
			}
			else if ((right[1] == 0 && left[1] == 0 && left[3] == 0 && left[2] == who) && (right[2] == 0 || left[4] == 0)) {//dlive2
				dlive2++;//010100
			}
			else if ((left[1] == 0 && right[1] == 0 && right[2] == 0 && right[4] == 0 && right[3] == who) || (right[1] == 0 && left[1] == 0 && left[2] == 0 && left[4] == 0 && left[3] == who)) {//dlive2
				dlive2++;//010010
			}
			else if ((left[1] == 0 && left[3] == 0 && left[4] == 0 && left[2] == who) || (right[1] == 0 && right[3] == 0 && right[4] == 0 && right[2] == who)) {//die2
				die2++;//00101
			}
			else if ((left[1] == 0 && right[1] == 0 && right[2] == 0 && left[2] == who) || (right[1] == 0 && left[1] == 0 && left[2] == 0 && right[2] == who)) {//die2
				die2++;//10100
			}
			else if ((left[1] == 0 && left[2] == 0 && left[4] == 0 && left[3] == who) || (right[1] == 0 && right[2] == 0 && right[4] == 0 && right[3] == who)) {//die2
				die2++;//01001
			}
			else if ((left[1] == 0 && left[2] == 0 && right[1] == 0 && left[3] == who) || (right[1] == 0 && right[2] == 0 && left[1] == 0 && right[3] == who)) {//die2
				die2++;//10010
			}
			else if ((left[1] == 0 && left[2] == 0 && left[3] == 0 && left[4] == who) || (right[1] == 0 && right[2] == 0 && right[3] == 0 && right[4] == who)) {//die2
				die2++;//10001
			}
			else {//die
				die++;
			}
		}
	}

	int pscore;
	pscore = win5 * 5000 + live4 * 1000 + die4 * 70 + ddie4 * 50 + live3 * 30 + dlive3 * 20 + die3 * 10 + live2 * 5 + dlive2 * 2 + die2 + die;
	return pscore;
}

void Game::Draw() {
	window.clear();
	sBackground.setPosition(0, 0);
	window.draw(sBackground);
	DrawChessboard();
	DrawButton();
	if (isGameOverState) DrawGameEnd();
	window.display();
}

void Game::DrawChessboard() {
	//画棋盘
	int i, j;
	for (j = 0; j < stageHeight; ++j) {
		for (i = 0; i < stageWidth; ++i) {

			if (j == 0 && i == 0) { sMap.setTexture(tMap6); }
			else if (j == 0 && i == stageWidth - 1) { sMap.setTexture(tMap7); }
			else if (j == stageHeight - 1 && i == 0) { sMap.setTexture(tMap8); }
			else if (j == stageHeight - 1 && i == stageWidth - 1) { sMap.setTexture(tMap9); }
			else if (j == 0) { sMap.setTexture(tMap2); }
			else if (i == 0) { sMap.setTexture(tMap4); }
			else if (j == stageHeight - 1) { sMap.setTexture(tMap3); }
			else if (i == stageWidth - 1) { sMap.setTexture(tMap5); }
			else { sMap.setTexture(tMap1); }

			sMap.setPosition(mCornerPoint.x + i * CHESSBOARDSIZE, mCornerPoint.y + j * CHESSBOARDSIZE);
			window.draw(sMap);
		}
	}
	//画棋子
	for (j = 0; j < stageHeight; ++j) {
		for (i = 0; i < stageWidth; ++i) {
			if (mGameData[j][i].isPress == true) {
				if (mGameData[j][i].mState == ncWHITE) {
					sChess.setTexture(tWhite);
				}
				else if (mGameData[j][i].mState == ncBLACK) {
					sChess.setTexture(tBlack);
				}
				sChess.setPosition(mCornerPoint.x + i * CHESSBOARDSIZE, mCornerPoint.y + j * CHESSBOARDSIZE);
				window.draw(sChess);
			}
		}
	}

	if (gameMode != 0 && isGameBegin) {
		sLast.setPosition(mCornerPoint.x + AIpoint.x * CHESSBOARDSIZE, mCornerPoint.y + AIpoint.y * CHESSBOARDSIZE);
		window.draw(sLast);
	}

	i = (nowPoint.x - mCornerPoint.x) / chessboardSize;
	j = (nowPoint.y - mCornerPoint.y) / chessboardSize;
	if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight) {
		if (mGameData[j][i].isPress == false) {
			sAnalog.setPosition(mCornerPoint.x + i * CHESSBOARDSIZE, mCornerPoint.y + j * CHESSBOARDSIZE);
			window.draw(sAnalog);
		}
	}
}

void Game::DrawButton() {
	Vector2i LeftCorner;
	int ButtonWidth = 60;
	int ButtonHeight = 40;
	int detaY;

	detaY = (Window_height - ButtonHeight * 6) / 7;
	LeftCorner.x = Window_width - 100;

	//ButtonRectPVP
	LeftCorner.y = detaY;
	sButtons.setTextureRect(IntRect(0 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);

	ButtonRectPVP.left = LeftCorner.x;
	ButtonRectPVP.top = LeftCorner.y;
	ButtonRectPVP.width = ButtonWidth;
	ButtonRectPVP.height = ButtonHeight;

	window.draw(sButtons);

	//ButtonRectPVE
	LeftCorner.y = detaY * 2 + ButtonHeight;
	sButtons.setTextureRect(IntRect(1 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);

	ButtonRectPVE.left = LeftCorner.x;
	ButtonRectPVE.top = LeftCorner.y;
	ButtonRectPVE.width = ButtonWidth;
	ButtonRectPVE.height = ButtonHeight;

	window.draw(sButtons);

	//ButtonRectEVP
	LeftCorner.y = detaY * 3 + ButtonHeight * 2;
	sButtons.setTextureRect(IntRect(2 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);

	ButtonRectEVP.left = LeftCorner.x;
	ButtonRectEVP.top = LeftCorner.y;
	ButtonRectEVP.width = ButtonWidth;
	ButtonRectEVP.height = ButtonHeight;

	window.draw(sButtons);

	//ButtonRectBG
	LeftCorner.y = detaY * 4 + ButtonHeight * 3;
	sButtons.setTextureRect(IntRect(4 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);

	ButtonRectBG.left = LeftCorner.x;
	ButtonRectBG.top = LeftCorner.y;
	ButtonRectBG.width = ButtonWidth;
	ButtonRectBG.height = ButtonHeight;

	window.draw(sButtons);

	//ButtonRectRestart
	LeftCorner.y = detaY * 5 + ButtonHeight * 4;
	sButtons.setTextureRect(IntRect(5 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);

	ButtonRectRestart.left = LeftCorner.x;
	ButtonRectRestart.top = LeftCorner.y;
	ButtonRectRestart.width = ButtonWidth;
	ButtonRectRestart.height = ButtonHeight;

	window.draw(sButtons);

	//ButtonRectQuit
	LeftCorner.y = detaY * 6 + ButtonHeight * 5;
	sButtons.setTextureRect(IntRect(6 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);

	ButtonRectQuit.left = LeftCorner.x;
	ButtonRectQuit.top = LeftCorner.y;
	ButtonRectQuit.width = ButtonWidth;
	ButtonRectQuit.height = ButtonHeight;

	window.draw(sButtons);
}

void Game::DrawGameEnd() {
	Vector2i LeftCorner;
	int ButtonWidth = 200;
	int ButtonHeight = sGameOver.getLocalBounds().height;
	LeftCorner.x = (Window_width - ButtonWidth) / 2;
	LeftCorner.y = (Window_height - ButtonHeight) / 2;

	sGameOver.setPosition(LeftCorner.x, LeftCorner.y);

	if (isGameOverState == ncWIN) {
		sGameOver.setTextureRect(IntRect(0 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	}
	if (isGameOverState == ncLOSE) {
		sGameOver.setTextureRect(IntRect(1 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	}
	window.draw(sGameOver);
}

//void Game::DrawGameStart() {
//    Vector2i LeftCorner;
//    int ButtonWidth = 400;
//    int ButtonHeight = 122;
//    LeftCorner.x = (Window_width - ButtonWidth) / 2;
//    LeftCorner.y = 10;
//
//    sGameStart.setPosition(LeftCorner.x, LeftCorner.y);
//    sGameStart.setTextureRect(IntRect(0 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
//    window.draw(sGameStart);
//}

void Game::Run() {
	do {
		Initial();
		while (window.isOpen() && gameOver == false)
		{
			Input();
			Logic();
			Draw();
		}
	} while (!gameQuit && window.isOpen());
}
