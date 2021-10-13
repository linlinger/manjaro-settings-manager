/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Ramon Buldó <ramon@manjaro.org>
 *
 *  Manjaro Settings Manager is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Manjaro Settings Manager is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Manjaro Settings Manager.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "LocaleAuthHelper.h"
#include "LanguageCommon.h"

#include <QtDBus/QDBusInterface>
#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtCore/QTextStream>
#include <QtCore/QSet>

#include <QDebug>

ActionReply
LocaleAuthHelper::save( const QVariantMap& args )
{
    if ( args["isLocaleListModified"].toBool() )
    {
        updateLocaleGen( args["locales"].toStringList() );
        generateLocaleGen();
    }
    if ( args["isSystemLocalesModified"].toBool() )
        setSystemLocale( args["localeList"].toStringList() );
    return ActionReply::SuccessReply();
}


bool
LocaleAuthHelper::updateLocaleGen( QStringList locales )
{

    const QString localeGen = "/etc/locale.gen";
    QFile file( localeGen );
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug() << QString( "error: failed to open file '%1'" ).arg( localeGen );
        return false;
    }

    QStringList content;

    QTextStream in( &file );
    while ( !in.atEnd() )
    {
        QString line = in.readLine();
        content.append( line );
        line = line.trimmed();

        bool found = false;

        foreach ( const QString locale, locales )
        {
            if ( line.startsWith( locale + " " ) )
            {
                found = true;
                locales.removeAll( locale );
                break;
            }

            if ( line.startsWith( "#" + locale + " " ) )
            {
                content.removeLast();
                content.append( line.remove( 0, 1 ) );
                found = true;
                locales.removeAll( locale );
                break;
            }
        }

        if ( !found && !line.split( "#", QString::KeepEmptyParts ).first()
                .trimmed().isEmpty() )
        {
            content.removeLast();
            content.append( "#" + line );
        }
    }
    file.close();

    // Add missing locales in the file
    foreach ( const QString locale, locales )
    {
        QString validLocale = LanguageCommon::localeToLocaleGenFormat( locale );
        if ( validLocale.isEmpty() )
        {
            qDebug() << QString( "warning: failed to obtain valid locale string"
                                 "for locale '%1'!" ).arg( locale );
            continue;
        }
        content.append( validLocale );
    }

    if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
        qDebug() << QString( "error: failed to open file '%1'!" ).arg( localeGen );
        return false;
    }
    QTextStream out( &file );
    out << content.join( "\n" );
    out.flush();
    file.close();

    return true;
}


bool
LocaleAuthHelper::generateLocaleGen()
{


    //新建对象，参数含义：对话框正文，取消按钮名称，进度条范围
    pd = new QProgressDialog("正在保存...","取消",0,100,this);
    //模态对话框
    pd->setWindowModality(Qt::WindowModal);
    //如果进度条运行的时间小于5，进度条就不会显示，默认是4S
    pd->setMinimumDuration(5);
    //设置标题
    pd->setWindowTitle("请稍后");
    //显示处理框
    pd->show();
    //处理过程。。。
    t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(perform()));
    t->start(1000);

    QProcess localeGen;
    localeGen.start( "/usr/bin/locale-gen" );
    connect( &localeGen, &QProcess::readyRead,
             [&] ()
    {
        QString data = QString::fromUtf8( localeGen.readAll() ).trimmed();
        if ( !data.isEmpty() )
        {
            QVariantMap map;
            map.insert( QLatin1String( "Data" ), data );
            HelperSupport::progressStep( map );
        }
    } );
    if ( !localeGen.waitForStarted() )
        return false;
    if ( !localeGen.waitForFinished( 60000 ) )
        return false;
    return true;

    t->stop();
    steps = 0;
    delete pd;


}


bool
LocaleAuthHelper::setSystemLocale( const QStringList localeList )
{
    QDBusInterface dbusInterface( "org.freedesktop.locale1",
                                  "/org/freedesktop/locale1",
                                  "org.freedesktop.locale1",
                                  QDBusConnection::systemBus() );
    /*
     * asb
     * array_string -> locale
     * boolean -> arg_ask_password
     */
    QDBusMessage reply;
    reply = dbusInterface.call( "SetLocale", localeList, true );
    if ( reply.type() == QDBusMessage::ErrorMessage )
        return false;
    return true;
}


KAUTH_HELPER_MAIN( "org.spanningtree.msm.locale", LocaleAuthHelper )
#include "moc_LocaleAuthHelper.cpp"
