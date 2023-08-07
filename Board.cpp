#include "Board.h"

#include <algorithm>

#include <qsizepolicy.h>
#include <qlistwidget.h>


namespace board {

	const char* MyBoard::XImage = R"(QLabel {border-image: url(Media/biglX.png)})";

	const char* MyBoard::OImage = R"(QLabel {border-image: url(Media/biglO.png)})";

	const QPixmap* MyBoard::board = nullptr;


	MyBoard::MyBoard(QWidget* parent, int* comp, int pos)
		:QLabel(parent), complete(*comp), pos(pos)
	{
		if (board == nullptr)
			board = new QPixmap(R"(Media/board.png)", "png", Qt::ColorOnly);

		this->setObjectName("MyBoard");

		layout = new QGridLayout(this);

		this->setScaledContents(true);
		this->setPixmap(*board);
		
		this->setLayout(layout);
	}

	bool MyBoard::setMassEnabled(bool enabled) // sets *ALL* Children to "enabled" returns true if child else false
	{
		if (!children().empty()) {
			QList<MyBoard*> widgets = this->findChildren<MyBoard*>("MyBoard");
			for (int i = 0; i < widgets.size(); i++) {
				MyBoard* w = widgets.at(i);
				w->setEnabled(enabled);
				if (enabled) {
					w->setStyleSheet("QLabel {background-color: qradialgradient("
						"cx: 0.5, cy: 0.5, radius: 1.2, fx: 0.5, fy: 0.5, "
						"stop: 0 rgba(240,210,20,144), stop: 0.2 rgba(240,210,20,100), stop: 0.4 rgba(240,240,240,0));}");
				} else {
					w->setStyleSheet("");
				}
			}
			return true;
		}
		return false;
	}

	bool MyBoard::setChildEnabled(bool enabled, int pos) // sets child to "enabled" returns true if child else false
	{
		if (!children().empty()) {
			QList<MyBoard*> widgets = this->findChildren<MyBoard*>("MyBoard", Qt::FindDirectChildrenOnly);
			MyBoard* w = widgets.at(pos);
			if (std::find_if(w->completes.begin(), w->completes.end(), [](int* i) { return *i == 9; }) != w->completes.end()) { //checks if there are empty quares in board
				w->setEnabled(enabled);
				w->setStyleSheet("QLabel {background-color: qradialgradient("
					"cx: 0.5, cy: 0.5, radius: 1.2, fx: 0.5, fy: 0.5, "
					"stop: 0 rgba(240,210,20,144), stop: 0.2 rgba(240,210,20,100), stop: 0.4 rgba(240,240,240,0));}");
				return true;
			} // if no empty squares exsist, then it enables all squares
			setMassEnabled(true);
			return true;
		}
		return false;
	}

	template<typename type>
	void MyBoard::makeBoard() //Creates children based on type given
	{
		for (int x = 0; x < 3; x++) {
			for (int y = 0; y < 3; y++) {
				int* complete = new int(9); // 9 is an arbitrary value to mean not complete
				layout->addWidget(new type(this, complete, 3 * x + y), x, y);
				completes[3 * x + y] = complete;
			}
		}
		connectToChildren(); //connects to those children after
	}

	void MyBoard::connectToChildren()
	{
		QList<MyBoard*> widgets = this->findChildren<MyBoard*>("MyBoard");
		for (int i = 0; i < widgets.size(); i++) {
			MyBoard* w = widgets.at(i);
			connect(w, &MyBoard::checkSet, this, &MyBoard::checkComplete);
			connect(w, &MyBoard::posChanged, this, &MyBoard::changePos);
		}
	}

	void MyBoard::checkComplete() //not complete
	{
		if (complete == 9) {
			for (int x = 0; x < 4; x++) {
				int test1 = 1;
				int test2 = 1;
				for (int y = 0; y < 3; y++) {
					if (x == 3) { // Handles the cross exceptions
						test1 = test1 << *(completes[y * 4]);
						test2 = test2 << *(completes[y * 2 + 2]);
					} else {
						test1 = test1 << *(completes[(3 * x) + y]);
						test2 = test2 << *(completes[(3 * y) + x]);
					}
				}
				for (int test : {test1, test2}) {
					if (test == 1) { //checks for a row of X (false)
						this->setStyleSheet(XImage);
						setComplete(0);
					}
					else if (test == 8) { //check for a row of O (true)
						this->setStyleSheet(OImage);
						setComplete(1);
					}
				}
			}
		}
	}

	void MyBoard::setComplete(int val) 
	{
		complete = val;
		emit checkSet();
	}

	void MyBoard::changePos(int pos)
	{
		emit posChanged(pos);
	}

	//   MainBoard   //

	MainBoard::MainBoard(QWidget* parent, int* boardNumber)
		:MyBoard(parent, boardNumber, 1)
	{
		makeChildBoards(this, *boardNumber);
		complete = 9;
	}

	void MainBoard::makeChildBoards(MyBoard* board, int number) //Recursivly generates child boards based on number
	{
		number--;
		if (number) { 
			board->makeBoard<MyBoard>();
			QList<MyBoard*> children = board->findChildren<MyBoard*>("MyBoard");
			for (int i = 0; i < children.size(); i++)
				makeChildBoards(children.at(i), number);
		}
		else { 
			board->makeBoard<XOButton>(); 
		}
		
	}

	void MainBoard::checkComplete() //does nothing
	{
		MyBoard::checkComplete();
	}

	void MainBoard::changePos(int pos)
	{
		this->setMassEnabled(false);
		if (complete == 9) //handles the game ending
			this->setChildEnabled(true, pos);
	}

	//   XOButton   //

	bool XOButton::globalCheck = false;

	const QIcon* XOButton::XImage = nullptr;

	const QIcon* XOButton::OImage = nullptr;


	XOButton::XOButton(QWidget* parent, int* comp, int pos)
		:QPushButton(parent), complete(*comp), pos(pos)
	{
		if (XImage == nullptr && OImage == nullptr) {
			XImage = new QIcon(R"(Media/X.png)");
			OImage = new QIcon(R"(Media/O.png)");
		}

		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		this->setStyleSheet("border: none");

		connect(this, &XOButton::clicked, this, &XOButton::setCheck);
		connect(this, &XOButton::checkSet, (MyBoard*)parent, &MyBoard::checkComplete);
		connect(this, &XOButton::checkSet, (MyBoard*)parent, &MyBoard::changePos);
	}

	void XOButton::setCheck()
	{
		complete = (int)globalCheck;
		if (globalCheck) {
			this->setIcon(*OImage);
		} else {
			this->setIcon(*XImage);
		}
		globalCheck = !globalCheck;

		emit checkSet(pos);

		this->blockSignals(true);

	}

	void XOButton::resizeEvent(QResizeEvent* event) // resizing icons to fit squares
	{
		QRect geo = this->geometry();
		geo.setHeight(std::min((int)(geo.width() * .9), (int)(geo.height() * .9)));
		geo.setWidth(std::min((int)(geo.width() * .9), (int)(geo.height() * .9)));

		this->setIconSize(geo.size());
	}

} //end namespace board