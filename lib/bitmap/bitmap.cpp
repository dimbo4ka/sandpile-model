#include "bitmap.h"

#include <fstream>

static const uint8_t kFileHeaderSize = 14;
static const uint8_t kInfoHeaderSize = 40;
static const uint8_t kPaletteSize = 20;

static const uint8_t kPlanesNumber = 1;
static const uint8_t kBitsPerPixel = 4;
static const uint8_t kColorsNumber = 5;

void AddValueToHeader(uint8_t* header, size_t i, uint32_t value) {
    header[i] = value & 0xFF;
    header[i + 1] = (value >> 8) & 0xFF;
    header[i + 2] = (value >> 16) & 0xFF;
    header[i + 3] = (value >> 24) & 0xFF;
}

bool CreateBmp(SandPile& sand_pile, const char* output) {
    int32_t bmp_width = (sand_pile.width() + 1) / 2;
    uint8_t padding_bytes = (4 - bmp_width % 4) % 4;
    uint32_t bmp_size =
        kFileHeaderSize + kInfoHeaderSize + kPaletteSize +   
            sand_pile.height() * (bmp_width + padding_bytes);

    uint8_t file_header[kFileHeaderSize] = {
        'B', 'M',
        0, 0, 0, 0,
        0, 0,
        0, 0,
        0, 0, 0, 0
    };
    file_header[10] = kFileHeaderSize + kInfoHeaderSize + kPaletteSize;
    AddValueToHeader(file_header, 2, bmp_size);

    uint8_t info_header[kInfoHeaderSize] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0,
        0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    info_header[0] = kInfoHeaderSize;
    info_header[12] = kPlanesNumber;
    info_header[14] = kBitsPerPixel;
    info_header[32] = kColorsNumber;
    AddValueToHeader(info_header, 4, sand_pile.width());
    AddValueToHeader(info_header, 8, sand_pile.height());

    uint8_t palette[kPaletteSize] = {
        255, 255, 255, 0,
        0, 255, 0, 0,
        128, 0, 128, 0,
        0, 255, 255, 0,
        0, 0, 0, 0
    };
    
    std::ofstream file;
    file.open(output, std::ios::out | std::ios::binary);
    
    if (!file.is_open()) {
        std::cerr << "File opening error\n";
        return false;
    }

    file.write(reinterpret_cast<char*>(file_header), kFileHeaderSize);
    file.write(reinterpret_cast<char*>(info_header), kInfoHeaderSize);
    file.write(reinterpret_cast<char*>(palette), kPaletteSize);

    for (size_t y = sand_pile.height() - 1; y >= 0; --y) {
        for (size_t x = 0; x < sand_pile.width(); x += 2) {
            uint8_t prev_color =
                static_cast<uint8_t>(sand_pile.sand_grains()[x][y]);
            uint8_t next_color = 0;
            if (x + 1 != sand_pile.width()) {
                next_color =
                    static_cast<uint8_t>(sand_pile.sand_grains()[x + 1][y]);
            }
            prev_color = (prev_color > 3) ? 4 : prev_color;
            next_color = (next_color > 3) ? 4 : next_color;

            uint8_t color_byte = (prev_color << 4) | next_color;
            file.write(reinterpret_cast<char*>(&color_byte), 1);
        }
        uint8_t buffer = 0; 
        for (size_t i = 0; i < padding_bytes; ++i) {
            file.write(reinterpret_cast<char*>(&buffer), 1);
        }
        
        if (y == 0)
            break;
    }
    return true;
}
