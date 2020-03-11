#include "username.h"
#include <QLayout>
#include <QPushButton>

UserNameWindow::UserNameWindow(QWidget *parent) : QDialog(parent)
{
	QPushButton* submitB = new QPushButton(tr("Submit"),this);

	m_pTitle = new QLabel( this );
	m_pUsernameBox = new QLineEdit( this );

	QWidget* centrWidget = new QWidget(this);
		QVBoxLayout* vBox = new QVBoxLayout();
		vBox->addWidget( m_pTitle );
			QHBoxLayout* userBox = new QHBoxLayout();
			userBox->addWidget(new QLabel( tr("<b>Username:</b>") ));
			userBox->addWidget(m_pUsernameBox);
			userBox->addWidget(submitB);
		vBox->addLayout(userBox);
	centrWidget->setLayout(vBox);

	this->setLayout( vBox );
	setFixedSize( 320, 140 );
	setWindowTitle( tr("Username") );
	setModal( true );

	connect( submitB, &QPushButton::clicked, this, &UserNameWindow::accept);
}

QString UserNameWindow::getUsername()
{
	return m_pUsernameBox->text();
}
