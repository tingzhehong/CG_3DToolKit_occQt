/*
*    Copyright (c) 2024 Tim Hong All Rights Reserved.
*
*           File : occQt.cpp
*         Author : Tim Hong(hotize@163.com)
*           Date : 2024-01-01 00:00
*        Version : OpenCASCADE7.3.0 & Qt5.12.12
*
*    Description : Qt main window for OpenCASCADE.
*/

#include "occQt.h"
#include "occView.h"

#include <QToolBar>
#include <QTreeView>
#include <QMessageBox>
#include <QDockWidget>
#include <QTextCodec>
#include <QFileDialog>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>

#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Pln.hxx>

#include <gp_Lin2d.hxx>

#include <Geom_ConicalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <Geom_CylindricalSurface.hxx>

#include <GCE2d_MakeSegment.hxx>

#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TColgp_Array1OfPnt2d.hxx>

#include <BRepLib.hxx>
#include <BRepTools.hxx>
#include <BRep_Tool.hxx>
#include <BRep_Builder.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>

#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>

#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>

#include <dxfReader.h>

#include <AIS_Shape.hxx>

#include <Interface_Static.hxx>
#include <IGESControl_Reader.hxx>
#include <IGESControl_Writer.hxx>
#include <IGESControl_Controller.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
#include <StlAPI_Reader.hxx>
#include <StlAPI_Writer.hxx>


occQt::occQt(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    myOccView = new OccView(this);

    setCentralWidget(myOccView);

    createActions();
    createMenus();
    createToolBars();

    showMaximized();
    setAcceptDrops(true);
}

occQt::~occQt()
{

}

void occQt::createActions( void )
{
    // File
    connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));

    // View
    connect(ui.actionZoom, SIGNAL(triggered()), myOccView, SLOT(zoom()));
    connect(ui.actionPan, SIGNAL(triggered()), myOccView, SLOT(pan()));
    connect(ui.actionRotate, SIGNAL(triggered()), myOccView, SLOT(rotate()));

    connect(ui.actionReset, SIGNAL(triggered()), myOccView, SLOT(reset()));
    connect(ui.actionFitAll, SIGNAL(triggered()), myOccView, SLOT(fitAll()));
    connect(ui.actionDelete, SIGNAL(triggered()), this, SLOT(shapeDelete()));

    // Primitive
    connect(ui.actionBox, SIGNAL(triggered()), this, SLOT(makeBox()));
    connect(ui.actionCone, SIGNAL(triggered()), this, SLOT(makeCone()));
    connect(ui.actionSphere, SIGNAL(triggered()), this, SLOT(makeSphere()));
    connect(ui.actionCylinder, SIGNAL(triggered()), this, SLOT(makeCylinder()));
    connect(ui.actionTorus, SIGNAL(triggered()), this, SLOT(makeTorus()));

    // Modeling
    connect(ui.actionFillet, SIGNAL(triggered()), this, SLOT(makeFillet()));
    connect(ui.actionChamfer, SIGNAL(triggered()), this, SLOT(makeChamfer()));
    connect(ui.actionExtrude, SIGNAL(triggered()), this, SLOT(makeExtrude()));
    connect(ui.actionRevolve, SIGNAL(triggered()), this, SLOT(makeRevol()));
    connect(ui.actionLoft, SIGNAL(triggered()), this, SLOT(makeLoft()));

    connect(ui.actionCut, SIGNAL(triggered()), this, SLOT(testCut()));
    connect(ui.actionFuse, SIGNAL(triggered()), this, SLOT(testFuse()));
    connect(ui.actionCommon, SIGNAL(triggered()), this, SLOT(testCommon()));

    connect(ui.actionHelix, SIGNAL(triggered()), this, SLOT(testHelix()));

    // Visual
    connect(ui.actionViewBack, SIGNAL(triggered()), this, SLOT(viewBack()));
    connect(ui.actionViewBottom, SIGNAL(triggered()), this, SLOT(viewBottom()));
    connect(ui.actionViewFront, SIGNAL(triggered()), this, SLOT(viewFront()));
    connect(ui.actionViewIso, SIGNAL(triggered()), this, SLOT(viewIso()));
    connect(ui.actionViewLeft, SIGNAL(triggered()), this, SLOT(viewLeft()));
    connect(ui.actionViewRight, SIGNAL(triggered()), this, SLOT(viewRight()));
    connect(ui.actionViewTop, SIGNAL(triggered()), this, SLOT(viewTop()));
    connect(ui.actionWireFrame, SIGNAL(triggered()), this, SLOT(showWireFrame()));
    connect(ui.actionShading, SIGNAL(triggered()), this, SLOT(showShading()));

    // Dimension
    connect(ui.actionMeasure, SIGNAL(triggered(bool)), myOccView, SLOT(dimension(bool)));
    connect(ui.actionLength, SIGNAL(triggered()), myOccView, SLOT(length()));
    connect(ui.actionVertices, SIGNAL(triggered()), myOccView, SLOT(vertices()));
    connect(ui.actionAngle, SIGNAL(triggered()), myOccView, SLOT(angle()));
    connect(ui.actionRadius, SIGNAL(triggered()), myOccView, SLOT(radius()));
    connect(ui.actionDiameter, SIGNAL(triggered()), myOccView, SLOT(diameter()));

    // Help
    connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(about()));
}

