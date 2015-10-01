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

#ifndef QIMAGEGRID_HPP
#define QIMAGEGRID_HPP

#include <QColor>
#include <QImage>
#include <QMap>
#include <QPair>
#include <QSize>

class QImageGrid
{
    //! Represents a position on the grid
    using RowColumn = QPair<int, int>;

    //! Grid contains the images and their position
    QMap<RowColumn, QImage> grid_;

    //! Computed sizes for each row
    mutable QMap<int, QSize> sizes_;

    //! Pixels between each image
    int spacing_;

    //! Color of the space between images
    QColor color_;

    //! Output image format
    QImage::Format format_;

    //! Resize to width
    int width_;

    /**
     * @brief Calculate output image size
     * @post Modifies sizes_ with correct images sizes for each row
     * @return Output image size
     */
    QSize calculateSize() const;

public:
    explicit QImageGrid(QImage::Format format = QImage::Format_RGB32);
    explicit QImageGrid(const QImageGrid &other) = default;
    explicit QImageGrid(QImageGrid &&other) = default;
    QImageGrid &operator=(const QImageGrid &other) = default;
    QImageGrid &operator=(QImageGrid &&other) = default;
    ~QImageGrid() = default;

    /**
     * @brief Add new image to grid (0-indexed)
     * @param row Row on the grid
     * @param column Column on the grid
     * @param image Image to add
     */
    void addImage(int row, int column, const QImage &image);

    /**
     * @brief Add new image to grid (0-indexed)
     * @param pos Position to add to
     * @param image Image to add
     */
    void addImage(const QPair<int, int> &position, const QImage &image);

    /**
     * @brief Remove image from grid (0-indexed)
     * @param row Row to remove from
     * @param column Column to remove from
     */
    void remove(int row, int column);

    /**
     * @brief Remove image from grid (0-indexed)
     * @param position Position to remove from
     */
    void remove(const QPair<int, int> &position);

    /**
     * @brief Set output image format
     * @param format Image format
     */
    void setImageFormat(QImage::Format format);

    /**
     * @brief Set spacing in pixels between images
     * @param spacing Spacing in pixels
     */
    void setSpacing(int spacing);

    /**
     * @brief Set color between images
     * @param color Color between images
     */
    void setSpacingColor(const QColor &color);

    /**
     * @brief Set generated image width
     *
     * A value of 0 will use normal image widths for
     * row with fewest columns
     * @param width New width
     */
    void setWidth(int width);

    /**
     * @brief Clear all images
     */
    void clear();

    /**
     * @brief Generate output image
     * @return Generated image
     */
    QImage generateImage() const;

    /**
     * @brief Save final image in filename
     * @param filename Filename to save to
     * @param format Output image format
     * @param quality Output image quality
     * @return true on success, otherwise false
     */
    bool save(const QString &filename, const char *format = 0, int quality = -1) const;

    /**
     * @brief Save final image in QIODevice
     * @param device Device to save to
     * @param format Output image format
     * @param quality Output image quality
     * @return true on success, otherwise false
     */
    bool save(QIODevice *device, const char *format = 0, int quality = -1) const;

    /**
     * @brief Get row count
     * @return Number of rows or 0 if no rows
     */
    int getRowCount() const;

    /**
     * @brief Get column count for given row
     * @param row Row to get column count for
     * @return Number of columns or 0 if no columns
     */
    int getColumnCount(int row) const;
};

#endif // QIMAGEGRID_HPP
