#include "mainviewmanager.h"
#include "Model/mapdownloader.h"
#include "View/mainview.h"

namespace Ps
{
    MainViewManager::MainViewManager(QObject *parent,
                                     MainView &mainView,
                                     MapDownloader &mapDownload) :
        QObject(parent),
        m_mainView(mainView),
        m_mapDownload(mapDownload)
    {
        WireControls();
        WireStatus();
    }

    void MainViewManager::WireControls()
    {
          connect(&m_mainView, &MainView::NotifyUrlChanged, &m_mapDownload, &MapDownloader::onUrlChanged);
          connect(&m_mainView, &MainView::NotifyLatitudeMinChanged, &m_mapDownload, &MapDownloader::onLatitudeMinChanged);
          connect(&m_mainView, &MainView::NotifyLatitudeMaxChanged, &m_mapDownload, &MapDownloader::onLatitudeMaxChanged);
          connect(&m_mainView, &MainView::NotifyLongitudeMinChanged, &m_mapDownload, &MapDownloader::onLongitudeMinChanged);
          connect(&m_mainView, &MainView::NotifyLongitudeMaxChanged, &m_mapDownload, &MapDownloader::onLongitudeMaxChanged);
          connect(&m_mainView, &MainView::NotifyZoomMinChanged, &m_mapDownload, &MapDownloader::onZoomMinChanged);
          connect(&m_mainView, &MainView::NotifyZoomMaxChanged, &m_mapDownload, &MapDownloader::onZoomMaxChanged);
          connect(&m_mainView, &MainView::NotifyDestinationDirChanged, &m_mapDownload, &MapDownloader::onDestinationDirChanged);
          connect(&m_mainView, &MainView::NotifyStopClicked, &m_mapDownload, &MapDownloader::onStopClicked);
          connect(&m_mainView, &MainView::NotifyStartFromZoomChanged, &m_mapDownload, &MapDownloader::onStartFromZoomChanged);
          connect(&m_mainView, &MainView::NotifyStartFromTileXChanged, &m_mapDownload, &MapDownloader::onStartFromTileXChanged);
          connect(&m_mainView, &MainView::NotifyStartFromTileYChanged, &m_mapDownload, &MapDownloader::onStartFromTileYChanged);
          connect(&m_mainView, &MainView::NotifyStartFromChanged, &m_mapDownload, &MapDownloader::onStartFromChanged);
          connect(&m_mainView, &MainView::NotifyShowOnlyErrorsChanged, &m_mapDownload, &MapDownloader::onShowOnlyErrorsChanged);

          connect(&m_mainView, &MainView::NotifyDownloadClicked, &m_mapDownload, &MapDownloader::onDownload);
          connect(&m_mainView, &MainView::NotifyCheckClicked, &m_mapDownload, &MapDownloader::onCheck);
    }

    void MainViewManager::WireStatus()
    {
         connect(&m_mapDownload, &MapDownloader::NotifyProcessFinished, &m_mainView, &MainView::onProcessFinished);
         connect(&m_mapDownload, &MapDownloader::NotifyStatusMessage, &m_mainView, &MainView::onStatusMessage);
         connect(&m_mapDownload, &MapDownloader::NotifyDownloadedStatusChanged, &m_mainView, &MainView::onDownloadedStatusChanged);

    }
}