void occQt::createMenus( void )
{
    QMenuBar *menuBar = new QMenuBar(this);
    menuBar->setObjectName(QString::fromUtf8("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 680, 21));

    QMenu *menu_1 = new QMenu(menuBar);
    menu_1->setTitle(QString::fromUtf8("File"));
    menu_1->addAction(ui.actionOpen);
    menu_1->addAction(ui.actionSave);
    menu_1->addAction(ui.actionExit);

    QMenu *menu_2 = new QMenu(menuBar);
    menu_2->setTitle(QString::fromUtf8("View"));
    menu_2->addAction(ui.actionZoom);
    menu_2->addAction(ui.actionPan);
    menu_2->addAction(ui.actionRotate);
    menu_2->addSeparator();
    menu_2->addAction(ui.actionReset);
    menu_2->addAction(ui.actionFitAll);
    menu_2->addAction(ui.actionDelete);

    QMenu *menu_3 = new QMenu(menuBar);
    menu_3->setTitle(QString::fromUtf8("Primitive"));
    menu_3->addAction(ui.actionBox);
    menu_3->addAction(ui.actionCone);
    menu_3->addAction(ui.actionSphere);
    menu_3->addAction(ui.actionCylinder);
    menu_3->addAction(ui.actionTorus);

    QMenu *menu_4 = new QMenu(menuBar);
    menu_4->setTitle(QString::fromUtf8("Modeling"));
    menu_4->addAction(ui.actionFillet);
    menu_4->addAction(ui.actionChamfer);
    menu_4->addAction(ui.actionExtrude);
    menu_4->addAction(ui.actionRevolve);
    menu_4->addAction(ui.actionLoft);
    menu_4->addSeparator();
    menu_4->addAction(ui.actionCut);
    menu_4->addAction(ui.actionFuse);
    menu_4->addAction(ui.actionCommon);
    menu_4->addSeparator();
    menu_4->addAction(ui.actionHelix);

    QMenu *menu_5 = new QMenu(menuBar);
    menu_5->setTitle(QString::fromUtf8("Dimension"));
    menu_5->addAction(ui.actionLength);
    menu_5->addAction(ui.actionVertices);
    menu_5->addAction(ui.actionAngle);
    menu_5->addAction(ui.actionRadius);
    menu_5->addAction(ui.actionDiameter);

    QMenu *menu_6 = new QMenu(menuBar);
    menu_6->setTitle(QString::fromUtf8("Help"));
    menu_6->addAction(ui.actionAbout);

    menuBar->addAction(menu_1->menuAction());
    menuBar->addAction(menu_2->menuAction());
    menuBar->addAction(menu_3->menuAction());
    menuBar->addAction(menu_4->menuAction());
    menuBar->addAction(menu_5->menuAction());
    menuBar->addAction(menu_6->menuAction());

    this->setMenuBar(menuBar);
}

void occQt::createToolBars( void )
{
    QToolBar* aToolBar = addToolBar(tr("&Navigate"));
    aToolBar->addAction(ui.actionZoom);
    aToolBar->addAction(ui.actionPan);
    aToolBar->addAction(ui.actionRotate);

    aToolBar = addToolBar(tr("&View"));
    aToolBar->addAction(ui.actionReset);
    aToolBar->addAction(ui.actionFitAll);
    aToolBar->addAction(ui.actionDelete);

    aToolBar = addToolBar(tr("&Primitive"));
    aToolBar->addAction(ui.actionBox);
    aToolBar->addAction(ui.actionCone);
    aToolBar->addAction(ui.actionSphere);
    aToolBar->addAction(ui.actionCylinder);
    aToolBar->addAction(ui.actionTorus);

    aToolBar = addToolBar(tr("&Modeling"));
    aToolBar->addAction(ui.actionFillet);
    aToolBar->addAction(ui.actionChamfer);
    aToolBar->addAction(ui.actionExtrude);
    aToolBar->addAction(ui.actionRevolve);
    aToolBar->addAction(ui.actionLoft);
    aToolBar->addSeparator();
    aToolBar->addAction(ui.actionCut);
    aToolBar->addAction(ui.actionFuse);
    aToolBar->addAction(ui.actionCommon);
    aToolBar->addSeparator();
    aToolBar->addAction(ui.actionHelix);

    aToolBar = addToolBar(tr("&Visual"));
    aToolBar->addAction(ui.actionViewFront);
    aToolBar->addAction(ui.actionViewBack);
    aToolBar->addAction(ui.actionViewTop);
    aToolBar->addAction(ui.actionViewBottom);
    aToolBar->addAction(ui.actionViewLeft);
    aToolBar->addAction(ui.actionViewRight);
    aToolBar->addAction(ui.actionViewIso);
    aToolBar->addSeparator();
    aToolBar->addAction(ui.actionWireFrame);
    aToolBar->addAction(ui.actionShading);

    aToolBar = addToolBar(tr("&Dimension"));
    aToolBar->addAction(ui.actionMeasure);

    aToolBar = addToolBar(tr("&Help"));
    aToolBar->addAction(ui.actionAbout);

    this->addToolBar(Qt::TopToolBarArea, aToolBar);
}

void occQt::about()
{
    QMessageBox::about(this, tr("About occQt"),
        tr("<h2>occQt 2.2</h2>"
        "<p>Copyright &copy; 2024 hotize@163.com"
        "<p>occQt is a demo applicaton about Qt and OpenCASCADE."));
}

void occQt::open()
{
    QString FileName = QFileDialog::getOpenFileName(this, tr(u8"Open CAD file"), dirPath, "*.*;; *.brep;; *.dxf;; *.iges *.igs;; *.step *.stp;; *.stl");

    if (FileName.isEmpty())
    {
        QMessageBox::information(this, tr(u8"Information"), tr(u8"Please select CAD document!"));
    }
    else
    {
        QFileInfo Info(FileName);
        dirPath = Info.path();
        QTextCodec *code = QTextCodec::codecForName("GB2312");
        std::string filename = code->fromUnicode(FileName).data();

        TopoDS_Shape aShape;

        //brep
        if (Info.suffix().toLower() == "brep")
        {
            BRep_Builder aBuilder;
            Standard_Boolean result = BRepTools::Read(aShape, filename.c_str(), aBuilder);
            if (result)
                theShape = aShape; 
        }
        //dxf
        if (Info.suffix().toLower() == "dxf")
        {
            DxfReader aDxfReader(filename);
            aShape = aDxfReader.GetShape();
            theShape = aShape;
        }
        //igs
        if (Info.suffix().toLower() == "igs" || Info.suffix().toLower() == "iges")
        {
            IGESControl_Reader aReader_IGES;
            aReader_IGES.ReadFile(filename.c_str());
            aReader_IGES.PrintCheckLoad(Standard_False, IFSelect_ItemsByEntity);
            Standard_Integer NbRoots = aReader_IGES.NbRootsForTransfer();
            Standard_Integer num = aReader_IGES.TransferRoots();
            aShape = aReader_IGES.OneShape();
            theShape = aShape;
        }
        //stp
        if (Info.suffix().toLower() == "stp" || Info.suffix().toLower() == "step")
        {
            STEPControl_Reader aReader_Step;
            aReader_Step.ReadFile(filename.c_str());
            aReader_Step.PrintCheckLoad(Standard_False, IFSelect_ItemsByEntity);
            Standard_Integer NbRoots = aReader_Step.NbRootsForTransfer();
            Standard_Integer num = aReader_Step.TransferRoots();
            aShape = aReader_Step.OneShape();
            theShape = aShape;
        }
        //stl
        if (Info.suffix().toLower() == "stl")
        {
            StlAPI_Reader aReader_Stl;
            aReader_Stl.Read(aShape, filename.c_str());
            theShape = aShape;
        }

        Handle(AIS_Shape) anAisModel = new AIS_Shape(aShape);
        anAisModel->SetColor(Quantity_NOC_GRAY);
        anAisModel->SetTransparency(0);
        myOccView->getContext()->Display(anAisModel, Standard_True);
        myOccView->fitAll();
    }
}

void occQt::save()
{
    QString FileName = QFileDialog::getSaveFileName(this, tr(u8"Information"), dirPath, "*.brep;; *.iges;; *.step;; *.stl");

    if (FileName.isEmpty())
    {
        QMessageBox::information(this, tr(u8"Information"), tr(u8"Please select CAD document!"));
    }
    else
    {
        QFileInfo file(FileName);
        dirPath = file.path();
        QTextCodec *code = QTextCodec::codecForName("GB2312");
        std::string filename = code->fromUnicode(FileName).data();

        const QString ext = file.suffix().toLower();

        Handle(TopTools_HSequenceOfShape) aSequence = new TopTools_HSequenceOfShape();
        aSequence->Clear();
        aSequence->Append(theShape);

        //brep
        if (ext == "brep")
        {
            if (aSequence.IsNull() || aSequence->IsEmpty())
                return;

            TopoDS_Shape shape = aSequence->Value(1);
            BRepTools::Write(shape, filename.c_str());
        }
        //igs
        if (ext == "iges")
        {
            if (aSequence.IsNull() || aSequence->IsEmpty())
                return;

            IGESControl_Controller::Init();
            IGESControl_Writer writer(Interface_Static::CVal("XSTEP.iges.unit"),
                                      Interface_Static::IVal("XSTEP.iges.writebrep.mode"));

            for (int i = 1; i <= aSequence->Length(); i++)
            {
                writer.AddShape(aSequence->Value(i));
            }
            writer.ComputeModel();
            writer.Write(filename.c_str());
        }
        //stp
        if (ext == "step")
        {
            if (aSequence.IsNull() || aSequence->IsEmpty())
                return;

            STEPControl_StepModelType type = STEPControl_AsIs;
            IFSelect_ReturnStatus status;

            STEPControl_Writer writer;
            for (int i = 1; i <= aSequence->Length(); i++)
            {
                status = writer.Transfer(aSequence->Value(i), type);
                if (status != IFSelect_RetDone)
                    return;
            }
            writer.Write(filename.c_str());
        }
        //stl
        if (ext == "stl")
        {
            if (aSequence.IsNull() || aSequence->IsEmpty())
                    return;

                TopoDS_Compound res;
                BRep_Builder builder;
                builder.MakeCompound(res);

                for (int i = 1; i <= aSequence->Length(); i++)
                {
                    TopoDS_Shape shape = aSequence->Value(i);
                    if (shape.IsNull())
                        return;

                    builder.Add(res, shape);
                }
                StlAPI_Writer writer;
                writer.Write(res, filename.c_str());
        }
    }
}

void occQt::makeBox()
{
    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(3.0, 4.0, 5.0).Shape();
    Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);

    anAisBox->SetColor(Quantity_NOC_AZURE);

    myOccView->getContext()->Display(anAisBox, Standard_True);
    myOccView->fitAll();
}

void occQt::makeCone()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 10.0, 0.0));

    TopoDS_Shape aTopoReducer = BRepPrimAPI_MakeCone(anAxis, 3.0, 1.5, 5.0).Shape();
    Handle(AIS_Shape) anAisReducer = new AIS_Shape(aTopoReducer);

    anAisReducer->SetColor(Quantity_NOC_BISQUE);

    anAxis.SetLocation(gp_Pnt(8.0, 10.0, 0.0));
    TopoDS_Shape aTopoCone = BRepPrimAPI_MakeCone(anAxis, 3.0, 0.0, 5.0).Shape();
    Handle(AIS_Shape) anAisCone = new AIS_Shape(aTopoCone);

    anAisCone->SetColor(Quantity_NOC_CHOCOLATE);

    myOccView->getContext()->Display(anAisReducer, Standard_True);
    myOccView->getContext()->Display(anAisCone, Standard_True);
    myOccView->fitAll();
}

