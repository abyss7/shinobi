// NOTICE: This file should be included only inside .cc files after all other
//         includes.

#pragma once

#undef FATAL

#undef ERROR
#undef WARNING
#undef INFO
#undef VERBOSE
#undef TRACE

using namespace shinobi::named_levels;

#define LOG(level) shinobi::Log(level)
#if defined(NDEBUG)
#define DLOG(level) std::stringstream()
#else
#define DLOG(level) LOG(level)
#endif
