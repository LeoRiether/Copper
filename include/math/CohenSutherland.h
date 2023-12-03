#pragma once

#include "math/Rect.h"

///////////////////////////
//        Credits        //
////////////////////////////////////////////////////////////////////////////////////
//                           copied and modified from
//        https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
////////////////////////////////////////////////////////////////////////////////////

namespace cohen_sutherland {

typedef int OutCode;
constexpr static int INSIDE = 0;  // 0000
constexpr static int LEFT = 1;    // 0001
constexpr static int RIGHT = 2;   // 0010
constexpr static int BOTTOM = 4;  // 0100
constexpr static int TOP = 8;     // 1000

// Compute the bit code for a point (x, y) using the clip rectangle
// bounded diagonally by (xmin, ymin), and (xmax, ymax)
// ASSUME THAT xmax, xmin, ymax and ymin are global constants.
OutCode ComputeOutCode(double x, double y, Rect r);

// Cohen–Sutherland clipping algorithm clips a line from
// P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with
// diagonal from (xmin, ymin) to (xmax, ymax).
bool LineClip(Vec2<Cart> p, Vec2<Cart> q, Rect r);

};  // namespace cohen_sutherland
