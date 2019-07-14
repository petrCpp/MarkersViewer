#include "MarkersXmlFileReader.h"
#include <QFile>
#include <QXmlStreamReader>

MarkersXmlFileReader::MarkersXmlFileReader(QObject *parent) :
        QObject(parent),
        mMinRange({32768, 32768}),
        mMaxRange({-32768, -32768})
{

}


//static QXmlStreamReader::TokenType tokenType;
//static QString tokenString;
//static QString myText;

bool MarkersXmlFileReader::xmlRead(const QString &fileName)
{
    QFile fl(fileName);
    bool ok = true;
    if(!fl.open(QIODevice::ReadOnly))
    {
        return false;
    }
    uint16_t points = 0;
    mMinRange = {32768, 32768};
    mMaxRange = {-32768, -32768};

    do // Одноразовый цикл. Нужен для закрытия файла в любом случае
    {
        QXmlStreamReader reader(&fl);
        if(!reader.readNextStartElement())
        {
            break; // Ошибка
        }
        QString s = reader.name().toString();
        if(reader.name() != "Plot")
        {
            break;
        }

        if(!reader.attributes().hasAttribute("Caption"))
        {
            break; // Ошибка
        }

        QString val = reader.attributes().value("Caption").toString();
        QRegExp regExpr("\\d{1,5}");
        if(regExpr.indexIn(val) < 0)
        {
            break; // Ошибка
        }

        points = QString(*regExpr.capturedTexts().begin()).toInt();



        mMarkers.reserve(points);
        mMarkers.clear();
        ok = true;
        QXmlStreamReader::TokenType tokenType;
        while (!reader.atEnd() && points)
        {
            tokenType = reader.readNext();
            if(tokenType != QXmlStreamReader::StartElement)
            {
                continue;
            }
            points --;
            if(reader.name() != "Point")
            {
                ok = false;
                break;  // Ошибка
            }
            if(!reader.attributes().hasAttribute("X"))
            {
                ok = false;
                break; // Ошибка
            }
            if(!reader.attributes().hasAttribute("Y"))
            {
                ok = false;
                break; // Ошибка
            }
            MyTypes::SMarkerType marker;
            marker.x = reader.attributes().value("X").toFloat();
            marker.y = reader.attributes().value("Y").toFloat();
            mMarkers.append(marker);

            if(mMinRange.x > marker.x)  mMinRange.x = marker.x;
            if(mMinRange.y > marker.y)  mMinRange.y = marker.y;

            if(mMaxRange.x < marker.x)  mMaxRange.x = marker.x;
            if(mMaxRange.y < marker.y)  mMaxRange.y = marker.y;

            //tokenString = reader.tokenString();
            //myText = reader.text().toString();
            // do processing
        }

    } while(0);

    fl.close();
    if(points != 0)
    {
        ok = false; // Ошибка. Заявленное в шапке количество маркеров не
                    // совпало со
                    // считанным из файла
    }

    if(ok == false)
    {
        mMinRange = {32768, 32768};
        mMaxRange = {-32768, -32768};
        mMarkers.clear();
    }

    return ok;
}


const QList<MyTypes::SMarkerType> &MarkersXmlFileReader::getMarkers() const
{
    return mMarkers;
}

MyTypes::SMarkerType MarkersXmlFileReader::getMarkersRange() const
{
    MyTypes::SMarkerType range;
    range.x = mMaxRange.x - mMinRange.x;
    range.y = mMaxRange.y - mMinRange.y;
    if(range.x < 0 || range.y < 0)
    {
        range.x = 0;
        range.y = 0;
    }
    return range;
}
