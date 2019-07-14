#ifndef POINTSBINFILEWORKER_H
#define POINTSBINFILEWORKER_H

#include <QObject>
#include <QString>
#include <mysimpletypes.h>
#include <PointsBinFile/SharedPixMaps.h>
//#include <QQueue>
#include <memory>
#include <QMap>
#include <QFile>
#include <QVector>
#include <memory>
#include <QImage>

class PointsBinFileWorker : public QObject
{
    Q_OBJECT
    friend class BinFileReader;
public:
    ~PointsBinFileWorker() override;
public slots:
    void startReadFile(const QString filename);
    void terminate();
    void setPointsRange(int16_t x, int16_t y);
signals:
    void porcionDataReady();
    void errorResult(QString errDescription);
    void percenteges(float perc);
private slots:
    void scanFile();
    //bool scanFilePortion(QFile &file, int64_t startPos, int64_t porcByteCount, QMap<QString, uint16_t> &dictionary);

protected:
    PointsBinFileWorker(SharedPixMaps *pixMapSincr, QObject *parent = nullptr);
    //const uint32_t OFFSET_SIMBOLS = 70;
    const uint32_t PORC_POINT_COUNT = 1000;
    const uint32_t MAX_POINT_COUNT = 16000;
    //QMap<QString, uint16_t> mDictionary;
    //QQueue<QString> tasksFiles;
    QVector<MyTypes::SPointType> mPorcion;
    bool mScaningFileState {false};
    bool mTerminate {false};
    QString mFileName;
    SharedPixMaps *mPixMapSincr;
    std::unique_ptr<QImage> mCurImage;
    MyTypes::SPointType mPointsRange;
    //static QString remove0D0A(const QString &str);
};


#endif // POINTSBINFILEWORKER_H
