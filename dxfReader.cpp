#include "dxfReader.h"

#include <gp_Circ.hxx>
#include <gp_Elips.hxx>

#include <GC_MakeSegment.hxx>
#include <GC_MakeCircle.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeEllipse.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>

DxfReader::DxfReader(const std::string& fileName) : m_Dxf(new DL_Dxf())
{
    m_Builder.MakeCompound(m_Shape);

    m_Dxf->in(fileName, this);  
}

DxfReader::~DxfReader(void)
{
}

const TopoDS_Shape& DxfReader::GetShape() const
{
    return m_Shape;
}

void DxfReader::addPoint(const DL_PointData& point)
{
    m_Builder.Add(m_Shape, BRepBuilderAPI_MakeVertex(gp_Pnt(point.x, point.y, point.z)));
}

void DxfReader::addLine(const DL_LineData& line)
{
    Handle_Geom_Curve theSegment = GC_MakeSegment(gp_Pnt(line.x1, line.y1, line.z1), gp_Pnt(line.x2, line.y2, line.z2)).Value();

    m_Builder.Add(m_Shape, BRepBuilderAPI_MakeEdge(theSegment));
}

void DxfReader::addArc(const DL_ArcData &arc)
{
    gp_Circ theCircle;
    theCircle.SetRadius(arc.radius);
    theCircle.SetLocation(gp_Pnt(arc.cx, arc.cy, arc.cz));

    Handle_Geom_Curve theArc = GC_MakeArcOfCircle(theCircle, (arc.angle1 * M_PI / 180), (arc.angle2 * M_PI / 180), false).Value();

    m_Builder.Add(m_Shape, BRepBuilderAPI_MakeEdge(theArc));
}

void DxfReader::addCircle(const DL_CircleData& circle)
{
    gp_Circ aCircle;
    aCircle.SetRadius(circle.radius);
    aCircle.SetLocation(gp_Pnt(circle.cx, circle.cy, circle.cz));

    Handle_Geom_Curve theCircle = GC_MakeCircle(aCircle).Value();

    m_Builder.Add(m_Shape, BRepBuilderAPI_MakeEdge(theCircle));
}

void DxfReader::addEllipse(const DL_EllipseData& ellipse)
{
}

void DxfReader::addPolyline(const DL_PolylineData& polyline)
{
}

void DxfReader::addVertex(const DL_VertexData& vertex)
{
    m_Builder.Add(m_Shape, BRepBuilderAPI_MakeVertex(gp_Pnt(vertex.x, vertex.y, vertex.z)));
}

void DxfReader::addSpline(const DL_SplineData& spline)
{
}

void DxfReader::addKnot(const DL_KnotData& knot)
{
}

void DxfReader::addControlPoint(const DL_ControlPointData& cp)
{
}

void DxfReader::add3dFace(const DL_3dFaceData& face)
{
    BRepBuilderAPI_MakePolygon polygon(
        gp_Pnt(face.x[0], face.y[0], face.z[0]), 
        gp_Pnt(face.x[1], face.y[1], face.z[1]),
        gp_Pnt(face.x[2], face.y[2], face.z[2]), 
        gp_Pnt(face.x[3], face.y[3], face.z[3]), true);

    BRepBuilderAPI_MakeFace makeFace(polygon.Wire());

    if (makeFace.IsDone())
    {
        m_Builder.Add(m_Shape, makeFace.Face());
    }
}

void DxfReader::addSolid(const DL_SolidData &solid)
{
    add3dFace(solid);
}
