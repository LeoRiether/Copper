#include "Trace.h"

#include "SDL_timer.h"
#include "util.h"

#define MODULE "Trace"

namespace trace {

struct Trace {
    int start;
    const char* id;
    int th;
};

vector<Trace> ongoing;

void Push(const char* id, int th = 5) {
    ongoing.push_back(Trace{(int)SDL_GetTicks(), id, th});
}

int Pop() {
    int now = SDL_GetTicks();
    auto t = ongoing.back();

    if (now - t.start > t.th) {
        warn2("%s took %dms", t.id, now - t.start);
    }

    ongoing.pop_back();
    return now - t.start;
}

};  // namespace trace
