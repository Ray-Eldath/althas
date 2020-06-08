//
// Created by Ray Eldath on 2020/6/9 0009.
//

#include <cstdio>
#include "althas/termcolor.hpp"

int main() {
    setColor(ForegroundColor::BLUE);
    printf("blue\n");
    printf("blue\n");

    setColor(ForegroundColor::BLACK, BackgroundColor::RED, Effect::UNDERLINE);
    printf(" blue black underline");
    resetColor();
    printf("\n");

    printf("%s\n", colorize("[  OK  ]", ForegroundColor::GREEN));
    printf("%s\n", colorize("[ FAIL ]", ForegroundColor::BLACK, BackgroundColor::RED, Effect::BOLD));
}