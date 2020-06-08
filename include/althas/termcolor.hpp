//
// Created by Ray Eldath on 2020/6/9 0009.
//

#ifndef ALTHAS_TERMCOLOR_HPP
#define ALTHAS_TERMCOLOR_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

typedef enum class ForegroundColor {
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37,
    UNSPECIFIED = 0,
} ForegroundColor;

typedef enum class BackgroundColor {
    BLACK = 40,
    RED = 41,
    GREEN = 42,
    YELLOW = 43,
    BLUE = 44,
    MAGENTA = 45,
    CYAN = 46,
    WHITE = 47,
    UNSPECIFIED = 0,
} BackgroundColor;

typedef enum class Effect {
    BOLD = 1,
    ITALIC = 3,
    UNDERLINE = 4,
    STRIKETHROUGH = 9,
    UNSPECIFIED = 0,
} Effect;

const auto RESET_COLOR = "\033[0m";

inline void resetColor() {
    printf("%s", RESET_COLOR);
}

const char *color(ForegroundColor fc = ForegroundColor::UNSPECIFIED,
                  BackgroundColor bc = BackgroundColor::UNSPECIFIED,
                  Effect e = Effect::UNSPECIFIED) {
    if (fc == ForegroundColor::UNSPECIFIED && bc == BackgroundColor::UNSPECIFIED && e == Effect::UNSPECIFIED)
        return RESET_COLOR;

    static char code[15];
    strcpy(code, "\033[");

    if (fc != ForegroundColor::UNSPECIFIED)
        sprintf(code, "%s;%d", code, (int) fc);
    if (bc != BackgroundColor::UNSPECIFIED)
        sprintf(code, "%s;%d", code, (int) bc);
    if (e != Effect::UNSPECIFIED)
        sprintf(code, "%s;%d", code, (int) e);

    strcat(code, "m");
    return code;
}

inline void setColor(ForegroundColor fc = ForegroundColor::UNSPECIFIED,
                     BackgroundColor bc = BackgroundColor::UNSPECIFIED,
                     Effect e = Effect::UNSPECIFIED) {
    printf("%s", color(fc, bc, e));
}

const char *colorize(const char *text,
                     ForegroundColor fc = ForegroundColor::UNSPECIFIED,
                     BackgroundColor bc = BackgroundColor::UNSPECIFIED,
                     Effect e = Effect::UNSPECIFIED) {
    static char buf[50];
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s%s%s", color(fc, bc, e), text, RESET_COLOR);
    return buf;
}

#endif //ALTHAS_TERMCOLOR_HPP