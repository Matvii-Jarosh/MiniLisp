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
#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "value.h"
#include "environment.h"
#include <functional>
#include <map>

class Evaluator
{
public:
    Evaluator();

    Value Eval(std::shared_ptr<ListObject> exp, std::shared_ptr<Environment> env);
    Value Apply(std::shared_ptr<ListObject> proccedure, std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);

    bool isPrimitive(const std::string& name) const;
    std::function<Value(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval)> getPrimitive(const std::string& name) const;

private:
    std::map<std::string, std::function<Value(std::vector<std::shared_ptr<ListObject>>, std::shared_ptr<Environment>, Evaluator&)>> primitives;

    void initPrimitives();
};

#endif // EVALUATOR_H
