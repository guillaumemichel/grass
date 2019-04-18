#include "../../include/File.h"

using namespace std;

size_t File::fileSize() const {
    // File size
    streampos fsize = 0;
    ifstream file(filename, ios::binary);

    fsize = file.tellg();
    file.seekg(0, ios::end);
    fsize = file.tellg() - fsize;
    file.close();

    return (size_t) fsize;
}
