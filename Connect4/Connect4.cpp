// Connect4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

static bool gameOn = true;
static int mode;


class GameBoard {
public:
	enum {
		SUCCESS,
		UNSUCCESS,
		DRAW
	};

	int status;
	int width, height;
	GameBoard(int num1, int num2);

	void build() {
		for (int i = 0; i < height; i++) {
			string line;
			for (int j = 0; j < width; j++) {
				line.push_back('.');
			}
			//cout << line << endl;
			board.push_back(line);
		}
	}

	void toString() {
		string res;
		cout << "toString" << endl;
		res.append("width:");
		res.append(to_string(width));
		cout << res << endl;
	}

	void movesCount() {
		moves++;
	}

	int getMoves() {
		return moves;
	}

	int maxConnect(int x, int y) {

		board[x][y] = 'x';
		for (int i = 0; i < 4; i++) {
			int count = 0;
			int nextX = x + directions[i][0];
			int nextY = y + directions[i][1];
			int preX = x - directions[i][0];
			int preY = y - directions[i][1];

			nextX = getNext(nextX, board.size());
			nextY = getNext(nextY, board[0].size());
			preX = getNext(preX, board.size());
			preY = getNext(preY, board[0].size());

			while (!(nextX == x && nextY == y))
			{
				if (board[nextX][nextY] == board[x][y]) {
					count++;
				}
				else {
					break;
				}
				nextX = nextX + directions[i][0];
				nextY = nextY + directions[i][1];
				nextX = getNext(nextX, board.size());
				nextY = getNext(nextY, board[0].size());
			}

			while (!(preX == x && preY == y)) {
				if (board[preX][preY] == board[x][y]) {
					count++;
				}
				else {
					break;
				}
				preX = preX - directions[i][0];
				preY = preY - directions[i][1];
				preX = getNext(preX, board.size());
				preY = getNext(preY, board[0].size());
			}

			if (count >= 3) {
				cout << "connect 4!" << endl;
				board[x][y] = '.';
				return 100;
			}
			else {
				board[x][y] = '.';
				if (x > 0 && x < board.size() - 1 && board[x + 1][y] == 'o') {
					return count - 2;
				}
				return count;
			}

		}

	}


	void gameAI(bool flag) {

		int curMax = -1000;
		int maxCol = 0;
		for (int num = 0; num < board[0].size(); num++) {
			int tmp = 0;
			for (int i = 0; i < board.size(); i++) {
				if (board[i][num] == '.') {
					tmp++;
					continue;
				}
			}
			tmp = tmp - 1;
			if (tmp < 0) {
				maxCol++;
				continue;
			}
			else {
				int mc = maxConnect(tmp, num);
				if (mc > curMax) {
					curMax = mc;
					maxCol = num;
				}
			}
		}

		placePiece(getNext(maxCol + 1, board[0].size()), flag);
	}


	bool placePiece(int num, bool flag) {
		int tmp = 0;
		for (int i = 0; i < board.size(); i++) {
			if (board[i][num - 1] == '.') {
				//cout << "it is a ." + to_string(i) << endl;
				tmp++;
				continue;
			}
		}
		tmp = tmp - 1;
		if (tmp < 0) {
			cout << "sorry, you can't place a piece here since this line is full" << endl;
			return false;
		}
		else {
			board[tmp][num - 1] = flag ? 'o' : 'x';
			movesCount();
			int checkRes = 0;
			if (mode == 1) {
				checkRes = checkBasic(tmp, num - 1);
			}
			else if (mode == 2) {
				checkRes = check(tmp, num - 1);
			}
			else if (mode == 3) {
				checkRes = check(tmp, num - 1);
			}
			if (checkRes == UNSUCCESS) {
				cout << "OK, next turn!" << endl;
			}
			else if (checkRes == SUCCESS) {
				if (flag) {
					cout << "-------------------------------------" << endl;
					cout << "          | Player O won! |          " << endl;
					cout << "-------------------------------------" << endl;
					gameOn = false;

				}
				else {
					cout << "-------------------------------------" << endl;
					cout << "          | Player X won! |          " << endl;
					cout << "-------------------------------------" << endl;
					gameOn = false;
				}
			}
			else if (checkRes == DRAW) {
				gameOn = false;
			}
		}
		return true;
	}

	int directions[4][2] = { {1,0},{0,1},{1,1},{1,-1} };

	int getNext(int num, int mod) {
		int res = num;
		if (res >= mod) {
			res %= mod;
		}
		else if (res < 0) {
			res += mod;
		}
		return res;
	}


