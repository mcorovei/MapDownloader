
#pragma once
#include <QObject>
#include <QNetworkReply>

namespace Ps{
    class Tile;

    class MapDownloader : public QObject
    {
        Q_OBJECT
    public:
        explicit MapDownloader(QObject *parent=0);

    signals:
        void NotifyProcessFinished(const QString result) const;
        void NotifyStatusMessage(const QString message) const;
        void NotifyStopEnabled() const;
        void NotifyDownloadedStatusChanged(const int downloaded, const int total);

    public slots:
        void onUrlChanged(const QString& value);
        void onLatitudeMinChanged(double value);
        void onLatitudeMaxChanged(double value);
        void onLongitudeMinChanged(double value);
        void onLongitudeMaxChanged(double value);
        void onZoomMinChanged(quint16 value);
        void onZoomMaxChanged(quint16 value);
        void onDestinationDirChanged(const QString& value);
        void onStopClicked();
        void onStartFromZoomChanged(int value);
        void onStartFromTileXChanged(int value);
        void onStartFromTileYChanged(int value);
        void onStartFromChanged(bool value);
        void onShowOnlyErrorsChanged(bool value);

        void onDownload();
        void onCheck();

    private:
        QString m_url;
        double m_latitudeMin;
        double m_latitudeMax;
        double m_longitudeMin;
        double m_longitudeMax;
        quint16 m_zoomMin;
        quint16 m_zoomMax;
        bool m_stop;
        QString m_destinationDirectory;
        bool m_startFrom;
        int m_startFromZoom;
        int m_startFromTileX;
        int m_startFromTileY;
        bool m_showOnlyErrors;


        explicit MapDownloader(const MapDownloader& rhs) = delete;
        MapDownloader& operator= (const MapDownloader& rhs) = delete;

        void RequestDownload();
        QList<Tile*> SelectTiles(bool output = true);
        void DownloadTileList(QList<Tile*> tileList);
        void DownloadTile(Tile *tile);
        void WriteToFile(QString const fileName, Tile* tile, QNetworkReply* reply);
        void CheckTilesDownloaded(QList<Tile *> tileList);

        long lat2tiley(double lat, quint16 zoom);
        long lon2tilex(double lon, quint16 zoom);
        QString tile2path(Tile *tile);
    };
}


