#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "editor/circleeditor.h"
#include "editor/polygoneditor.h"
#include "editor/recteditor.h"
#include "editor/regioneditor.h"
#include "editor/ringeditor.h"
#include "editor/rotatedrecteditor.h"
#include "label/circlelabel.h"
#include "label/polygonlabel.h"
#include "label/rectlabel.h"
#include "label/regionlabel.h"
#include "label/ringlabel.h"
#include "label/rotatedrectlabel.h"

#include <QAction>
#include <QActionGroup>
#include <QFileDialog>
#include <QFontDatabase>
#include <QLabel>
#include <QToolButton>
#include <qmath.h>

enum ICON {
    RECT          = 0xe6b9,
    POLYGON       = 0xe603,
    SCALE_UP      = 0xec32,
    SCALE_DOWN    = 0xec13,
    FIT_WINDOW    = 0xe656,
    ORIGINAL_SIZE = 0xe912,
    PIXEL_PICKER  = 0xe728,
    FILE_OPEN     = 0xe670,
    FILE_SAVE     = 0xe873,
    EDITOR        = 0xe660,
    ROTATED_RECT  = 0xe88b,
    CIRCLE        = 0xe6d7,
    RING          = 0xe644,
    REGION        = 0xe609

};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mUi(new Ui::MainWindow)
    , mViewer(new ImageViewer) {
    mUi->setupUi(this);

    // icon
    auto  fontId       = QFontDatabase::addApplicationFont(":/iconfont.ttf");
    auto  fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    auto  fontName     = fontFamilies[ 0 ];
    QFont font(fontName, mIconFontSize);

    mUi->actionOpenFile->setFont(font);
    mUi->actionSaveFile->setFont(font);
    mUi->actionScaleUp->setFont(font);
    mUi->actionScaleDown->setFont(font);
    mUi->actionFitWindow->setFont(font);
    mUi->actionOriginalSize->setFont(font);
    mUi->actionPixelPicker->setFont(font);

    mUi->actionOpenFile->setText(QChar(ICON::FILE_OPEN));
    mUi->actionSaveFile->setText(QChar(ICON::FILE_SAVE));
    mUi->actionScaleUp->setText(QChar(ICON::SCALE_UP));
    mUi->actionScaleDown->setText(QChar(ICON::SCALE_DOWN));
    mUi->actionFitWindow->setText(QChar(ICON::FIT_WINDOW));
    mUi->actionOriginalSize->setText(QChar(ICON::ORIGINAL_SIZE));
    mUi->actionPixelPicker->setText(QChar(ICON::PIXEL_PICKER));

    // tool button
    auto *actionRect        = new QAction(this);
    auto *actionRotatedRect = new QAction(this);
    auto *actionRing        = new QAction(this);
    auto *actionCircle      = new QAction(this);
    auto *actionPolygon     = new QAction(this);
    auto *actionRegion      = new QAction(this);
    auto *menu              = new QMenu(this);
    for (auto &action :
         {actionRect, actionRotatedRect, actionRing, actionCircle, actionPolygon, actionRegion}) {
        action->setFont(font);
        menu->addAction(action);
    }

    actionRect->setText(QChar(ICON::RECT));
    actionRotatedRect->setText(QChar(ICON::ROTATED_RECT));
    actionRing->setText(QChar(ICON::RING));
    actionCircle->setText(QChar(ICON::CIRCLE));
    actionPolygon->setText(QChar(ICON::POLYGON));
    actionRegion->setText(QChar(ICON::REGION));

    actionRect->setToolTip("rectangle");
    actionRotatedRect->setToolTip("rotated rectangle");
    actionRing->setToolTip("ring");
    actionCircle->setToolTip("circle");
    actionPolygon->setToolTip("polygon");
    actionRegion->setToolTip("region");

    auto *toolBtn = new QToolButton(this);
    toolBtn->setPopupMode(QToolButton::MenuButtonPopup);
    toolBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    toolBtn->setDefaultAction(actionRect);
    toolBtn->setMenu(menu);

    mUi->toolBar->addWidget(toolBtn);

    // viewer
    setCentralWidget(mViewer);

    // event
    connect(mUi->actionFitWindow, &QAction::triggered, mViewer, &ImageViewer::fitToView);
    connect(mUi->actionOriginalSize, &QAction::triggered, mViewer,
            &ImageViewer::resetToOriginalSize);
    connect(mUi->actionOpenFile, &QAction::triggered, this, &MainWindow::openFile);
    connect(mUi->actionSaveFile, &QAction::triggered, this, &MainWindow::saveFile);
    connect(mUi->actionScaleUp, &QAction::triggered, mViewer, &ImageViewer::zoomIn);
    connect(mUi->actionScaleDown, &QAction::triggered, mViewer, &ImageViewer::zoomOut);
    connect(mUi->actionPixelPicker, &QAction::triggered, mViewer, &ImageViewer::setInSelect);
    connect(toolBtn, &QToolButton::triggered,
            [ toolBtn ](QAction *action) { toolBtn->setDefaultAction(action); });
    connect(actionCircle, &QAction::triggered,
            [ & ]() { mViewer->addEditor(QSharedPointer<CircleEditor>(new CircleEditor)); });
    connect(actionRing, &QAction::triggered,
            [ & ]() { mViewer->addEditor(QSharedPointer<RingEditor>(new RingEditor)); });
    connect(actionRect, &QAction::triggered,
            [ & ]() { mViewer->addEditor(QSharedPointer<RectEditor>(new RectEditor)); });
    connect(actionPolygon, &QAction::triggered,
            [ & ]() { mViewer->addEditor(QSharedPointer<PolygonEditor>(new PolygonEditor)); });
    connect(actionRegion, &QAction::triggered,
            [ & ]() { mViewer->addEditor(QSharedPointer<RegionEditor>(new RegionEditor)); });
    connect(actionRotatedRect, &QAction::triggered, [ & ]() {
        mViewer->addEditor(QSharedPointer<RotatedRectEditor>(new RotatedRectEditor));
    });
}

MainWindow::~MainWindow() {
    delete mViewer;
    delete mUi;
}

void MainWindow::openFile() {
    auto filepath = QFileDialog::getOpenFileName(
        this, tr("Open image"), "", tr("Image File(*.png *.jpg *.jpeg *.bmp *.tif);;All Files(*)"));
    if (filepath.isEmpty()) {
        return;
    }

    if (mViewer != nullptr) {
        mViewer->loadImage(filepath);
    }
}

void MainWindow::saveFile() {
    auto renderingImg = mViewer->rendering();
    if (renderingImg.isNull()) {
        return;
    }

    auto filepath = QFileDialog::getSaveFileName(
        this, tr("Save image"), "", tr("Image File(*.png *.jpg *.jpeg *.bmp *.tif);;All Files(*)"));
    if (filepath.isEmpty()) {
        return;
    }

    renderingImg.save(filepath);
}
