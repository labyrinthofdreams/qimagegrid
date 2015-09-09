# QImageGrid

Requires C++14, tested with Qt5

Create a grid of images with an optional image spacing

```c++
QImage img;
img.load("image.png");

// If not supplied, QImage::Format_RGB32 is the default format
QImageGrid grid(QImage::Format_RGB32);

// Set image format
grid.setImageFormat(QImage::Format_RGB32);

// Set space between images
grid.setSpacing(10);

// Set space color
grid.setSpacingColor(Qt::black);

// Add the image to the grid
grid.addImage(0, 0, img);

// Or...
grid.addImage(qMakePair(0, 0), img);

// Generate image
const QImage out = grid.generateImage();

// Save to filename
grid.save("out.png");

// Save to QIODevice
QBuffer buffer;
grid.save(&buffer);

// Remove images
grid.remove(0, 0);

// Or...
grid.remove(qMakePair(0, 0));

// Or remove all at once...
grid.clear();

// Create a 3x1 grid
grid.addImage(0, 0, img);
grid.addImage(0, 1, img);
grid.addImage(0, 2, img);
grid.addImage(1, 0, img);

// Replace image
QImage img2;
img2.load("image2.png");
grid.addImage(0, 1, img2);

// Get row count (=2)
const int rows = grid.getRowCount();

// Get column count for row 0 (=3)
const int columns = grid.getColumnCount(0);
```

# Example

<img src="http://i.imgur.com/ZQu9uyn.png">