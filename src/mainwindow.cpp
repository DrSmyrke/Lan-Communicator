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
	m_pContactEditor = new ContactEditor( this );

	connect( m_pTimer, &QTimer::timeout, this, &MainWindow::slot_timerUpdate );
	connect( m_pSearcher, &Searcher::signal_updateList, this, &MainWindow::slot_updateList );
	connect( m_pLocalChat, &TabWidget::signal_sendMess, m_pSearcher, &Searcher::slot_sendMess );
	connect( m_pSearcher, &Searcher::signal_readMess, m_pLocalChat, &TabWidget::slot_readMess );
	connect( m_pTrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::slot_trayIconActivated );
	connect( m_pSearcher, &Searcher::signal_readMess, this, &MainWindow::slot_newMessage );
	connect( ui->contsctsListWidget, &QTreeWidget::customContextMenuRequested, this, &MainWindow::slot_contactListContextMenu );
	connect( m_pContactEditor, &ContactEditor::signal_updateContacts, this, &MainWindow::slot_updateList );

	setWindowTitle( "Lan Communicator v" + app::conf.version );
	setWindowIcon( QIcon( "://index.ico" ) );
	setMinimumSize( 640, 480 );

	m_pTimer->setInterval( 3000 );
	m_pTimer->start();
	m_pSearcher->search();
	m_pLocalContacts->setText( 0, tr( "Local Network" ) );
	m_pLocalContacts->setIcon( 0, QIcon( "://images/lan.png" ) );
	m_pLocalContacts->setFlags( Qt::NoItemFlags | Qt::ItemIsEnabled );
	m_pContacts->setText( 0, tr( "Contacts" ) );
	m_pContacts->setFlags( Qt::NoItemFlags | Qt::ItemIsEnabled );
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

void MainWindow::slot_updateList()
{
	uint currentUnixTime = app::getUnixTime();
	auto countLocal = m_pLocalContacts->childCount();
	auto count = m_pContacts->childCount();
	for( int i = 0; i < countLocal; i++ ){
		m_pLocalContacts->removeChild( m_pLocalContacts->child( i ) );
	}
	for( int i = 0; i < count; i++ ){
		m_pContacts->removeChild( m_pContacts->child( i ) );
	}

	QMutableListIterator< UserData > i( app::lanUsersData );
	while (i.hasNext()) {
		auto elem = i.next();
		QTreeWidgetItem* item = new QTreeWidgetItem();
		auto username = ( elem.username.isEmpty() ) ? elem.id : elem.username;
		uint r = currentUnixTime - elem.timestamp;
		auto tooltip = QString( "last update %1 sec ago" ).arg( r );
		item->setText( 0, username );
		item->setIcon( 0, QIcon( "://images/PC.png" ) );
		item->setToolTip( 0, tooltip );
		item->setData( 0, Qt::UserRole, QString( "L:%1" ).arg( elem.id ) );
		m_pLocalContacts->addChild( item );
	}

	QMutableListIterator< UserData > ic( app::conctacts );
	while (ic.hasNext()) {
		auto elem = ic.next();
		QTreeWidgetItem* item = new QTreeWidgetItem();
		auto username = ( elem.username.isEmpty() ) ? elem.id : elem.username;
		uint r = currentUnixTime - elem.timestamp;
		auto tooltip = QString( "last update %1 sec ago" ).arg( r );
		item->setText( 0, username );
		item->setIcon( 0, QIcon( "://images/user.png" ) );
		item->setToolTip( 0, tooltip );
		item->setData( 0, Qt::UserRole, QString( "C:%1:%2:%3:%4" ).arg( elem.id ).arg( elem.username ).arg( elem.addr.toString() ).arg( elem.port ) );
		m_pContacts->addChild( item );
	}
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

void MainWindow::slot_contactListContextMenu(const QPoint &pos)
{
	QMenu *menu = new QMenu(this);

	menu->addAction( tr("Add contact"), this, [this](){
		m_pContactEditor->newContact();
	} );

	if( ui->contsctsListWidget->currentItem()->data( 0, Qt::UserRole ).isValid() ){
		auto str = ui->contsctsListWidget->currentItem()->data( 0, Qt::UserRole ).toString();
		auto tmp = str.split( ":" );
		if( tmp.size() == 5 && tmp[0] == "C" ){
			auto id = tmp[1];
			auto userName = tmp[2];
			auto addr = tmp[3];
			auto port = tmp[4];
			menu->addSeparator();
			menu->addAction( tr("Edit contact"), this, [this, id, userName, addr, port](){
				m_pContactEditor->editContact( id, userName, addr, port );
			} );
		}
	}

	menu->popup( ui->contsctsListWidget->mapToGlobal( pos ) );
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if( !m_pTrayIcon->isVisible() ){
		this->hide();
		m_pTrayIcon->show();
		event->ignore();
	}
}
