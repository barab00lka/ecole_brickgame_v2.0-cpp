#pragma once

#ifdef DEBUG
#define LOG_DEBUG(f) f
void debug_print_ctx();
#else
#define LOG_DEBUG(f) ((void)0)
#endif
