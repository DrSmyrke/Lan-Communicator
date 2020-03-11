#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDir>
#include <QString>
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
};

namespace app {
	extern Config conf;

	void loadSettings();
	void saveSettings();
	bool parsArgs(int argc, char *argv[]);
	void setLog(const uint8_t logLevel, const QString &mess);

	void generateID(const QString &login);
}

#endif // GLOBAL_H
