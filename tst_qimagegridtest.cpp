/******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 https://github.com/labyrinthofdreams

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

#include <QString>
#include <QtTest>
#include "qimagegrid.hpp"

class QImageGridTest : public QObject
{
    Q_OBJECT

public:
    QImageGridTest();

private Q_SLOTS:
    void getRowCount()
    {
        QImage img;
        img.load(":/image/taxingwoman.png");
        QImageGrid grid;
        grid.addImage(0, 0, img);
        grid.addImage(1, 0, img);
        grid.addImage(2, 0, img);
        QCOMPARE(grid.getRowCount(), 3);
        grid.clear();
        QCOMPARE(grid.getRowCount(), 0);
    }

    void getColumnCount()
    {
        QImage img;
        img.load(":/image/taxingwoman.png");
        QImageGrid grid;
        grid.addImage(0, 0, img);
        grid.addImage(0, 1, img);
        grid.addImage(0, 2, img);
        QCOMPARE(grid.getColumnCount(0), 3);
        QCOMPARE(grid.getColumnCount(1), 0);
    }

    void emptyImage()
    {
        QImageGrid grid;
        QImage out = grid.generateImage();
        QCOMPARE(out.isNull(), true);
    }

    void singleImageWithoutSpacing()
    {
        QImage img;
        img.load(":/image/taxingwoman.png");
        QImageGrid grid;
        grid.addImage(0, 0, img);
        QImage out = grid.generateImage();
        QCOMPARE(out.width(), 688);
        QCOMPARE(out.height(), 512);
    }

    void singleImageWithSpacing()
    {
        QImage img;
        img.load(":/image/taxingwoman.png");
        QImageGrid grid;
        grid.setSpacing(10);
        grid.addImage(0, 0, img);
        QImage out = grid.generateImage();
        QCOMPARE(out.width(), 688);
        QCOMPARE(out.height(), 512);
    }

    void twoImagesWithoutSpacing()
    {
        QImage img;
        img.load(":/image/taxingwoman.png");
        // horizontal
        QImageGrid grid;
        grid.addImage(0, 0, img);
        grid.addImage(0, 1, img);
        QImage out = grid.generateImage();
        QCOMPARE(out.width(), 1376);
        QCOMPARE(out.height(), 512);
        grid.clear();
        // vertical
        grid.addImage(0, 0, img);
        grid.addImage(1, 0, img);
        out = grid.generateImage();
        QCOMPARE(out.width(), 688);
        QCOMPARE(out.height(), 1024);
    }

    void twoImagesWithSpacing()
    {
        QImage img;
        img.load(":/image/taxingwoman.png");
        // horizontal
        QImageGrid grid;
        grid.setSpacing(10);
        grid.addImage(0, 0, img);
        grid.addImage(0, 1, img);
        QImage out = grid.generateImage();
        QCOMPARE(out.width(), 1386);
        QCOMPARE(out.height(), 512);
        grid.clear();
        // vertical
        grid.addImage(0, 0, img);
        grid.addImage(1, 0, img);
        out = grid.generateImage();
        QCOMPARE(out.width(), 688);
        QCOMPARE(out.height(), 1034);
    }

    void minimumWidthSingleImage()
    {
        QImage img;
        img.load(":/image/taxingwoman.png");
        QImageGrid grid;
        grid.addImage(0, 0, img);
        grid.addImage(1, 0, img);
        grid.addImage(1, 1, img);
        QImage out = grid.generateImage();
        QCOMPARE(out.width(), img.width());
        QCOMPARE(out.height(), 768);
        grid.addImage(1, 2, img);
        out = grid.generateImage();
        QCOMPARE(out.width(), img.width());
        QCOMPARE(out.height(), 682);
    }

    void minimumWidthTwoImages()
    {
        QImage img;
        img.load(":/image/taxingwoman.png");
        QImageGrid grid;
        grid.addImage(0, 0, img);
        grid.addImage(0, 1, img);
        grid.addImage(1, 0, img);
        grid.addImage(1, 1, img);
        grid.addImage(1, 2, img);
        QImage out = grid.generateImage();
        QCOMPARE(out.width(), 2 * img.width());
        QCOMPARE(out.height(), 852);
    }
};

QImageGridTest::QImageGridTest()
{
}

QTEST_APPLESS_MAIN(QImageGridTest)

#include "tst_qimagegridtest.moc"
