#include "bnf-scanner.hpp"
#include "input-file.hpp"
#include "output-json.hpp"
#include "parser-by-bnf.hpp"
#include "parser.hpp"
#include <getopt.h>
#include <iostream>
#include <output-xml.hpp>
#include <sstream>

enum format_t : int { XML, JSON };

format_t format = format_t::XML;
int pretty_out = false;

static struct option long_options[] = { //
    {"format-out-xml", no_argument, (int *)&format, format_t::XML},
    {"format-out-json", no_argument, (int *)&format, format_t::JSON},
    {"pretty", no_argument, &pretty_out, true},
    {"bnf", required_argument, nullptr, 'b'},
    {"output", required_argument, nullptr, 'o'},
    {"help", required_argument, nullptr, 'h'},
    {0, 0, 0, 0}};

void help_print() {
    fprintf(stderr, "bnf-compiller --bnf/-b file.bnf input [...input]\n");
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

int main(int argc, char **argv) {
    std::string bnfFileName; // = "simple/debug.bnf";
    std::string input;       // = "debug.txt";
    std::string outFileName;
    int opt, option_index;
    while ((opt = getopt_long(argc, argv, "b:i:", long_options, &option_index)) != -1) {
        switch (opt) {
        case 'b':
            bnfFileName = optarg;
            break;
        case 'o':
            outFileName = optarg;
            break;
        case 'h':
            help_print();
            break;
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
#ifdef PRINT_BNF
        for (auto item : bnf) {
            fmt::print("{} ::= ", item.first);
            std::cout << item.second << ";" << std::endl;
        }
#endif
        auto tree = bnf::scan_by_bnf(utils::open(input), bnf);

        std::stringstream ss;
        switch (format) {
        case format_t::XML:
            bnf::xml::print(ss, tree);
            break;
        case format_t::JSON:
            bnf::json::print(ss, tree);
            break;
        default:
            break;
        }
        std::cout << ss.str() << std::endl;
    } catch (const std::exception &e) {
        fprintf(stderr, "%s\n", e.what());
        return 1;
    }
    return 0;
}