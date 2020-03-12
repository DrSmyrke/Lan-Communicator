#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QWidget>
#include "global.h"

namespace Ui {
class TabWidget;
}

class TabWidget : public QWidget
{
	Q_OBJECT

public:
	explicit TabWidget(QWidget *parent = 0);
	~TabWidget();
signals:
	void signal_sendMess(const QString &text);
public slots:
	void slot_readMess(const QString &text, const QString &id);
private slots:
	void slot_sendMess();
private:
	Ui::TabWidget *ui;
};

#endif // TABWIDGET_H
