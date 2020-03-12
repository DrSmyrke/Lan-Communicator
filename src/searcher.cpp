#include "searcher.h"

#include <QNetworkInterface>

Searcher::Searcher(QObject *parent) : QObject(parent)
{
	m_pktCounter = 0;
	m_pSocket = new QUdpSocket( this );

	bool res = m_pSocket->bind( app::conf.broadCastPort, QUdpSocket::ShareAddress );

	connect( m_pSocket, &QUdpSocket::readyRead, this, &Searcher::slot_readyRead );

	app::setLog( 1, QString( "Searcher binding at %1 port %2" ).arg( app::conf.broadCastPort ).arg( res ) );
}

void Searcher::search()
{
	QNetworkInterface iface;
	m_myIPs = iface.allAddresses();

	myproto::Pkt pkt;
	pkt.head.channel = myproto::Channel::comunication;
	pkt.head.type = myproto::PktType::hello;
	myproto::addData( pkt.rawData, myproto::DataType::id, app::conf.id.toUtf8() );
	myproto::addData( pkt.rawData, myproto::DataType::login, app::conf.username.toUtf8() );
	sendData( pkt );

	updateList();
}

void Searcher::slot_sendMess(const QString &text)
{
	myproto::Pkt pkt;
	pkt.head.channel = myproto::Channel::comunication;
	pkt.head.type = myproto::PktType::data;
	myproto::addData( pkt.rawData, myproto::DataType::text, text.toUtf8() );
	sendData( pkt );
}

void Searcher::slot_readyRead()
{
	QHostAddress ha;
	uint16_t port;

	while( m_pSocket->hasPendingDatagrams() ){
		QByteArray buff;
		buff.resize( m_pSocket->pendingDatagramSize() );
		m_pSocket->readDatagram( buff.data(), buff.size(), &ha, &port );
		bool find = false;
		for( auto elem:m_myIPs ){
			if( elem == ha ){
				find = true;
				break;
			}
		}
		if( find ){
		//	continue;
		}
		m_rxBuff.append( buff );

		myproto::Pkt pkt;

		while( m_rxBuff.size() > 0 ){
			pkt = myproto::parsPkt( m_rxBuff );
			if( pkt.error ){
				break;
			}
			if( pkt.next ){
				break;
			}
			if( pkt.retry ){
				continue;
			}

			myproto::parsData( pkt );

			switch ( pkt.head.channel ) {
				case myproto::Channel::comunication:
					parsCommunicationPkt( pkt, ha, port );
				break;
				default: break;
			}
		}
	}
}

void Searcher::foundID(const QString &id, const QHostAddress &addr, const uint16_t port, const QString &username)
{
	bool find = false;
	QMutableListIterator< UserData > i( app::lanUsersData );
	while (i.hasNext()) {
		auto elem = i.next();
		if( elem.id == id ){
			elem.timestamp = app::getUnixTime();
			i.setValue( elem );
			find = true;
			break;
		}
	}
	if( !find ){
		UserData data;
		data.addr.setHost( addr.toString() );
		data.addr.setPort( port );
		data.id = id;
		data.username = username;
		data.timestamp = app::getUnixTime();
		app::lanUsersData.push_back( data );
		emit signal_updateList();
	}
}

void Searcher::updateList()
{
	uint currentTimestamp = app::getUnixTime();
	bool removeF = false;

	QMutableListIterator< UserData > i( app::lanUsersData );
	while (i.hasNext()) {
		auto elem = i.next();
		uint r = currentTimestamp - elem.timestamp;
		if( r >= 120 ){
			i.remove();
		}
	}

	if( removeF ){
		emit signal_updateList();
	}
}

void Searcher::parsCommunicationPkt(const myproto::Pkt &pkt, const QHostAddress &ha, const uint16_t port)
{
	if( pkt.head.type == myproto::PktType::hello ){
		auto id = myproto::findData( pkt, myproto::DataType::id );
		auto username = myproto::findData( pkt, myproto::DataType::login );
		if( id.size() == app::conf.id.size() ){
			foundID( id, ha, port, username );
		}
		return;
	}
	if( pkt.head.type == myproto::PktType::data ){
		auto message = myproto::findData( pkt, myproto::DataType::text );
		QString id;
		for( auto elem:app::lanUsersData ){
			if( elem.addr.host() == ha.toString() && elem.addr.port() == port ){
				id = elem.id;
				break;
			}
		}
		if( !id.isEmpty() ){
			emit signal_readMess( message, id );
		}
		return;
	}
}

void Searcher::sendData(myproto::Pkt &pkt)
{
	pkt.head.pktCounter = ++m_pktCounter;
	auto ba = myproto::buidPkt( pkt );
	m_pSocket->writeDatagram( ba, QHostAddress::Broadcast, m_pSocket->localPort() );
}
