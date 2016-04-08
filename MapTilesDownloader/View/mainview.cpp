#include "mainview.h"
#include "ui_mainview.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

namespace Ps{
    MainView::MainView(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainView)
    {
        ui->setupUi(this);
    }

    MainView::~MainView()
    {
        delete ui;
    }

    void MainView::onProcessFinished(const QString message)
    {
        ui->btnDownload->setEnabled(true);
        ui->gbConfigPanel->setEnabled(true);
        ui->btnStop->setEnabled(false);
        ui->tbMessages->append(message);
    }

    void MainView::onDownloadedStatusChanged(const int downloaded, const int total)
    {
        ui->lineDownloadedStatus->setText(QString::number(downloaded) + " / " + QString::number(total));
    }

    void MainView::onStatusMessage(const QString message)
    {
        ui->tbMessages->append(message);
    }

    void MainView::on_lineUrl_editingFinished()
    {
        emit NotifyUrlChanged(ui->lineUrl->text());
    }

    void MainView::on_dsbLatitudeMin_valueChanged(double arg1)
    {
        emit NotifyLatitudeMinChanged(arg1);
    }

    void MainView::on_dsbLatitudeMax_valueChanged(double arg1)
    {
        emit NotifyLatitudeMaxChanged(arg1);
    }

    void MainView::on_dsbLongitudeMin_valueChanged(double arg1)
    {
        emit NotifyLongitudeMinChanged(arg1);
    }

    void MainView::on_dsbLongitudeMax_valueChanged(double arg1)
    {
        emit NotifyLongitudeMaxChanged(arg1);
    }

    void MainView::on_sbZoomMin_valueChanged(int arg1)
    {
        emit NotifyZoomMinChanged(arg1);
    }

    void MainView::on_sbZoomMax_valueChanged(int arg1)
    {
        emit NotifyZoomMaxChanged(arg1);
    }

    void MainView::on_lineDestinationDir_editingFinished()
    {
        emit NotifyDestinationDirChanged(ui->lineDestinationDir->text());
    }

    void MainView::on_btnDestinationDir_clicked()
    {
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::DirectoryOnly);
        dialog.setOption(QFileDialog::ShowDirsOnly);
        bool result = dialog.exec();

        if(result)
        {
            ui->lineDestinationDir->setText(dialog.selectedFiles()[0]);
        }
    }

    void MainView::on_btnDownload_clicked()
    {
        if(!CheckValidConfigPanelSettings())
            return;

        ui->btnDownload->setEnabled(false);
        ui->gbConfigPanel->setEnabled(false);
        ui->btnStop->setEnabled(true);
        ui->lineDownloadedStatus->setText("");
        emit NotifyDownloadClicked();
    }

    void MainView::on_lineDestinationDir_selectionChanged()
    {
        emit NotifyDestinationDirChanged(ui->lineDestinationDir->text());
    }

    void MainView::on_lineDestinationDir_textChanged(const QString &arg1)
    {
        emit NotifyDestinationDirChanged(arg1);
    }

    void MainView::on_btnStop_clicked()
    {
        emit NotifyStopClicked();
    }

    void MainView::on_sbZoom_valueChanged(int arg1)
    {
        emit NotifyStartFromZoomChanged(arg1);
    }

    void MainView::on_sbTileX_valueChanged(int arg1)
    {
        emit NotifyStartFromTileXChanged(arg1);
    }

    void MainView::on_sbTileY_valueChanged(int arg1)
    {
        emit NotifyStartFromTileYChanged(arg1);
    }

    void MainView::on_cbStartFrom_toggled(bool checked)
    {
        emit NotifyStartFromChanged(checked);
        ui->loStartFrom->setEnabled(checked);
    }

    void MainView::on_btnClear_clicked()
    {
        ui->tbMessages->clear();
    }

    void MainView::on_cbShowOnlyErrors_toggled(bool checked)
    {
        emit NotifyShowOnlyErrorsChanged(checked);
    }

    void MainView::on_btnOpenDir_clicked()
    {
        if(ui->lineDestinationDir->text() != "")
        {
            QString path = ui->lineDestinationDir->text();
            QDesktopServices::openUrl(QUrl::fromLocalFile(path));
        }
    }

    void MainView::on_btnCheck_clicked()
    {
        if(!CheckValidConfigPanelSettings())
            return;

        ui->btnDownload->setEnabled(false);
        ui->gbConfigPanel->setEnabled(false);
        ui->btnStop->setEnabled(false);
        ui->lineDownloadedStatus->setText("");
        emit NotifyCheckClicked();
    }

    bool MainView::CheckValidConfigPanelSettings()
    {
        if(ui->lineDestinationDir->text() == "")
        {
            QMessageBox::warning(this,"Warning","Please select a destination directory.");
            return false;
        }
        if(ui->sbZoomMin->value() > ui->sbZoomMax->value())
        {
            QMessageBox::warning(this,"Warning","Min zoom must be smaller than max zoom.");
            return false;
        }
        if(ui->dsbLatitudeMin->value() > ui->dsbLatitudeMax->value())
        {
            QMessageBox::warning(this,"Warning","Min latitude must be smaller than max latitude.");
            return false;
        }
        if(ui->dsbLongitudeMin->value() > ui->dsbLongitudeMax->value())
        {
            QMessageBox::warning(this,"Warning","Min longitude must be smaller than max longitude.");
            return false;
        }
        return true;
    }
}




