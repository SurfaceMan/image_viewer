#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "editor/circleeditor.h"
#include "editor/polygoneditor.h"
#include "editor/recteditor.h"
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
#include <qmath.h>

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

    // display message
    auto sizeLabel  = new QLabel(this);
    auto scaleLabel = new QLabel(this);
    auto pixelLabel = new QLabel(this);
    mUi->statusbar->addWidget(sizeLabel);
    mUi->statusbar->addWidget(scaleLabel);
    mUi->statusbar->addWidget(pixelLabel);
    connect(mViewer, &ImageViewer::imageSizeChanged, [ sizeLabel ](const QSize &size) {
        sizeLabel->setText(tr("Size:[%1x%2]").arg(size.width()).arg(size.height()));
    });
    connect(mViewer, &ImageViewer::scaleFactorChanged,
            [ scaleLabel ](double scale) { scaleLabel->setText(tr("Scale:[%1]").arg(scale)); });
    connect(mViewer, &ImageViewer::pixelValueChanged,
            [ pixelLabel ](const QPoint &pos, const QColor &color) {
                pixelLabel->setText(tr("Position:[x:%1,y:%2] Color:[r:%3,g:%4,b:%5]")
                                        .arg(pos.x())
                                        .arg(pos.y())
                                        .arg(color.red())
                                        .arg(color.green())
                                        .arg(color.blue()));
            });

    // event
    connect(mUi->actionFitWindow, &QAction::triggered, mViewer, &ImageViewer::fitToView);
    connect(mUi->actionOriginalSize, &QAction::triggered, mViewer,
            &ImageViewer::resetToOriginalSize);
    connect(mUi->actionOpenFile, &QAction::triggered, this, &MainWindow::openFile);
    connect(mUi->actionScaleUp, &QAction::triggered, mViewer, &ImageViewer::zoomIn);
    connect(mUi->actionScaleDown, &QAction::triggered, mViewer, &ImageViewer::zoomOut);
    connect(labelGroup, &QActionGroup::triggered, [ & ](QAction *action) {
        QSharedPointer<LabelCategory> category(new LabelCategory);
        category->setId(0);
        category->setName("GOOD");
        category->setColor(Qt::green);
        category->setLineWidth(2);

        QSharedPointer<LabelEditor> editor;
        if (action == mUi->actionDrawRect) {
            editor.reset(new RotatedRectEditor);
            editor->setCategory(category);
        } else if (action == mUi->actionDrawPolygon) {
            editor.reset(new PolygonEditor);
            editor->setCategory(category);
        }
        mViewer->addEditor(editor);
    });
    connect(mUi->actionPixelPicker, &QAction::triggered, mViewer, &ImageViewer::setInSelect);
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

void MainWindow::saveFile() {}
