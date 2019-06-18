#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>

struct Options {
    std::string type;
    int grouping;
    int newline;
    bool big_endian;
};

#define IS_OPT(opt) (str.compare(0, strlen(opt), opt, strlen(opt)) == 0)

bool verify_args(int argc, char** argv) {
    int file_count = 0;
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'h') {
                std::cout << "Usage: " << argv[0] << " [options...] <infile1> [options...] (infile...) <outfile>\n"
                            "Options:\n"
                            "  -t  Specify type (and grouping)\n"
                            "  -w  Specify number of values per line\n"
                            "  -e  Specify endianness\n"
                            "  -h  Show help\n"
                            "Options are applied to all following input files\n";
                return false;
            }
        } else {
            ++file_count;
        }
    }

    if (file_count < 2) {
        std::cerr << "Usage: " << argv[0] << " [options...] <infile1> [options...] (infile...) <outfile>\n";
        return false;
    }

    return true;
}

bool parse_option(const std::string& str, Options* opts) {
    if (IS_OPT("-t")) {
        opts->type = str.substr(2);
        if (opts->type == "char") {
            opts->grouping = 1;
        } else if (opts->type == "short") {
            opts->grouping = 2;
        } else if (opts->type == "int") {
            opts->grouping = 4;
        } else if (opts->type == "long") {
            opts->grouping = 8;
        }
        return true;
    } else if (IS_OPT("-w")) {
        opts->newline = std::strtol(str.substr(2).c_str(), 0, 10);
        return true;
    } else if (IS_OPT("-e")) {
        opts->big_endian = str[2] == 'b';
        return true;
    }

    return false;
}

std::vector<unsigned char> read(std::ifstream& file) {
    file.seekg(0, std::ios::end);
    auto size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> result;
    result.resize(size);
    file.read(reinterpret_cast<char*>(&result[0]), size);

    return result;
}

void output(std::ofstream& file, const std::string& name, const std::vector<unsigned char>& data, const Options& opts) {
    file << "extern const unsigned " << opts.type << ' ' << name << "[] = {";

    int j = 0;
    for (int i = 0; i < data.size(); i += opts.grouping) {
        if (j++ % opts.newline == 0) {
            file << "\n\t";
        }

        file << "0x";
        for (int k = 0; k < opts.grouping; ++k) {
            file << std::setfill('0') << std::setw(2) << static_cast<int>(data[i + (opts.big_endian ? k : opts.grouping - k - 1)]);
        }
        file << ", ";
    }

    file << "\n};\n\n";
}

int main(int argc, char** argv) {
    if (!verify_args(argc, argv)) {
        return 1;
    }

    Options opts { "char", 1, 8 };

    std::ofstream outfile(argv[argc - 1]);
    if (!outfile) {
        std::cerr << "Couldn't open outfile\n";
        return 2;
    }
    outfile.flags(std::ios::hex);

    for (int i = 1; i < argc - 1; ++i) {
        if (parse_option(argv[i], &opts)) {
            continue;
        }

        std::ifstream infile(argv[i], std::ios::binary);
        if (!infile) {
            std::cerr << "Couldn't open infile " << argv[i] << '\n';
            return 3;
        }

        auto data = read(infile);

        std::string name(argv[i]);
        auto start = name.find_last_of('/') + 1;
        auto end = name.find_last_of('.');
        output(outfile, name.substr(start, end - start), data, opts);
    }
}
