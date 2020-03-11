#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "searcher.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
private slots:
	void slot_timerUpdate();
private:
	Ui::MainWindow *ui;
	Searcher* m_pSearcher;
	QTimer* m_pTimer;
};

#endif // MAINWINDOW_H
