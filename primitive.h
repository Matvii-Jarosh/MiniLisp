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
#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "value.h"
#include "environment.h"
#include "evaluator.h"

class Primitive
{
public:
    using PrimitiveFunc = std::function<Value(std::vector<std::shared_ptr<ListObject>>, std::shared_ptr<Environment>, Evaluator&)>;
    Primitive();

    // main
    static Value std_define(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_begin(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_cond(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);

    // cond
    static Value std_equal(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_gt(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_lt(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_ge(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_le(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_and(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_or(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_not(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_is_number(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_is_bool(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_is_string(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_is_lambda(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);

    // math
    static Value std_plus(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_minus(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_mul(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_div(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_sqrt(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_pow(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_sin(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_cos(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_tan(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_asin(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_acos(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_atan(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);

    // system
    static Value std_exit(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);
    static Value std_load_file(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval);

};

#endif // PRIMITIVE_H
