#include "searcher.h"

Searcher::Searcher(QObject *parent) : QObject(parent)
{
	m_pSocket = new QUdpSocket( this );

	bool res = m_pSocket->bind( app::conf.port, QUdpSocket::ShareAddress );

	connect( m_pSocket, &QUdpSocket::readyRead, this, &Searcher::slot_readyRead );

	app::setLog( 1, QString( "Searcher binding at %1 port %2" ).arg( app::conf.port ).arg( res ) );
}

void Searcher::search()
{
	myproto::Pkt pkt;
	myproto::addData( pkt.rawData, myproto::DataType::id, app::conf.id.toUtf8() );
	auto ba = myproto::buidPkt( pkt, "" );
	m_pSocket->writeDatagram( ba, QHostAddress::Broadcast, app::conf.port );
}

void Searcher::slot_readyRead()
{
	QByteArray buff;
	while( m_pSocket->hasPendingDatagrams() ){
		buff.resize( m_pSocket->pendingDatagramSize() );
		m_pSocket->readDatagram( buff.data(), buff.size() );
	}
	qDebug()<<buff.toHex();
}
