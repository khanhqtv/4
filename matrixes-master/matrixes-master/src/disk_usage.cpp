#include <sys/statvfs.h>

#define MPATH "./" 
namespace DiskUsage {
 long long GetAvailableSpace(const char* path){
    struct statvfs stat;
    if (statvfs(MPATH, &stat) != 0) {
      // error happens, just quits here
      return -1;
    }
    return stat.f_bsize * stat.f_bavail;
  }
 

 bool is_disk_space_available(string path){
    long long space = GetAvailableSpace(path.c_str());
    if ( space < 512000) {
      // error happens, just quits here
      std::cout << std::endl;
      std::cerr << "ERROR: failed to get available space" << std::endl;
      return false;
    }
    return true;
  } 
}
