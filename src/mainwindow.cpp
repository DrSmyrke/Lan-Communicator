#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_pSearcher = new Searcher( this );
	m_pTimer = new QTimer( this );

	connect( m_pTimer, &QTimer::timeout, this, &MainWindow::slot_timerUpdate );

	m_pTimer->setInterval( 3000 );
	m_pTimer->start();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::slot_timerUpdate()
{
	m_pSearcher->search();
}
