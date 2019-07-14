#include "SharedPixMaps.h"
#include <QMutexLocker>

SharedPixMaps::SharedPixMaps()
{

}

void SharedPixMaps::addPreparedPixMap(std::unique_ptr<QImage> &&pixmap)
{
    QMutexLocker locker(&mMutex);
    if(pixmap.get() != nullptr)
    {
        mImages.push(std::move(pixmap));
    }
}

QPixmap *SharedPixMaps::extractPreparedPixmap()
{
    QMutexLocker locker(&mMutex);
    std::unique_ptr<QImage> tmp = std::move(mImages.front());
    mImages.pop();
    if(tmp != nullptr)
    {
        mPixmap = QPixmap::fromImage(std::move(*tmp.get()));
    }
    return &mPixmap;
}

