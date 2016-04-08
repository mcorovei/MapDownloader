#include "startup.h"
#include "View/mainview.h"
#include "Model/mapdownloader.h"
#include "ViewModel/mainviewmanager.h"

namespace Ps
{
    Startup::Startup() :
        QObject(nullptr),
        m_mainView(*new MainView(nullptr)),
        m_mapDownload(*new MapDownloader(this)),
        m_mainViewManager(*new MainViewManager(this, m_mainView, m_mapDownload))
    {
    }

    Startup::~Startup()
    {
        delete &m_mainView;
    }

    void Startup::show() const
    {
        m_mainView.show();
    }
}
