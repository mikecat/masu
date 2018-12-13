#ifndef MASU_H_GUARD_AE8EA53E_46A8_478D_B44A_38C3D707C0F7
#define MASU_H_GUARD_AE8EA53E_46A8_478D_B44A_38C3D707C0F7

#include <vector>

class masu {
private:
	int width, height, numTarget;
	std::vector<int> board;
	std::vector<bool> opened, flagged;
	bool gameStarted, targetPlaced, dead;

	int numOpened, numFlagged;
	bool isValidCoord(int x, int y) const;
	void initializeBoard(int sx, int sy);
public:
	masu() : width(0), height(0), numTarget(0),
		gameStarted(false), targetPlaced(false), dead(false),
		numOpened(0), numFlagged(0) {}

	void startGame(int width, int height, int nTarget);
	bool isActive() const;
	bool isDead() const;
	bool isSolved() const;
	int getWidth() const;
	int getHeight() const;
	int getNumTargetsOnStart() const;
	int getNumTargetsLeft() const;
	void hit(int x, int y);
	void toggleFlag(int x, int y);
	// 0-8 : opened, no target
	// 9   : unopened, not flagged
	// 10  : unopened, flagged
	// 11  : target, not flagged
	// 12  : target, flagged
	int getStatus(int x, int y) const;
	bool isOpened(int x, int y) const;
	bool isTarget(int x, int y) const;
	bool isFlagged(int x, int y) const;
};

#endif
