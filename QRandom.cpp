//
// Created by laoqin on 2020/7/22.
//

#include "QRandom.h"

int getRandNum(int min, int max) {
    static bool init_ = false;
    if (!init_) {
        srand(time(NULL));
	init_ = true;
    }
    return (min + rand() % (max - min + 1));
}
