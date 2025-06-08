#include "parser.h"

bool CheckArgument(
        const char* arg, const char* str1,
        const char* str2, size_t str_size) {
    return strncmp(arg, str1, str_size) == 0
        || strncmp(arg, str2, str_size) == 0;
}

bool ValidateNumber(const char* str) {
    size_t str_size = strlen(str);
    while (str[0] != '\0') {
        if (!isdigit(str[0])) {
            return false;
        }
        ++str;
    }
    return str_size != 0;
}

std::optional<Arguments> ParseArguments(int argc, char** argv) {
    Arguments args;
    for (int i = 1; i < argc; ++i) {
        size_t arg_size = strlen(argv[i]);

        if (CheckArgument(argv[i], "-i", "--input", arg_size)) {
            args.input_file = argv[i + 1];
            ++i;
        } else if (CheckArgument(argv[i], "-o", "--output", arg_size)) {
            args.output_directory = argv[i + 1];
            ++i;
        } else if (CheckArgument(argv[i], "-m", "--max-iter", arg_size)) {
            char* num = argv[i + 1];
            if (ValidateNumber(num)) {
                args.max_iterations = std::atoi(num);
                ++i;
                continue;
            }
            std::cerr << "Invalid max_iter value\n";
            return std::nullopt;
        } else if (CheckArgument(argv[i], "-f", "--freq", arg_size)) {
            char* num = argv[i + 1];
            if (ValidateNumber(num)) {
                args.frequency = std::atoi(num);
                ++i;
                continue;
            }
            std::cerr << "Invalid frequency value\n";
            return std::nullopt;
        } else {
            std::cerr << "Unknown argument: " << argv[i] << '\n';
            return std::nullopt;
        }
    }

    if (args.input_file == nullptr) {
        std::cerr << "Enter the input file name\n";
        return std::nullopt;
    }
    if (args.output_directory == nullptr) {
        std::cerr << "Enter the output directory name\n";
        return std::nullopt;
    }

    return args;
}
