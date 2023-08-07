#pragma once

#include <array>
#include <iostream>

#include <qlabel.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qgridlayout.h>
#include <qpixmap.h>
#include <qevent.h>

namespace board {
	class MyBoard :public QLabel
	{
		Q_OBJECT

		int pos;

	protected:
		static const char* XImage;
		static const char* OImage;
		static const QPixmap* board;

		QGridLayout* layout;
		std::array<int*, 9> completes;

	public:
		int& complete;

	public:
		MyBoard(QWidget* parent, int* comp, int pos);
		bool setMassEnabled(bool enabled);
		bool setChildEnabled(bool enabled, int pos);
		template<typename type>
		void makeBoard();

	protected:
		void connectToChildren();

	signals:
		void checkSet();
		void posChanged(int);

	public slots:
		virtual void changePos(int pos);
		virtual void checkComplete();
		void setComplete(int val);
	};


	//   MainBoard   //

	class MainBoard : public MyBoard
	{
	public:
		MainBoard(QWidget* parent, int* i);

	private:
		void makeChildBoards(MyBoard* board, int number);

	public:
		virtual void checkComplete() override;
		virtual void changePos(int pos) override;

	};


	//   XOButton   //

	class XOButton : public QPushButton
	{
		Q_OBJECT

		static bool globalCheck;
		static const QIcon* XImage;
		static const QIcon* OImage;

	public:
		int& complete;
		int pos;

	public:
		XOButton(QWidget* parent, int* comp, int pos);

	protected:
		void resizeEvent(QResizeEvent* event);

	signals:
		void checkSet(int);

	private slots:
		void setCheck();

	};
} //end namespace board