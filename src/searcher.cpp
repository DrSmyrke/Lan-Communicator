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
	pkt.head.pktCounter = ++m_pktCounter;
	myproto::addData( pkt.rawData, myproto::DataType::id, app::conf.id.toUtf8() );
	myproto::addData( pkt.rawData, myproto::DataType::login, app::conf.username.toUtf8() );
	auto ba = myproto::buidPkt( pkt );
	m_pSocket->writeDatagram( ba, QHostAddress::Broadcast, m_pSocket->localPort() );

	updateList();
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

			auto id = myproto::findData( pkt, myproto::DataType::id );
			auto username = myproto::findData( pkt, myproto::DataType::login );
			if( id.size() == 32 ){
				foundID( id, ha, port, username );
			}
		}
	}
}

void Searcher::foundID(const QString &id, const QHostAddress &addr, const uint16_t port, const QString &username)
{
	bool find = false;
	for( auto &elem:m_data ){
		if( elem.id == id ){
			elem.timestamp = app::getUnixTime();
			find = true;
			break;
		}
	}
	if( !find ){
		Searcher::Data data;
		data.addr = addr;
		data.port = port;
		data.id = id;
		data.username = username;
		data.timestamp = app::getUnixTime();
		m_data.push_back( data );
		emit signal_updateList();
	}
}

void Searcher::updateList()
{
	uint currentTimestamp = app::getUnixTime();
	bool removeF = false;

	QMutableListIterator< Data > i(m_data);
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
