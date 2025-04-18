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
#ifndef VALUE_H
#define VALUE_H

#include "lambda.h"

class Value
{
public:
    using VariantType = std::variant<long double, std::string, bool, std::shared_ptr<Lambda>>;

    Value();
    Value(long double num);
    Value(const std::string& str);
    Value(const char* str);
    Value(bool b);
    Value(std::shared_ptr<Lambda> lambda);

    bool isNumber() const;
    bool isString() const;
    bool isBool() const;
    bool isLambda() const;

    long double asNumber() const;
    const std::string& asString() const;
    bool asBool() const;
    std::shared_ptr<Lambda> asLambda() const;

    void print(std::ostream& out = std::cout) const;

private:
    VariantType data;
};

#endif // VALUE_H
