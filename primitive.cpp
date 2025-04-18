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
#include "primitive.h"
#include "utils.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>

Primitive::Primitive() {}

// ====================================== main ======================================
Value Primitive::std_define(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.size() != 2)
        throw std::runtime_error("'define' requires exactly 2 arguments: (define name value)");

    auto name = args[0];
    auto valueExpr = args[1];

    if (!name->isAtom())
        throw std::runtime_error("'define' first argument must be a symbol");

    std::string varName = name->asAtom();
    Value val = eval.Eval(valueExpr, env);

    env->define(varName, val);

    return val;
}

Value Primitive::std_begin(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.empty())
        throw std::runtime_error("'begin': at least one argument is required");
    Value result;
    std::shared_ptr<Environment> newEnv = std::make_shared<Environment>(env);
    for(auto arg: args) {
        result = eval.Eval(arg, newEnv);
    }
    return result;
}

Value Primitive::std_cond(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.empty())
        throw std::runtime_error("'cond': at least one argument is required");

    for (auto& clause : args) {
        if (clause->isAtom())
            throw std::runtime_error("'cond': each clause must be a list");

        auto inner = clause->asList();
        if (inner.empty())
            throw std::runtime_error("'cond': clause must not be empty");

        auto condition = inner[0];

        if (condition->isAtom() && condition->asAtom() == "else") {
            if (inner.size() < 2)
                throw std::runtime_error("'cond': 'else' clause must have a body");
            return eval.Eval(inner[1], env);
        }

        Value condResult = eval.Eval(condition, env);

        if (!condResult.isBool() || condResult.asBool()) {
            if (inner.size() < 2)
                throw std::runtime_error("'cond': true condition must have a body");
            return eval.Eval(inner[1], env);
        }
    }

    return Value();
}

// ==================================== cond ===================================
Value Primitive::std_equal(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.size() != 2)
        throw std::runtime_error("'=' expects exactly 2 arguments");

    Value left = eval.Eval(args[0], env);
    Value right = eval.Eval(args[1], env);

    if (left.isNumber() && right.isNumber())
        return Value(left.asNumber() == right.asNumber());
    if (left.isBool() && right.isBool())
        return Value(left.asBool() == right.asBool());
    if (left.isString() && right.isString())
        return Value(left.asString() == right.asString());

    return Value(false); // not equal if types differ
}

Value Primitive::std_gt(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    std::vector<Value> vals;
    for (auto& arg : args)
        vals.push_back(eval.Eval(arg, env));
    return ensureSingleTypeAndCompare(vals, [](auto a, auto b){ return a > b; });
}

Value Primitive::std_lt(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    std::vector<Value> vals;
    for (auto& arg : args)
        vals.push_back(eval.Eval(arg, env));
    return ensureSingleTypeAndCompare(vals, [](auto a, auto b){ return a < b; });
}

Value Primitive::std_ge(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    std::vector<Value> vals;
    for (auto& arg : args)
        vals.push_back(eval.Eval(arg, env));
    return ensureSingleTypeAndCompare(vals, [](auto a, auto b){ return a >= b; });
}

Value Primitive::std_le(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    std::vector<Value> vals;
    for (auto& arg : args)
        vals.push_back(eval.Eval(arg, env));
    return ensureSingleTypeAndCompare(vals, [](auto a, auto b){ return a <= b; });
}

Value Primitive::std_and(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.empty())
        throw std::runtime_error("'and': at least one argument is required");

    for (auto& arg : args) {
        Value value = eval.Eval(arg, env);
        if (value.isBool() && value.asBool() == false)
            return false;
    }
    return Value(true);
}

Value Primitive::std_or(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.empty())
        throw std::runtime_error("'and': at least one argument is required");

    for (auto& arg : args) {
        Value value = eval.Eval(arg, env);
        if (value.isBool() && value.asBool() == true)
            return true;
    }
    return Value(false);
}

Value Primitive::std_not(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.size() != 1)
        throw std::runtime_error("'not' expects exactly 1 arguments");
    Value value = eval.Eval(args[0], env);
    if (value.isBool() && value.asBool() == false)
        return true;
    else
        return false;
}

Value Primitive::std_is_number(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.size() != 1)
        throw std::runtime_error("'number?': requires exactly 1 argument");

    Value val = eval.Eval(args[0], env);
    return Value(val.isNumber());
}

Value Primitive::std_is_string(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.size() != 1)
        throw std::runtime_error("'string?': requires exactly 1 argument");

    Value val = eval.Eval(args[0], env);
    return Value(val.isString());
}

Value Primitive::std_is_bool(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.size() != 1)
        throw std::runtime_error("'bool?': requires exactly 1 argument");

    Value val = eval.Eval(args[0], env);
    return Value(val.isBool());
}

Value Primitive::std_is_lambda(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.size() != 1)
        throw std::runtime_error("'lambda?': requires exactly 1 argument");

    Value val = eval.Eval(args[0], env);
    return Value(val.isLambda());
}

// ====================================== math ======================================
Value Primitive::std_plus(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    long double result = 0;
    for (const auto& arg : args) {
        Value valArg = eval.Eval(arg, env);
        if (!valArg.isNumber())
            throw std::runtime_error(" '+' only works with numbers");
        result += valArg.asNumber();
    }
    return result;
}

