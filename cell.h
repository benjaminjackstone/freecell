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
class Card
{
public:
	Card() {};
	Card(int i, int c, int s, int n);
	int GetIndex() { return mIndex; };
	int GetColor() { return mColor; };
	int GetSuit() { return mSuit; };
	int GetNum() { return mNum; };
	void SetIndex(int index) { mIndex = index; };
private:
	int mIndex;
	int mColor;
	int mSuit;
	int mNum;
};
class Cell
{
public:
	Cell(int l, int t, int r, int b);
	~Cell();
	virtual void Draw(CPaintDC &dc); //set selected flag to true if selected entire cell, but only bottom card if selecting from bottom portion
	virtual bool CanRemoveCard();
	virtual bool CanAcceptCard(int index, int color, int suit, int number);
	int GetLeft() { return mLeft; };
	int GetRight() { return mRight; };
	int GetTop() { return mTop; };
	int GetBottom() { return mBottom; };
	void SetTop(int t) { mTop = t; };
	void SetBottom(int b) { mBottom = b; };
	bool GetSelected() { return mSelected; };
	int GetCardsIndex(int index) { return mCards[index].GetIndex(); };
	int GetCardsSize() { return mCards.size(); };
	std::vector<Card> GetCards() { return mCards; };
	void SetCards(std::vector<Card> c) { mCards = c; };
	void AddCard(Card c, bool init);
	Card RemoveCard(bool init);
	bool IsPointWithinCell(CPoint point); //loop through all 16 and ask if click inside, if so, return true else false
	Card GetCardOnTop(); // or -1 if none there. return last index
	void SetSelected(bool selected); //first click -- highlight top card, tell entire cell it was selected

protected:
	std::vector <Card> mCards; //given index is added here
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
	virtual bool CanAcceptCard(int index, int color, int suit, int number);
	virtual void Draw(CPaintDC &dc);
};

class StartCell : public Cell
{
public:
	StartCell(int l, int t, int r, int b);
	virtual void Draw(CPaintDC &dc);
	virtual bool CanAcceptCard(int index, int color, int suit, int number);
};