	int checkBasic(int x, int y) {
		system("cls");
		for (int i = 0; i < 4; i++) {
			int count = 0;
			int nextX = x + directions[i][0];
			int nextY = y + directions[i][1];
			int preX = x - directions[i][0];
			int preY = y - directions[i][1];

			while ((nextX < board.size() && nextX >= 0) && (nextY < board[0].size() && nextY >= 0)) {
				if (board[nextX][nextY] == board[x][y]) {
					count++;
				}
				else {
					break;
				}
				nextX += directions[i][0];
				nextY += directions[i][1];
			}

			while ((preX < board.size() && preX >= 0) && (preY < board[0].size() && preY >= 0)) {
				if (board[preX][preY] == board[x][y]) {
					count++;
				}
				else {
					break;
				}
				preX -= directions[i][0];
				preY -= directions[i][1];
			}

			if (count >= 3) {
				cout << "connect 4!" << endl;
				return SUCCESS;
			}
			if (getMoves() == width * height) {
				cout << "Draw!" << endl;
				return DRAW;
			}

		}
		return UNSUCCESS;
	}


	int check(int x, int y) {
		system("cls");
		//cout << "I'm checking ..." << x << "," << y << endl;
		for (int i = 0; i < 4; i++) {
			/*
			cout << "-----------------" << endl;
			cout << "it is " << i << " th" << endl;
			cout << "-----------------" << endl;
			*/
			int count = 0;
			int nextX = x + directions[i][0];
			int nextY = y + directions[i][1];
			int preX = x - directions[i][0];
			int preY = y - directions[i][1];
			/*
			cout << "nextX , nextY" << nextX << "," << nextY << endl;
			cout << "preX, preY" << preX << "," << preY << endl;
			*/
			nextX = getNext(nextX, board.size());
			nextY = getNext(nextY, board[0].size());
			preX = getNext(preX, board.size());
			preY = getNext(preY, board[0].size());
			//cout << "nextX,nextY -> " + to_string(nextX) << "," + to_string(nextY) << endl;

			/*
			cout << board.size() << "," << board[0].size() << endl;
			cout << "nextX , nextY" << nextX << "," << nextY << endl;
			cout << "preX, preY" << preX << "," << preY << endl;
			*/

			while (!(nextX == x && nextY == y))
			{
				//cout << "I'm searching other forward directions" << endl;
				if (board[nextX][nextY] == board[x][y]) {
					count++;
				}
				else {
					break;
				}
				nextX = nextX + directions[i][0];
				nextY = nextY + directions[i][1];
				nextX = getNext(nextX, board.size());
				nextY = getNext(nextY, board[0].size());
			}

			while (!(preX == x && preY == y)) {
				//cout << "I'm searching other backward directions" << endl;
				if (board[preX][preY] == board[x][y]) {
					count++;
				}
				else {
					break;
				}
				preX = preX - directions[i][0];
				preY = preY - directions[i][1];
				preX = getNext(preX, board.size());
				preY = getNext(preY, board[0].size());
			}
			/*
			cout << "-----------------" << endl;
			cout << "it is connected: " << count << endl;
			cout << "-----------------" << endl;
			*/
			if (count >= 3) {
				cout << "connect 4!" << endl;
				return SUCCESS;
			}
			if (getMoves() == width * height) {
				cout << "Draw!" << endl;
				return DRAW;
			}

			/*
			while (!(nextX == x && nextY == y) && !(preX == nextX && preY == nextY)) {

				if (board[nextX][nextY] == board[x][y]) {
					count++;
				}
				if (board[preX][preY] == board[x][y]) {
					count++;
				}
				cout << "I'm in while" << endl;
				cout << "nextX,nextY -> " + to_string(nextX) << "," + to_string(nextY) << endl;

				cout << "connect : " + to_string(count) << endl;
				if (count == 3) {
					cout << "congratulation! connected 4!" << endl;
					return true;
				}
				else {
					nextX = nextX + directions[i][0];
					nextY = nextY + directions[i][1];
					nextX = getNext(nextX, board.size());
					nextY = getNext(nextY, board[0].size());
					preX = x - directions[i][0];
					preY = y - directions[i][1];
					preX = getNext(preX, board.size());
					preY = getNext(preY, board[0].size());
				}
			}
			*/
		}
		return UNSUCCESS;
	}

	void printBoard() {
		string res;
		for (int t = 0; t < board[0].size(); t++) {
			res.append(to_string(t + 1));
		}
		res.append("\n");
		for (int i = 0; i < board.size(); i++) {
			res.append("\n");
			res.append(board[i]);
		}
		cout << res << endl;

		/*
			string res;
			res.append("\n");
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					res.append(".");
				}
				res += "\n";
			}
			cout << res << endl;
		*/
	}


private:
	vector<string> board;
	int moves;
};


