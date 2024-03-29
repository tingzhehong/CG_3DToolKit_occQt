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

#ifndef _OCCVIEW_H_
#define _OCCVIEW_H_

#include <QOpenGLWidget>

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>

class QMenu;
class QRubberBand;
class occDimensionDlg;

//! Adapted a QWidget for OpenCASCADE viewer.
class OccView : public QWidget
{
    Q_OBJECT

public:
    //! mouse actions.
    enum CurrentAction3d
    {
        CurAction3d_Nothing,
        CurAction3d_DynamicZooming,
        CurAction3d_WindowZooming,
        CurAction3d_DynamicPanning,
        CurAction3d_GlobalPanning,
        CurAction3d_DynamicRotation
    };

public:
    //! constructor.
    OccView(QWidget* parent);

    const Handle(AIS_InteractiveContext)& getContext() const;
    const Handle(V3d_View)& getView() const;

signals:
    void selectionChanged(void);

public slots:
    //! operations for the view.
    void pan(void);
    void fitAll(void);
    void reset(void);
    void zoom(void);
    void rotate(void);

    void dimension(bool checked);
    void length(void);
    void angle(void);
    void vertices(void);
    void radius(void);
    void diameter(void);

protected:
    virtual QPaintEngine* paintEngine() const;

    // Paint events.
    virtual void paintEvent(QPaintEvent* theEvent);
    virtual void resizeEvent(QResizeEvent* theEvent);

    // Mouse events.
    virtual void mousePressEvent(QMouseEvent* theEvent);
    virtual void mouseReleaseEvent(QMouseEvent* theEvent);
    virtual void mouseMoveEvent(QMouseEvent * theEvent);
    virtual void wheelEvent(QWheelEvent * theEvent);

    // Button events.
    virtual void onLButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onMButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onRButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onMouseWheel(const int theFlags, const int theDelta, const QPoint thePoint);
    virtual void onLButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onMButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onRButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onMouseMove(const int theFlags, const QPoint thePoint);

    // Popup menu.
    virtual void addItemInPopup(QMenu* theMenu);

protected:
    void init(void);
    void popup(const int x, const int y);
    void dragEvent(const int x, const int y);
    void inputEvent(const int x, const int y);
    void moveEvent(const int x, const int y);
    void multiMoveEvent(const int x, const int y);
    void multiDragEvent(const int x, const int y);
    void multiInputEvent(const int x, const int y);
    void drawRubberBand(const int minX, const int minY, const int maxX, const int maxY);
    void panByLeftButton(const QPoint& thePoint);

private:

    //! the occ viewer.
    Handle(V3d_Viewer) myViewer;

    //! the occ view.
    Handle(V3d_View) myView;

    //! the occ context.
    Handle(AIS_InteractiveContext) myContext;

    //! save the mouse position.
    Standard_Integer myXmin;
    Standard_Integer myYmin;
    Standard_Integer myXmax;
    Standard_Integer myYmax;

    //! the mouse current mode.
    CurrentAction3d myCurrentMode;

    //! save the degenerate mode state.
    Standard_Boolean myDegenerateModeIsOn;

    //! rubber rectangle for the mouse selection.
    QRubberBand* myRectBand;

private:
    short myDimensionCounter = 0;
    TopoDS_Vertex myFirstVertex;
    TopoDS_Vertex mySecondVertex;
    TopoDS_Vertex myThirdVertex;
    TopoDS_Shape myShape;
    TopoDS_Edge myFirstEdge;
    TopoDS_Edge mySecondEdge;

    occDimensionDlg *myDimDlg;
};

#endif // _OCCVIEW_H_
