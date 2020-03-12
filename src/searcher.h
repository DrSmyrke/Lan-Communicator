#ifndef SEARCHER_H
#define SEARCHER_H

#include <QObject>
#include <QUdpSocket>
#include "myproto.h"
#include "global.h"

class Searcher : public QObject
{
	Q_OBJECT
public:
	struct Data{
		QHostAddress addr;
		uint16_t port;
		QString id;
		QString username;
		uint timestamp;
	};
	explicit Searcher(QObject *parent = nullptr);
	void search();
	QList< Data > getData(){ return m_data; }
signals:
	void signal_updateList();
private slots:
	void slot_readyRead();
private:
	QUdpSocket* m_pSocket;
	QList< QHostAddress > m_myIPs;
	QByteArray m_rxBuff;
	uint16_t m_pktCounter;
	QList< Data > m_data;

	void foundID(const QString &id, const QHostAddress &addr, const uint16_t port, const QString &username);
	void updateList();
};

#endif // SEARCHER_H
