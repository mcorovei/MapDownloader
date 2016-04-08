#include "mapdownloader.h"
#include <QtMath>
#include <QDir>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QNetworkProxy>
#include <QException>
#include <QThread>
#include <QTimer>
#include "Model/tile.h"
#include <QException>

namespace Ps
{
    MapDownloader::MapDownloader(QObject *parent) :
        QObject(parent),
        m_url(""),
        m_latitudeMin(0),
        m_latitudeMax(0),
        m_longitudeMin(0),
        m_longitudeMax(0),
        m_zoomMin(0),
        m_zoomMax(0),
        m_stop(false),
        m_destinationDirectory(""),
        m_showOnlyErrors(true){}

    void MapDownloader::onUrlChanged(const QString &value)
    {
        m_url = value;
    }

    void MapDownloader::onLatitudeMinChanged(double value)
    {
        m_latitudeMin = value;
    }

    void MapDownloader::onLatitudeMaxChanged(double value)
    {
        m_latitudeMax = value;
    }

    void MapDownloader::onLongitudeMinChanged(double value)
    {
        m_longitudeMin = value;
    }

    void MapDownloader::onLongitudeMaxChanged(double value)
    {
        m_longitudeMax = value;
    }

    void MapDownloader::onZoomMinChanged(quint16 value)
    {
        m_zoomMin = value;
    }

    void MapDownloader::onZoomMaxChanged(quint16 value)
    {
        m_zoomMax = value;
    }

    void MapDownloader::onDestinationDirChanged(const QString &value)
    {
        m_destinationDirectory = value;
    }

    void MapDownloader::onStopClicked()
    {
        m_stop = true;
        emit NotifyStopEnabled();
    }

    void MapDownloader::onStartFromZoomChanged(int value)
    {
        m_startFromZoom = value;
    }

    void MapDownloader::onStartFromTileXChanged(int value)
    {
        m_startFromTileX = value;
    }

    void MapDownloader::onStartFromTileYChanged(int value)
    {
        m_startFromTileY = value;
    }

    void MapDownloader::onStartFromChanged(bool value)
    {
        m_startFrom = value;
    }

    void MapDownloader::onShowOnlyErrorsChanged(bool value)
    {
        m_showOnlyErrors = value;
    }

    void MapDownloader::onDownload()
    {
        m_stop = false;
        RequestDownload();

        QString msg = (m_stop)? "Download stopped!": "Download finished!";
        emit NotifyProcessFinished(msg);
    }

    void MapDownloader::onCheck()
    {
        auto tileList = SelectTiles(false);
        CheckTilesDownloaded(tileList);

        emit NotifyProcessFinished("Check finished!");
    }

    void MapDownloader::RequestDownload()
    {
        auto tileList = SelectTiles();
        DownloadTileList(tileList);

    }

    QList<Tile*> MapDownloader::SelectTiles(bool output)
    {
        QList<Tile*> tileList = *new QList<Tile*>;
        for(auto z = m_zoomMin; z <= m_zoomMax; z++)
        {
            if(m_startFrom && z < m_startFromZoom)
                continue;
            long txmax = lon2tilex(m_longitudeMax, z);
            long txmin = lon2tilex(m_longitudeMin, z);

            long tymin = lat2tiley(m_latitudeMax, z);
            long tymax = lat2tiley(m_latitudeMin, z);

            long ntx = txmax - txmin + 1;
            long nty = tymax - tymin + 1;
            auto prod = ntx*nty;
            QString msg = "Schedule %1 (%2 x %3) tiles for zoom level %4 for download ...";
            if(output)
                emit NotifyStatusMessage(msg.arg(QString::number(prod),
                                             QString::number(ntx),
                                             QString::number(nty),
                                             QString::number(z)));

            for(long x = txmin; x <= txmax; x++)
            {
                for(long y = tymin; y <= tymax; y++)
                {
                    if(m_startFrom && z <= m_startFromZoom && x <= m_startFromTileX && y < m_startFromTileY)
                        continue;
                    Tile *t = new Tile(x,y,z);
                    tileList.append(t);
                }
            }
        }

        return tileList;
    }

