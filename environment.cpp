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
#include "environment.h"
#include <stdexcept>

Environment::Environment() : parent(nullptr) {}
Environment::~Environment() {
    //std::cout << "End of ENV" << std::endl;
}

Environment::Environment(Ptr parentEnv) : parent(parentEnv) {}

void Environment::define(const std::string& name, const Value& value) {
    map[name] = value;
    
}

bool Environment::set(const std::string& name, const Value& value) {
    if (map.count(name)) {
        map[name] = value;
        return true;
    } else if (parent) {
        return parent->set(name, value);
    }
    return false;
}

Value Environment::get(const std::string& name) const {
    auto it = map.find(name);
    if (it != map.end()) {
        return it->second;
    } else if (parent) {
        return parent->get(name);
    } else {
        throw std::runtime_error("Variable not found: " + name);
    }
}

bool Environment::has(const std::string& name) const {
    if (map.count(name)) {
        return true;
    } else if (parent) {
        return parent->has(name);
    }
    return false;
}
