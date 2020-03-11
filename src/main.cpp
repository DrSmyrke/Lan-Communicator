#include "mainwindow.h"
#include "global.h"
#include "version.h"
#include "windows/username.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	app::conf.version = QString("%1.%2").arg(APP_VER_FIRST).arg(APP_VER_SECOND);

	if( !app::parsArgs(argc, argv) ) return 0;

	if( !app::conf.logFile.isEmpty() ) QFile( app::conf.logFile ).remove();

	QString filename = QLocale::system().name();
	QTranslator translator(&a);
	if(translator.load(filename,"://lang/")) a.installTranslator(&translator);

	app::loadSettings();
	if( !app::parsArgs(argc, argv) ) return 0;

	if( app::conf.id.isEmpty() ){
		UserNameWindow* unwin = new UserNameWindow();
		if( unwin->exec() == QDialog::Accepted ){
			app::generateID( unwin->getUsername() );
		}
		unwin->deleteLater();
	}

	if( app::conf.id.isEmpty() ){
		return 0;
	}

	MainWindow w;
	w.show();

	return a.exec();
}
