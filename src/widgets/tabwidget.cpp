#include "tabwidget.h"
#include "ui_tabwidget.h"

#include <QDateTime>

TabWidget::TabWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TabWidget)
{
	ui->setupUi(this);

	connect( ui->sendB, &QPushButton::clicked, this, &TabWidget::slot_sendMess );
}

TabWidget::~TabWidget()
{
	delete ui;
}

void TabWidget::slot_readMess(const QString &text, const QString &id)
{
	QString username = id;
	for( auto elem:app::lanUsersData ){
		if( elem.id == id ){
			username = elem.username;
			break;
		}
	}
	QDateTime dt = QDateTime::currentDateTime();
	QString str = "[" + username + "] " + dt.toString("yyyy.MM.dd [hh:mm:ss] ") + "<br><i style=\"color: gray;\">>: ";
	ui->chatBox->insertHtml( str );
	ui->chatBox->insertPlainText( text );
	ui->chatBox->insertHtml( "</i><br>" );
}

void TabWidget::slot_sendMess()
{
	auto text = ui->sendBox->toPlainText();
	emit signal_sendMess( text );
	ui->sendBox->clear();
}
