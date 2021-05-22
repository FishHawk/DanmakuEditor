#pragma once

#include "../components/Launchable.hpp"

namespace repetition {

Launchable::RepetitionGenerator at(Time time_point, size_t n = 1);
Launchable::RepetitionGenerator at_start(size_t n = 1);
Launchable::RepetitionGenerator at_end(size_t n = 1);
Launchable::RepetitionGenerator every_n(Time period, size_t n = 1);

} // namespace repetition
