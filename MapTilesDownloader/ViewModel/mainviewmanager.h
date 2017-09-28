
#pragma once
#include <QObject>

namespace Ps{
    class MapDownloader;
    class MainView;

    class MainViewManager : public QObject
    {
        Q_OBJECT
    public:
        explicit MainViewManager(QObject *parent,
                                 MainView &mainView,
                                 MapDownloader &mapDownload);

    private:
        MainView& m_mainView;
        MapDownloader& m_mapDownload;
        bool WireControls();
        bool WireStatus();

        explicit MainViewManager(const MainViewManager& rhs) = delete;
        MainViewManager& operator= (const MainViewManager& rhs) = delete;
    };
}
