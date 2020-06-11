#ifndef ALTHAS_HPP
#define ALTHAS_HPP

#include "termcolor.hpp"

// include config macros
#ifndef ALTHAS_CONFIG_H

#include "config.h"

#define ALTHAS_CONFIG_H
#endif //ALTHAS_CONFIG_H
//

#include <cstdio>
#include <cstdint>
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include <verilated_vcd_c.h>

#define TBASSERT(TB, A) \
    do { \
        if (!(A)) { \
            (TB)->stop_trace(); \
        } \
    assert(A); \
    } while(0)

class TPRINTER {
private:
    const std::string testname;
    int indent;

public:
    explicit TPRINTER(std::string testname) : testname(std::move(testname)), indent(0) {}

    inline void printIndent() const {
        for (int i = 0; i < indent; i++)
            VL_PRINTF("\t");
    }

    inline void increaseIndent() { indent += 1; };

    inline void decreaseIndent() { indent -= 1; };

    inline void TPRINTF(const char *content) const {
        printIndent();
        printf("%s", content);
    }

    template<class ...Args>
    inline void TPRINTF(const char *format, Args... args) const {
        printIndent();
        printf(format, args...);
    }

    inline void TPRINTFI(const char *content) {
        increaseIndent();
        TPRINTF(content);
        decreaseIndent();
    }

    template<class ...Args>
    inline void TPRINTFI(const char *format, Args... args) {
        increaseIndent();
        TPRINTF(format, args...);
        decreaseIndent();
    }
};

template<class DEVICE>
class TESTBENCH {
private:
    const bool d_trace_on;
    const std::string d_testname;
    VerilatedVcdC *d_trace;
    vluint64_t d_tickcount;
protected:
    DEVICE *DUT;
public:
    explicit TESTBENCH(std::string testname, const bool trace_on = true) :
            d_testname(std::move(testname)), d_trace(nullptr), d_tickcount(0l), d_trace_on(trace_on) {

        DUT = new DEVICE;
        if (trace_on)
            Verilated::traceEverOn(true);
#ifdef RESET_LINE_ENABLED
        DUT->RESET_WIRE = 0;
#endif
    }

    ~TESTBENCH() {
        stop_trace();
        DUT->final();
        delete DUT;
        DUT = NULL;
    }

    void start_trace(const char *vcdname) {
        if (d_trace_on == 0)
            return;
        if (!d_trace) {
            d_trace = new VerilatedVcdC;
            DUT->trace(d_trace, 99);
            d_trace->open(vcdname);
        }
    }

    void stop_trace() {
        if (d_trace_on == 0)
            return;
        if (d_trace) {
            d_trace->close();
            delete d_trace;
            d_trace = nullptr;
        }
    }

    inline virtual void eval() {
        DUT->eval();
    }

    virtual void tick() {
        d_tickcount++;
#ifdef CLOCK_LINE_ENABLED
        DUT->CLOCK_WIRE = 0;
#endif
        eval();
        if (d_trace) d_trace->dump(10 * d_tickcount - 2);

#ifdef CLOCK_LINE_ENABLED
        DUT->CLOCK_WIRE = 1;
#endif
        eval();
        if (d_trace) d_trace->dump(10 * d_tickcount);

#ifdef CLOCK_LINE_ENABLED
        DUT->CLOCK_WIRE = 0;
#endif
        eval();
        if (d_trace) {
            d_trace->dump(10 * d_tickcount + 5);
            d_trace->flush();
        }
    }

#ifdef RESET_LINE_ENABLED

    void reset() {
        DUT->RESET_WIRE = 1;
        tick();
        DUT->RESET_WIRE = 0;
        // printf("RESET\n");
    }

#endif

    inline unsigned long tickcount() {
        return d_tickcount;
    }

    inline std::string name() {
        return d_testname;
    }
};


template<class DEVICE, class TESTCASE>
class TESTER : protected TESTBENCH<DEVICE> {
private:
    typedef TESTBENCH<DEVICE> SUPER;
    const std::string d_vcdname;

