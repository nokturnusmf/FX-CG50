#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

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

unsigned short convert(unsigned char r, unsigned char g, unsigned char b) {
    unsigned short val = 0;

    val |= r >= 248 ? 31 : (r + 4) >> 3;
    val <<= 6;

    val |= g >= 252 ? 63 : (g + 2) >> 2;
    val <<= 5;

    val |= b >= 248 ? 31 : (b + 4) >> 3;

    return val;
}

void output(std::ofstream& file, const std::string& name, Image& img) {
    file << "extern const unsigned short " << name.c_str() << "[] = {";

    int k = 0;
    for (int i = (img.h - 1) * img.w * 3; i >= 0; i -= img.w * 3) {
        for (int j = 0; j < img.w * 3; j += 3) {
            if (k++ % 8 == 0) {
                file << "\n\t";
            }

            file << "0x" << std::setfill('0') << std::setw(4) << convert(img.pixels[i + j + 2], img.pixels[i + j + 1], img.pixels[i + j]) << ", ";
        }
    }

    file << "\n};\n\n";
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <infile1> (infile...) <outfile>\n";
        return 1;
    }

    std::ofstream outfile(argv[argc - 1]);
    if (!outfile) {
        std::cerr << "Couldn't open outfile\n";
        return 2;
    }
    outfile.flags(std::ios::hex);

    for (int i = 1; i < argc - 1; ++i) {
        std::ifstream infile(argv[i], std::ios::binary);
        if (!infile) {
            std::cerr << "Couldn't open infile " << argv[i] << '\n';
            outfile.close();
            return 3;
        }

        auto img = read(infile);
        infile.close();

        std::string name(argv[i]);
        auto start = name.find_last_of('/') + 1;
        auto end = name.find(".bmp");
        output(outfile, name.substr(start, end - start), img);

        delete[] img.pixels;
    }

    outfile.close();
}
