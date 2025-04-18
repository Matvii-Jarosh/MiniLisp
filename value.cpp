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
#include "value.h"

Value::Value() : data(0.0L) {}
Value::Value(long double num) : data(num) {}
Value::Value(const std::string& str) {
    if (str.size() >= 2 && str.front() == '"' && str.back() == '"') {
        data = str.substr(1, str.size() - 2);
    } else {
        data = str;
    }
}
Value::Value(const char* str) : data(std::string(str)) {}
Value::Value(bool b) : data(b) {}
Value::Value(std::shared_ptr<Lambda> lambda) : data(lambda) {}

bool Value::isNumber() const { return std::holds_alternative<long double>(data); }
bool Value::isString() const { return std::holds_alternative<std::string>(data); }
bool Value::isBool()   const { return std::holds_alternative<bool>(data); }
bool Value::isLambda() const { return std::holds_alternative<std::shared_ptr<Lambda>>(data); }

long double Value::asNumber() const { return std::get<long double>(data); }
const std::string& Value::asString() const { return std::get<std::string>(data); }
bool Value::asBool() const { return std::get<bool>(data); }
std::shared_ptr<Lambda> Value::asLambda() const { return std::get<std::shared_ptr<Lambda>>(data); }

void Value::print(std::ostream& out) const {
    if (isNumber()) out << asNumber();
    else if (isString()) out << "\"" << asString() << "\"";
    else if (isBool()) out << (asBool() ? "TRUE" : "FALSE");
    else if (isLambda()) out << "<lambda>";
}
