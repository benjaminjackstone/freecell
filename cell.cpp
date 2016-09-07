#include "stdafx.h"
#include "cell.h"
int CARDH = 98;
int CARDW = 84;
int CARDM = 24;
Cell::Cell(int l, int t, int r, int b) 
	:mLeft(l),mRight(r),mBottom(b),mTop(t)
{
	//pass
}

void Cell::Draw(CPaintDC &dc) {
	CBrush whiteBrush(RGB(255, 0, 0));
	CBrush blueBrush(RGB(0, 0, 255));
	for (int t = 0; t < 8; t++) {
		if (t < 4) dc.SelectObject(blueBrush);
		else dc.SelectObject(whiteBrush);
		int x1 = CARDM + t*(CARDW + CARDM);
		int x2 = x1 + CARDW;
		int y1 = 1;
		int y2 = y1 + CARDH;
		dc.Rectangle(x1 * CARDW / 100, CARDH - y1 * CARDH / 100, x2 * CARDW / 100, CARDH - y2 * CARDH / 100);
	}
} //set selected falg to true if selected entire cell, but only bottom card if selecting from bottom portion
bool Cell::CanRemoveCard() {
	return false;
}

bool Cell::CanAcceptCard(int index) {
	return false;
}

void Cell::AddCard(int index) {

}

int Cell::RemoveCard() {
	return 0;
}
bool Cell::IsPointWithinCell(CPoint point) {
	return false;
}//loop through all 16 and ask if click inside, if so, return true else false

int Cell::GetCardOnTop() {
	return 0;
}// or -1 if none there. return last index

void Cell::SetSelected(bool selected) {

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
bool EndCell::CanAcceptCard(int index) {
	return false;
}

StartCell::StartCell(int l, int t, int r, int b) 
	: Cell(l, t, r, b)
{

}
void StartCell::Draw(CPaintDC &dc) {
	CBrush whiteBrush(RGB(255, 0, 255));
	dc.SelectObject(whiteBrush);
	// Draw the base of a tower
	for (int t = 0; t < 8; t++) {
		int x1 = CARDM + t*(CARDW + CARDM);
		int x2 = x1 + CARDW;
		int y1 = -120;
		int y2 = y1 + CARDH;
		dc.Rectangle(x1 * CARDW / 100, CARDH - y1 * CARDH / 100, x2 * CARDW / 100, CARDH - y2 * CARDH / 100);
	}
}

bool StartCell::CanAcceptCard(int index) {
	return false;
}