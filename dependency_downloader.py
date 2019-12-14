import sys
import os
import urllib.request

def error_exit(why):
   print("-- Failed to install OpenGL dependencies! Reason: {}".format(why))
   sys.exit(1)

if len(sys.argv) != 2:
    print("Usage: python dependency_downloader.py <project-include-path> \
         \nPlease note that this script is meant to be executed with CMake and not manually!")
    sys.exit(1)
if not os.path.isdir(sys.argv[1]):
    error_exit("no such directory: {}".format(sys.argv[1]))
try:
    print("-- Downloading OpenGL dependencies...")
    gl_path = sys.argv[1] + "/GL"
    khr_path = sys.argv[1] + "/KHR"
    os.mkdir(gl_path)
    os.mkdir(khr_path)
    urllib.request.urlretrieve("https://www.khronos.org/registry/OpenGL/api/GL/glcorearb.h", gl_path + "/glcorearb.h")
    urllib.request.urlretrieve("https://www.khronos.org/registry/OpenGL/api/GL/wglext.h", gl_path + "/wglext.h")
    urllib.request.urlretrieve("https://www.khronos.org/registry/EGL/api/KHR/khrplatform.h", khr_path + "/khrplatform.h")
except Exception as ex:
    error_exit(ex)    

print("-- Successfully downloaded OpenGL dependencies.")
sys.exit(0)
