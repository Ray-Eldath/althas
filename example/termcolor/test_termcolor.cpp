//
// Created by Ray Eldath on 2020/6/9 0009.
//

#include <cstdio>
#include "althas/termcolor.hpp"

int main() {
    setColor(ForegroundColor::BLUE);
    printf("blue\n");

    printf("%s\n", colorize(lrpad("blue"), ForegroundColor::BLACK, BackgroundColor::BLUE));
    printf("%s\n", colorize(lrpad("yellow"), ForegroundColor::BLACK, BackgroundColor::YELLOW));
    setColor(ForegroundColor::BLACK, BackgroundColor::RED, Effect::UNDERLINE);
    printf(" blue black underline ");
    resetColor();
    printf("\n");

    printf("%s\n", colorize("[  OK  ]", ForegroundColor::GREEN));
    printf("%s\n", colorize("[ FAIL ]", ForegroundColor::BLACK, BackgroundColor::RED, Effect::BOLD));
}