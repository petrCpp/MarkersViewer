#include "PointsBinFileWorker.h"
#include <QTimer>
#include <QEventLoop>
#include <QThread>
#include <QPainter>



PointsBinFileWorker::PointsBinFileWorker(SharedPixMaps *pixMapSincr,
                                         QObject *parent):
                                        QObject(parent),
                                         mPixMapSincr(pixMapSincr),
                                         mPointsRange({128, 128})
{

}


PointsBinFileWorker::~PointsBinFileWorker()
{
    ; //mScaningFileState = true;
    ;
}

void PointsBinFileWorker::startReadFile(const QString filename)
{
    if(!mScaningFileState)
    {
        mScaningFileState = true;
        mTerminate = false;
        QTimer::singleShot(0, this, &PointsBinFileWorker::scanFile); // отложенный вызов сканирования
        mFileName = filename;
    }
}

void PointsBinFileWorker::terminate()
{
    //tasksFiles.clear();
    mTerminate = true;
}

void PointsBinFileWorker::setPointsRange(int16_t x, int16_t y)
{
    mPointsRange.x = x;
    mPointsRange.y = y;
}

void PointsBinFileWorker::scanFile()
{

    mScaningFileState = true;
    //mDictionary.clear();
    bool err = false;
    QString errMessage;

    do  // Цикл с одной итерацией
    {

        QFile file(mFileName);
        if(!file.open(QIODevice::ReadOnly))
        {
            err = true;
            errMessage = tr("Не удалось открыть файл %1").arg(mFileName);
            break;
        }
        if(mTerminate)
        {
            err = true;
            errMessage = tr("Прервано пользователем");
            break;
        }

        uint64_t pos = 0;
        uint64_t fileSize = file.size();
        float percentage = 0.0f;

        int range = (mPointsRange.x > mPointsRange.y) ?
                    mPointsRange.x: mPointsRange.y;
        range *= MyTypes::POINTS_SCALE;

        int maxOffset = range/2;
        QImage tmpPixMap(range, range, QImage::Format_RGB32);
        tmpPixMap.fill(Qt::white);
        QPainter painter(&tmpPixMap);
        painter.setBrush(QBrush(QColor(0x00, 0x00, 0xFF, 0x50)));
        painter.drawRect(QRect(0, 0, tmpPixMap.width(), tmpPixMap.height()));
        uint32_t totalPoints = 0;
        while(1)
        {           
            file.seek(pos);
            QByteArray arr = file.read(PORC_POINT_COUNT * 2 * sizeof(int16_t));
            const uint32_t  POINTS_COUNT = arr.size() / 4;
            //QVector<SPointType> points(POINTS_COUNT);
            uint8_t *p = reinterpret_cast<uint8_t*>(arr.data());
            for(uint32_t i = 0; i<POINTS_COUNT; i++)
            {
                MyTypes::SPointType point; // = points[i];
                *reinterpret_cast<uint16_t*>(&point.x) = static_cast<uint16_t>(*p) |
                        (static_cast<uint16_t>(*(p+1))<<8);
                p += 2;
                *reinterpret_cast<uint16_t*>(&point.y) = static_cast<uint16_t>(*p) |
                        (static_cast<uint16_t>(*(p+1))<<8);
                p += 2;

                point.x *= MyTypes::POINTS_SCALE;
                point.y *= MyTypes::POINTS_SCALE;

                point.x = point.x + maxOffset;
                point.y = point.y + maxOffset;
                if(point.x < 0) point.x = 0;
                if(point.y <0) point.y = 0;
                if(point.x >= tmpPixMap.width()) point.x = tmpPixMap.width()-1;
                if(point.y >= tmpPixMap.height()) point.y = tmpPixMap.height()-1;

                painter.drawPoint(point.x, point.y);
                totalPoints ++;
                if(totalPoints >= MAX_POINT_COUNT)
                {
                    break;
                }
            }
            pos += arr.size();

            percentage = (100.0f *pos)/float(fileSize);

            mCurImage.reset(new QImage(tmpPixMap));

            mPixMapSincr->addPreparedPixMap(std::move(mCurImage));

            emit percenteges(percentage);
            emit porcionDataReady();
            QEventLoop loop;
            loop.processEvents();
            this->thread()->msleep(0);

            if(POINTS_COUNT < PORC_POINT_COUNT ||
                    totalPoints >= MAX_POINT_COUNT) // Достигли конца файла
                                                    // или больше точек рисовать нельзя
            {
                break;
            }
        }
        file.close();

    } while(0);

    if(err)
    {
        emit errorResult(errMessage);
    }
    emit percenteges(100.0f);
    mScaningFileState = false;
}
