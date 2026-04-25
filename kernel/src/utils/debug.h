#ifndef DEBUG
#define DEBUG


typedef enum {
    typeINT,
    typeString,
    typeBOOL
} Type;

void _kprintf_impl(Type type, unsigned *args);

// some helper for kprintf
#define kprintf_int(x) do { int _v = (x); _kprintf_impl(typeINT, (unsigned*)&_v); } while(0)
#define kprintf_str(x) do { const char *_v = (x); _kprintf_impl(typeString, (unsigned*)&_v); } while(0)
#define kprintf_bool(x) do { unsigned char _v = (x); _kprintf_impl(typeBOOL, (unsigned*)&_v); } while(0)

#define kprintf(type, x) do { typeof(x) _v = (x); _kprintf_impl(type, (unsigned*)&_v); } while(0)

#endif