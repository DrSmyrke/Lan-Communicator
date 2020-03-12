#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_pSearcher = new Searcher( this );
	m_pTimer = new QTimer( this );
	m_pLocalContacts = new QTreeWidgetItem();
	m_pContacts = new QTreeWidgetItem();
	m_pLocalChat = new TabWidget( this );
	m_pTrayMenu = new QMenu( this );
	m_pTrayIcon = new QSystemTrayIcon( this );

	connect( m_pTimer, &QTimer::timeout, this, &MainWindow::slot_timerUpdate );
	connect( m_pSearcher, &Searcher::signal_updateList, this, &MainWindow::slot_searcherUpdateList );
	connect( m_pLocalChat, &TabWidget::signal_sendMess, m_pSearcher, &Searcher::slot_sendMess );
	connect( m_pSearcher, &Searcher::signal_readMess, m_pLocalChat, &TabWidget::slot_readMess );
	connect( m_pTrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::slot_trayIconActivated );
	connect( m_pSearcher, &Searcher::signal_readMess, this, &MainWindow::slot_newMessage );

	setWindowTitle( "Lan Communicator v" + app::conf.version );
	setWindowIcon( QIcon( "://index.ico" ) );
	setMinimumSize( 640, 480 );

	m_pTimer->setInterval( 3000 );
	m_pTimer->start();
	m_pSearcher->search();
	m_pLocalContacts->setText( 0, tr( "Local Network" ) );
	m_pLocalContacts->setIcon( 0, QIcon( "://images/lan.png" ) );
	m_pContacts->setText( 0, tr( "Contacts" ) );
	ui->contsctsListWidget->addTopLevelItem( m_pLocalContacts );
	ui->contsctsListWidget->addTopLevelItem( m_pContacts );
	ui->tabWidget->addTab( m_pLocalChat, QIcon( "://images/lan.png" ), tr( "Local Newtwork" ) );
	m_pTrayIcon->setIcon( QIcon( "://index.ico") );
	m_pTrayIcon->setContextMenu( m_pTrayMenu );

	m_pTrayMenu->addAction( tr("Exit"), this, &MainWindow::close );
}

MainWindow::~MainWindow()
{
	delete ui;
	app::saveSettings();
}

void MainWindow::slot_timerUpdate()
{
	m_pSearcher->search();
}

void MainWindow::slot_searcherUpdateList()
{
	auto countLocal = m_pLocalContacts->childCount();
	//auto count = m_pContacts->childCount();
	for( int i = 0; i < countLocal; i++ ){
		m_pLocalContacts->removeChild( m_pLocalContacts->child( i ) );
	}

	QMutableListIterator< UserData > i( app::lanUsersData );
	while (i.hasNext()) {
		auto elem = i.next();
		QTreeWidgetItem* item = new QTreeWidgetItem();
		auto help = ( elem.username.isEmpty() ) ? elem.id : elem.username;
		auto text = QString( "%1:%2\n%3" ).arg( elem.addr.toString() ).arg( elem.port ).arg( help );
		item->setText( 0, text );
		item->setIcon( 0, QIcon( "://images/PC.png" ) );
		m_pLocalContacts->addChild( item );
	}

//	ui->localHubListW->clear();
//

//	for( auto elem:m_pSearcher->getData() ){
//		QListWidgetItem* item = new QListWidgetItem();
//		uint r = currentTimestamp - elem.timestamp;
//		auto text = QString( "%1:%2<sub>%3</sub>" ).arg( elem.addr.toString() ).arg( elem.port ).arg( r );
//		item->setText( text );
//		ui->localHubListW->addItem( item );
//	}

	//ui->contsctsListWidget->addTopLevelItem(  );
}

void MainWindow::slot_trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
		case QSystemTrayIcon::Trigger:
		case QSystemTrayIcon::DoubleClick:
			//iconComboBox->setCurrentIndex((iconComboBox->currentIndex() + 1) % iconComboBox->count());
			if( this->isHidden() ){
				this->show();
				m_pTrayIcon->hide();
			}
		break;
		case QSystemTrayIcon::MiddleClick:
			//showMessage();
		break;
		default: break;
	}
}

void MainWindow::slot_newMessage(const QString &text, const QString &id)
{
	if( !m_pTrayIcon->isVisible() ){
		return;
	}

	QString username = id;
	for( auto elem:app::lanUsersData ){
		if( elem.id == id ){
			username = elem.username;
			break;
		}
	}

	auto title = QString( "Lan Network message" );
	auto message = QString( "[%1]\n>: %2" ).arg( username ).arg( text );

	m_pTrayIcon->showMessage( title, message, QSystemTrayIcon::Information, 10000 );
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if( !m_pTrayIcon->isVisible() ){
		this->hide();
		m_pTrayIcon->show();
		event->ignore();
	}
}
