//
// Created by Ray Eldath on 2020/6/7 0007.
//
#include <vector>
#include "AddOne.h"
#include "verilated.h"

#include "althas/config.h" // redefine config macros as demonstrated here

#undef CLOCK_WIRE
#define CLOCK_WIRE clock
#undef RESET_WIRE
#define RESET_WIRE reset

#include "althas/althas.hpp"

struct AddOne_TestCase {
    char name[10];
    uint8_t reset;
    uint8_t cmd;
    uint64_t expected_value;
};

class AddOne_TESTER : public TESTER<AddOne, AddOne_TestCase> {
public:
    AddOne_TESTER(const std::string &testname, const std::string &vcdname,
                  const std::vector<AddOne_TestCase> &testcases) :
            TESTER(testname, vcdname, testcases) {}

    AddOne_TESTER(const std::string &testname, const std::vector<AddOne_TestCase> &testcases) :
            TESTER(testname, testcases) {}

private:
    void beforeAll() override {
        this->reset();
    }

    void onEach(AddOne_TestCase testcase, TESTING *t) override {
        this->DUT->cmd = testcase.cmd;
        this->DUT->reset = testcase.reset;
    }

    void afterEach(AddOne_TestCase testcase, TESTING *t) override {
        TASSERT_INFO(DUT->value, testcase.expected_value, "[%-8s] cmd=%-5d reset=%-5d -> %-5hu (expected: %hu)",
                     testcase.name, testcase.cmd, testcase.reset, DUT->value, testcase.expected_value);
    }
};

std::vector<AddOne_TestCase> testcases = {
        AddOne_TestCase{"addone1", 0, 1, 1},
        AddOne_TestCase{"addone2", 0, 1, 2},
        AddOne_TestCase{"addone3", 0, 1, 2}, // will fail here to show how a failure be printed
        AddOne_TestCase{"reset", 1, 0, 0}
};

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    auto addone = new AddOne_TESTER("AddOne", testcases);
    addone->run();
    delete addone;

    exit(EXIT_SUCCESS);
}