#include "cloudimageprovider.h"
#include <QImage>
#include <QPixmap>

#include <QObject>
#include <QDebug>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QApplication>
#include <QDir>
#include <QCryptographicHash>
#include <QXmlStreamReader>

#include "cloudwindow.h"
#include "pobject.h"
#include "mainwindowpockemul.h"

extern QString workDir;
extern MainWindowPockemul *mainwindow;

CloudImageProvider::CloudImageProvider(QObject *parent) : QObject(parent),
    QQuickImageProvider(QQuickImageProvider::Image , QQmlImageProviderBase::ForceAsynchronousImageLoading)
//    QDeclarativeImageProvider(QDeclarativeImageProvider::Image)
{
    // This space intentionally left blank.

    mgr = new QNetworkAccessManager;
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(loadfinished(QNetworkReply*)));

    // load cache
    // fetch workdir+"/imgcache" directory
    QDir dir;
    dir.mkpath(workDir+"/imgcache/");
    dir.setPath(workDir+"/imgcache/");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    //std::cout << "     Bytes Filename" << std::endl;
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.suffix() == "jpg") {
//            qWarning()<<"load ["<< fileInfo.baseName()<<"]="+fileInfo.absoluteFilePath();
            cache[fileInfo.baseName()] = QImage(fileInfo.absoluteFilePath());
        }

    }
}

CloudImageProvider::~CloudImageProvider()
{
    // This space intentionally left blank.
}



// CACHE MODULE
QImage CloudImageProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize)
{
    Q_UNUSED(size)
    Q_UNUSED(requestedSize)

    qWarning()<<"id"<<id;
//    qWarning()<<id<<"   auth_token="<<CloudWindow::getValueFor("auth_token")<<" size="<<requestedSize;
//    QByteArray _ba = "api_key=7118206e08fed2c5ec8c0f2db61bbbdc09ab2dfa&auth_token=" +
//            Cloud::getValueFor("auth_token").toUtf8();

    QString _id = id;
    _id.remove(QChar('#'));

    QNetworkRequest req("http://"+_id);
    qWarning()<<req.url();
//    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QString key = toKey( req.url().toString());

    if (cache.contains(key))
        return cache[key];

qWarning()<<"YES!!1:"<<req.url();
    cache[key] = QImage();

    QNetworkAccessManager *_mgr = new QNetworkAccessManager;
    connect(_mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(loadfinished(QNetworkReply*)));

    QNetworkReply *_reply = _mgr->get(req);
    Q_UNUSED(_reply)
//    qWarning()<<_reply;
qWarning()<<"YES!!2";
    return QImage();
}


QPixmap CloudImageProvider::requestPixmap(const QString& id, QSize* size, const QSize& requestedSize)
{
    QString rsrcid = ":/" + id;
    QPixmap image(rsrcid);
    QPixmap result;

    if (requestedSize.isValid()) {
        result = image.scaled(requestedSize, Qt::KeepAspectRatio);
    } else {
        result = image;
    }
    *size = result.size();
    return result;
}

void CloudImageProvider::clearCache(QString s) {
    qWarning()<<"CloudImageProvider::clearCache:"<<s;
    QString key = toKey(s);
    cache.remove(key);

    QString fn =workDir+"imgcache/"+key+".jpg";
    qWarning()<<fn;
    QFile file(fn);
    if (file.remove()) qWarning()<<"removed";
}

void CloudImageProvider::loadfinished(QNetworkReply *reply)
{
    qWarning()<<"Download finished*******";

    QByteArray Data = reply->readAll();
//    qWarning() << "data="<<xmlData.left(200);
                QImage image;
                image.loadFromData(Data);

                QString key = toKey( reply->url().toString());
//                qWarning()<<key<<" <- "<<reply->url().toString();
                cache[key] = image;

//                qWarning()<<"pre image save:"+workDir+"imgcache/"+key+".jpg";
                image.save(workDir+"imgcache/"+key+".jpg");
//                qWarning()<<"post image save";




//    reply->manager()->deleteLater();
    reply->deleteLater();

    emit cacheUpdated();
}

