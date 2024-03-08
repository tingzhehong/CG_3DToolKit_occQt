#include "occDimensionDlg.h"
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QSlider>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QColorDialog>


occDimensionDlg::occDimensionDlg(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr(u8"Dimension Parameter Setting"));
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setWindowIcon(QIcon(":/Resources/lamp.png"));
    resize(400, 300);

    InitUi();
    InitConnections();

    color.setRgb(0, 255, 0);
}

void occDimensionDlg::InitUi()
{
    pTextSizeSpb = new QSpinBox(this);
    pTextSizeSpb->setRange(1, 50);
    pTextSizeSpb->setValue(15);
    pFlyoutSpb = new QSpinBox(this);
    pFlyoutSpb->setRange(1, 50);
    pFlyoutSpb->setValue(8);

    pTextSizeLb = new QLabel(this);
    pTextSizeLb->setText(tr(u8"Text Size: "));
    pFlyoutLb = new QLabel(this);
    pFlyoutLb->setText(tr(u8"Flyout:    "));

    pTextSizeSld = new QSlider();
    pTextSizeSld->setOrientation(Qt::Horizontal);
    pTextSizeSld->setRange(1, 50);
    pTextSizeSld->setValue(15);
    pFlyoutSld = new QSlider();
    pFlyoutSld->setOrientation(Qt::Horizontal);
    pFlyoutSld->setRange(1, 50);
    pFlyoutSld->setValue(8);

    pCommonColorBtn = new QPushButton(tr(u8"Common Color"), this);
    pCommonColorBtn->setFixedSize(100, 50);

    pOKBtn = new QPushButton(tr(u8"OK"), this);
    pOKBtn->setFixedSize(100, 45);

    QHBoxLayout *pFirstLayout = new QHBoxLayout();
    pFirstLayout->addWidget(pTextSizeLb);
    pFirstLayout->addWidget(pTextSizeSpb);
    pFirstLayout->addWidget(pTextSizeSld);

    QHBoxLayout *pSecondLayout = new QHBoxLayout();
    pSecondLayout->addWidget(pFlyoutLb);
    pSecondLayout->addWidget(pFlyoutSpb);
    pSecondLayout->addWidget(pFlyoutSld);

    QHBoxLayout *pThirdLayout = new QHBoxLayout();
    pThirdLayout->addStretch();
    pThirdLayout->addWidget(pCommonColorBtn);
    pThirdLayout->addStretch();

    QHBoxLayout *pLastLayout = new QHBoxLayout();
    pLastLayout->addStretch();
    pLastLayout->addWidget(pOKBtn);
    pLastLayout->addStretch();

    QGroupBox *pDimensionGroupBox = new QGroupBox();
    pDimensionGroupBox->setTitle(tr(u8"Dimension"));

    QVBoxLayout *pDimensionLayout = new QVBoxLayout(pDimensionGroupBox);
    pDimensionLayout->addSpacing(10);
    pDimensionLayout->addLayout(pFirstLayout);
    pDimensionLayout->addSpacing(20);
    pDimensionLayout->addLayout(pSecondLayout);
    pDimensionLayout->addSpacing(30);
    pDimensionLayout->addLayout(pThirdLayout);

    QVBoxLayout *pMainLayout = new QVBoxLayout();
    pMainLayout->addWidget(pDimensionGroupBox);
    pMainLayout->addStretch();
    pMainLayout->addLayout(pLastLayout);

    setLayout(pMainLayout);
}

void occDimensionDlg::InitConnections()
{
    connect(pTextSizeSpb, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), pTextSizeSld, &QSlider::setValue);
    connect(pFlyoutSpb, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), pFlyoutSld, &QSlider::setValue);
    connect(pTextSizeSld, &QSlider::valueChanged, pTextSizeSpb, &QSpinBox::setValue);
    connect(pFlyoutSld, &QSlider::valueChanged, pFlyoutSpb, &QSpinBox::setValue);

    connect(pOKBtn, &QPushButton::clicked, this, &occDimensionDlg::close);

    connect(pCommonColorBtn, &QPushButton::clicked, [this]{
            QColor initial;
            initial.setRgbF(0, 1, 0);
            color = QColorDialog::getColor(initial, this, tr(u8"Common Color"));
    });
}
