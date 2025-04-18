/*
 * Copyright (c) 2025 Matvii Jarosh
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/
#include "listobject.h"
#include "tokenstream.h"
#include <iostream>
#include <algorithm>

ListObject::ListObject(const std::string& atom) : value(atom) {}
ListObject::ListObject(const List& list) : value(list) {}

bool ListObject::isAtom() const {
    return std::holds_alternative<std::string>(value);
}

std::string ListObject::asAtom() const {
    return std::get<std::string>(value);
}

const ListObject::List& ListObject::asList() const {
    return std::get<List>(value);
}

void ListObject::print(std::ostream& out , int indent) const {
    if (isAtom()) {
        out << asAtom();
    } else {
        out << "[";
        const auto& list = asList();
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(out, indent + 1);
            if (i < list.size() - 1) out << " ";
        }
        out << "]";
    }
}

ListObject::Ptr ListObject::parse_tokens(TokenStream& ts) {
    if (!ts.hasNext()) throw std::runtime_error("Unexpected end of input");

    std::string token = ts.next();

    if (token == "(") {
        std::vector<ListObject::Ptr> list;
        while (ts.hasNext() && ts.peek() != ")") {
            list.push_back(parse_tokens(ts));
        }
        if (!ts.hasNext()) throw std::runtime_error("Missing closing ')'");
        ts.next();
        return std::make_shared<ListObject>(list);
    } else if (token == ")") {
        throw std::runtime_error("Unexpected ')'");
    } else {
        if (token.size() >= 2 && token.front() == '"' && token.back() == '"') {
            return std::make_shared<ListObject>(token);
        } else {
            std::transform(token.begin(), token.end(), token.begin(), ::toupper);
            return std::make_shared<ListObject>(token);
        }
    }
}

