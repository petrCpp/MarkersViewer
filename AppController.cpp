#include "AppController.h"
#include <mainwindow.h>
#include <MarkersXmlFile/MarkersXmlFileReader.h>
#include <memory>
#include <QApplication>
#include <AppInfo/capplicationinfo.h>

AppController::AppController(QObject *parent) :
        QObject(parent)
{
    mBinFileReader.reset(new BinFileReader());
    mMainView.reset(new MainWindow()); // В дальнейшем здесь
                                      // можно будет создавать нужный
                                      // тип представления в зависимости
                                      // от используемой платформы
                                      // И в других местах обращаемся
                                      // к представлению через его интерфейс
    mMainView->show();

    mMainView->setMainViewEventsInterface(this); // События от mMainView
                                        // обрабатываем в текущем классе

    CApplicationInfo appInfo;

    mMainView->setAppName(appInfo.getApplicationName());
    mMainView->setAppDescription(appInfo.getAppDescription());
    mMainView->setAppBuildDate(appInfo.getAppBuildDate());
    mMainView->setAppVersion(appInfo.getAppVersion());
    mMainView->setContacts(appInfo.getContacts());
    mMainView->setMyPhoto(appInfo.getmyPhoto());

    connect(mBinFileReader.get(), &BinFileReader::porcionDataReady,
            this, &AppController::onPorcionDataReady,
            Qt::DirectConnection);
    connect(mBinFileReader.get(), &BinFileReader::percenteges, this,
            &AppController::onPercenteges, Qt::DirectConnection);

    connect(mBinFileReader.get(), &BinFileReader::errorHappened, this,
            &AppController::onErrorHappened, Qt::DirectConnection);

    MyTypes::SPointType pointsRange {32767, 32767};
    mBinFileReader->setPointsRange(pointsRange);



}

AppController::~AppController()
{

}

bool AppController::markersXmlFileOpen(const QString &fileName)
{
    std::unique_ptr<MarkersXmlFileReader> reader(new MarkersXmlFileReader());
    if(!reader->xmlRead(fileName))
    {
        return false;
    }

    MyTypes::SMarkerType markRange = reader->getMarkersRange();

    MyTypes::SPointType pointsRange;
    float tmp = 1.2 * markRange.x * MyTypes::MARKERS_SCALE /
            MyTypes::POINTS_SCALE;
    if(tmp <0)
    {
        tmp = 0;
    }
    else if(tmp >32767)
    {
        tmp = 32767;
    }
    pointsRange.x = tmp;

    tmp = 1.2 * markRange.y * MyTypes::MARKERS_SCALE /
            MyTypes::POINTS_SCALE;

    if(tmp <0)
    {
        tmp = 0;
    }
    else if(tmp >32767)
    {
        tmp = 32767;
    }
    pointsRange.y = tmp;
    mMainView->showMarkers(reader->getMarkers(), pointsRange);

    mBinFileReader->setPointsRange(pointsRange);
    return true;
}

void AppController::pointsFileStartRead(const QString &fileName)
{
    //mMainView->clearAllPoints();


    mBinFileReader->startReadFile(fileName);
}

void AppController::quit()
{
    //delete this;
    mBinFileReader.reset();
    QApplication::quit();
}

void AppController::onPercenteges(float percentage)
{
    mMainView->showProgress(percentage);
}

void AppController::onPorcionDataReady()
{
    mMainView->showPoints(*mBinFileReader->extractPreparedPixMap());
}

void AppController::onErrorHappened(const QString &message)
{
    mMainView->showErrorMessage(message);
}


