#pragma once

using namespace System::Drawing;

/** This contains a few math helper functions. */

namespace Math2D {
    bool LineIntersects(PointF p1, PointF p2, RectangleF r);
    bool LineIntersects(PointF l1p1, PointF l1p2, PointF l2p1, PointF l2p2);
    float Distance(PointF p1, PointF p2);
}