    template<bool VALUE_FORMAT_ENABLED = false, class VALUE, class ...Args>
    void TASSERT_(const char *value_f, VALUE actual, VALUE expected, const char *message_f, Args &... args) {
        if (!(actual == expected)) {
//            [ FAIL ]  expected: XXX   actual: XXX
//
//            [ FAIL ]  [none    ] cmd=0     reset=0     load_pc=0     -> 1     (expected: 1    )
//                expected: XXX   actual: XXX

            d_printer->TPRINTF(colorize("[ FAIL ]", ForegroundColor::BLACK, BackgroundColor::RED, Effect::BOLD));
            resetColor();
            printf("  ");

            setColor(ForegroundColor::RED);

            if constexpr(0 != sizeof...(args)) {
                printf(message_f, args...);
                printf("\n");
                d_printer->increaseIndent();
                d_printer->printIndent();
                d_printer->decreaseIndent();
            }

            if constexpr(VALUE_FORMAT_ENABLED)
                printExpectedActual<true>(value_f, actual, expected);
            else
                printExpectedActual<false>("", actual, expected);

            resetColor();
#ifdef FAIL_ASSERTION
            this->stop_trace();
#endif
        } else {
            if constexpr(0 == sizeof...(args)) {
                d_printer->TPRINTF(colorize("[  OK  ]", ForegroundColor::GREEN));
            } else {
                char info_format[150];
                sprintf(info_format, "%s[  OK  ]%s  %s\n", color(ForegroundColor::GREEN), RESET_COLOR, message_f);
                d_printer->TPRINTF(info_format, args...);
            }
        }
#ifdef FAIL_ASSERTION
        assert(actual == expected);
#endif
    }

    template<bool VALUE_FORMAT_ENABLED = false, class VALUE>
    inline void printExpectedActual(const char *value_f, VALUE actual, VALUE expected) {
        if constexpr(VALUE_FORMAT_ENABLED) {
            const char *message_ff = value_f;
            if (strncmp(value_f, "%", 1) == 0) // if starts with %
                message_ff = value_f + 1; // substring
            char format[100];
            sprintf(format,
                    "expected: %%%s actual: %%%s\n", message_ff, message_ff); // e.g. expected: %10d actual: %10d

            printf(format, expected, actual);
        } else
            std::cout << "expected: " << std::setiosflags(std::ios::left) << std::setw(10)
                      << expected << "actual: " << actual << std::endl;
    }

protected:
    const std::vector<TESTCASE> d_testcases;
    TPRINTER *d_printer;
public:
    template<class VALUE, class ...Args>
    inline void TASSERT_INFO(VALUE actual, VALUE expected, const char *message_f, Args &... args) {
        TASSERT_<false>("", actual, expected, message_f, args...);
    }

    template<class VALUE, class ...Args>
    inline void
    TASSERTF_INFO(const char *value_f, VALUE actual, VALUE expected, const char *message_f, Args &... args) {
        TASSERT_<true>(value_f, actual, expected, message_f, args...);
    }

    template<class VALUE>
    inline void TASSERT(VALUE actual, VALUE expected) {
        this->TASSERT_<false>("", actual, expected, "");
    }

    template<class VALUE>
    inline void TASSERTF(const char *value_f, VALUE actual, VALUE expected) {
        this->TASSERT_<true>(value_f, actual, expected, "");
    }

    template<class VALUE>
    inline void TASSERTI(VALUE actual, VALUE expected) {
        d_printer->increaseIndent();
        TASSERT(actual, expected);
        d_printer->decreaseIndent();
    }

    template<class VALUE>
    inline void TASSERTIF(const char *value_f, VALUE actual, VALUE expected) {
        d_printer->increaseIndent();
        TASSERTF(value_f, actual, expected);
        d_printer->decreaseIndent();
    }

    TESTER(const std::string &testname, std::string vcdname, const std::vector<TESTCASE> &testcases)
            : SUPER(testname), d_vcdname(std::move(vcdname)), d_testcases(testcases) {
        d_printer = new TPRINTER(testname);
    }

    TESTER(const std::string &testname, const std::vector<TESTCASE> &testcases)
            : TESTER(testname, testname + ".vcd", testcases) {}

    ~TESTER() { delete d_printer; }

    virtual void beforeTraceStarts() {};

    virtual void beforeAll() {};

    virtual void afterAll() {};

    virtual void afterTraceEnds() {};

    virtual void beforeEach(TESTCASE testcase, TPRINTER *t) {};

    virtual void onEach(TESTCASE testcase, TPRINTER *t) = 0;

    virtual void afterEach(TESTCASE testcase, TPRINTER *t) {};

    void run() {
        setColor(ForegroundColor::UNSPECIFIED, BackgroundColor::UNSPECIFIED, Effect::BOLD);
        std::cout << "[ now test " << SUPER::name() << " with " << d_testcases.size() << " test case(s) ]"
                  << std::endl;
        resetColor();
        beforeTraceStarts();

        d_printer->increaseIndent();
        this->start_trace(d_vcdname.c_str());

        beforeAll();
        for (auto testcase: d_testcases) {
            beforeEach(testcase, d_printer);
            onEach(testcase, d_printer);
            this->tick();
            afterEach(testcase, d_printer);
        }
        afterAll();

        this->stop_trace();
        d_printer->decreaseIndent();

        afterTraceEnds();

        setColor(ForegroundColor::UNSPECIFIED, BackgroundColor::UNSPECIFIED, Effect::BOLD);
        std::cout << "[ test " << SUPER::name() << " finished ]" << std::endl;
        resetColor();
    }

    TPRINTER *TP() { return d_printer; }
};

#endif //ALTHAS_HPP
