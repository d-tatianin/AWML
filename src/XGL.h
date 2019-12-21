#include <stdexcept>

typedef GLXContext(*GLXCREATECONTEXTATTRIBSARBPROC)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

extern  GLXCREATECONTEXTATTRIBSARBPROC awml_glXCreateContextAttribsARB;
#define glXCreateContextAttribsARB     awml_glXCreateContextAttribsARB

namespace awml {
    class glLoader
    {
    public:
        static bool Init();
        static bool LoadVersion(char major, char minor);
    private:
        static void* try_load(const char* func);
        static bool valid(void* func_ptr);
    };
}
