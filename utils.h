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
#ifndef UTILS_H
#define UTILS_H

#include "listobject.h"
#include "environment.h"
#include "evaluator.h"
#include <string>
#include <vector>

std::vector<std::string> tokenizeLisp(const std::string& input);
bool isNumber(std::shared_ptr<ListObject> exp);
bool isBoolean(std::shared_ptr<ListObject> exp);
bool isString(std::shared_ptr<ListObject> exp);
bool isLambda(std::shared_ptr<ListObject> exp);
bool isVariable(std::shared_ptr<ListObject> exp, std::shared_ptr<Environment> env);
bool isApplication(std::shared_ptr<ListObject> exp, std::shared_ptr<Environment> env, Evaluator& eval);
Value ensureSingleTypeAndCompare(const std::vector<Value>& vals, std::function<bool(long double, long double)> cmp);

#endif // UTILS_H
