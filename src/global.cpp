#include "global.h"
#include "myfunctions.h"
#include <QSettings>
#include <QDateTime>

namespace app {
	Config conf;
	QList< UserData > lanUsersData;
	QList< UserData > conctacts;

	void loadSettings()
	{
		QSettings settings("MySoft","LanCommunicator");

		app::conf.id = settings.value( "MAIN/id", app::conf.id ).toString();
		app::conf.username = settings.value( "MAIN/username", app::conf.username ).toString();
		app::conf.port = settings.value( "MAIN/port", app::conf.port ).toUInt();

		settings.beginGroup("CONTACTS");
		app::conctacts.clear();
		for(auto elem:settings.childKeys()){
			auto tmp = settings.value(elem).toString().split(";");
			if( tmp.size() < 4 ) continue;

			UserData user;
			user.id = tmp[0];
			user.username = tmp[1];
			user.addr = QHostAddress( tmp[2] );
			user.port = tmp[3].toUInt();
			app::conctacts.push_back( user );
		}
		settings.endGroup();

		if( settings.allKeys().size() == 0 ) app::saveSettings();
	}

	void saveSettings()
	{
		uint16_t i = 0;

		QSettings settings("MySoft","LanCommunicator");
		settings.clear();

		settings.setValue( "MAIN/id", app::conf.id );
		settings.setValue( "MAIN/port", app::conf.port );
		settings.setValue( "MAIN/username", app::conf.username );

//		i = 0;
//		for(auto elem:app::conf.sync.saveDirs){
//			settings.setValue("SYNC_SAVE_DIRS/" + QString::number(i),elem);
//			i++;
//		}

		i = 0;
		for( auto elem:app::conctacts ){
			auto str = QString( "%1;%2;%3;%4" ).arg( elem.id ).arg( elem.username ).arg( elem.addr.toString() ).arg( elem.port );
			settings.setValue("CONTACTS/" + QString::number(i),str);
			i++;
		}
	}

	bool parsArgs(int argc, char *argv[])
	{
		bool ret = true;
		for(int i=0;i<argc;i++){
			if(QString(argv[i]).indexOf("-")==0){
				if(QString(argv[i]) == "--help" or QString(argv[1]) == "-h"){
					printf("Usage: %s [OPTIONS]\n"
							"  -l <FILE>    log file\n"
							"  -v    Verbose output\n"
							"  --version	print current version\n"
							"\n", argv[0]);
					ret = false;
				}
				if(QString(argv[i]) == "-l") app::conf.logFile = QString(argv[++i]);
				if(QString(argv[i]) == "-v") app::conf.verbose = true;
				if(QString(argv[i]) == "--version"){
					printf( "%s\n", app::conf.version.toUtf8().data() );
					ret = false;
				}
			}
		}
		return ret;
	}

	void setLog(const uint8_t logLevel, const QString &mess)
	{
		if(app::conf.logLevel < logLevel or app::conf.logLevel == 0) return;

		QDateTime dt = QDateTime::currentDateTime();
		QString str = dt.toString("yyyy.MM.dd [hh:mm:ss] ") + mess + "\n";

		if( app::conf.verbose ){
			printf( "%s", str.toUtf8().data() );
			fflush( stdout );
		}

		if( app::conf.logFile.isEmpty() ) return;
		QFile f;
		f.setFileName( app::conf.logFile );
		if( f.open( QIODevice::WriteOnly | QIODevice::Append ) ){
			f.write( str.toUtf8().data() );
			f.close();
		}
	}

	void generateID(const QString &login)
	{
		QByteArray ba;
		QDateTime dt = QDateTime::currentDateTime();
		ba.append( login );
		ba.append( ">:LanCommutator>:" );
		ba.append( dt.toString("yyyy.MM.dd [hh:mm:ss:zzz] ") );
		ba.append( ">:LanCommutator>:" );
		ba.append( QLocale::system().name() );

		app::conf.id = mf::sha1( ba );
	}

	uint getUnixTime()
	{
		return QDateTime::currentDateTimeUtc().toTime_t();
	}

}
