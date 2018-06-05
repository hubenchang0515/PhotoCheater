#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>
#include <QColor>
#include <QQueue>
#include <Lolita/lolita.h>

class ImageProcessor
{
public:
    ImageProcessor();
    void qt2Lolita(const QImage& src, lolita::Image& dst);
    void lolita2Qt(const lolita::Image& src, QImage& dst);

    QImage gray(const QImage& src);
    QImage binary(const QImage& src);

    QImage averageBlur(const QImage& src);
    QImage medianBlur(const QImage& src);
    QImage gaussianBlur(const QImage& src);

    QImage changeBackground(const QImage& src, QColor color);

    QImage red(const QImage& src);
    QImage blue(const QImage& src);
    QImage white(const QImage& src);
};

#endif // IMAGEPROCESSOR_H
