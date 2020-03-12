#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTreeWidgetItem>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include "widgets/tabwidget.h"
#include "windows/contacteditor.h"
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
	void slot_updateList();
	void slot_trayIconActivated(QSystemTrayIcon::ActivationReason reason);
	void slot_newMessage(const QString &text, const QString &id);
	void slot_contactListContextMenu(const QPoint &pos);
private:
	Ui::MainWindow *ui;
	Searcher* m_pSearcher;
	QTimer* m_pTimer;
	QTreeWidgetItem* m_pLocalContacts;
	QTreeWidgetItem* m_pContacts;
	TabWidget* m_pLocalChat;
	QMenu* m_pTrayMenu;
	QSystemTrayIcon* m_pTrayIcon;
	ContactEditor* m_pContactEditor;

	// QWidget interface
protected:
	void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
