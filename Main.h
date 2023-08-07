#pragma once
#include <qmainwindow.h>
#include <qboxlayout.h>
#include <qmenu.h>

#include "Board.h"

namespace ult {
	class MainWindow :public QMainWindow
	{
	private:
		QHBoxLayout* layout;
		QWidget* centralWidget;
		board::MainBoard* board;
		QMenu* fileMenu;

	public:
		MainWindow();

	private:
		void newGame();
	};
} // end namespace ult