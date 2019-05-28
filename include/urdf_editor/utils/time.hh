//
// Created by Denys Kotelovych on 22.04.19.
//

#ifndef URDF_EDITOR_TIME_HH
#define URDF_EDITOR_TIME_HH

#include <chrono>

namespace urdf_editor {
namespace utils {

inline std::int32_t TimeNowMilliseconds() {
    using namespace std::chrono;
    const auto now = system_clock::now().time_since_epoch();
    const auto ms = duration_cast<milliseconds>(now);
    return static_cast<std::int32_t>(ms.count());
}

}
}

#endif //URDF_EDITOR_TIME_HH
