#pragma once

#include <cinttypes>
#include <iostream>
#include <limits>
#include <optional>

struct Arguments {
    char* input_file        = nullptr;
    char* output_directory  = nullptr;
    uint64_t max_iterations = std::numeric_limits<uint64_t>::max();
    uint64_t frequency      = 0;
};

std::optional<Arguments> ParseArguments(int argc, char** argv);
