#include <iostream>
#include <fstream>
#include <iomanip>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <infile> <outfile>\n";
        return 1;
    }

    std::ifstream infile(argv[1], std::ios::binary);
    if (!infile) {
        std::cerr << "Couldn't open infile\n";
        return 2;
    }

    std::ofstream outfile(argv[2]);
    if (!outfile) {
        std::cerr << "Couldn't open outfile\n";
    }
    outfile.flags(std::ios::hex);

    int skip;
    infile.seekg(10, std::ios::beg);
    infile.read((char*)&skip, 4);

    int h, w;
    infile.seekg(18, std::ios::beg);
    infile.read((char*)&w, 4);
    infile.read((char*)&h, 4);
    int size = h * w * 3;

    infile.seekg(skip, std::ios::beg);

    unsigned char* data = new unsigned char[size];
    infile.read((char*)data, size);
    infile.close();

    int k = 0;
    for (int i = (h - 1) * w * 3; i >= 0; i -= w * 3) {
        for (int j = 0; j < w * 3; j += 3) {
            unsigned short val = 0;

            auto b = data[i + j + 2];
            val |= b >= 248 ? 31 : (b + 4) >> 3;
            val <<= 6;

            b = data[i + j + 1];
            val |= b >= 252 ? 63 : (b + 2) >> 2;
            val <<= 5;

            b = data[i + j];
            val |= b >= 248 ? 31 : (b + 4) >> 3;

            outfile << "0x" << std::setfill('0') << std::setw(4) << val << ", ";

            if (++k % 8 == 0) {
                outfile << '\n';
            }
        }
    }
    outfile.close();

    delete[] data;
}
