#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>

namespace Ui {
    class MainView;
}

namespace Ps{
    class MainView : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainView(QWidget *parent = 0);
        ~MainView();

        void onStatusMessage(const QString message);
    signals:
        void NotifyUrlChanged(const QString& value) const;
        void NotifyLatitudeMinChanged(double value) const;
        void NotifyLatitudeMaxChanged(double value) const;
        void NotifyLongitudeMinChanged(double value) const;
        void NotifyLongitudeMaxChanged(double value) const;
        void NotifyZoomMinChanged(quint16 value) const;
        void NotifyZoomMaxChanged(quint16 value) const;
        void NotifyDestinationDirChanged(const QString& value) const;
        void NotifyStopClicked() const;
        void NotifyStartFromZoomChanged(int value) const;
        void NotifyStartFromTileXChanged(int value) const;
        void NotifyStartFromTileYChanged(int value) const;
        void NotifyStartFromChanged(bool value) const;
        void NotifyShowOnlyErrorsChanged(bool value) const;

        void NotifyDownloadClicked() const;
        void NotifyCheckClicked() const;

    public slots:
        void onProcessFinished(const QString message);
        void onDownloadedStatusChanged(const int downloaded, const int total);

    private slots:
        void on_dsbLatitudeMin_valueChanged(double arg1);
        void on_dsbLatitudeMax_valueChanged(double arg1);
        void on_dsbLongitudeMin_valueChanged(double arg1);
        void on_dsbLongitudeMax_valueChanged(double arg1);
        void on_sbZoomMin_valueChanged(int arg1);
        void on_sbZoomMax_valueChanged(int arg1);
        void on_lineDestinationDir_editingFinished();
        void on_btnDestinationDir_clicked();
        void on_btnDownload_clicked();
        void on_lineUrl_editingFinished();

        void on_lineDestinationDir_selectionChanged();

        void on_lineDestinationDir_textChanged(const QString &arg1);

        void on_btnStop_clicked();

        void on_sbZoom_valueChanged(int arg1);

        void on_sbTileX_valueChanged(int arg1);

        void on_sbTileY_valueChanged(int arg1);

        void on_cbStartFrom_toggled(bool checked);

        void on_btnClear_clicked();

        void on_cbShowOnlyErrors_toggled(bool checked);

        void on_btnOpenDir_clicked();

        void on_btnCheck_clicked();

    private:
        Ui::MainView *ui;
        bool CheckValidConfigPanelSettings();
    };
}

#endif // MAINVIEW_H
