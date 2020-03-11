#ifndef USERNAME_H
#define USERNAME_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include "global.h"

class UserNameWindow : public QDialog
{
	Q_OBJECT
public:
	struct Data
	{
		QString username;
	};
	explicit UserNameWindow(QWidget *parent = 0);
	QString getUsername();
private:
	QLineEdit* m_pUsernameBox;
	QLabel* m_pTitle;
};

#endif // USERNAME_H