void occQt::makeSphere()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 20.0, 0.0));

    TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 3.0).Shape();
    Handle(AIS_Shape) anAisSphere = new AIS_Shape(aTopoSphere);

    anAisSphere->SetColor(Quantity_NOC_BLUE1);

    myOccView->getContext()->Display(anAisSphere, Standard_True);
    myOccView->fitAll();
}

void occQt::makeCylinder()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 30.0, 0.0));

    TopoDS_Shape aTopoCylinder = BRepPrimAPI_MakeCylinder(anAxis, 3.0, 5.0).Shape();
    Handle(AIS_Shape) anAisCylinder = new AIS_Shape(aTopoCylinder);

    anAisCylinder->SetColor(Quantity_NOC_RED);

    anAxis.SetLocation(gp_Pnt(8.0, 30.0, 0.0));
    TopoDS_Shape aTopoPie = BRepPrimAPI_MakeCylinder(anAxis, 3.0, 5.0, M_PI_2 * 3.0).Shape();
    Handle(AIS_Shape) anAisPie = new AIS_Shape(aTopoPie);

    anAisPie->SetColor(Quantity_NOC_TAN);

    myOccView->getContext()->Display(anAisCylinder, Standard_True);
    myOccView->getContext()->Display(anAisPie, Standard_True);
    myOccView->fitAll();
}

