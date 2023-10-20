#pragma once

#include <memory>
#include <vector>

#include "GameObject.h"

using std::shared_ptr;
using std::vector;

// Honestly, this is not a class, this is a function
class CollisionEngine {
   public:
    static void Solve(const vector<shared_ptr<GameObject>>& objects);
};
