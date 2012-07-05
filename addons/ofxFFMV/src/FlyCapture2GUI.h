//=============================================================================
// A dummy header to switch between Windows and Linux
//=============================================================================

#ifdef TARGET_WIN32
#error "TODO MIGRATION: add Windows FlyCapture2"
#else
#include "flycapture-linux/FlyCapture2GUI.h"
#endif
