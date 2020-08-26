/// Copyright (c) 2020 Viktor Lazarev 
//! @version 0.1
//! @author vaksick@gmail.com 

#include "output-xml.hpp"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

extern int pretty_out;

namespace bnf::xml {
    using namespace rapidxml;

    namespace {
        using document = xml_document<>;

        xml_node<> *allocate(document &doc, const std::string &name) {
            auto key = doc.allocate_string(name.c_str());
            return doc.allocate_node(node_element, key);
        }

        xml_node<> *allocate(document &doc, const std::string &name, const std::string &value) {
            auto string = doc.allocate_string(value.c_str());
            auto key = doc.allocate_string(name.c_str());
            return doc.allocate_node(node_element, key, string);
        }

        void add_child(document &doc, xml_node<> &root, const tree_ptr &tree);

        void set_value(document &doc, xml_node<> &root, const data_t &value) {
            if (is_null(value)) {
                root.append_node(allocate(doc, "tag:null"));
            } else if (is_string(value)) {
                root.append_node(allocate(doc, "tag:string", get_string(value)));
            } else {
                add_child(doc, root, get_tree(value));
            }
        }

        void add_child(document &doc, xml_node<> &root, const tree_ptr &tree) {
            if (tree->is_data()) {
                auto value = tree->data();
                if (is_string(value)) {
                    auto child = allocate(doc, tree->name, get_string(value));
                    root.append_node(child);
                } else {
                    set_value(doc, root, value);
                }
            } else if (tree->is_list()) {
                auto child = allocate(doc, tree->name);
                for (auto value : tree->list()) {
                    set_value(doc, *child, value);
                }
                root.append_node(child);
            }
        }

    } // namespace

    void print(std::ostream &os, tree_ptr tree) {
        document doc;
        if (auto decl = doc.allocate_node(node_declaration)) {
            decl->append_attribute(doc.allocate_attribute("version", "1.0"));
            decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
            doc.append_node(decl);
        }
        if (tree && !tree->is_null()) {
            add_child(doc, doc, tree);
        }
        int flags = rapidxml::print_no_indenting;
        if (pretty_out)
            flags = 0;
        rapidxml::print(os, doc, flags);

        doc.clear();
    }
} // namespace bnf::xml