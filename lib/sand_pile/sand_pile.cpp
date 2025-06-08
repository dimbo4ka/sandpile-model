#include <fstream>

#include "sand_pile.h"

bool SandPile::ReadFromFile(const char* filename) {
    if (!InitPile(filename)) 
        return false;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "File reading error\n";
        return false;
    }
    
    int16_t x;
    int16_t y;
    int64_t grain_count;
    while (file >> x >> y >> grain_count) {
        sand_grains_[x - x_min_][y - y_min_] += grain_count;
    }
    return true;
}

bool SandPile::InitPile(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "File reading error\n";
        return false;
    }

    int16_t x;
    int16_t y;
    int64_t grain_count;
    while (file >> x >> y >> grain_count) {
        x_max_ = std::max(x_max_, x);
        y_max_ = std::max(y_max_, y);
        x_min_ = std::min(x_min_, x);
        y_min_ = std::min(y_min_, y);
    }

    width_ = x_max_ - x_min_ + 1;
    height_ = y_max_ - y_min_ + 1;

    sand_grains_ = new uint64_t*[width_];
    for (size_t x = 0; x < width_; ++x) {
        sand_grains_[x] = new uint64_t[height_];
        for (size_t y = 0; y < height_; ++y) {
            sand_grains_[x][y] = 0;
        }
    }
    return true;
}

void SandPile::UpdateGrain(
        uint64_t** new_pile, size_t x, size_t y, uint64_t value) {
    if (value < 4) {
        new_pile[x][y] += value;
        is_stable_ &= (new_pile[x][y] < 4);
        return;
    }

    new_pile[x][y] += value - 4;
    is_stable_ &= (new_pile[x][y] < 4);

    new_pile[x - 1][y]++;
    is_stable_ &= (new_pile[x - 1][y] < 4);

    new_pile[x][y + 1]++;
    is_stable_ &= (new_pile[x][y + 1] < 4);

    new_pile[x + 1][y]++;
    is_stable_ &= (new_pile[x + 1][y] < 4);

    new_pile[x][y - 1]++;
    is_stable_ &= (new_pile[x][y - 1] < 4);
}

void SandPile::Update() {
    bool need_shift_to_left = false;
    bool need_shift_to_right = false;
    bool need_shift_down = false;
    bool need_shift_up = false;
    
    for (size_t x = 0; x < width_; ++x) {
        if (sand_grains_[x][0] >= 4 && !need_shift_down) {
            y_min_--;
            need_shift_down = true;
        }
        if (sand_grains_[x][height_ - 1] >= 4 && !need_shift_up) {
            y_max_++;
            need_shift_up = true;
        }
    }
    for (size_t y = 0; y < height_; ++y) {
        if (sand_grains_[0][y] >= 4 && !need_shift_to_left) {
            x_min_--;
            need_shift_to_left = true;
        }
        if (sand_grains_[width_ - 1][y] >= 4 && !need_shift_to_right) {
            x_max_++;
            need_shift_to_right = true;
        }
    }

    uint64_t** new_pile;
    int32_t new_height = y_max_ - y_min_ + 1;
    int32_t new_width = x_max_ - x_min_ + 1;
    
    new_pile = new uint64_t*[new_width];
    for (size_t new_x = 0; new_x < new_width; ++new_x) {
        new_pile[new_x] = new uint64_t[new_height];
        for (size_t new_y = 0; new_y < new_height; ++new_y) {
            new_pile[new_x][new_y] = 0;
        }
    }

    SandPile new_sp;
    new_sp.sand_grains_ = new_pile;
    new_sp.height_ = new_height;
    new_sp.width_ = new_width;
    
    is_stable_ = true;
    for (size_t x = 0; x < width_; ++x) {
        for (size_t y = 0; y < height_; ++y) {
            UpdateGrain(
                new_pile, 
                x + need_shift_to_left, y + need_shift_down, 
                sand_grains_[x][y]);
        }
    }
    std::swap(sand_grains_, new_pile);

    for (size_t x = 0; x < width_; ++x) {
        delete[] new_pile[x];
    }
    delete[] new_pile;

    width_ = new_width;
    height_ = new_height;
}

bool SandPile::is_stable() const {
    return is_stable_;
}

int32_t SandPile::width() const {
    return width_;
}   
int32_t SandPile::height() const {
    return height_;
}

uint64_t** SandPile::sand_grains() const {
    return sand_grains_;
}
