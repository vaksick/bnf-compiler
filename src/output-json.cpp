#include "output-json.hpp"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/reader.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <iostream>

namespace bnf::json {

    using namespace rapidjson;

    namespace {
        using json_writer = Writer<StringBuffer>;

        void to_json(json_writer &writer, const tree_ptr &tree);

        void to_json(json_writer &writer, const data_t &value) {
            if (is_null(value)) {
                writer.Key("tag:null");
                writer.Null();
            } else if (is_string(value)) {
                writer.Key("tag:string");
                writer.String(get_string(value).c_str());
            } else {
                to_json(writer, get_tree(value));
            }
        }

        void to_json(json_writer &writer, const tree_ptr &tree) {
            if (tree->is_data()) {
                auto value = tree->data();
                if (is_string(value)) {
                    writer.Key(tree->name.c_str());
                    writer.String(get_string(value).c_str());
                } else {
                    to_json(writer, value);
                }
            } else if (tree->is_list()) {
                writer.Key(tree->name.c_str());
                writer.StartObject();
                for (auto value : tree->list()) {
                    to_json(writer, value);
                }
                writer.EndObject();
            }
        }
    } // namespace

    void print(std::ostream &os, tree_ptr tree) {
        StringBuffer buffer;
        json_writer writer(buffer);
        writer.StartObject();
        if (tree && !tree->is_null()) {
            to_json(writer, tree);
        }
        writer.EndObject();
        os << buffer.GetString() << std::endl;
    }
} // namespace bnf::json