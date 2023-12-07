#pragma once

#include <vector>
using std::vector;

namespace trace {
extern vector<int> starts;

void Push();
int Pop();
};  // namespace trace
