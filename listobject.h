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
#ifndef LISPVALUE_H
#define LISPVALUE_H

#include "tokenstream.h"
#include <iostream>
#include <vector>
#include <string>
#include <variant>
#include <memory>

class ListObject
{
public:
    using Ptr = std::shared_ptr<ListObject>;
    using List = std::vector<Ptr>;
    ListObject(const std::string& atom);
    ListObject(const List& list);
    bool isAtom() const;
    std::string asAtom() const;
    const List& asList() const;
    void print(std::ostream& out = std::cout, int indent = 0) const;
    static Ptr parse_tokens(TokenStream& ts);
private:
    std::variant<std::string, List> value;
};

#endif // LISPVALUE_H
