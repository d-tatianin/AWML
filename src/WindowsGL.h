#include <stdexcept>

namespace awml {
    class glLoader
    {
    public:
        static bool Init();
        static bool LoadVersion(char major, char minor);
    private:
        static void* try_load(const char* func);
    };
}
