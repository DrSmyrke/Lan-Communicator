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
	explicit Searcher(QObject *parent = nullptr);
	void search();
signals:
	void signal_updateList();
	void signal_readMess(const QString &text, const QString &id);
public slots:
	void slot_sendMess(const QString &text);
private slots:
	void slot_readyRead();
private:
	QUdpSocket* m_pSocket;
	QList< QHostAddress > m_myIPs;
	QByteArray m_rxBuff;
	uint16_t m_pktCounter;

	void foundID(const QString &id, const QHostAddress &addr, const uint16_t port, const QString &username);
	void updateList();
	void parsCommunicationPkt(const myproto::Pkt &pkt, const QHostAddress &ha, const uint16_t port);
	void sendData(myproto::Pkt &pkt);
};

#endif // SEARCHER_H
