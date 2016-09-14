
// FreeCellStarter2010Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "FreeCellStarter2010.h"
#include "FreeCellStarter2010Dlg.h"
#include "WindowsCards.h"
#include "cell.h"
#include "afxdialogex.h"
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About
std::vector<Cell *> mCells;
std::vector<Card> UndoList;
std::vector<int> UndoLCellTo;
std::vector<int> UndoLCellFrom;
std::vector<Card> RedoList;
std::vector<int> RedoLCellTo;
std::vector<int> RedoLCellFrom;
int gCardH = 100;
int gCARDW = 82;
int gCARDspace = 10;
int ggap = 26;
bool gClicked = false;
bool gDoubleClicked = false;
bool gLoadgame = false;
int gSelected = -1;
time_t gseed = -1;
int CLUB = 0;
int DIAMOND = 1;
int HEARTS = 2;
int SPADE = 3;
int BLACK = 0;
int RED = 1;
int gLastcellMovedfrom = -1;
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFreeCellStarter2010Dlg dialog




CFreeCellStarter2010Dlg::CFreeCellStarter2010Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFreeCellStarter2010Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFreeCellStarter2010Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFreeCellStarter2010Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CLOSE()
	ON_WM_KEYDOWN()
	ON_COMMAND(IDR_MENU1, &CFreeCellStarter2010Dlg::OnFileNewgame)
	ON_COMMAND(IDR_MENU2, &CFreeCellStarter2010Dlg::OnUndo)
	ON_COMMAND(IDR_MENU3, &CFreeCellStarter2010Dlg::OnRedo)
	ON_COMMAND(IDR_MENU4, &CFreeCellStarter2010Dlg::OnMenu4)
	ON_COMMAND(IDR_UNDO, &CFreeCellStarter2010Dlg::OnMenu5)
	ON_COMMAND(IDRETRY, &CFreeCellStarter2010Dlg::OnRetry)
END_MESSAGE_MAP()

// CFreeCellStarter2010Dlg message handlers
void CFreeCellStarter2010Dlg::ShuffleDeck() {
	int card = 0;
	for (int i = 0; i < 16; i++) {
		if (i < 4) {
			TempCell * c = new TempCell(i*gCARDW + gCARDspace, gCARDspace, gCARDW*(i + 1), gCardH + 7); //coordinates in Pixels, top is kind of down
			mCells.push_back(c);
		}
		if (i >= 4 && i < 8) {
			EndCell * c = new EndCell(i*gCARDW + gCARDspace, gCARDspace, gCARDW*(i + 1), gCardH + 7); //coordinates in Pixels, top is kind of down
			mCells.push_back(c);
		}
		if (i >= 8) {
			StartCell * c = new StartCell((i - 8)*gCARDW + gCARDspace, gCardH + gCARDspace * 2, gCARDW*(i - 7), (ggap * 19) + gCardH + gCARDspace * 2); //coordinates in Pixels, top is kind of down
			mCells.push_back(c);
			std::vector<Card> _cards;
			gseed = time(NULL);
			srand(gseed);
			int cardindex = 0;
			int cardcolor = 0;
			int cardsuit = 0;
			int cardnumber = 0;
			int cardnumberswitch = 0;
			for (int i = 0; i < 52; i++) {
				cardnumber = i / 4 + 1;// integer division. sequence of fours. plus one for the Ace
				if (cardcolor == 0 || cardcolor == 3) {
					Card newcard(cardindex, BLACK, cardsuit, cardnumber);
					_cards.push_back(newcard);
				}
				else {
					Card newcard(cardindex, RED, cardsuit, cardnumber);
					_cards.push_back(newcard);
				}
				cardindex++;
				cardcolor++;
				cardsuit++;
				if (cardcolor > 3) {
					cardcolor = 0;
					cardsuit = 0;
				}
			}
			for (int i = 0; i < 52; i++) {
				int randindex = rand() % 52;
				Card temp = _cards[i];
				_cards[i] = _cards[randindex];
				_cards[randindex] = temp;
			}
			for (int c = 0; c < 6; c++) {
				mCells[i]->AddCard(_cards[card], true); //pass in true for initialization to bypass CanAcceptCard
				card += 1;
			}
			if (i < 12) {
				mCells[i]->AddCard(_cards[card], true);
				card++;
			}
		}
	}
	bool ok = InitializeCards();
	if (!ok)
	{
		MessageBox(L"cards.dll failed to initialize", L"Error");
		//return true;
	}
}

