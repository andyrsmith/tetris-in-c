#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>

int nFieldWidth = 12;
int nFieldHeight = 18;
char pField[12*18];
char tetromino[7][16] ={ "..X...X...X...X.",
												 "..X..XX...X.....",
												 ".....XX..XX.....",
												 "..X..XX..X......",
												 ".X...XX...X.....",
												 ".X...X...XX.....",
												 "..X...X..XX.....",
};
int rotate(int px, int py, int cRotation);
bool pieceFit(int tetro, int cRotation, int posX, int posY);
//rotate
// 0 y * 4 + x
// 90 12 + y-(x*4)
// 180 15 - (y*4)-x
// 270 3 - y +(x*4)
//

bool pieceFit(int tetro, int cRotation, int posX, int posY)
{
	for(int px = 0; px < 4; px++)
	{
		for(int py = 0; py < 4; py++)
		{
			int pi = rotate(px, py, cRotation);

			int fi = (posY + py) * nFieldWidth + (posX + px);

			if(posX + px >= 0 && posX + px < nFieldWidth)
			{
				if(posY + py >= 0 && posY + py < nFieldHeight)
			{
					if(tetromino[tetro][pi] != '.' && pField[fi] != 0)
						return false;
				}
			}
		}
	}

	return true;
}
int rotate(int px, int py, int r) {
	int pi = 0;

	switch(r % 4)
	{
		//
		case 0:
			pi = py * 4 + px;
			break;
		case 1: // 90 degrees
			pi = 12 + py - (px * 4);
			break;
		case 2: //180 degrees
			pi = 15 - (py*4) - px;
			break;
		case 3:  // 270 degrees
			pi = 3 - py + (px *4);
			break;
	}

		return pi;

}

void drawScoreAndLevel(int score, int level) {
	//Draw the score and level
	move(1, nFieldWidth + 5);
	printw("Level: %d\n", level);
	move(3, nFieldWidth + 5);
	printw("Score: %d\n", score);
	refresh();
}

void showGameOverScreen(int score) {
	clear();
	refresh();
	mvaddstr(8, 8,"Game Over");
	move(10, 8);
	printw("Score: %d\n", score);
	refresh();
}

int main(void) {

	bool gameOver = false;
	int score = 0;
	int level = 1;
	initscr();
	//No Cursor
	curs_set(0);

	addstr("Start");
	refresh();
	drawScoreAndLevel(score,level);
	//Symbols used for the game
	char symbols[] = {
		' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', '=', '#' };
	//Playing field will be the width times height
	//char pField[nFieldWidth * nFieldHeight];

	//Initial playing field as an array
	for(int x = 0; x < nFieldWidth; x++)
		for(int y = 0; y < nFieldHeight; y++)
			if(x == 0 || x == nFieldWidth - 1 || y == nFieldHeight -1) {
				pField[(y*nFieldWidth)+x] = 9;
			} else {
				pField[(y*nFieldWidth)+x] = 0;
			}

	refresh();

	//initialize vars
	int currentY = 0;
	int currentX = (nFieldWidth - 2) / 2;
	int currentPiece = 6;
	int nSpeed = 20;
	int nSpeedCount = 0;
	int cRotation = 0;
	int ch;
	bool bForceDown = false;
	int numLines = 0;
	int totalCompletedLines = 0;
	int hightestComLine = 0;

	//no delay in getch() function
	nodelay(stdscr,true);

	while(!gameOver) {
		//sets speed for game
		napms(50);
		nSpeedCount++;
		bForceDown = (nSpeed == nSpeedCount);



		ch = getch();

		//Adds const for key direction
		keypad(stdscr,TRUE);
		if(ch == ' ' && pieceFit(currentPiece, cRotation + 1, currentX, currentY)) {
			cRotation = cRotation >= 3 ? 0 : cRotation + 1;
		} else if (ch == KEY_LEFT && pieceFit(currentPiece, cRotation, currentX - 1, currentY)) {
			currentX -= 1;
		} else if (ch == KEY_RIGHT && pieceFit(currentPiece, cRotation, currentX + 1, currentY)) {
			currentX += 1;
		} else if (ch == KEY_DOWN && pieceFit(currentPiece, cRotation, currentX, currentY + 1)) {
			currentY += 1;
		}

		if(bForceDown) {
			nSpeedCount = 0;
			if(pieceFit(currentPiece, cRotation, currentX, currentY + 1))
				currentY++;
			else {
				//Piece can not move anymore
				//Add it to the playing field
				for(int px = 0; px < 4; px++)
					for(int py = 0; py < 4; py++)
						if(tetromino[currentPiece][rotate(px,py,cRotation)] != '.')
								pField[(currentY+py) * nFieldWidth + (currentX + px)] = currentPiece + 1;

				for(int py = 0; py < 4; py++) {

					int pieceYPos = currentY + py;
					//if y pos is less then field height
					if(currentY + py < nFieldHeight - 1) {

						bool completedLine = true;
						for(int px = 1; px < nFieldWidth-1; px++)
							completedLine = completedLine && (pField[(currentY+py) * nFieldWidth + px] !=0);

						if(completedLine) {
							hightestComLine = pieceYPos > hightestComLine ? pieceYPos : hightestComLine;	
							numLines++;
							totalCompletedLines++;
							for(int px = 1; px < nFieldWidth-1; px++)
								//first just set to 0 and pull down other lines
								//then try setting to 8 to animate the then pull down lines
								pField[(currentY+py) * nFieldWidth +px] = 0;
						}
					}

				}

				currentY = 0;
				currentX = nFieldWidth / 2;
				cRotation = 0;
				currentPiece = rand() % 7;

				gameOver = !pieceFit(currentPiece, cRotation, currentX, currentY);
			}
		}

		//Draw the Screen
		for(int x = 0; x<nFieldWidth; x++)
			for(int y = 0; y< nFieldHeight; y++) {
				move(y,x);
				printw("%c",symbols[pField[(y*nFieldWidth)+x]]);
			}

		//Draw the piece
		for(int px = 0; px < 4; px++)
			for(int py = 0; py < 4; py++)
				if(tetromino[currentPiece][rotate(px,py,cRotation)] != '.') {
					move(currentY+py, currentX+px);
					printw("%c", symbols[currentPiece+1]);
				}

		//refresh to display contents on screen
		refresh();

		//if completedLine
		if(numLines > 0) {
			//Animate the completedLines
			//push down the other lines above it
			for(int y = hightestComLine; y > 0; y--){
				for(int x = 1; x < nFieldWidth-1; x++) {
					if(y - numLines < nFieldHeight-1 ) {
						pField[y*nFieldWidth+x] = pField[(y-numLines)*nFieldWidth+x];
					}
				}
			}
			score += numLines * 40;
			//Just so we know to update the speed
			//if we move to the next level then speed increases
			int updateLevel = totalCompletedLines / 10 + 1;
			if(updateLevel != level) {
				level = updateLevel;
				nSpeed--;
			}
			drawScoreAndLevel(score, level);
			hightestComLine = 0;	
			numLines = 0;
		}
		
	}
	nodelay(stdscr,false);

	showGameOverScreen(score);
	getch();	
	endwin();
	return 0;
}
