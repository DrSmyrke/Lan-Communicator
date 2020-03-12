#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTreeWidgetItem>
#include "widgets/tabwidget.h"
#include "searcher.h"
#include "global.h"

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
	void slot_searcherUpdateList();
private:
	Ui::MainWindow *ui;
	Searcher* m_pSearcher;
	QTimer* m_pTimer;
	QTreeWidgetItem* m_pLocalContacts;
	QTreeWidgetItem* m_pContacts;
	TabWidget* m_pLocalChat;
};

#endif // MAINWINDOW_H
