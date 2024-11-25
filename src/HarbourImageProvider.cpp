/*
 * Copyright (C) 2018-2024 Slava Monich <slava@monich.com>
 * Copyright (C) 2018-2019 Jolla Ltd.
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer
 *     in the documentation and/or other materials provided with the
 *     distribution.
 *
 *  3. Neither the names of the copyright holders nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation
 * are those of the authors and should not be interpreted as representing
 * any official policies, either expressed or implied.
 */

#include "HarbourImageProvider.h"
#include "HarbourTheme.h"
#include "HarbourDebug.h"

#include <QtGui/QImageReader>
#include <QtQuick/QQuickWindow>

#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
#  define qImageSizeInBytes(image) ((image).sizeInBytes())
#else
#  define qImageSizeInBytes(image) size_t((image).byteCount())
#endif

// ==========================================================================
// HarbourImageProvider::TextureFactory
// ==========================================================================

class HarbourImageProvider::TextureFactory :
    public QQuickTextureFactory
{
public:
    TextureFactory(const QString&, const QSize&);

    QSGTexture* createTexture(QQuickWindow*) const Q_DECL_OVERRIDE;
    QSize textureSize() const Q_DECL_OVERRIDE;
    int textureByteCount() const Q_DECL_OVERRIDE;
    QImage image() const Q_DECL_OVERRIDE;

    QImage load() const;
    static QImage colorize(QImage, const QColor&);

private:
    QString iPath;
    QString iHighlight;
    QSize iRequestedSize;
    mutable QImage iImage;
    HarbourTheme iTheme;
};

HarbourImageProvider::TextureFactory::TextureFactory(
    const QString& aPath,
    const QSize& aRequestedSize) :
    iRequestedSize(aRequestedSize)
{
    const int sep1 = aPath.lastIndexOf('?');
    const int sep2 = aPath.lastIndexOf('/');
    if (sep1 >= 0 && sep1 > sep2) {
        iPath = aPath.left(sep1);
        iHighlight = aPath.mid(sep1 + 1);
        HDEBUG(iPath << iHighlight);
    } else {
        iPath = aPath;
    }
}

QImage
HarbourImageProvider::TextureFactory::load() const
{
    if (iImage.isNull() && !iPath.isEmpty()) {
        if (iRequestedSize.isEmpty()) {
            if (iImage.load(iPath) && !iImage.size().isEmpty()) {
                QSize size(iImage.size());
                if (iRequestedSize.width() > 0) {
                    size.setHeight(qRound(iRequestedSize.width()*
                        (qreal)size.height()/size.width()));
                    size.setWidth(iRequestedSize.width());
                } else if (iRequestedSize.height() > 0) {
                    size.setWidth(qRound(iRequestedSize.height()*
                        (qreal)size.width()/size.height()));
                    size.setHeight(iRequestedSize.height());
                }
                if (iImage.size() != size) {
                    HDEBUG(iImage.size() << "=>" << size);
                    QImageReader imageReader(iPath);
                    imageReader.setScaledSize(size);
                    imageReader.read(&iImage);
                }
            }
        } else {
            QImageReader imageReader(iPath);
            imageReader.setScaledSize(iRequestedSize);
            imageReader.read(&iImage);
        }
        if (!iImage.isNull()) {
            if (iHighlight.isEmpty()) {
                // Colorize grayscale to match ambience
                if (iImage.isGrayscale()) {
                    QColor primaryColor(iTheme.primaryColor());
                    if (primaryColor.isValid()) {
                        iImage = colorize(iImage, primaryColor);
                    }
                }
            } else {
                // Colorization required
                QColor highlightColor(iHighlight);
                if (highlightColor.isValid()) {
                    HDEBUG(highlightColor);
                    iImage = colorize(iImage, highlightColor);
                }
            }
        } else {
            HWARN("can't load" << qPrintable(iPath));
        }
    }
    return iImage;
}

QSize
HarbourImageProvider::TextureFactory::textureSize() const
{
    return image().size();
}

int
HarbourImageProvider::TextureFactory::textureByteCount() const
{
    QSize size(image().size());
    return size.width() * size.height() * 4;
}

QImage
HarbourImageProvider::TextureFactory::image() const
{
    return load();
}

QSGTexture*
HarbourImageProvider::TextureFactory::createTexture(
    QQuickWindow* aWindow) const
{
    if (!load().isNull()) {
        QSGTexture* texture = aWindow->createTextureFromImage(iImage);
        if (qEnvironmentVariableIsSet("QSG_TRANSIENT_IMAGES")) {
            iImage = QImage();
        }
        return texture;
    }
    return NULL;
}

QImage
HarbourImageProvider::TextureFactory::colorize(
    QImage aImage,
    const QColor& aColor)
{
    QImage::Format format = aImage.format();
    switch (format) {
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        break;
    default:
        HWARN("TextureFactory: Image format not supported, doing format conversion");
        aImage = aImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        format = QImage::Format_ARGB32_Premultiplied;
    }

    const QRgb rgb = aColor.rgba() & 0x00ffffff;
    QRgb* bits = (QRgb*)aImage.bits();
    const int numPixels = qImageSizeInBytes(aImage)/sizeof(QRgb);

    if (format == QImage::Format_ARGB32) {
        for (int i=0; i<numPixels; i++) {
            QRgb alpha = bits[i] & 0xff000000;
            bits[i] = alpha | rgb;
        }
    } else if (format == QImage::Format_ARGB32_Premultiplied) {
        QRgb colorPremultiplied[256];
        for (int alpha = 0; alpha<256; alpha++) {
            colorPremultiplied[alpha] =
                    (alpha << 24) |
                    (alpha*qRed(rgb)/255 << 16) |
                    (alpha*qGreen(rgb)/255 <<  8) |
                    (alpha*qBlue(rgb) /255);
        }
        for (int i=0; i<numPixels; i++) {
            int alpha = (bits[i] & 0xff000000) >> 24;
            bits[i] = colorPremultiplied[alpha];
        }
    }

    return aImage;
}

// ==========================================================================
// HarbourImageProvider
// ==========================================================================

HarbourImageProvider::HarbourImageProvider() :
    QQuickImageProvider(QQuickImageProvider::Texture)
{
}

QQuickTextureFactory*
HarbourImageProvider::requestTexture(
    const QString& aId,
    QSize* aSize,
    const QSize& aRequestedSize)
{
    QString path(aId);
    HDEBUG(aId << aRequestedSize);
    if (path.startsWith("file://")) {
        path = path.mid(7);
        HDEBUG(path);
    }
    TextureFactory* factory = new TextureFactory(path, aRequestedSize);
    if (aSize) {
        *aSize = factory->textureSize();
    }
    return factory;
}
