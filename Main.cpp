#include "Main.h"

#include <qapplication.h>
#include <qaction.h>
#include <qmenubar.h>

namespace ult {
	MainWindow::MainWindow()
		:QMainWindow()
	{
		setWindowTitle("Test");
		resize(800, 500);
		int* val = new int(2); //hmm

		board = new board::MainBoard(this, val);
		layout = new QHBoxLayout(this);
		centralWidget = new QWidget(this);
		fileMenu = new QMenu("File", this);

		QAction* newAction = new QAction("New Game", this);
		connect(newAction, &QAction::triggered, this, &MainWindow::newGame);

		QAction* closeAction = new QAction("Close", this);
		connect(closeAction, &QAction::triggered, this, &MainWindow::close);

		fileMenu->addAction(newAction);
		fileMenu->addAction(closeAction);

		this->menuBar()->addMenu(fileMenu);

		layout->addWidget(board);
		centralWidget->setLayout(layout);
		this->setCentralWidget(centralWidget);
	}

	void MainWindow::newGame()
	{
		board->deleteLater();
		int* val = new int(2);
		board = new board::MainBoard(this, val);
		layout->addWidget(board);
	}

} // end namespace ult

int main(int argc, char* argv[])
{
	QApplication app = QApplication(argc, argv);

	ult::MainWindow window;
	window.show();

	return app.exec();
}