    void MapDownloader::DownloadTileList(QList<Tile*> tileList)
    {
        quint16 oldzoom = -1;
        int tilecount = tileList.count();
        for(int i = 0; i< tilecount; i++)
        {
            emit NotifyDownloadedStatusChanged(i, tilecount);

            if(tileList.at(i)->zoom != oldzoom)
            {
                oldzoom = tileList.at(i)->zoom;
                QString msg = "Downloading tiles for zoom level %1 ...";
                if(!m_showOnlyErrors)
                    emit NotifyStatusMessage(msg.arg(QString::number(oldzoom)));

            }
            bool flag;
            do {
                if(m_stop)
                    return;

                flag = true;
                if(!m_showOnlyErrors)
                {
                    QString msg = "Downloading tile %1/%2/%3.";
                    emit NotifyStatusMessage(msg.arg(QString::number(tileList.at(i)->zoom),
                                                     QString::number(tileList.at(i)->tx),
                                                     QString::number(tileList.at(i)->ty)));
                }
                try
                {
                    DownloadTile(tileList.at(i));
                }
                catch(...)
                {
                    QString msg = "Cannot download tile %1/%2/%3.";
                    emit NotifyStatusMessage(msg.arg(QString::number(tileList.at(i)->zoom),
                                                     QString::number(tileList.at(i)->tx),
                                                     QString::number(tileList.at(i)->ty)));
                    flag = false;
                }
            }while(flag == false);

        }
        tileList.clear();
        emit NotifyDownloadedStatusChanged(tilecount, tilecount);
    }

    void MapDownloader::DownloadTile(Tile* tile)
    {
        auto tilePath = tile2path(tile);
        const QString url = m_url + "/" + tilePath + QString::number(tile->ty) + ".png";
        auto const fileName = m_destinationDirectory + "/" + tilePath;

        QDir dir;
        dir.mkpath(fileName);

        bool ok = false;
        do{
            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            QNetworkReply* reply;
            manager->setProxy(QNetworkProxy::NoProxy);
            QNetworkRequest req;
            req.setUrl(QUrl(url) );
            req.setRawHeader( "User-Agent" , "Chrome" );
            reply = manager->get(req);

            QTimer *timer = new QTimer;
            timer->setSingleShot(true);

            QEventLoop *loop = new QEventLoop;
            connect(timer, SIGNAL(timeout()), loop, SLOT(quit()));
            connect(this, SIGNAL(NotifyStopEnabled()), loop, SLOT(quit()));
            connect(reply, SIGNAL(finished()), loop, SLOT(quit()));

            timer->start(60000);
            loop->exec();

            if(timer->isActive() && !m_stop)
            {
                switch(reply->error())
                {
                case QNetworkReply::NoError:
                {
                    ok = true;
                    WriteToFile(fileName, tile, reply);
                    break;
                }
                default:
                {
                    qDebug(reply->errorString().toLatin1());
                    emit NotifyStatusMessage(reply->errorString().toLatin1());
                    ok = false;
                    break;
                }
                }
            }
            else
            {
                QString msg = "Timeout reached. Tile %1/%2/%3.";
                emit NotifyStatusMessage(msg.arg(QString::number(tile->zoom),
                                                 QString::number(tile->tx),
                                                 QString::number(tile->ty)));
                qDebug() << msg.arg(QString::number(tile->zoom),
                                    QString::number(tile->tx),
                                    QString::number(tile->ty));
                ok = false;
            }
            delete timer;
            delete reply;
            delete manager;

        }while(!ok && !m_stop);
    }

    void MapDownloader::WriteToFile(QString const fileName, Tile* tile, QNetworkReply* reply)
    {
        QByteArray data = reply->readAll();
        QFile file(fileName + QString::number(tile->ty) + ".png");
        if(!file.open(QIODevice::WriteOnly))
            emit NotifyStatusMessage("File " + fileName + QString::number(tile->ty) + ".png" + " was not written!");
        auto bytes = file.write(data);
        if(bytes == 0)
            emit NotifyStatusMessage("File " + fileName + QString::number(tile->ty) + ".png" + " was not written!");

        file.close();
    }

    void MapDownloader::CheckTilesDownloaded(QList<Tile*> tileList)
    {
        bool errorFound = false;
        for(auto tile : tileList)
        {
            auto tilePath = m_destinationDirectory + "/" + tile2path(tile) + QString::number(tile->ty) + ".png";
            QFile a(tilePath);
            if(!a.exists())
            {
                errorFound = true;
                QString msg = "Tile %1 does not exist!";
                emit NotifyStatusMessage(msg.arg(tilePath));
            }
        }
        if(!errorFound)
        {
            QString msg = "%1 files checked, no error found!";
            emit NotifyStatusMessage(msg.arg(QString::number(tileList.count())));
        }
    }

    QString MapDownloader::tile2path(Tile *tile)
    {
        return QString::number(tile->zoom) + "/" +
                QString::number(tile->tx) + "/";
    }

    long MapDownloader::lat2tiley(double lat, quint16 zoom)
    {
        auto lata = lat*M_PI/180;
        return (long)((1 - qLn(qTan(lata) + 1/qCos(lata))/M_PI)/2 * qPow(2,zoom));
    }

    long MapDownloader::lon2tilex(double lon, quint16 zoom)
    {
        return (long) ((lon+180)/360 * qPow(2,zoom));
    }
}
