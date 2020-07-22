//
// Created by laoqin on 2020/7/22.
//

#include "QRandom.h"

int getRandNum(int min, int max) {
    srand(time(NULL));
    return (min + rand() % (max - min + 1));
}