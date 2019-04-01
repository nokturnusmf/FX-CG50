#include <iostream>
#include <fstream>
#include <memory>
#include <cstdint>

using uint24_t = unsigned char[3];

#pragma pack(1)
struct BMPHeader {
    BMPHeader() {}
    BMPHeader(int x, int y, int colour_depth_bytes = 3) : x(x), y(y) {
        offset = sizeof(BMPHeader);
        pixels_size = x * y * colour_depth_bytes;
        size = pixels_size + offset;
        depth = colour_depth_bytes * 8;
    }

    // header
    uint16_t id = 0x4D42;
    uint32_t size;
    uint16_t reserved0 = 0;
    uint16_t reserved1 = 0;
    uint32_t offset;
    // bitmapinfoheader
    uint32_t bitmapinfoheader_size = 40;
    int32_t x;
    int32_t y;
    uint16_t planes = 1;
    uint16_t depth;
    uint32_t compression = 0;
    uint32_t pixels_size;
    int32_t horizontal_res = 2835;
    int32_t vertical_res = 2835;
    uint32_t palette = 0;
    uint32_t important = 0;
};

inline uint16_t swap_endianness(uint16_t v) {
    return v << 8 | v >> 8;
}

void convert_16_24(uint16_t v, uint24_t& res) {
    if (v == 65535) {
        res[0] = 255;
        res[1] = 255;
        res[2] = 255;
    } else {
        res[0] = 8 * (v & 0x1F);
        res[1] = 4 * ((v & 0x7FF) >> 5);
        res[2] = 8 * (v >> 11);
    }
}

void process(uint16_t* input, uint24_t* output, int n) {
    for (int i = 0; i < n; ++i) {
        convert_16_24(swap_endianness(input[i]), output[i]);
    }
}

void write_bmp(const char* filename, const BMPHeader& header, const std::unique_ptr<uint24_t>& pixels) {
    std::ofstream file(filename, std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(&header), sizeof(BMPHeader));
        for (int i = 0; i < header.y; ++i) {
            file.write(reinterpret_cast<const char*>(&*pixels + (header.y - 1 - i) * header.x), header.x * 3);
        }
    }
}

void extract(std::ifstream& input, size_t offset, const char* filename) {
    static std::unique_ptr<uint16_t> data(new uint16_t[92 * 64]);
    static std::unique_ptr<uint24_t> pixels(new uint24_t[92 * 64]);

    input.seekg(offset, std::ios::beg);
    input.read(reinterpret_cast<char*>(&*data), 0x2E00);

    process(&*data, &*pixels, 92 * 64);

    write_bmp(filename, BMPHeader(92, 64), pixels);
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <input> <unselected name> <selected name>\n";
        return 1;
    }

    std::ifstream input(argv[1], std::ios::binary);
    if (!input) {
        std::cerr << "Couldn't open input for reading\n";
        return 2;
    }

    extract(input, 0x1000, argv[2]);
    extract(input, 0x4000, argv[3]);
}
