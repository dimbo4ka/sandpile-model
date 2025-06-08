#pragma once

#include <iostream>
#include <limits>

class SandPile {
public:
    bool InitPile(const char* filename);
    bool ReadFromFile(const char* filename);
    void UpdateGrain(uint64_t** new_pile, size_t x, size_t y, uint64_t value);
    void Update();

    bool is_stable() const;
    int32_t width() const;
    int32_t height() const;
    uint64_t** sand_grains() const;

private:
    uint32_t height_ = 0;
    uint32_t width_  = 0;

    int16_t x_max_ = std::numeric_limits<int16_t>::min();
    int16_t y_max_ = std::numeric_limits<int16_t>::min();
    int16_t x_min_ = std::numeric_limits<int16_t>::max();
    int16_t y_min_ = std::numeric_limits<int16_t>::max();

    uint64_t** sand_grains_ = nullptr;

    bool is_stable_ = false;
};
