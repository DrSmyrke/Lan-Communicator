#include "contacteditor.h"
#include "ui_contacteditor.h"

ContactEditor::ContactEditor(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ContactEditor)
{
	ui->setupUi(this);

	connect( ui->addB, &QPushButton::clicked, this, [this](){
		UserData user;
		user.id = ui->idBox->text();
		user.username = ui->userNameBox->text();
		user.addr = QUrl( ui->addrBox->text() );

		app::conctacts.push_back( user );

		emit signal_updateContacts();
		this->hide();
	} );
	connect( ui->saveB, &QPushButton::clicked, this, [this](){
		UserData user;
		user.id = ui->idBox->text();
		user.username = ui->userNameBox->text();
		user.addr = QUrl( ui->addrBox->text() );

		QMutableListIterator< UserData > i( app::conctacts );
		while (i.hasNext()) {
			auto elem = i.next();
			if( elem.id == m_prewId && elem.username == m_prewUserName && elem.addr.toString() == m_prewAddr ){
				elem.username = user.username;
				elem.addr = user.addr;
				i.setValue( elem );
				break;
			}
		}

		emit signal_updateContacts();
		this->hide();
	} );
}

ContactEditor::~ContactEditor()
{
	delete ui;
}

void ContactEditor::newContact()
{
	ui->idBox->clear();
	ui->addrBox->clear();
	ui->userNameBox->clear();

	ui->addB->show();
	ui->saveB->hide();
	ui->idBox->setEnabled( true );
	if( this->isHidden() ){
		this->show();
	}
}

void ContactEditor::editContact(const QString &id, const QString &userName, const QString &addr)
{
	ui->idBox->setText( id );
	ui->addrBox->setText( addr );
	ui->userNameBox->setText( userName );

	m_prewId = id;
	m_prewUserName = userName;
	m_prewAddr = addr;

	ui->addB->hide();
	ui->saveB->show();
	ui->idBox->setEnabled( false );
	if( this->isHidden() ){
		this->show();
	}
}
