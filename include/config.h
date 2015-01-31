
#if defined(WIN32)
# ifndef NDEBUG
#  pragma comment(linker, "/SUBSYSTEM:CONSOLE")
# else
#  pragma comment(linker, "/SUBSYSTEM:WINDOWS /NODEFAULTLIB:msvcrt.lib")
#  define WIN32_LEAN_AND_MEAN
# endif
#endif

