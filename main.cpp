#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <lz4.h>

struct Header {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char modifed[12];
    char checksum[8];
    char type;
    char linkname[100];
};

int main(int argc, char *argv[]) {
    std::ifstream file("csc.tar.md5", std::ios::binary);
    std::vector<char> buffer(std::istreambuf_iterator<char>(file), {});
    std::vector<char>::iterator base = buffer.begin();
    std::vector<char> prism;
    file.close();

    while (base != buffer.end()) {
        Header *header = reinterpret_cast<Header *>(&*base);
        base += 512;
        int offset;
        try {
            offset = std::stoi(header->size, nullptr, 8);
        } catch (std::invalid_argument) {
            break;
        }
        if (std::string(header->name) == "prism.img.lz4") {
            prism = std::vector<char>(base, base + offset);
            break;
        }
        base += offset + 512 - offset % 512;
    }

    if (prism.empty()) {
        std::cout << "prism.img.lz4 not found" << std::endl;
        return 1;
    }

    std::ofstream output("prism.img.lz4", std::ios::binary);
    output.write(prism.data(), prism.size());
    output.close();

    return 0;
}