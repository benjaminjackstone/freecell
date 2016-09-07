#pragma once
//cards reside in cells, two clicks move from cell to cell

// Enhancements:
//
// Close event
// double buffering
// menu
// undo
// quit
// restart
// new game
// select game
// scooby cards
// resizeable
// check for winner
// cheat mode

#include <vector>

const int cellToCardMargin = 2;
const int spaceBetweenCells = 10;
const int verticalDropBetweenCards = 22;
const int maxCardsPerCell = 19; // the original 7 + 12 moved onto it.

class Cell
{
public:
	Cell(int l, int t, int r, int b);
	virtual void Draw(CPaintDC &dc); //set selected flag to true if selected entire cell, but only bottom card if selecting from bottom portion
	virtual bool CanRemoveCard();
	virtual bool CanAcceptCard(int index);
	void AddCard(int index);
	int RemoveCard();
	bool IsPointWithinCell(CPoint point); //loop through all 16 and ask if click inside, if so, return true else false
	int GetCardOnTop(); // or -1 if none there. return last index
	void SetSelected(bool selected); //first click -- highlight top card, tell entire cell it was selected
protected:
	std::vector <int> mCards; //given index is added here
	int mLeft, mTop, mRight, mBottom;
	bool mSelected;
};

class TempCell : public Cell
{
public:
	TempCell(int l, int t, int r, int b);
};

class EndCell : public Cell
{
public:
	EndCell(int l, int t, int r, int b);
	virtual bool CanRemoveCard(); //always false
	virtual bool CanAcceptCard(int index);
};

class StartCell : public Cell
{
public:
	StartCell(int l, int t, int r, int b);
	virtual void Draw(CPaintDC &dc);
	virtual bool CanAcceptCard(int index);
};
