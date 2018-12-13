#include <vector>
#include <queue>
#include <cstdlib>
#include "masu.h"

struct coord {
	int x, y;

	coord(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
};

void masu::startGame(int width, int height, int numTarget) {
	int nPlaces = width * height;
	board.resize(nPlaces);
	opened.resize(nPlaces);
	flagged.resize(nPlaces);
	for (int i = 0; i < nPlaces; i++) {
		opened[i] = false;
		flagged[i] = false;
	}
	this->width = width;
	this->height = height;
	this->numTarget = numTarget;
	gameStarted = true;
	targetPlaced = false;
	dead = false;
}

bool masu::isActive() const {
	return gameStarted && !isDead() && !isSolved();
}

bool masu::isDead() const {
	return dead;
}

bool masu::isSolved() const {
	return gameStarted && !isDead() && width * height == numOpened + numTarget;
}

int masu::getWidth() const {
	return width;
}

int masu::getHeight() const {
	return height;
}

int masu::getNumTargetsOnStart() const {
	return numTarget;
}

int masu::getNumTargetsLeft() const {
	return numTarget - numFlagged;
}

bool masu::isValidCoord(int x, int y) const {
	return 0 <= x && x < width && 0 <= y && y < height;
}

void masu::hit(int x, int y) {
	if (!gameStarted || dead || !isValidCoord(x, y)) return;
	if (!targetPlaced) {
		initializeBoard(x, y);
		targetPlaced = true;
	}
	std::queue<coord> q;
	q.push(coord(x, y));
	while (!q.empty()) {
		coord c = q.front();
		q.pop();
		if (isValidCoord(c.x, c.y) && !opened[c.y * width + c.x]) {
			opened[c.y * width + c.x] = true;
			if (flagged[c.y * width + c.x]) {
				flagged[c.y * width + c.x] = false;
				numFlagged--;
			}
			if (board[c.y * width + c.x] == 9) {
				dead = true;
			} else {
				numOpened++;
				if (board[c.y * width + c.x] == 0) {
					for (int dy = -1; dy <= 1; dy++) {
						for (int dx = -1; dx <= 1; dx++) {
							if (dx != 0 || dy != 0) q.push(coord(c.x + dx, c.y + dy));
						}
					}
				}
			}
		}
	}
}

void masu::toggleFlag(int x, int y) {
	if (!gameStarted || dead) return;
	if (isValidCoord(x, y) && !opened[y * width + x]) {
		if (flagged[y * width + x]) {
			flagged[y * width + x] = false;
			numFlagged--;
		} else {
			flagged[y * width + x] = true;
			numFlagged++;
		}
	}
}

int masu::getStatus(int x, int y) const {
	if (isValidCoord(x, y)) {
		if (dead && board[y * width + x] == 9) {
			return flagged[y * width + x] ? 12 : 11;
		} else if (opened[y * width + x]) {
			return board[y * width + x];
		} else {
			return flagged[y * width + x] ? 10 : 9;
		}
	} else {
		return 9;
	}
}

bool masu::isOpened(int x, int y) const {
	if (isValidCoord(x, y)) {
		return opened[y * width + x];
	} else {
		return false;
	}
}

bool masu::isTarget(int x, int y) const {
	if (dead && isValidCoord(x, y)) {
		return board[y * width + x] == 9;
	} else {
		return false;
	}
}

bool masu::isFlagged(int x, int y) const {
	if (isValidCoord(x, y)) {
		return flagged[y * width + x];
	} else {
		return false;
	}
}

void masu::initializeBoard(int sx, int sy) {
	std::vector<coord> candidates;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// initialize board
			board[y * width + x] = 0;
			// don't put targets around the first hit point
			if (x < sx - 1 || sx + 1 < x || y < sy - 1 || sy + 1 < y) {
				candidates.push_back(coord(x, y));
			}
		}
	}
	if (numTarget > (int)candidates.size()) {
		// if there are too many targets, also put targets around the first hit point
		for (int y = sy - 1; y <= sy + 1; y++) {
			for (int x = sx - 1; x <= sx + 1; x++) {
				if ((x != sx || y != sy) && 0 <= x && x < width && 0 <= y && y < height) {
					candidates.push_back(coord(x, y));
				}
			}
		}
		// as the last move, put target on the first hit point
		if (numTarget > (int)candidates.size()) {
			if (0 <= sx && sx < width && 0 <= sy && sy < height) {
				candidates.push_back(coord(sx, sy));
			}
			// too too too many targets... cannot do that
			if (numTarget > (int)candidates.size()) numTarget = candidates.size();
		}
	}
	for (int i = 0; i < numTarget; i++) {
		// take one from candidate and put target there
		int idx = rand() % candidates.size();
		board[candidates[idx].y * width + candidates[idx].x] = 9;
		// remove it from the candidates
		candidates[idx] = candidates.back();
		candidates.pop_back();
	}
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (board[y * width + x] != 9) {
				int count = 0;
				for (int dy = -1; dy <= 1; dy++) {
					for (int dx = -1; dx <= 1; dx++) {
						int cx = x + dx;
						int cy = y + dy;
						if (0 <= cx && cx < width && 0 <= cy && cy < height && board[cy * width + cx] == 9) {
							count++;
						}
					}
				}
				board[y * width + x] = count;
			}
		}
	}
}