Value Primitive::std_minus(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.empty())
        throw std::runtime_error("'-': at least one argument is required");

    Value resultVal = eval.Eval(args[0], env);
    if (!resultVal.isNumber())
        throw std::runtime_error("'-': the first argument must be a number");

    long double result = resultVal.asNumber();

    if (args.size() == 1) {
        return Value(-result);
    }

    for (size_t i = 1; i < args.size(); ++i) {
        Value val = eval.Eval(args[i], env);
        if (!val.isNumber())
            throw std::runtime_error("'-': all arguments must be numbers");
        result -= val.asNumber();
    }

    return Value(result);
}

Value Primitive::std_mul(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    long double result = 1;
    for (const auto& arg : args) {
        Value valArg = eval.Eval(arg, env);
        if (!valArg.isNumber())
            throw std::runtime_error(" '*' only works with numbers");
        result *= valArg.asNumber();
    }
    return result;
}

Value Primitive::std_div(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.empty())
        throw std::runtime_error("'/' expects at least one argument");
    Value firstVal = eval.Eval(args[0], env);
    if (!firstVal.isNumber())
        throw std::runtime_error("'/' expects all arguments to be numbers");

    long double result = firstVal.asNumber();

    if (args.size() == 1) {
        if (result == 0)
            throw std::runtime_error("'/' division by zero");
        return Value(1.0L / result);
    }

    for (size_t i = 1; i < args.size(); ++i) {
        Value val = eval.Eval(args[i], env);
        if (!val.isNumber())
            throw std::runtime_error("'/' expects all arguments to be numbers");

        long double divisor = val.asNumber();
        if (divisor == 0)
            throw std::runtime_error("'/' division by zero");

        result /= divisor;
    }

    return Value(result);
}

Value Primitive::std_sqrt(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.size() != 1)
        throw std::runtime_error("'sqrt': requires exactly 1 argument");

    Value val = eval.Eval(args[0], env);
    if (!val.isNumber())
        throw std::runtime_error("'sqrt': argument must be a number");

    return Value(std::sqrt(val.asNumber()));
}

Value Primitive::std_pow(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.size() != 2)
        throw std::runtime_error("'pow': requires exactly 2 arguments");

    Value base = eval.Eval(args[0], env);
    Value exponent = eval.Eval(args[1], env);

    if (!base.isNumber() || !exponent.isNumber())
        throw std::runtime_error("'pow': both arguments must be numbers");

    return Value(std::pow(base.asNumber(), exponent.asNumber()));
}

Value Primitive::std_sin(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.size() != 1)
        throw std::runtime_error("'sin': requires exactly 1 argument");

    Value val = eval.Eval(args[0], env);
    if (!val.isNumber())
        throw std::runtime_error("'sin': argument must be a number");

    return Value(std::sin(val.asNumber()));
}

Value Primitive::std_cos(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.size() != 1)
        throw std::runtime_error("'cos': requires exactly 1 argument");

    Value val = eval.Eval(args[0], env);
    if (!val.isNumber())
        throw std::runtime_error("'cos': argument must be a number");

    return Value(std::cos(val.asNumber()));
}

Value Primitive::std_tan(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.size() != 1)
        throw std::runtime_error("'tan': requires exactly 1 argument");

    Value val = eval.Eval(args[0], env);
    if (!val.isNumber())
        throw std::runtime_error("'tan': argument must be a number");

    return Value(std::tan(val.asNumber()));
}

Value Primitive::std_asin(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.size() != 1)
        throw std::runtime_error("'asin': requires exactly 1 argument");

    Value val = eval.Eval(args[0], env);
    if (!val.isNumber())
        throw std::runtime_error("'asin': argument must be a number");

    return Value(std::asin(val.asNumber()));
}

Value Primitive::std_acos(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.size() != 1)
        throw std::runtime_error("'acos': requires exactly 1 argument");

    Value val = eval.Eval(args[0], env);
    if (!val.isNumber())
        throw std::runtime_error("'acos': argument must be a number");

    return Value(std::acos(val.asNumber()));
}

Value Primitive::std_atan(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.size() != 1)
        throw std::runtime_error("'atan': requires exactly 1 argument");

    Value val = eval.Eval(args[0], env);
    if (!val.isNumber())
        throw std::runtime_error("'atan': argument must be a number");

    return Value(std::atan(val.asNumber()));
}

// ====================================== system ======================================
Value Primitive::std_exit(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    env->has("");
    eval.isPrimitive("");
    if (!args.empty())
        throw std::runtime_error("'/' no need to have arguments");
    exit(0);
}

Value Primitive::std_load_file(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (args.size() != 1)
        throw std::runtime_error("'load-file' requires exactly 1 argument");

    Value filename = eval.Eval(args[0], env);
    if (!filename.isString())
        throw std::runtime_error("'load-file' filename is not a string");

    const char* fname = filename.asString().c_str();
    FILE* file = fopen(fname, "rb");

    if (!file) {
        throw std::runtime_error("Could not open file: " + filename.asString());
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    std::string content;
    content.resize(size);
    size_t readBytes = fread(&content[0], 1, size, file);
    fclose(file);

    if (readBytes != (size_t)size) {
        throw std::runtime_error("Error reading file: " + filename.asString());
    }

    auto tokens = tokenizeLisp(content);
    TokenStream ts(tokens);

    while (ts.hasNext()) {
        auto exp = ListObject::parse_tokens(ts);
        std::shared_ptr<Environment> _env = std::shared_ptr<Environment>(env);
        auto result = eval.Eval(exp, _env);
    }

    return Value(true);
}
