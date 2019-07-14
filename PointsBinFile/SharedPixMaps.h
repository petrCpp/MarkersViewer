#ifndef SHAREDPIXMAPS_H
#define SHAREDPIXMAPS_H

#include <QImage>
#include <QPixmap>
#include <QMutex>
#include <memory>
#include <queue>

// Обеспечивает потокобезопасный доступ к QImage

class SharedPixMaps
{
    friend class BinFileReader;
public:

    void addPreparedPixMap(std::unique_ptr<QImage> &&pixmap);
    QPixmap* extractPreparedPixmap();

private:
    SharedPixMaps();
    const int MAX_PIXMAP_COUNT = 2;
    QMutex mMutex;
    QPixmap mPixmap;
    std::queue<std::unique_ptr<QImage>> mImages;
};

#endif // SHAREDPIXMAPS_H
