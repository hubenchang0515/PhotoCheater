#include "imageprocessor.h"
#include <QDebug>


ImageProcessor::ImageProcessor()
{

}

void ImageProcessor::qt2Lolita(const QImage& src, lolita::Image& dst)
{
    dst.resize(src.width(), src.height());

    for(int row = 0; row < src.height(); row++)
    {
        for(int column = 0; column < src.width(); column++)
        {
            dst[row][column].red = src.pixelColor(column, row).red();
            dst[row][column].green = src.pixelColor(column, row).green();
            dst[row][column].blue = src.pixelColor(column, row).blue();
        }
    }



}


void ImageProcessor::lolita2Qt(const lolita::Image& src, QImage& dst)
{
    dst = QImage(src.width(), src.height(), QImage::Format_RGB888);
    for(uint32_t row = 0; row < src.height(); row++)
    {
        for(uint32_t column = 0; column < src.width(); column++)
        {
            dst.setPixelColor(column, row, QColor(src[row][column].red, src[row][column].green, src[row][column].blue));
        }
    }
}

QImage ImageProcessor::gray(const QImage& src)
{
    QImage dst;
    lolita::Image mat;
    qt2Lolita(src, mat);
    lolita::grayScale(mat);
    lolita2Qt(mat, dst);

    return dst;
}


QImage ImageProcessor::binary(const QImage& src)
{
    QImage dst;
    lolita::Image mat;
    qt2Lolita(src, mat);
    lolita::grayScale(mat);
    lolita::binaryzation(mat);
    lolita2Qt(mat, dst);

    return dst;
}


QImage ImageProcessor::averageBlur(const QImage& src)
{
    QImage dst;
    lolita::Image mat;
    qt2Lolita(src, mat);
    lolita::averageBlur(mat, 2);
    lolita2Qt(mat, dst);

    return dst;
}


QImage ImageProcessor::medianBlur(const QImage& src)
{
    QImage dst;
    lolita::Image mat;
    qt2Lolita(src, mat);
    lolita::medianBlur(mat,2);
    lolita2Qt(mat, dst);

    return dst;
}


QImage ImageProcessor::gaussianBlur(const QImage& src)
{
    QImage dst;
    lolita::Image mat;
    qt2Lolita(src, mat);
    lolita::gaussianBlur(mat, 2);
    lolita2Qt(mat, dst);

    return dst;
}


QImage ImageProcessor::changeBackground(const QImage& src, QColor color)
{
    QImage dst = src;
    lolita::Image mat;
    lolita::HsvImage hsv;
    qt2Lolita(src, mat);
    convertRgb2Hsv(mat, hsv);

    uintmax_t H=0,S=0,V=0;
    uint32_t cols = hsv.height() / 10;
    uint32_t rows = hsv.width() / 10;
    for(uint32_t y = 0; y < cols; y++)
    {
        for(uint32_t x = 0; x < rows; x++)
        {
            H += hsv[y][x].hue;
            S += hsv[y][x].saturation;
            V += hsv[y][x].value;
        }
    }

    lolita::HsvPixel p;
    p.hue = H / cols / rows;
    p.saturation = S / cols / rows;
    p.value = V / cols / rows;

    QQueue<QPoint> queue;
    queue.push_back(QPoint(0,0));

    lolita::Mat<lolita::Pixel> flag(hsv.width(), hsv.height());
    flag.map([](lolita::Pixel& p){p = 0;});
    mat[0][0].red = color.red();
    mat[0][0].green = color.green();
    mat[0][0].blue = color.blue();
    flag[0][0] = 0xffffff;

    while(!queue.isEmpty())
    {
        QPoint point = queue.first();

        for(int x = point.x() - 1; x <= point.x() + 1; x++)
        {
            for(int y = point.y() - 1; y <= point.y() + 1; y++)
            {
                if(x != point.x() && y != point.y())  // 4连通而非8连通
                    continue;
                else if(!dst.valid(x, y) || 0 != flag[y][x])  // 不在范围内
                    continue;
                else if(lolita::distance(hsv[y][x], p) < 50) // 颜色差距极小，直接替换
                {
                    mat[y][x].red = color.red();
                    mat[y][x].green = color.green();
                    mat[y][x].blue = color.blue();

                    queue.push_back(QPoint(x,y));  // 递归搜索连通区域
                    flag[y][x] = 1; // 防止重复递归
                }
                else if(lolita::distance(hsv[y][x], p) < 150) // 颜色差距在范围内但较大，判断为边缘
                {
                    lolita::RgbPixel rgbPix;
                    rgbPix.red = color.red();
                    rgbPix.green = color.green();
                    rgbPix.blue = color.blue();

                    lolita::HsvPixel hsvPix = lolita::rgb2hsv(mat[y][x]);
                    hsvPix.hue = lolita::rgb2hsv(rgbPix).hue;
                    hsvPix.saturation = lolita::rgb2hsv(rgbPix).saturation;
                    mat[y][x] = lolita::hsv2rgb(hsvPix);

                    queue.push_back(QPoint(x,y));
                    flag[y][x] = 0xffffff; // 记录边缘 进行滤波
                }
            }
        }

        queue.pop_front();
    }

    // 对边缘进行滤波
    lolita::Mat<double> kernel;
    lolita::gaussian(kernel, 1, 3);
    lolita::dilate(flag,3);
    for(uint32_t y = 0; y < flag.height(); y++)
    {
        for(uint32_t x= 0; x < flag.width(); x++)
        {
            if(flag[y][x] == 0xffffff)
            {
                mat[y][x] = lolita::convolutionElement(mat, y, x, kernel);
            }
        }
    }

    lolita2Qt(mat, dst);
    return dst;
}

QImage ImageProcessor::red(const QImage& src)
{
    return changeBackground(src, QColor(255,0,0));
}


QImage ImageProcessor::blue(const QImage& src)
{
    return changeBackground(src, QColor(0x63,0xb8,0xff));
}

QImage ImageProcessor::white(const QImage& src)
{
    return changeBackground(src, QColor(0xff,0xff,0xff));
}
