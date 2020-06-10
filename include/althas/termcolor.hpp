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
    char buf[5];
    strcpy(code, "\033[");

    if (fc != ForegroundColor::UNSPECIFIED) {
        sprintf(buf, "%d", (int) fc);
        strcat(strcat(code, ";"), buf);
    }
    if (bc != BackgroundColor::UNSPECIFIED) {
        sprintf(buf, "%d", (int) bc);
        strcat(strcat(code, ";"), buf);
    }
    if (e != Effect::UNSPECIFIED) {
        sprintf(buf, "%d", (int) e);
        strcat(strcat(code, ";"), buf);
    }

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

inline const char *lrpad(const char *text) {
    static char buf[50];
    strcpy(buf, " ");
    strcat(buf, text);
    strcat(buf, " ");
    return buf;
}

#endif //ALTHAS_TERMCOLOR_HPP
