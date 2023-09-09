#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "imageviewer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void openFile();
    void saveFile();

private:
    Ui::MainWindow *mUi;
    ImageViewer    *mViewer;

    const int mIconFontSize = 18;
};

#endif // MAINWINDOW_H