void occQt::makeTorus()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 40.0, 0.0));

    TopoDS_Shape aTopoTorus = BRepPrimAPI_MakeTorus(anAxis, 3.0, 1.0).Shape();
    Handle(AIS_Shape) anAisTorus = new AIS_Shape(aTopoTorus);

    anAisTorus->SetColor(Quantity_NOC_YELLOW);

    anAxis.SetLocation(gp_Pnt(8.0, 40.0, 0.0));
    TopoDS_Shape aTopoElbow = BRepPrimAPI_MakeTorus(anAxis, 3.0, 1.0, M_PI_2).Shape();
    Handle(AIS_Shape) anAisElbow = new AIS_Shape(aTopoElbow);

    anAisElbow->SetColor(Quantity_NOC_THISTLE);

    myOccView->getContext()->Display(anAisTorus, Standard_True);
    myOccView->getContext()->Display(anAisElbow, Standard_True);
    myOccView->fitAll();
}

void occQt::makeFillet()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 50.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    BRepFilletAPI_MakeFillet MF(aTopoBox);

    // Add all the edges to fillet.
    for (TopExp_Explorer ex(aTopoBox, TopAbs_EDGE); ex.More(); ex.Next())
    {
        MF.Add(1.0, TopoDS::Edge(ex.Current()));
    }

    Handle(AIS_Shape) anAisShape = new AIS_Shape(MF.Shape());
    anAisShape->SetColor(Quantity_NOC_VIOLET);

    myOccView->getContext()->Display(anAisShape, Standard_True);
    myOccView->fitAll();
}

void occQt::makeChamfer()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(8.0, 50.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    BRepFilletAPI_MakeChamfer MC(aTopoBox);
    TopTools_IndexedDataMapOfShapeListOfShape aEdgeFaceMap;

    TopExp::MapShapesAndAncestors(aTopoBox, TopAbs_EDGE, TopAbs_FACE, aEdgeFaceMap);

    for (Standard_Integer i = 1; i <= aEdgeFaceMap.Extent(); ++i)
    {
        TopoDS_Edge anEdge = TopoDS::Edge(aEdgeFaceMap.FindKey(i));
        TopoDS_Face aFace = TopoDS::Face(aEdgeFaceMap.FindFromIndex(i).First());

        MC.Add(0.6, 0.6, anEdge, aFace);
    }

    Handle(AIS_Shape) anAisShape = new AIS_Shape(MC.Shape());
    anAisShape->SetColor(Quantity_NOC_TOMATO);

    myOccView->getContext()->Display(anAisShape, Standard_True);
    myOccView->fitAll();
}

