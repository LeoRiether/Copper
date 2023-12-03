#include "math/CohenSutherland.h"

#define MODULE "CohenSutherland"

///////////////////////////
//        Credits        //
/////////////////////////////////////////////////////////////////////////////////////
//                           copied and modified from
//        https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
/////////////////////////////////////////////////////////////////////////////////////

namespace cohen_sutherland {

OutCode ComputeOutCode(double x, double y, Rect r) {
    OutCode code = INSIDE;  // initialised as being inside of clip window

    if (x < r.x)  // to the left of clip window
        code |= LEFT;
    else if (x > r.x + r.w)  // to the right of clip window
        code |= RIGHT;
    if (y < r.y)  // below the clip window
        code |= BOTTOM;
    else if (y > r.y + r.h)  // above the clip window
        code |= TOP;

    return code;
}

bool LineClip(Vec2<Cart> p, Vec2<Cart> q, Rect r) {
    // compute outcodes for P0, P1, and whatever point lies outside the clip
    // rectangle
    OutCode outcode0 = ComputeOutCode(p.x, p.y, r);
    OutCode outcode1 = ComputeOutCode(q.x, q.y, r);
    bool accept = false;

    while (true) {
        if (!(outcode0 | outcode1)) {
            // bitwise OR is 0: both points inside window; trivially accept
            // and exit loop
            accept = true;
            break;
        } else if (outcode0 & outcode1) {
            // bitwise AND is not 0: both points share an outside zone
            // (LEFT, RIGHT, TOP, or BOTTOM), so both must be outside
            // window; exit loop (accept is false)
            break;
        } else {
            // failed both tests, so calculate the line segment to clip
            // from an outside point to an intersection with clip edge
            double x, y;

            // At least one endpoint is outside the clip rectangle; pick it.
            OutCode outcodeOut = outcode1 > outcode0 ? outcode1 : outcode0;

            // Now find the intersection point;
            // use formulas:
            //   slope = (q.y - p.y) / (q.x - p.x)
            //   x = p.x + (1 / slope) * (ym - p.y), where ym is r.y or (r.y
            //   + r.h) y = p.y + slope * (xm - p.x), where xm is r.x or
            //   (r.x + r.w)
            // No need to worry about divide-by-zero because, in each case,
            // the outcode bit being tested guarantees the denominator is
            // non-zero
            if (outcodeOut & TOP) {  // point is above the clip window
                x = p.x + (q.x - p.x) * ((r.y + r.h) - p.y) / (q.y - p.y);
                y = (r.y + r.h);
            } else if (outcodeOut & BOTTOM) {  // point is below the clip window
                x = p.x + (q.x - p.x) * (r.y - p.y) / (q.y - p.y);
                y = r.y;
            } else if (outcodeOut &
                       RIGHT) {  // point is to the right of clip window
                y = p.y + (q.y - p.y) * ((r.x + r.w) - p.x) / (q.x - p.x);
                x = (r.x + r.w);
            } else if (outcodeOut &
                       LEFT) {  // point is to the left of clip window
                y = p.y + (q.y - p.y) * (r.x - p.x) / (q.x - p.x);
                x = r.x;
            }

            // Now we move outside point to intersection point to clip
            // and get ready for next pass.
            if (outcodeOut == outcode0) {
                p.x = x;
                p.y = y;
                outcode0 = ComputeOutCode(p.x, p.y, r);
            } else {
                q.x = x;
                q.y = y;
                outcode1 = ComputeOutCode(q.x, q.y, r);
            }
        }
    }
    return accept;
}
};  // namespace cohen_sutherland
