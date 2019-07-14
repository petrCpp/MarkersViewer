#ifndef BINFILEREADER_H
#define BINFILEREADER_H

#include <QObject>
#include <PointsBinFile/PointsBinFileWorker.h>
#include <PointsBinFile/SharedPixMaps.h>
#include <QThread>
#include <QQueue>
#include <QVector>
#include <QImage>
#include <memory>
#include <queue>

class BinFileReader : public QObject
{
    Q_OBJECT
public:
    explicit BinFileReader(QObject *parent = nullptr);
    QPixmap *extractPreparedPixMap();
    void setPointsRange(const MyTypes::SPointType &range);
    //void transmitUnckuckedPixMap(std::unique_ptr<QImage>&& pixmap);

    ~BinFileReader() override;
signals:
    void porcionDataReady();
    void percenteges(float perc);
    void errorHappened(QString descr);
public slots:
    void startReadFile(const QString &fileName);
private slots:
    void onPorcionDataReady();
private:
    std::unique_ptr<SharedPixMaps> mSincronizer;
    //std::queue<std::unique_ptr<QImage>> mPixMaps;
    PointsBinFileWorker *mWorker;
    QThread mWorkerThread;
};

#endif // BINFILEREADER_H
