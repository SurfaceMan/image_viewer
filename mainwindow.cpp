#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QAction>
#include <QFileDialog>
#include <QFontDatabase>
#include <QActionGroup>

enum ICON {
    RECT          = 0xe6b9,
    POLYGON       = 0xe6ba,
    SCALE_UP      = 0xe61a,
    SCALE_DOWN    = 0xe619,
    FIT_WINDOW    = 0xe8e3,
    ORIGINAL_SIZE = 0xe912,
    PIXEL_PICKER  = 0xe633,
    FILE_OPEN     = 0xe670,
    FILE_SAVE     = 0xe706,
    SELECT        = 0xe6bc
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mUi(new Ui::MainWindow)
    , mViewer(new ImageViewer) {
    mUi->setupUi(this);

    // Action
    auto *labelGroup = new QActionGroup(this);
    labelGroup->addAction(mUi->actionDrawPolygon);
    labelGroup->addAction(mUi->actionDrawRect);
    labelGroup->addAction(mUi->actionSelect);
    labelGroup->setExclusive(true);
    mUi->actionSelect->setChecked(true);

    // icon
    auto  fontId       = QFontDatabase::addApplicationFont(":/iconfont.ttf");
    auto  fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    auto  fontName     = fontFamilies[ 0 ];
    QFont font(fontName, mIconFontSize);

    mUi->actionOpenFile->setFont(font);
    mUi->actionSaveFile->setFont(font);
    mUi->actionDrawRect->setFont(font);
    mUi->actionDrawPolygon->setFont(font);
    mUi->actionScaleUp->setFont(font);
    mUi->actionScaleDown->setFont(font);
    mUi->actionFitWindow->setFont(font);
    mUi->actionOriginalSize->setFont(font);
    mUi->actionPixelPicker->setFont(font);
    mUi->actionSelect->setFont(font);

    mUi->actionOpenFile->setText(QChar(ICON::FILE_OPEN));
    mUi->actionSaveFile->setText(QChar(ICON::FILE_SAVE));
    mUi->actionDrawRect->setText(QChar(ICON::RECT));
    mUi->actionDrawPolygon->setText(QChar(ICON::POLYGON));
    mUi->actionScaleUp->setText(QChar(ICON::SCALE_UP));
    mUi->actionScaleDown->setText(QChar(ICON::SCALE_DOWN));
    mUi->actionFitWindow->setText(QChar(ICON::FIT_WINDOW));
    mUi->actionOriginalSize->setText(QChar(ICON::ORIGINAL_SIZE));
    mUi->actionPixelPicker->setText(QChar(ICON::PIXEL_PICKER));
    mUi->actionSelect->setText(QChar(ICON::SELECT));

    // viewer
    setCentralWidget(mViewer);

    // event
    connect(mUi->actionFitWindow, &QAction::triggered, mViewer, &ImageViewer::fitToView);
    connect(mUi->actionOriginalSize, &QAction::triggered, mViewer,
            &ImageViewer::resetToOriginalSize);
    connect(mUi->actionOpenFile, &QAction::triggered, this, &MainWindow::openFile);
    connect(mUi->actionScaleUp, &QAction::triggered, mViewer, &ImageViewer::zoomIn);
    connect(mUi->actionScaleDown, &QAction::triggered, mViewer, &ImageViewer::zoomOut);
    connect(labelGroup, &QActionGroup::triggered, [ & ](QAction *action) {
        if (action == mUi->actionSelect) {
            mViewer->setInCreation(false);
        } else if (action == mUi->actionDrawRect) {
            mViewer->setInCreation(true);
            mViewer->setLabelType(LabelCategory::TYPE::RECT);
        } else if (action == mUi->actionDrawPolygon) {
            mViewer->setInCreation(true);
            mViewer->setLabelType(LabelCategory::TYPE::POLYGON);
        }
    });
}

MainWindow::~MainWindow() {
    delete mViewer;
    delete mUi;
}

void MainWindow::openFile() {
    auto filepath = QFileDialog::getOpenFileName(
        this, tr("Open image"), ".",
        tr("Image File(*.png *.jpg *.jpeg *.bmp *.tif);;All Files(*)"));
    if (filepath.isEmpty()) {
        return;
    }

    if (mViewer != nullptr) {
        mViewer->loadImage(filepath);
    }
}

void MainWindow::saveFile() {}
