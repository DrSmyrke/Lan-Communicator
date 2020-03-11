#include "searcher.h"

Searcher::Searcher(QObject *parent) : QObject(parent)
{
	m_pSocket = new QUdpSocket( this );

	m_pSocket->bind( 45454, QUdpSocket::ShareAddress );

	connect( m_pSocket, &QUdpSocket::readyRead, this, &Searcher::slot_readyRead );
}

void Searcher::search()
{
	QByteArray datagram = "Broadcast message";
	m_pSocket->writeDatagram(datagram, QHostAddress::Broadcast, 45454);
}

void Searcher::slot_readyRead()
{
	QByteArray buff;
	while( m_pSocket->hasPendingDatagrams() ){
		buff.resize( m_pSocket->pendingDatagramSize() );
		m_pSocket->readDatagram( buff.data(), buff.size() );
	}
	qDebug()<<buff;
}
