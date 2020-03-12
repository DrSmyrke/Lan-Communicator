#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDir>
#include <QString>
#include <QUrl>
#include <vector>

struct Config{
	bool verbose						= false;
	uint8_t logLevel					= 3;
#ifdef __linux__
	QString logFile						= "/var/log/LanCommunicator.log";
#elif _WIN32
	QString logFile						= QDir::homePath() + "/LanCommunicator.log";
#endif
	QString version;
	QString id;
	QString username;
	uint16_t port						= 7373;
	uint16_t broadCastPort				= 37373;
};
struct UserData{
	QUrl addr;
	QString id;
	QString username;
	uint timestamp;
	bool online							= false;
};

namespace app {
	extern Config conf;
	extern QList< UserData > lanUsersData;
	extern QList< UserData > conctacts;

	void loadSettings();
	void saveSettings();
	bool parsArgs(int argc, char *argv[]);
	void setLog(const uint8_t logLevel, const QString &mess);

	void generateID(const QString &login);
	uint getUnixTime();
}

#endif // GLOBAL_H
