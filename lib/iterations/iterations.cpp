#include "iterations.h"

bool NeedToContinue(Arguments args, SandPile& sand_pile, size_t i) {
    return (i <= args.max_iterations) && !sand_pile.is_stable();
}

char* ConvertIntToStr(size_t number) {
    const size_t BUFFER_SIZE = 18;
    char* buffer = new char[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "%zu", number);
    return buffer;
}

void CopyStr(char* source, size_t size, const char* suffix) {
    for (size_t i = 0; i + 1 < size && *suffix != '\0'; ++source, ++suffix) {
        *source = *suffix;
    }
    *source = '\0';
}

void ConcatStr(char* source, size_t size, const char* suffix) {
    char* end = source;
    for (;end < source + size && *end != '\0'; ++end);
    for (; *suffix != '\0'; ++end, ++suffix) {
        *end = *suffix;
    }
    *end = '\0';
}

char* MakePath(size_t i, const char* output_directory) {
    size_t FILE_PATH_SIZE = 128;
    char* full_path = new char[FILE_PATH_SIZE];
    CopyStr(full_path, FILE_PATH_SIZE, output_directory);
    ConcatStr(full_path, FILE_PATH_SIZE, "/img");

    char* num = ConvertIntToStr(i);
    ConcatStr(full_path, FILE_PATH_SIZE, num);
    ConcatStr(full_path, FILE_PATH_SIZE, ".bmp");
    delete[] num;
    return full_path;
}

void Iterate(Arguments args) {
    SandPile sand_pile;
    if (!sand_pile.ReadFromFile(args.input_file))
        return;

    for (size_t i = 1; NeedToContinue(args, sand_pile, i); ++i) {
        if (args.frequency != 0 && i % args.frequency == 0) {
            char* full_path =
                MakePath(i / args.frequency, args.output_directory);
            CreateBmp(sand_pile, full_path);
            delete[] full_path;
        }
        sand_pile.Update();
    }

    if (args.frequency == 0) {
        char* full_path = MakePath(1, args.output_directory);
        CreateBmp(sand_pile, full_path);
        delete[] full_path;
    }
}
