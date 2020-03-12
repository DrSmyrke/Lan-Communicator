#ifndef CONTACTEDITOR_H
#define CONTACTEDITOR_H

#include <QMainWindow>
#include "global.h"

namespace Ui {
class ContactEditor;
}

class ContactEditor : public QMainWindow
{
	Q_OBJECT

public:
	explicit ContactEditor(QWidget *parent = 0);
	~ContactEditor();
	void newContact();
	void editContact(const QString &id, const QString &userName, const QString &addr);
signals:
	void signal_updateContacts();
private:
	Ui::ContactEditor *ui;
	QString m_prewId;
	QString m_prewUserName;
	QString m_prewAddr;
};

#endif // CONTACTEDITOR_H
