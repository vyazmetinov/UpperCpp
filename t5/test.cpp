#include "log.h"

#include <algorithm>
#include <cassert>
#include <sstream>
#include <string>

int main() {
    Log* log = Log::Instance();
    for (int i = 0; i < 12; ++i) {
        log->message(i % 3 == 0 ? LOG_ERROR : (i % 2 == 0 ? LOG_WARNING : LOG_NORMAL), "event_" + std::to_string(i));
    }

    std::ostringstream out;
    log->print(out);
    const std::string printed = out.str();

    assert(printed.find("event_0") == std::string::npos);
    assert(printed.find("event_2") != std::string::npos);
    assert(printed.find("event_11") != std::string::npos);
    assert(static_cast<int>(std::count(printed.begin(), printed.end(), '\n')) == 10);
    return 0;
}
