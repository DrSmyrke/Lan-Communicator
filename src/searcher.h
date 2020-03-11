#ifndef SEARCHER_H
#define SEARCHER_H

#include <QObject>
#include <QUdpSocket>

class Searcher : public QObject
{
	Q_OBJECT
public:
	explicit Searcher(QObject *parent = nullptr);
	void search();
private slots:
	void slot_readyRead();
private:
	QUdpSocket* m_pSocket;
};

#endif // SEARCHER_H
