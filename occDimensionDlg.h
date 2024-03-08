#ifndef OCCDIMENSIONDLG_H
#define OCCDIMENSIONDLG_H

#include <QDialog>

class QLabel;
class QSpinBox;
class QSlider;
class QPushButton;

class occDimensionDlg : public QDialog
{
    Q_OBJECT

public:
    explicit occDimensionDlg(QWidget *parent = nullptr);

public:
    void InitUi();
    void InitConnections();

signals:
    void SignalSetParameter();

private:
    QPushButton *pOKBtn;
    QPushButton *pCommonColorBtn;

 public:
    QSpinBox *pTextSizeSpb;
    QSpinBox *pFlyoutSpb;
    QLabel *pTextSizeLb;
    QLabel *pFlyoutLb;
    QSlider *pTextSizeSld;
    QSlider *pFlyoutSld;

    QColor color;
};

#endif // OCCDIMENSIONDLG_H