void occQt::makeExtrude()
{
    // prism a vertex result is an edge.
    TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(gp_Pnt(0.0, 60.0, 0.0));
    TopoDS_Shape aPrismVertex = BRepPrimAPI_MakePrism(aVertex, gp_Vec(0.0, 0.0, 5.0));
    Handle(AIS_Shape) anAisPrismVertex = new AIS_Shape(aPrismVertex);

    // prism an edge result is a face.
    TopoDS_Edge anEdge = BRepBuilderAPI_MakeEdge(gp_Pnt(5.0, 60.0, 0.0), gp_Pnt(10.0, 60.0, 0.0));
    TopoDS_Shape aPrismEdge = BRepPrimAPI_MakePrism(anEdge, gp_Vec(0.0, 0.0, 5.0));
    Handle(AIS_Shape) anAisPrismEdge = new AIS_Shape(aPrismEdge);

    // prism a wire result is a shell.
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(16.0, 60.0, 0.0));

    TopoDS_Edge aCircleEdge = BRepBuilderAPI_MakeEdge(gp_Circ(anAxis, 3.0));
    TopoDS_Wire aCircleWire = BRepBuilderAPI_MakeWire(aCircleEdge);
    TopoDS_Shape aPrismCircle = BRepPrimAPI_MakePrism(aCircleWire, gp_Vec(0.0, 0.0, 5.0));
    Handle(AIS_Shape) anAisPrismCircle = new AIS_Shape(aPrismCircle);

    // prism a face or a shell result is a solid.
    anAxis.SetLocation(gp_Pnt(24.0, 60.0, 0.0));
    TopoDS_Edge aEllipseEdge = BRepBuilderAPI_MakeEdge(gp_Elips(anAxis, 3.0, 2.0));
    TopoDS_Wire aEllipseWire = BRepBuilderAPI_MakeWire(aEllipseEdge);
    TopoDS_Face aEllipseFace = BRepBuilderAPI_MakeFace(gp_Pln(gp::XOY()), aEllipseWire);
    TopoDS_Shape aPrismEllipse = BRepPrimAPI_MakePrism(aEllipseFace, gp_Vec(0.0, 0.0, 5.0));
    Handle(AIS_Shape) anAisPrismEllipse = new AIS_Shape(aPrismEllipse);

    anAisPrismVertex->SetColor(Quantity_NOC_PAPAYAWHIP);
    anAisPrismEdge->SetColor(Quantity_NOC_PEACHPUFF);
    anAisPrismCircle->SetColor(Quantity_NOC_PERU);
    anAisPrismEllipse->SetColor(Quantity_NOC_PINK);

    myOccView->getContext()->Display(anAisPrismVertex, Standard_True);
    myOccView->getContext()->Display(anAisPrismEdge, Standard_True);
    myOccView->getContext()->Display(anAisPrismCircle, Standard_True);
    myOccView->getContext()->Display(anAisPrismEllipse, Standard_True);
    myOccView->fitAll();
}

void occQt::makeRevol()
{
    gp_Ax1 anAxis;

    // revol a vertex result is an edge.
    anAxis.SetLocation(gp_Pnt(0.0, 70.0, 0.0));
    TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(gp_Pnt(2.0, 70.0, 0.0));
    TopoDS_Shape aRevolVertex = BRepPrimAPI_MakeRevol(aVertex, anAxis);
    Handle(AIS_Shape) anAisRevolVertex = new AIS_Shape(aRevolVertex);

    // revol an edge result is a face.
    anAxis.SetLocation(gp_Pnt(8.0, 70.0, 0.0));
    TopoDS_Edge anEdge = BRepBuilderAPI_MakeEdge(gp_Pnt(6.0, 70.0, 0.0), gp_Pnt(6.0, 70.0, 5.0));
    TopoDS_Shape aRevolEdge = BRepPrimAPI_MakeRevol(anEdge, anAxis);
    Handle(AIS_Shape) anAisRevolEdge = new AIS_Shape(aRevolEdge);

    // revol a wire result is a shell.
    anAxis.SetLocation(gp_Pnt(20.0, 70.0, 0.0));
    anAxis.SetDirection(gp::DY());

    TopoDS_Edge aCircleEdge = BRepBuilderAPI_MakeEdge(gp_Circ(gp_Ax2(gp_Pnt(15.0, 70.0, 0.0), gp::DZ()), 1.5));
    TopoDS_Wire aCircleWire = BRepBuilderAPI_MakeWire(aCircleEdge);
    TopoDS_Shape aRevolCircle = BRepPrimAPI_MakeRevol(aCircleWire, anAxis, M_PI_2);
    Handle(AIS_Shape) anAisRevolCircle = new AIS_Shape(aRevolCircle);

    // revol a face result is a solid.
    anAxis.SetLocation(gp_Pnt(30.0, 70.0, 0.0));
    anAxis.SetDirection(gp::DY());

    TopoDS_Edge aEllipseEdge = BRepBuilderAPI_MakeEdge(gp_Elips(gp_Ax2(gp_Pnt(25.0, 70.0, 0.0), gp::DZ()), 3.0, 2.0));
    TopoDS_Wire aEllipseWire = BRepBuilderAPI_MakeWire(aEllipseEdge);
    TopoDS_Face aEllipseFace = BRepBuilderAPI_MakeFace(gp_Pln(gp::XOY()), aEllipseWire);
    TopoDS_Shape aRevolEllipse = BRepPrimAPI_MakeRevol(aEllipseFace, anAxis, M_PI_4);
    Handle(AIS_Shape) anAisRevolEllipse = new AIS_Shape(aRevolEllipse);

    anAisRevolVertex->SetColor(Quantity_NOC_LIMEGREEN);
    anAisRevolEdge->SetColor(Quantity_NOC_LINEN);
    anAisRevolCircle->SetColor(Quantity_NOC_MAGENTA1);
    anAisRevolEllipse->SetColor(Quantity_NOC_MAROON);

    myOccView->getContext()->Display(anAisRevolVertex, Standard_True);
    myOccView->getContext()->Display(anAisRevolEdge, Standard_True);
    myOccView->getContext()->Display(anAisRevolCircle, Standard_True);
    myOccView->getContext()->Display(anAisRevolEllipse, Standard_True);
    myOccView->fitAll();
}

