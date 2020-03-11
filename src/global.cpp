#include "global.h"
#include "myfunctions.h"
#include <QSettings>
#include <QDateTime>

namespace app {
	Config conf;

	void loadSettings()
	{
		QSettings settings("MySoft","LanCommunicator");

		app::conf.id = settings.value( "MAIN/id", app::conf.id ).toString();
		app::conf.port = settings.value( "MAIN/port", app::conf.port ).toUInt();

//		settings.beginGroup("SYNC_SAVE_DIRS");
//		app::conf.sync.saveDirs.clear();
//		for(auto elem:settings.childKeys()) app::conf.sync.saveDirs.push_back( settings.value(elem).toString() );
//		settings.endGroup();

//		settings.beginGroup("BOOKMARKS");
//		app::conf.bookmarks.clear();
//		for(auto elem:settings.childKeys()){
//			auto tmp = settings.value(elem).toString().split("	");
//			if( tmp.size() < 4 ) continue;

//			Bookmark bm;
//			bm.name = tmp[0];
//			bm.type = tmp[1];
//			bm.path = tmp[2];
//			bm.mount = ( tmp[3].toUInt() == 1 )?true:false;
//			if( tmp.size() >= 5 ) bm.mountDir = tmp[4];
//			if( tmp.size() >= 6 ) bm.mountOnStart = ( tmp[5].toUInt() == 1 )?true:false;
//			if( !bm.name.isEmpty() and !bm.type.isEmpty() and !bm.path.isEmpty() ) app::conf.bookmarks.push_back( bm );
//		}
//		settings.endGroup();

		if( settings.allKeys().size() == 0 ) app::saveSettings();
	}

	void saveSettings()
	{
		QSettings settings("MySoft","LanCommunicator");
		settings.clear();

		settings.setValue( "MAIN/id", app::conf.id );
		settings.setValue( "MAIN/port", app::conf.port );

//		i = 0;
//		for(auto elem:app::conf.sync.saveDirs){
//			settings.setValue("SYNC_SAVE_DIRS/" + QString::number(i),elem);
//			i++;
//		}

//		i = 0;
//		for(auto elem:app::conf.bookmarks){
//			QString mount = ( elem.mount )?"1":"0";
//			QString mountOnStart = ( elem.mountOnStart )?"1":"0";
//			QString str;
//			if( !elem.mountDir.isEmpty() and elem.mount ){
//				str = elem.name + "	" + elem.type + "	" + elem.path + "	" + mount + "	" + elem.mountDir + "	" + mountOnStart;
//			}else{
//				str = elem.name + "	" + elem.type + "	" + elem.path + "	" + mount ;
//			}
//			settings.setValue("BOOKMARKS/" + QString::number(i),str);
//			i++;
//		}
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

		app::conf.id = mf::md5( ba );
	}

}
