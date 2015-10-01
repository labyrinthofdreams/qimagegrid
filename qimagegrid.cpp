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

#include <algorithm>
#include <QBrush>
#include <QHash>
#include <QImage>
#include <QList>
#include <QPainter>
#include <QRect>
#include <QSize>
#include "qimagegrid.hpp"

namespace {

template <class T>
T calculateHeight(const QImage &img, const T newWidth) {
    return static_cast<double>(img.height()) / img.width() * newWidth;
}

} // namespace

QImageGrid::QImageGrid(const QImage::Format format)
    : grid_(), sizes_(), spacing_(0), color_(Qt::black), format_(format), width_(0)
{

}

void QImageGrid::addImage(const int row, const int column, const QImage &image)
{
   addImage(qMakePair(row, column), image);
}

void QImageGrid::addImage(const QPair<int, int> &position, const QImage &image)
{
   grid_.insert(position, image);
}

void QImageGrid::remove(const int row, const int column)
{
   remove(qMakePair(row, column));
}

void QImageGrid::remove(const QPair<int, int> &position)
{
   grid_.remove(position);
}

void QImageGrid::setImageFormat(const QImage::Format format)
{
   format_ = format;
}

void QImageGrid::setSpacing(const int spacing)
{
   spacing_ = spacing;
}

void QImageGrid::setSpacingColor(const QColor &color)
{
    color_ = color;
}

void QImageGrid::setWidth(const int width)
{
    width_ = width;
}

void QImageGrid::clear()
{
    grid_.clear();
}

bool QImageGrid::save(const QString &filename, const char *format, const int quality) const
{
   return generateImage().save(filename, format, quality);
}

bool QImageGrid::save(QIODevice *device, const char *format, const int quality) const
{
    return generateImage().save(device, format, quality);
}

int QImageGrid::getRowCount() const
{
    if(grid_.empty()) {
        return 0;
    }

    return grid_.lastKey().first + 1;
}

int QImageGrid::getColumnCount(const int row) const
{
    const QList<RowColumn> keys = grid_.keys();
    return std::count_if(keys.cbegin(), keys.cend(),
                         [&row](const RowColumn &key){ return key.first == row; });
}

QSize QImageGrid::calculateSize() const
{
   sizes_.clear();

   // 1. Calculate how many columns each row has
   const QList<RowColumn> keys = grid_.keys();
   QHash<int, int> counts;
   for(const RowColumn &key : keys) {
       counts[key.first]++;
   }

   // 2. Find the row with the fewest columns
   const QList<int> values = counts.values();
   const auto smallest = *std::min_element(values.cbegin(), values.cend());

   // 3. Minimum width will be used to calculate the new sizes
   // for each row where the column size differs
   const auto spacingWidth = (smallest - 1) * spacing_;
   const auto minWidthWithoutSpacing = smallest * grid_.first().width();
   const auto minWidth = width_ > 0 ? width_ : (minWidthWithoutSpacing + spacingWidth);

   // 4. Calculate the image widths for each row
   for(auto it = counts.cbegin(); it != counts.cend(); ++it) {
       const auto numberOfImages = it.value();
       const auto rowSpacing = (numberOfImages - 1) * spacing_;
       const auto widthWithoutSpacing = minWidth - rowSpacing;
       const auto rowImgWidth = widthWithoutSpacing / numberOfImages;
       sizes_.insert(it.key(), QSize(rowImgWidth,
                                     calculateHeight(grid_.first(), rowImgWidth)));
   }

   // 5. Calculate new image height
   const auto acc = [](const int last, const QSize &size){ return last + size.height(); };
   const auto newHeight = (sizes_.size() - 1) * spacing_
                          + std::accumulate(sizes_.cbegin(), sizes_.cend(), 0, acc);

   return {minWidth, newHeight};
}

QImage QImageGrid::generateImage() const
{
    if(grid_.empty()) {
        return {};
    }

    const QSize outputSize = calculateSize();
    int realY = 0;
    int lastRow = 0;
    int rowWidth = 0;
    QImage out(outputSize, format_);
    QPainter paint(&out);
    if(spacing_ > 0) {
        paint.setBrush(QBrush(color_));
        paint.drawRect(QRect(0, 0, out.width(), out.height()));
    }

    for(auto it = grid_.cbegin(); it != grid_.cend(); ++it) {
        const auto currentRow = it.key().first;
        if(lastRow != currentRow) {
           lastRow = currentRow;
           const auto previousRow = (it - 1).key().first;
           realY += sizes_.value(previousRow).height() + spacing_;
           rowWidth = 0;
        }

        const QSize size = sizes_.value(it.key().first);
        const auto column = it.key().second;
        // Next we increase the spacing if the image doesn't fill
        // all the way to the right edge
        rowWidth += column == 0 ? size.width() : size.width() + spacing_;
        const auto next = it + 1;
        const auto isLast = next != grid_.cend() && next.key().first != lastRow;
        const auto missingPixels = outputSize.width() - rowWidth;
        const auto pixelsToAdd = isLast && missingPixels ? missingPixels : 0;
        const auto x = column == 0 ? 0 : column * (size.width() + spacing_) + pixelsToAdd;
        const QImage img = it.value();
        paint.drawImage(x, realY, img.scaled(size));
    }

    paint.end();
    return out;
}
