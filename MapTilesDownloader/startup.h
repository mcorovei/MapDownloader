
#pragma once
#include <QObject>

namespace Ps{
    class MainView;
    class MainViewManager;
    class MapDownloader;

    class Startup final : public QObject
    {
        Q_OBJECT
    public:
        explicit Startup();
        ~Startup();

        void show() const;

    private:
        MainView& m_mainView;
        MapDownloader& m_mapDownload;
        MainViewManager& m_mainViewManager;


        explicit Startup(const Startup& rhs) = delete;
        Startup& operator= (const Startup& rhs) = delete;
    };
}


