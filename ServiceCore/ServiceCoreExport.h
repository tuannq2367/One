#pragma once

#ifdef SERVICE_CORE_IMPL
#	ifndef ServiceCore_Class
#		define ServiceCore_Class  __declspec(dllexport)
#	endif
#else
#	ifndef ServiceCore_Class
#		define ServiceCore_Class  __declspec(dllimport)
#	endif
#endif