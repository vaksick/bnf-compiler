/// Copyright (c) 2020 Viktor Lazarev
//! @version 0.1
//! @author vaksick@gmail.com

#include "bnf-scanner.hpp"
#include "input-file.hpp"
#include "output-json.hpp"
#include "parser-by-bnf.hpp"
#include "parser.hpp"
#include <getopt.h>
#include <iostream>
#include <output-xml.hpp>
#include <sstream>
#include <fstream>

enum format_t : int { XML, JSON };

format_t format = format_t::XML;
int pretty_out = false;
int print_bnf = false;

static struct option long_options[] = { //
    {"format-out-xml", no_argument, (int *)&format, format_t::XML},
    {"format-out-json", no_argument, (int *)&format, format_t::JSON},
    {"pretty", no_argument, &pretty_out, true},
    {"print-bnf", no_argument, &print_bnf, true},
    {"bnf", required_argument, nullptr, 'b'},
    {"output", required_argument, nullptr, 'o'},
    {"help", required_argument, nullptr, 'h'},
    {0, 0, 0, 0}};

void help_print() {
    fprintf(stderr, "bnfc --bnf/-b file.bnf [options] input\n");
    fprintf(stderr, "options\n");
    fprintf(stderr, "    --output/-o          :Output the results to a file. Default output to tty.\n");
    fprintf(stderr, "    --print-bnf          :Print parsing bnf to tty.\n");
    fprintf(stderr, "    --pretty             :Output the result in a pretty format.\n");
    fprintf(stderr, "    --format-out-xml     :Output the result in a xml format.[default]\n");
    fprintf(stderr, "    --format-out-json    :Output the result in a json format.\n");
    exit(0);
}

std::ostream &operator<<(std::ostream &os, bnf::rule_ptr rule) {
    bool is_first = true;
    if (rule) {
        switch (rule->type) {
        case bnf::STR:
            os << rule->str();
            break;
        case bnf::RULE:
            os << "<" << rule->str() << ">";
            break;
        case bnf::GROUP:
            os << "(";
            for (auto node : rule->list()) {
                os << node << " ";
            }
            os << ")";
            break;
        case bnf::CHOICE:
            os << "(";
            for (auto node : rule->list()) {
                if (!is_first)
                    os << " | ";
                os << node;
                is_first = false;
            }
            os << ")";
            break;
        default:
            throw std::runtime_error("unknown rule type");
        }
        switch (rule->variable) {
        case bnf::variable_t::LIST:
            os << "*";
            break;
        case bnf::variable_t::OPT:
            os << "?";
            break;
        case bnf::variable_t::ONE_OR_MORE:
            os << "+";
            break;
        default:
            break;
        }
    }
    return os;
}

void print(std::ostream &os, bnf::tree_ptr tree) {
    switch (format) {
    case format_t::XML:
        bnf::xml::print(os, tree);
        break;
    case format_t::JSON:
        bnf::json::print(os, tree);
        break;
    default:
        break;
    }
}

int main(int argc, char **argv) {
    std::string bnfFileName; // = "simple/debug.bnf";
    std::string input;       // = "debug.txt";
    std::string outFileName;
    int opt, option_index;
    while ((opt = getopt_long(argc, argv, "b:o:h", long_options, &option_index)) != -1) {
        switch (opt) {
        case 'b':
            bnfFileName = optarg;
            break;
        case 'o':
            outFileName = optarg;
            break;
        case 'h':
            help_print();
            return 0;
        default:
            break;
        }
    }

    for (int idx = optind; idx < argc; idx++) {
        input = argv[idx];
    }

    if (bnfFileName.empty()) {
        fprintf(stderr, "bnf file not specified\n");
        help_print();
    }

    try {
        scanner::parser parser(utils::open(bnfFileName));
        auto bnf = bnf::scan_bnf(parser);
        bnf::check_bnf(bnf);

        if (print_bnf) {
            for (auto item : bnf) {
                fmt::print("{} ::= ", item.first);
                std::cout << item.second << ";" << std::endl;
            }
            return 0;
        }

        auto tree = bnf::scan_by_bnf(utils::open(input), bnf);

        if (outFileName.empty()) {
            print(std::cout, tree);
            std::cout << std::endl;
        } else {
            std::ofstream file(outFileName);
            if(file.bad())
                throw std::runtime_error(fmt::format("[print] error create file '{}'", outFileName));
            print(file, tree);
            file << std::endl;
        }
    } catch (const std::exception &e) {
        fprintf(stderr, "%s\n", e.what());
        return 1;
    }
    return 0;
}