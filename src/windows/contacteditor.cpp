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
		auto addr = ui->addrBox->text();
		auto tmp = addr.split( ":" );
		if( tmp.size() == 2 ){
			user.addr = QHostAddress( tmp[0] );
			user.port = tmp[1].toUInt();
		}
		if( tmp.size() == 1 ){
			user.addr = QHostAddress( tmp[0] );
			user.port = app::conf.port;
		}

		app::conctacts.push_back( user );

		emit signal_updateContacts();
		this->hide();
	} );
	connect( ui->saveB, &QPushButton::clicked, this, [this](){
		UserData user;
		user.id = ui->idBox->text();
		user.username = ui->userNameBox->text();
		auto addr = ui->addrBox->text();
		auto tmp = addr.split( ":" );
		if( tmp.size() == 2 ){
			user.addr = QHostAddress( tmp[0] );
			user.port = tmp[1].toUInt();
		}
		if( tmp.size() == 1 ){
			user.addr = QHostAddress( tmp[0] );
			user.port = app::conf.port;
		}

		QMutableListIterator< UserData > i( app::conctacts );
		while (i.hasNext()) {
			auto elem = i.next();
			if( elem.id == user.id ){
				elem.username = user.username;
				elem.addr = user.addr;
				elem.port = user.port;
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

void ContactEditor::editContact(const QString &id, const QString &userName, const QString &addr, const QString &port)
{
	ui->idBox->setText( id );
	ui->addrBox->setText( QString( "%1:%2" ).arg( addr ).arg( port ) );
	ui->userNameBox->setText( userName );

	ui->addB->hide();
	ui->saveB->show();
	ui->idBox->setEnabled( false );
	if( this->isHidden() ){
		this->show();
	}
}
