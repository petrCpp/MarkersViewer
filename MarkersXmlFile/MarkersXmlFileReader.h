#ifndef MARKERSXMLFILEREADER_H
#define MARKERSXMLFILEREADER_H

#include <QObject>
#include <QString>
#include <mysimpletypes.h>

class MarkersXmlFileReader : public QObject
{
    Q_OBJECT
public:


    explicit MarkersXmlFileReader(QObject *parent = nullptr);
    bool xmlRead(const QString &fileName);
    const QList<MyTypes::SMarkerType>& getMarkers() const;
    MyTypes::SMarkerType getMarkersRange() const;

signals:

public slots:


private:
    QList<MyTypes::SMarkerType> mMarkers;
    MyTypes::SMarkerType mMinRange;
    MyTypes::SMarkerType mMaxRange;
};

#endif // MARKERSXMLFILEREADER_H