GameBoard::GameBoard(int num1, int num2) {
	width = num2;
	height = num1;
	moves = 0;
}


bool nextPlay() {
	cout << "Do you want to play again?(y/n)" << endl;
	do {
		char input;
		cin >> input;
		if (input == 'y') {
			return true;
		}
		else if (input == 'n') {
			return false;
		}
		else {
			cout << "Sorry, invalid input." << endl;
		}
	} while (true);
}


void gameSetting() {
	cout << "Hello!" << endl;
	cout << "You can choose you game mode!" << endl;
	cout << "1. Basic Mode" << endl;
	cout << "2. Wrap Around Mode" << endl;
	cout << "3. Play with the Computer(Wrap Around Mode)" << endl;
	cout << "4. Quit" << endl;
	cout << "Please choose a mode:" << endl;
	do {
		cin >> mode;
		if (cin.fail()) {
			system("cls");
			cin.clear();
			cin.ignore();
			cout << "Sorry, Invalid Input." << endl;
			cout << "-------------------------" << endl;
			cout << "Hello!" << endl;
			cout << "You can choose you game mode!" << endl;
			cout << "1. Basic Mode" << endl;
			cout << "2. Wrap Around Mode" << endl;
			cout << "3. Play with the Computer(Wrap Around Mode)" << endl;
			cout << "4. Quit" << endl;
			cout << "Please choose a mode:" << endl;
			continue;
		}
		if (mode == 1 || mode == 2 || mode == 3) {
			break;
		}
		else if (mode == 4) {
			break;
		}
		else {
			cout << "Sorry, Invalid Input." << endl;
			continue;
		}
	} while (true);

}



int main()
{
	bool quit = false;
	while (!quit) {
		gameSetting();
		bool play = true;
		if (mode == 4) {
			quit = true;
			continue;
		}
		else if (mode != 3) {
			do {
				GameBoard gameboard(6, 7);
				//gameboard.toString();
				gameboard.build();
				gameboard.printBoard();

				bool flag = true;
				while (gameOn) {
					int num;
					cout << "move:" << gameboard.getMoves() << endl;
					if (flag) {
						cout << "-----------------" << endl;
						cout << "Player O's turn !" << endl;
						cout << "-----------------" << endl;
					}
					else {
						cout << "-----------------" << endl;
						cout << "Player X's turn !" << endl;
						cout << "-----------------" << endl;
					}
					cout << "Please type a number you want to insert your piece:";

					cin >> num;
					if (cin.fail()) {
						cout << "Sorry, Invalid Input. Please type again." << endl;
						cin.clear();
						cin.ignore();
						continue;
					}
					else if (num > 0 && num <= gameboard.width) {
						if (gameboard.placePiece(num, flag)) {
							flag = !flag;
							gameboard.printBoard();
						}
					}
					else {
						cout << "Sorry, you input is out of range. Please type again." << endl;
						continue;
					}
				}

				if (nextPlay()) {
					play = true;
					gameOn = true;
					system("cls");
				}
				else {
					gameOn = true;
					play = false;
				}

			} while (play);
		}
		else if (mode == 3) {

			int first = 0;
			int mCol = 0;
			do {
				GameBoard gameboard(6, 7);
				//gameboard.toString();
				gameboard.build();
				gameboard.printBoard();
				if (gameOn && first == 0) {
					srand(time(NULL));
					gameboard.placePiece(rand() % 6 + 1, false);
					gameboard.printBoard();
					first++;
				}

				while (gameOn) {

					cout << "Please enter the col as your move:" << endl;
					cin >> mCol;
					if (cin.fail()) {
						cin.clear();
						cin.ignore();
						cout << "Sorry, Invalid Input." << endl;
						continue;
					}
					else if (mCol > 0 && mCol <= gameboard.width) {

						if (gameOn && gameboard.placePiece(mCol, true)) {
							gameboard.printBoard();
						}

						if (gameOn && first != 0) {
							gameboard.gameAI(false);
							gameboard.printBoard();
						}

					}
					else {

						cout << "Sorry, you input is out of range. Please type again." << endl;
						continue;
					}

				}
				if (nextPlay()) {
					gameOn = true;
					play = true;
					first = 0;
					system("cls");
				}
				else {
					gameOn = true;
					play = false;
				}
			} while (play);

		}
	}

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


