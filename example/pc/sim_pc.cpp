//
// Created by Ray Eldath on 2020/6/5 0005.
//
#include "PC.h"
#include "PC_pc_cmd.h"
#include "verilated.h"
#include "althas/althas.hpp"
#include <vector>
#include <string>

using namespace std;

struct PC_TestCase {
    char name[10];
    uint8_t reset;
    uint8_t cmd;
    uint64_t load_pc;
    uint64_t expected_pc;
};

class PC_TEST : public TESTER<PC, PC_TestCase> {
public:
    PC_TEST(const string &testname, const string &vcdname, const vector<PC_TestCase> &testcases) :
            TESTER(testname, vcdname, testcases) {}

    PC_TEST(const string &testname, const vector<PC_TestCase> &testcases) : TESTER(testname, testcases) {}

    void beforeAll() override {
        this->DUT->cmd = PC_pc_cmd::NONE;
        this->DUT->pc = 0;
    }

    void onEach(PC_TestCase testcase, TPRINTER *t) override {
        DUT->rst = testcase.reset;
        DUT->cmd = testcase.cmd;
        DUT->load_pc = testcase.load_pc;
    }

    void afterEach(PC_TestCase testcase, TPRINTER *t) override {
//        The following two sentences are using TPRINTF (Testing PRINTF) and TASSERTI (Testing ASSERT Indented)
//        to print example assertion and info on screen. There is a designated method TASSERT_INFO for such
//        purpose, i.e. print example assertion with related status, thus we recommend to use that method
//        as we demonstrated here.
//        Try to comment out TPRINTF+TASSERTI and TASSERT_INFO respectively and see what's different.
//
//        t->TPRINTF("[%-8s] cmd=%-5d reset=%-5d load_pc=%-5lu -> %-5lu (expected: %-5lu)\n",
//                   testcase.name, testcase.cmd, testcase.reset, testcase.load_pc, DUT->pc, testcase.expected_pc);
//        this->TASSERTI(DUT->pc, testcase.expected_pc);

        this->TASSERT_INFO(DUT->pc, testcase.expected_pc,
                           "[%-8s] cmd=%-5d reset=%-5d load_pc=%-5lu -> %-5lu (expected: %-5lu)",
                           testcase.name, testcase.cmd, testcase.reset,
                           testcase.load_pc, DUT->pc, testcase.expected_pc);
    }
};

vector<PC_TestCase> testcases = {
        PC_TestCase{"reset", 1, PC_pc_cmd::NONE, 0, 0},
        PC_TestCase{"inc", 0, PC_pc_cmd::cmd::INC, 0, 1},
        PC_TestCase{"none", 0, PC_pc_cmd::cmd::NONE, 0, 1},
//        next line is a example case that will leads to failure to show how failure will be printed out.
//        (expected_pc should be 1)
        PC_TestCase{"reset2", 1, PC_pc_cmd::cmd::NONE, 0, 1},
        PC_TestCase{"load", 0, PC_pc_cmd::cmd::LOAD, 0xFA, 0xFA},
        PC_TestCase{"inc", 0, PC_pc_cmd::cmd::INC, 0, 0xFB},
        PC_TestCase{"reset3", 1, PC_pc_cmd::cmd::NONE, 0, 0}
};

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    auto pc = new PC_TEST("PC", testcases);
    pc->run();
    delete pc;

    exit(EXIT_SUCCESS);
}