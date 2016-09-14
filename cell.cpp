#include "stdafx.h"
#include "cell.h"
#include <string>
#include <vector>
#include <fstream>
#include <istream>

extern int CARDH = 98;
int CARDW = 84;
int CARDM = 24;
Card::Card(int i, int c, int s, int n) 
	:mIndex(i), mColor(c), mSuit(s), mNum(n)
{

}
Cell::Cell(int l, int t, int r, int b) 
	:mLeft(l),mRight(r),mBottom(b),mTop(t),mSelected(false)
{
	//pass
}
Cell::~Cell() {
	//
}

void Cell::Draw(CPaintDC &dc) {
	CBrush whiteBrush(RGB(255, 0, 0));
	dc.SelectObject(whiteBrush);
	dc.Rectangle(mLeft, mBottom, mRight, mTop);
	dc.TextOutW(GetLeft() + 2, GetBottom()/2 - 15, L" Holding ");
} //set selected falg to true if selected entire cell, but only bottom card if selecting from bottom portion
bool Cell::CanRemoveCard() {
	if (mCards.size() > 0) return true;
	return false;
}

bool Cell::CanAcceptCard(int index, int color, int suit, int number) {
	int accepting_stack_index = GetCardOnTop().GetIndex();
	if (accepting_stack_index == -1) {
		return true;
	}
	return false;
}

void Cell::AddCard(Card c, bool init) {
	if (init) mCards.push_back(c);
	else if (CanAcceptCard(c.GetIndex(), c.GetColor(), c.GetSuit(), c.GetNum())) {
		mCards.push_back(c);
	}
}

Card Cell::RemoveCard(bool init) {
	if (CanRemoveCard() || init) { // bypass endcell restriction on shuffle
		Card card = GetCardOnTop();
		mCards.pop_back();
		return card;
	}
	Card c(-1, -1, -1, -1);
}
bool Cell::IsPointWithinCell(CPoint point) {
	//CHECK THE CELL
	if (point.x >= mLeft && point.y <= mBottom && point.x <= mRight && point.y >= mTop) {
		mSelected = true;
		return true;
	}
	return false;
}//loop through all 16 and ask if click inside, if so, return true else false

Card Cell::GetCardOnTop() {
	if (mCards.size() > 0) {
		return mCards[mCards.size() - 1];
	}
	Card c(-1, -1, -1, -1);
	return c; // no card in cell, so return a Card with -1 as index
}// or -1 if none there. return last index

void Cell::SetSelected(bool selected) {
	mSelected = selected;
}//first click -- highlight top card, tell entire cell it was selected

TempCell::TempCell(int l, int t, int r, int b) 
	:Cell(l,t,r,b)
{

}

EndCell::EndCell(int l, int t, int r, int b) 
	: Cell(l, t, r, b)
{

}

bool EndCell::CanRemoveCard() {
	return false;
}
bool EndCell::CanAcceptCard(int index, int color, int suit, int number) {
	int accept_stack_number = GetCardOnTop().GetNum();
	int accept_stack_suit = GetCardOnTop().GetSuit();
	if ((suit == accept_stack_suit && accept_stack_number + 1 == number) || (accept_stack_number < 0 && number == 1)) {
		return true;
	}
	return false;
}

StartCell::StartCell(int l, int t, int r, int b) 
	: Cell(l, t, r, b)
{

}
void EndCell::Draw(CPaintDC &dc) {
	CBrush whiteBrush(RGB(0, 0, 255));
	dc.SelectObject(whiteBrush);
	dc.Rectangle(mLeft, mBottom, mRight, mTop);
}
void StartCell::Draw(CPaintDC &dc) {
	CBrush whiteBrush(RGB(255, 255, 255));
	dc.SelectObject(whiteBrush);
	dc.Rectangle(mLeft, mBottom, mRight, mTop);
}

bool StartCell::CanAcceptCard(int selectedindex, int selectedcolor, int selectedsuit, int selectednumber) {
	int accept_stack_index = GetCardOnTop().GetIndex();
	int accept_stack_color = GetCardOnTop().GetColor();
	int accept_stack_number = GetCardOnTop().GetNum();
	if (GetCardsSize() <= 19 && (selectedcolor != accept_stack_color && accept_stack_number - 1 == selectednumber) || (accept_stack_index == -1 && selectednumber == 13)) {
		return true;
	}
	return false;
}