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

	connect( m_pTimer, &QTimer::timeout, this, &MainWindow::slot_timerUpdate );
	connect( m_pSearcher, &Searcher::signal_updateList, this, &MainWindow::slot_searcherUpdateList );

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

	for( auto elem:m_pSearcher->getData() ){
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
