#include <iostream>
#include <fstream>

struct Image {
    unsigned char* pixels;
    int h;
    int w;
};

Image read(std::ifstream& file) {
    int skip;
    file.seekg(10, std::ios::beg);
    file.read((char*)&skip, 4);

    int h, w;
    file.seekg(18, std::ios::beg);
    file.read((char*)&w, 4);
    file.read((char*)&h, 4);
    int size = h * w * 3;

    file.seekg(skip, std::ios::beg);

    unsigned char* data = new unsigned char[size];
    file.read((char*)data, size);

    return { data, h, w };
}

inline uint16_t swap_endianness(uint16_t v) {
    return v << 8 | v >> 8;
}

unsigned short convert(unsigned char r, unsigned char g, unsigned char b) {
    unsigned short val = 0;

    val |= r >= 248 ? 31 : (r + 4) >> 3;
    val <<= 6;

    val |= g >= 252 ? 63 : (g + 2) >> 2;
    val <<= 5;

    val |= b >= 248 ? 31 : (b + 4) >> 3;

    return swap_endianness(val);
}

unsigned short* process(const Image& img) {
    auto result = new unsigned short[img.w * img.h];
    int k = 0;
    #pragma omp parallel for
    for (int i = img.h - 1; i >= 0; --i) {
        for (int j = 0; j < img.w; ++j) {
            result[k++] = convert(img.pixels[3 * (i * img.w + j) + 2], img.pixels[3 * (i * img.w + j) + 1], img.pixels[3 * (i * img.w + j)]);
        }
    }
    return result;
}

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

    auto img = read(infile);
    auto pixels = process(img);

    std::ofstream outfile(argv[2], std::ios::binary);
    if (!outfile) {
        std::cerr << "Couldn't open outfile\n";
        return 3;
    }

    outfile.write(reinterpret_cast<const char*>(pixels), img.w * img.h * 2);

    delete[] img.pixels;
    delete[] pixels;
}