BOOL CFreeCellStarter2010Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	ShuffleDeck();
	
	// Setup the cards
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFreeCellStarter2010Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFreeCellStarter2010Dlg::OnPaint()
{
	if (IsIconic())
	{

		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		int cell_index = -1;
		bool highlighted = false;
		int stack = 0;
		std::vector<Cell *>::const_iterator sit;
		int iter = 0;
		for (sit = mCells.begin(); sit != mCells.end(); sit++)
		{
			int x= (*sit)->GetLeft();
			int y= (*sit)->GetTop();
			(*sit)->Draw(dc);
			if ((*sit)->GetSelected()) highlighted = true;
			for (int i = 0; i < (*sit)->GetCardsSize(); i++) {
				if (highlighted && (i+1 != (*sit)->GetCardsSize())) {
					DrawCard(dc, x, y, (*sit)->GetCardsIndex(i), false);
				}
				else DrawCard(dc, x, y, (*sit)->GetCardsIndex(i), highlighted);
				if(iter > 7) y += ggap;
			}
			(*sit)->SetSelected(false);
			highlighted = false;
			iter++;
		}
		CheckMoves(dc);
	}
}
void CFreeCellStarter2010Dlg::CheckMoves(CPaintDC &dc) {
	bool moveexists = false;
	for (int i = 0; i < mCells.size(); i++) {
		Card lastcard = mCells[i]->GetCardOnTop();
		for (int j = 0; j < mCells.size(); j++) {
			if (mCells[j]->CanAcceptCard(lastcard.GetIndex(), lastcard.GetColor(), lastcard.GetSuit(), lastcard.GetNum())) {
				moveexists = true;
			}
		}
	}
	if (!moveexists) dc.TextOutW(100, 100, L" NO MORE MOVES POSSIBLE ");
	//check win
	for (int i = 4; i < 8; i++) {
		if (mCells[i]->GetCardOnTop().GetNum() == 13 && mCells[i + 1]->GetCardOnTop().GetNum() == 13 &&
			mCells[i + 2]->GetCardOnTop().GetNum() == 13 &&
			mCells[i + 3]->GetCardOnTop().GetNum() == 13) {
			dc.TextOutW(100, 100, L" VICTORY!!! ");
		}
	}
}
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFreeCellStarter2010Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFreeCellStarter2010Dlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	std::vector<Cell *>::const_iterator sit;
	for (sit = mCells.begin(); sit != mCells.end(); sit++)
	{
		delete (*sit);
	}
	UninstallCards();
	CDialogEx::OnClose();
}
void CFreeCellStarter2010Dlg::OnLButtonDblClk(UINT nFlags, CPoint point) {
	int i = 0;
	for (i = 0; i < 16; i++) {
		gDoubleClicked = mCells[i]->IsPointWithinCell(point);
		if (gDoubleClicked) break;
	}
	if (gDoubleClicked) {
		gDoubleClicked = false;
		for (int j = 4; j < 8; j++) {
			int clicksuit = mCells[i]->GetCardOnTop().GetSuit();
			int clicknumber = mCells[i]->GetCardOnTop().GetNum();
			int endsuit = mCells[j]->GetCardOnTop().GetSuit();
			int endnumber = mCells[j]->GetCardOnTop().GetNum();
			if (((clicksuit == endsuit) && clicknumber - 1 == endnumber) || (endnumber < 0) && clicknumber == 1) {
				Card removedcard = mCells[i]->RemoveCard(false);
				if (removedcard.GetIndex() >= 0) {
					mCells[j]->AddCard(removedcard, false);
					mCells[i]->SetSelected(false);
					UndoLCellTo.push_back(j);
					UndoLCellFrom.push_back(i);
					UndoList.push_back(removedcard);
					break;
				}
			}
		}
	}
	Invalidate();
	//
	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CFreeCellStarter2010Dlg::OnLButtonDown(UINT nFlags, CPoint point) {
	bool clickedagain = false;
	for (int i = 0; i < 16; i++) {
		if (gClicked) {
			gClicked = false;
			mCells[gSelected]->SetSelected(false);
			for (int j = 0; j < 16; j++) {
				clickedagain = mCells[j]->IsPointWithinCell(point);
				if (clickedagain) {
					if (mCells[gSelected]->GetCardsSize() > 0) {
						if (mCells[j]->CanAcceptCard(mCells[gSelected]->GetCardOnTop().GetIndex(), mCells[gSelected]->GetCardOnTop().GetColor(), mCells[gSelected]->GetCardOnTop().GetSuit(), mCells[gSelected]->GetCardOnTop().GetNum())) {
							Card removedcard = mCells[gSelected]->RemoveCard(false);
							if (removedcard.GetIndex() >= 0) {
							mCells[j]->AddCard(removedcard, false);
							gLastcellMovedfrom = gSelected;
							UndoLCellTo.push_back(j);
							UndoLCellFrom.push_back(gSelected);
							UndoList.push_back(removedcard);
							}
						}
					}
					mCells[j]->SetSelected(false);
					gSelected = -1;
					clickedagain = false;
					break;
				}
			}
			break;
		}
		gClicked = mCells[i]->IsPointWithinCell(point);
		if (gClicked) {
			gSelected = i;
			break;
		}
	}
//
	Invalidate();
//
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CFreeCellStarter2010Dlg::OnFileNewgame()
{
	for (int t = 0; t < UndoLCellTo.size(); t++) {
		UndoLCellFrom.pop_back();
		UndoLCellTo.pop_back();
		UndoList.pop_back();
	}
	for (int t = 0; t < RedoLCellFrom.size(); t++) {
		RedoLCellFrom.pop_back();
		RedoLCellTo.pop_back();
		RedoList.pop_back();
	}
	// TODO: Add your command handler code here
	std::vector<std::vector<Card>> AllCards;
	int maxincell = 0;
	for (int i = 0; i < mCells.size(); i++) {
		if (i > 7) maxincell = 7;
		if (i > 11) maxincell = 6;
			int k = i + 1;
			int size = mCells[i]->GetCardsSize();
			while (size > maxincell) {
				Card c;
				if (i < 8 && mCells[i]->GetCardsSize() > 0) c = mCells[i]->RemoveCard(true);
				else c = mCells[i]->RemoveCard(false);
				if (c.GetIndex() >= 0) {
					int randindex = rand() % (16 - i) + (i + 1);
					while (randindex > 15) randindex = rand() % (16 - i) + (i + 1);
					mCells[randindex]->AddCard(c, true);
					size = mCells[i]->GetCardsSize();
				}
			}
			while (size < maxincell && k < 16 && mCells[k]->GetCardsSize() > 0) {
					Card c = mCells[k]->RemoveCard(false);
					if (c.GetIndex() < 0) k++;
					mCells[i]->AddCard(c, true);
					size = mCells[i]->GetCardsSize();
			}
			std::vector<Card> card;
			AllCards.push_back(card);
		}
	CPaintDC dc(this); // device context for painting
	CString a,b;
	if (!gLoadgame) gseed = time(NULL);
	srand(gseed);
	gLoadgame = false;
	dc.TextOutW(100, 615, a);
		std::vector<Card> _cards;
		int cardindex = 0;
		int cardcolor = 0;
		int cardsuit = 0;
		int cardnumber = 0;
		int count = 8;
		int max = 7;
		for (int i = 0; i < 52; i++) {
			cardnumber = i / 4 + 1;// integer division. sequence of fours. plus one for the Ace
			if (cardcolor == 0 || cardcolor == 3) {
				Card newcard(cardindex, BLACK, cardsuit, cardnumber);
				_cards.push_back(newcard);
			}
			else {
				Card newcard(cardindex, RED, cardsuit, cardnumber);
				_cards.push_back(newcard);
			}
			cardindex++;
			cardcolor++;
			cardsuit++;
			if (cardcolor > 3) {
				cardcolor = 0;
				cardsuit = 0;
			}
			
		}
		for (int i = 0; i < 52; i++) {
			int randindex = rand() % 52;
			Card temp = _cards[i];
			_cards[i] = _cards[randindex];
			_cards[randindex] = temp;
		}
		for (int i = 0; i < 52; i++) {
			if (i == 7) count++;
			if (i == 14) count++;
			if (i == 21) count++;
			if (i == 28) count++;
			if (i == 34) count++;
			if (i == 40) count++;
			if (i == 46) count++;
			if (i > 27) max = 6;
			AllCards[count].push_back(_cards[i]);
		}
	for (int i = 0; i < mCells.size(); i++) {
		mCells[i]->SetCards(AllCards[i]);
	}
}

void CFreeCellStarter2010Dlg::OnUndo()
{
	if (UndoList.size() > 0) {
		// TODO: Add your command handler code here
		//make a vector of last selected cells and the vector of removed cards. as long as they are in sync, they should map to each other
		int to = UndoLCellTo[UndoLCellTo.size() - 1];
		int from = UndoLCellFrom[UndoLCellFrom.size() - 1];
		Card c = UndoList[UndoList.size() - 1];
		Card removedcard = mCells[to]->RemoveCard(true);
		if (removedcard.GetIndex() > 0) {
			mCells[from]->AddCard(removedcard, true);
		}
		RedoLCellTo.push_back(from);
		RedoLCellFrom.push_back(to);
		RedoList.push_back(c);
		UndoLCellFrom.pop_back();
		UndoLCellTo.pop_back();
		UndoList.pop_back();
	}
}
void CFreeCellStarter2010Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	OnUndo();
	Invalidate();
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CFreeCellStarter2010Dlg::OnRedo()
{
	if (RedoList.size() > 0) {
		// TODO: Add your command handler code here
		//make a vector of last selected cells and the vector of removed cards. as long as they are in sync, they should map to each other
		int to = RedoLCellTo[RedoLCellTo.size() - 1];
		int from = RedoLCellFrom[RedoLCellFrom.size() - 1];
		Card c = RedoList[RedoList.size() - 1];
		Card removedcard = mCells[to]->RemoveCard(true);
		if (removedcard.GetIndex() > 0) {
			mCells[from]->AddCard(removedcard, true);
		}
		RedoLCellTo.pop_back();
		RedoLCellFrom.pop_back();
		RedoList.pop_back();
		UndoLCellFrom.push_back(to);
		UndoLCellTo.push_back(from);
		UndoList.push_back(c);
	}
}

void CFreeCellStarter2010Dlg::OnRButtonDown(UINT nFlags, CPoint point) {
	OnRedo();
	Invalidate();
	CDialogEx::OnRButtonDown(nFlags, point);
}

void CFreeCellStarter2010Dlg::OnMenu4()
{
	// TODO: Add your command handler code here
	CPaintDC dc(this); // device context for painting
	CString a, b;
	a.Format(_T("Your saved game is %d"), gseed);
	//b.Format(_T("freecell%d.txt"), gseed);
	b = _T("freecell.txt");
	std::ofstream fout(b);
	fout << gseed;
	fout.close();
	dc.TextOutW(100, 615, a);
}


void CFreeCellStarter2010Dlg::OnMenu5()
{
	// TODO: Add your command handler code here
	time_t t;
	std::ifstream fin("freecell.txt");
	fin >> t;
	gseed = t;
	gLoadgame = true;
	OnFileNewgame();
}


void CFreeCellStarter2010Dlg::OnRetry()
{
	// TODO: Add your command handler code here
	gLoadgame = true;
	OnFileNewgame();
}
