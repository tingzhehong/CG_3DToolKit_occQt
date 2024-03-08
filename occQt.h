/*
*    Copyright (c) 2024 Tim Hong All Rights Reserved.
*
*           File : occQt.h
*         Author : Tim Hong(hotize@163.com)
*           Date : 2024-01-01 00:00
*        Version : OpenCASCADE7.3.0 & Qt5.12.12
*
*    Description : OpenCASCADE in Qt.
*/

#ifndef OCCQT_H
#define OCCQT_H

#include "ui_occQt.h"

#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <TopoDS_Shape.hxx>

class OccView;

//! Qt main window which include OpenCASCADE for its central widget.
class occQt : public QMainWindow
{
    Q_OBJECT

public:
    //! constructor/destructor.
    occQt(QWidget *parent = nullptr);
    ~occQt();

protected:
    //! create all the actions.
    void createActions(void);

    //! create all the menus.
    void createMenus(void);

    //! create the toolbar.
    void createToolBars(void);

    //! make cylindrical helix.
    void makeCylindricalHelix(void);

    //! make conical helix.
    void makeConicalHelix(void);

    //! make toroidal helix.
    void makeToroidalHelix(void);

    //! drag event
    void dragEnterEvent(QDragEnterEvent *event);

    //! drop event
    void dropEvent(QDropEvent *event);

private slots:
    //! show about box.
    void about(void);

    //! Open step file.
    void open(void);

    //! Save modeling file.
    void save(void);

    //! make box test.
    void makeBox(void);

    //! make cone test.
    void makeCone(void);

    //! make sphere test.
    void makeSphere(void);

    //! make cylinder test.
    void makeCylinder(void);

    //! make torus test.
    void makeTorus(void);

    //! fillet test.
    void makeFillet(void);

    //! chamfer test.
    void makeChamfer(void);

    //! test extrude algorithm.
    void makeExtrude(void);

    //! test revol algorithm.
    void makeRevol(void);

    //! test loft algorithm.
    void makeLoft(void);

    //! test boolean operation cut.
    void testCut(void);

    //! test boolean operation fuse.
    void testFuse(void);

    //! test boolean operation common.
    void testCommon(void);

    //! test helix shapes.
    void testHelix(void);

    //! view Back.
    void viewBack(void);

    //! view Bottom.
    void viewBottom(void);

    //! view Front.
    void viewFront(void);

    //! view Iso.
    void viewIso(void);

    //! view Left.
    void viewLeft(void);

    //! view Right.
    void viewRight(void);

    //! view Top.
    void viewTop(void);

    //! delete.
    void shapeDelete(void);

    //! show WireFrame.
    void showWireFrame(void);

    //! show Shading.
    void showShading(void);

private:
    // dir
    QString dirPath;

    // shape
    TopoDS_Shape theShape;

private:
    // ui
    Ui::occQtClass ui;

    // wrapped the widget for occ.
    OccView* myOccView;
};

#endif // OCCQT_H