void occQt::makeLoft()
{
    // bottom wire.
    TopoDS_Edge aCircleEdge = BRepBuilderAPI_MakeEdge(gp_Circ(gp_Ax2(gp_Pnt(0.0, 80.0, 0.0), gp::DZ()), 1.5));
    TopoDS_Wire aCircleWire = BRepBuilderAPI_MakeWire(aCircleEdge);

    // top wire.
    BRepBuilderAPI_MakePolygon aPolygon;
    aPolygon.Add(gp_Pnt(-3.0, 77.0, 6.0));
    aPolygon.Add(gp_Pnt(3.0, 77.0, 6.0));
    aPolygon.Add(gp_Pnt(3.0, 83.0, 6.0));
    aPolygon.Add(gp_Pnt(-3.0, 83.0, 6.0));
    aPolygon.Close();

    BRepOffsetAPI_ThruSections aShellGenerator;
    BRepOffsetAPI_ThruSections aSolidGenerator(true);

    aShellGenerator.AddWire(aCircleWire);
    aShellGenerator.AddWire(aPolygon.Wire());

    aSolidGenerator.AddWire(aCircleWire);
    aSolidGenerator.AddWire(aPolygon.Wire());

    // translate the solid.
    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(18.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aSolidGenerator.Shape(), aTrsf);

    Handle(AIS_Shape) anAisShell = new AIS_Shape(aShellGenerator.Shape());
    Handle(AIS_Shape) anAisSolid = new AIS_Shape(aTransform.Shape());

    anAisShell->SetColor(Quantity_NOC_OLIVEDRAB);
    anAisSolid->SetColor(Quantity_NOC_PEACHPUFF);

    myOccView->getContext()->Display(anAisShell, Standard_True);
    myOccView->getContext()->Display(anAisSolid, Standard_True);
    myOccView->fitAll();
}

void occQt::testCut()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 90.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 2.5).Shape();
    TopoDS_Shape aCuttedShape1 = BRepAlgoAPI_Cut(aTopoBox, aTopoSphere);
    TopoDS_Shape aCuttedShape2 = BRepAlgoAPI_Cut(aTopoSphere, aTopoBox);

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(8.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform1(aCuttedShape1, aTrsf);

    aTrsf.SetTranslation(gp_Vec(16.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform2(aCuttedShape2, aTrsf);

    Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);
    Handle(AIS_Shape) anAisSphere = new AIS_Shape(aTopoSphere);
    Handle(AIS_Shape) anAisCuttedShape1 = new AIS_Shape(aTransform1.Shape());
    Handle(AIS_Shape) anAisCuttedShape2 = new AIS_Shape(aTransform2.Shape());

    anAisBox->SetColor(Quantity_NOC_SPRINGGREEN);
    anAisSphere->SetColor(Quantity_NOC_STEELBLUE);
    anAisCuttedShape1->SetColor(Quantity_NOC_TAN);
    anAisCuttedShape2->SetColor(Quantity_NOC_SALMON);

    myOccView->getContext()->Display(anAisBox, Standard_True);
    myOccView->getContext()->Display(anAisSphere, Standard_True);
    myOccView->getContext()->Display(anAisCuttedShape1, Standard_True);
    myOccView->getContext()->Display(anAisCuttedShape2, Standard_True);
    myOccView->fitAll();
}

void occQt::testFuse()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 100.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 2.5).Shape();
    TopoDS_Shape aFusedShape = BRepAlgoAPI_Fuse(aTopoBox, aTopoSphere);

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(8.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aFusedShape, aTrsf);

    Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);
    Handle(AIS_Shape) anAisSphere = new AIS_Shape(aTopoSphere);
    Handle(AIS_Shape) anAisFusedShape = new AIS_Shape(aTransform.Shape());

    anAisBox->SetColor(Quantity_NOC_SPRINGGREEN);
    anAisSphere->SetColor(Quantity_NOC_STEELBLUE);
    anAisFusedShape->SetColor(Quantity_NOC_ROSYBROWN);

    myOccView->getContext()->Display(anAisBox, Standard_True);
    myOccView->getContext()->Display(anAisSphere, Standard_True);
    myOccView->getContext()->Display(anAisFusedShape, Standard_True);
    myOccView->fitAll();
}

void occQt::testCommon()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 110.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 2.5).Shape();
    TopoDS_Shape aCommonShape = BRepAlgoAPI_Common(aTopoBox, aTopoSphere);

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(8.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aCommonShape, aTrsf);

    Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);
    Handle(AIS_Shape) anAisSphere = new AIS_Shape(aTopoSphere);
    Handle(AIS_Shape) anAisCommonShape = new AIS_Shape(aTransform.Shape());

    anAisBox->SetColor(Quantity_NOC_SPRINGGREEN);
    anAisSphere->SetColor(Quantity_NOC_STEELBLUE);
    anAisCommonShape->SetColor(Quantity_NOC_ROYALBLUE);

    myOccView->getContext()->Display(anAisBox, Standard_True);
    myOccView->getContext()->Display(anAisSphere, Standard_True);
    myOccView->getContext()->Display(anAisCommonShape, Standard_True);
    myOccView->fitAll();
}

void occQt::testHelix()
{
    makeCylindricalHelix();

    makeConicalHelix();

    makeToroidalHelix();
}

void occQt::viewBack()
{
    myOccView->getView()->SetProj(V3d_Ypos);
    myOccView->fitAll();
}

void occQt::viewBottom()
{
    myOccView->getView()->SetProj(V3d_Zneg);
    myOccView->fitAll();
}

