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
        bool ok = WireControls();
        ok &= WireStatus();

        if (ok)
            m_mainView.onStatusMessage("Initialization successful ...");
        else
            m_mainView.onStatusMessage("Initialization failed!");
    }

    bool MainViewManager::WireControls()
    {
          bool ok = (bool)connect(&m_mainView, &MainView::NotifyUrlChanged, &m_mapDownload, &MapDownloader::onUrlChanged);
          ok &= (bool)connect(&m_mainView, &MainView::NotifyLatitudeMinChanged, &m_mapDownload, &MapDownloader::onLatitudeMinChanged);
          ok &= (bool)connect(&m_mainView, &MainView::NotifyLatitudeMaxChanged, &m_mapDownload, &MapDownloader::onLatitudeMaxChanged);
          ok &= (bool)connect(&m_mainView, &MainView::NotifyLongitudeMinChanged, &m_mapDownload, &MapDownloader::onLongitudeMinChanged);
          ok &= (bool)connect(&m_mainView, &MainView::NotifyLongitudeMaxChanged, &m_mapDownload, &MapDownloader::onLongitudeMaxChanged);
          ok &= (bool)connect(&m_mainView, &MainView::NotifyZoomMinChanged, &m_mapDownload, &MapDownloader::onZoomMinChanged);
          ok &= (bool)connect(&m_mainView, &MainView::NotifyZoomMaxChanged, &m_mapDownload, &MapDownloader::onZoomMaxChanged);
          ok &= (bool)connect(&m_mainView, &MainView::NotifyDestinationDirChanged, &m_mapDownload, &MapDownloader::onDestinationDirChanged);
          ok &= (bool)connect(&m_mainView, &MainView::NotifyStopClicked, &m_mapDownload, &MapDownloader::onStopClicked);
          ok &= (bool)connect(&m_mainView, &MainView::NotifyStartFromZoomChanged, &m_mapDownload, &MapDownloader::onStartFromZoomChanged);
          ok &= (bool)connect(&m_mainView, &MainView::NotifyStartFromTileXChanged, &m_mapDownload, &MapDownloader::onStartFromTileXChanged);
          ok &= (bool)connect(&m_mainView, &MainView::NotifyStartFromTileYChanged, &m_mapDownload, &MapDownloader::onStartFromTileYChanged);
          ok &= (bool)connect(&m_mainView, &MainView::NotifyStartFromChanged, &m_mapDownload, &MapDownloader::onStartFromChanged);
          ok &= (bool)connect(&m_mainView, &MainView::NotifyShowOnlyErrorsChanged, &m_mapDownload, &MapDownloader::onShowOnlyErrorsChanged);

          ok &= (bool)connect(&m_mainView, &MainView::NotifyDownloadClicked, &m_mapDownload, &MapDownloader::onDownload);
          ok &= (bool)connect(&m_mainView, &MainView::NotifyCheckClicked, &m_mapDownload, &MapDownloader::onCheck);

          return ok;
    }

    bool MainViewManager::WireStatus()
    {
         bool ok = (bool)connect(&m_mapDownload, &MapDownloader::NotifyProcessFinished, &m_mainView, &MainView::onProcessFinished);
         ok &= (bool)connect(&m_mapDownload, &MapDownloader::NotifyStatusMessage, &m_mainView, &MainView::onStatusMessage);
         ok &= (bool)connect(&m_mapDownload, &MapDownloader::NotifyDownloadedStatusChanged, &m_mainView, &MainView::onDownloadedStatusChanged);

         return ok;
    }
}
