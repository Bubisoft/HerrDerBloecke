#include "Math.h"

using namespace SlimDX;

bool Math2D::LineIntersects(PointF p1, PointF p2, RectangleF r)
{
    return LineIntersects(p1, p2, PointF(r.X, r.Y), PointF(r.Right, r.Y)) ||
        LineIntersects(p1, p2, PointF(r.Right, r.Y), PointF(r.Right, r.Bottom)) ||
        LineIntersects(p1, p2, PointF(r.Right, r.Bottom), PointF(r.X, r.Bottom)) ||
        LineIntersects(p1, p2, PointF(r.X, r.Bottom), PointF(r.X, r.Y)) ||
        r.Contains(p1) && r.Contains(p2);
}

bool Math2D::LineIntersects(PointF l1p1, PointF l1p2, PointF l2p1, PointF l2p2)
{
    // See http://stackoverflow.com/questions/5514366/how-to-know-if-a-line-intersects-a-rectangle

    float q = (l1p1.Y - l2p1.Y) * (l2p2.X - l2p1.X) - (l1p1.X - l2p1.X) * (l2p2.Y - l2p1.Y);
    float d = (l1p2.X - l1p1.X) * (l2p2.Y - l2p1.Y) - (l1p2.Y - l1p1.Y) * (l2p2.X - l2p1.X);

    if (d == 0.f)
        return false;
    float r = q / d;

    q = (l1p1.Y - l2p1.Y) * (l1p2.X - l1p1.X) - (l1p1.X - l2p1.X) * (l1p2.Y - l1p1.Y);
    float s = q / d;

    if (r < 0.f || r > 1.f || s < 0.f || s > 1.f)
        return false;
    return true;
}

float Math2D::Distance(PointF p1, PointF p2) {
    return Vector2::Distance(Vector2(p1.X, p1.Y), Vector2(p2.X, p2.Y));
}
