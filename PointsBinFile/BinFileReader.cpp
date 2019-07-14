#include "BinFileReader.h"

BinFileReader::BinFileReader(QObject *parent) :
        QObject(parent),
        mSincronizer(new SharedPixMaps),
        mWorker(new PointsBinFileWorker(mSincronizer.get()))
{
    qRegisterMetaType<QVector<MyTypes::SPointType>>("QVector<MyTypes::SPointType>");
    qRegisterMetaType<int16_t>("int16_t");
    mWorker->moveToThread(&mWorkerThread);
    connect(&mWorkerThread, &QThread::finished, mWorker,
            &QObject::deleteLater);
    connect(mWorker, &PointsBinFileWorker::porcionDataReady, this,
            &BinFileReader::onPorcionDataReady, Qt::QueuedConnection);
    connect(mWorker, &PointsBinFileWorker::percenteges, this,
            &BinFileReader::percenteges, Qt::QueuedConnection);
    connect(mWorker, &PointsBinFileWorker::errorResult, this,
            &BinFileReader::errorHappened, Qt::QueuedConnection);
    mWorkerThread.start();

}

QPixmap*  BinFileReader::extractPreparedPixMap()
{
    return mSincronizer->extractPreparedPixmap();
    /*if(mPixMaps.size() != 0)
    {
        std::unique_ptr<QImage> val = std::move(mPixMaps.front());
        mPixMaps.pop();
        return std::move(val);
    }
    return std::move(std::unique_ptr<QImage>());*/
}

void BinFileReader::setPointsRange(const MyTypes::SPointType &range)
{
    // Асинхронный вызов метода в другом потоке
    this->metaObject()->invokeMethod(mWorker, "setPointsRange",
                                     Q_ARG(int16_t, range.x),
                                     Q_ARG(int16_t, range.y));
}





/*void BinFileReader::transmitUnckuckedPixMap(std::unique_ptr<QImage> &&pixmap)
{

}*/


BinFileReader::~BinFileReader()
{
    mWorkerThread.terminate();
    mWorkerThread.wait();
}




void BinFileReader::startReadFile(const QString &fileName)
{
    // Асинхронный вызов метода в другом потоке
    this->metaObject()->invokeMethod(mWorker, "startReadFile",
                                     Q_ARG(QString, fileName));
}

void BinFileReader::onPorcionDataReady()
{
    //std::unique_ptr<QImage> tmp = std::move(mSincronizer->extractPreparedPixmap());
    //if(tmp.get() != nullptr)
    //{
    //    mPixMaps.push(std::move(tmp));
    emit porcionDataReady();
    //}
}