void occQt::viewFront()
{
    myOccView->getView()->SetProj(V3d_Yneg);
    myOccView->fitAll();
}

void occQt::viewIso()
{
    myOccView->getView()->SetProj(V3d_XposYnegZpos);
    myOccView->fitAll();
}

void occQt::viewLeft()
{
    myOccView->getView()->SetProj(V3d_Xneg);
    myOccView->fitAll();
}

void occQt::viewRight()
{
    myOccView->getView()->SetProj(V3d_Xpos);
    myOccView->fitAll();
}

void occQt::viewTop()
{
    myOccView->getView()->SetProj(V3d_Zpos);
    myOccView->fitAll();
}

void occQt::shapeDelete()
{
    myOccView->getContext()->EraseSelected (Standard_False);
    myOccView->getContext()->ClearSelected (Standard_False);
    myOccView->getContext()->UpdateCurrentViewer();
}

void occQt::showWireFrame()
{
    myOccView->getContext()->SetDisplayMode(0, true);
}

void occQt::showShading()
{
    myOccView->getContext()->SetDisplayMode(1, true);
}

void occQt::makeCylindricalHelix()
{
    Standard_Real aRadius = 3.0;
    Standard_Real aPitch = 1.0;

    // the pcurve is a 2d line in the parametric space.
    gp_Lin2d aLine2d(gp_Pnt2d(0.0, 0.0), gp_Dir2d(aRadius, aPitch));

    Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(aLine2d, 0.0, M_PI * 2.0).Value();

    Handle(Geom_CylindricalSurface) aCylinder = new Geom_CylindricalSurface(gp::XOY(), aRadius);

    TopoDS_Edge aHelixEdge = BRepBuilderAPI_MakeEdge(aSegment, aCylinder, 0.0, 6.0 * M_PI).Edge();

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(0.0, 120.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aHelixEdge, aTrsf);

    Handle(AIS_Shape) anAisHelixCurve = new AIS_Shape(aTransform.Shape());

    myOccView->getContext()->Display(anAisHelixCurve, Standard_True);

    // sweep a circle profile along the helix curve.
    // there is no curve3d in the pcurve edge, so approx one.
    BRepLib::BuildCurve3d(aHelixEdge);

    gp_Ax2 anAxis;
    anAxis.SetDirection(gp_Dir(0.0, 4.0, 1.0));
    anAxis.SetLocation(gp_Pnt(aRadius, 0.0, 0.0));

    gp_Circ aProfileCircle(anAxis, 0.3);

    TopoDS_Edge aProfileEdge = BRepBuilderAPI_MakeEdge(aProfileCircle).Edge();
    TopoDS_Wire aProfileWire = BRepBuilderAPI_MakeWire(aProfileEdge).Wire();
    TopoDS_Face aProfileFace = BRepBuilderAPI_MakeFace(aProfileWire).Face();

    TopoDS_Wire aHelixWire = BRepBuilderAPI_MakeWire(aHelixEdge).Wire();

    BRepOffsetAPI_MakePipe aPipeMaker(aHelixWire, aProfileFace);

    if (aPipeMaker.IsDone())
    {
        aTrsf.SetTranslation(gp_Vec(8.0, 120.0, 0.0));
        BRepBuilderAPI_Transform aPipeTransform(aPipeMaker.Shape(), aTrsf);

        Handle(AIS_Shape) anAisPipe = new AIS_Shape(aPipeTransform.Shape());
        anAisPipe->SetColor(Quantity_NOC_CORAL);
        myOccView->getContext()->Display(anAisPipe, Standard_True);
        myOccView->fitAll();
    }
}

/**
 * make conical helix, it is the same as the cylindrical helix,
 * the only different is the surface.
 */
void occQt::makeConicalHelix()
{
    Standard_Real aRadius = 3.0;
    Standard_Real aPitch = 1.0;

    // the pcurve is a 2d line in the parametric space.
    gp_Lin2d aLine2d(gp_Pnt2d(0.0, 0.0), gp_Dir2d(aRadius, aPitch));

    Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(aLine2d, 0.0, M_PI * 2.0).Value();

    Handle(Geom_ConicalSurface) aCylinder = new Geom_ConicalSurface(gp::XOY(), M_PI / 6.0, aRadius);

    TopoDS_Edge aHelixEdge = BRepBuilderAPI_MakeEdge(aSegment, aCylinder, 0.0, 6.0 * M_PI).Edge();

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(18.0, 120.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aHelixEdge, aTrsf);

    Handle(AIS_Shape) anAisHelixCurve = new AIS_Shape(aTransform.Shape());

    myOccView->getContext()->Display(anAisHelixCurve, Standard_True);

    // sweep a circle profile along the helix curve.
    // there is no curve3d in the pcurve edge, so approx one.
    BRepLib::BuildCurve3d(aHelixEdge);

    gp_Ax2 anAxis;
    anAxis.SetDirection(gp_Dir(0.0, 4.0, 1.0));
    anAxis.SetLocation(gp_Pnt(aRadius, 0.0, 0.0));

    gp_Circ aProfileCircle(anAxis, 0.3);

    TopoDS_Edge aProfileEdge = BRepBuilderAPI_MakeEdge(aProfileCircle).Edge();
    TopoDS_Wire aProfileWire = BRepBuilderAPI_MakeWire(aProfileEdge).Wire();
    TopoDS_Face aProfileFace = BRepBuilderAPI_MakeFace(aProfileWire).Face();

    TopoDS_Wire aHelixWire = BRepBuilderAPI_MakeWire(aHelixEdge).Wire();

    BRepOffsetAPI_MakePipe aPipeMaker(aHelixWire, aProfileFace);

    if (aPipeMaker.IsDone())
    {
        aTrsf.SetTranslation(gp_Vec(28.0, 120.0, 0.0));
        BRepBuilderAPI_Transform aPipeTransform(aPipeMaker.Shape(), aTrsf);

        Handle(AIS_Shape) anAisPipe = new AIS_Shape(aPipeTransform.Shape());
        anAisPipe->SetColor(Quantity_NOC_DARKGOLDENROD);
        myOccView->getContext()->Display(anAisPipe, Standard_True);
        myOccView->fitAll();
    }
}

void occQt::makeToroidalHelix()
{
    Standard_Real aRadius = 1.0;
    Standard_Real aSlope = 0.05;

    // the pcurve is a 2d line in the parametric space.
    gp_Lin2d aLine2d(gp_Pnt2d(0.0, 0.0), gp_Dir2d(aSlope, 1.0));

    Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(aLine2d, 0.0, M_PI * 2.0).Value();

    Handle(Geom_ToroidalSurface) aCylinder = new Geom_ToroidalSurface(gp::XOY(), aRadius * 5.0, aRadius);

    TopoDS_Edge aHelixEdge = BRepBuilderAPI_MakeEdge(aSegment, aCylinder, 0.0, 2.0 * M_PI / aSlope).Edge();

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(45.0, 120.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aHelixEdge, aTrsf);

    Handle(AIS_Shape) anAisHelixCurve = new AIS_Shape(aTransform.Shape());

    myOccView->getContext()->Display(anAisHelixCurve, Standard_True);

    // sweep a circle profile along the helix curve.
    // there is no curve3d in the pcurve edge, so approx one.
    BRepLib::BuildCurve3d(aHelixEdge);

    gp_Ax2 anAxis;
    anAxis.SetDirection(gp_Dir(0.0, 0.0, 1.0));
    anAxis.SetLocation(gp_Pnt(aRadius * 6.0, 0.0, 0.0));

    gp_Circ aProfileCircle(anAxis, 0.3);

    TopoDS_Edge aProfileEdge = BRepBuilderAPI_MakeEdge(aProfileCircle).Edge();
    TopoDS_Wire aProfileWire = BRepBuilderAPI_MakeWire(aProfileEdge).Wire();
    TopoDS_Face aProfileFace = BRepBuilderAPI_MakeFace(aProfileWire).Face();

    TopoDS_Wire aHelixWire = BRepBuilderAPI_MakeWire(aHelixEdge).Wire();

    BRepOffsetAPI_MakePipe aPipeMaker(aHelixWire, aProfileFace);

    if (aPipeMaker.IsDone())
    {
        aTrsf.SetTranslation(gp_Vec(60.0, 120.0, 0.0));
        BRepBuilderAPI_Transform aPipeTransform(aPipeMaker.Shape(), aTrsf);

        Handle(AIS_Shape) anAisPipe = new AIS_Shape(aPipeTransform.Shape());
        anAisPipe->SetColor(Quantity_NOC_CORNSILK1);
        myOccView->getContext()->Display(anAisPipe, Standard_True);
        myOccView->fitAll();
    }
}

void occQt::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

void occQt::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    QString Filename = urls.first().toLocalFile();

    QFileInfo Info(Filename);
    dirPath = Info.path();
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string filename = code->fromUnicode(Filename).data();

    TopoDS_Shape aShape;

    //brep
    if (Info.suffix().toLower() == "brep")
    {
        BRep_Builder aBuilder;
        Standard_Boolean result = BRepTools::Read(aShape, filename.c_str(), aBuilder);
        if (result)
            theShape = aShape;
    }
    //dxf
    if (Info.suffix().toLower() == "dxf")
    {
        DxfReader aDxfReader(filename);
        aShape = aDxfReader.GetShape();
        theShape = aShape;
    }
    //igs
    if (Info.suffix().toLower() == "igs" || Info.suffix().toLower() == "iges")
    {
        IGESControl_Reader aReader_IGES;
        aReader_IGES.ReadFile(filename.c_str());
        aReader_IGES.PrintCheckLoad(Standard_False, IFSelect_ItemsByEntity);
        Standard_Integer NbRoots = aReader_IGES.NbRootsForTransfer();
        Standard_Integer num = aReader_IGES.TransferRoots();
        aShape = aReader_IGES.OneShape();
        theShape = aShape;
    }
    //stp
    if (Info.suffix().toLower() == "stp" || Info.suffix().toLower() == "step")
    {
        STEPControl_Reader aReader_Step;
        aReader_Step.ReadFile(filename.c_str());
        aReader_Step.PrintCheckLoad(Standard_False, IFSelect_ItemsByEntity);
        Standard_Integer NbRoots = aReader_Step.NbRootsForTransfer();
        Standard_Integer num = aReader_Step.TransferRoots();
        aShape = aReader_Step.OneShape();
        theShape = aShape;
    }
    //stl
    if (Info.suffix().toLower() == "stl")
    {
        StlAPI_Reader aReader_Stl;
        aReader_Stl.Read(aShape, filename.c_str());
        theShape = aShape;
    }

    Handle(AIS_Shape) anAisModel = new AIS_Shape(aShape);
    anAisModel->SetColor(Quantity_NOC_GRAY);
    anAisModel->SetTransparency(0);
    myOccView->getContext()->Display(anAisModel, Standard_True);
    myOccView->fitAll();
}
