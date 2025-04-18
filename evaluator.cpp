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
#include "evaluator.h"
#include "environment.h"
#include "utils.h"
#include "primitive.h"

Value Evaluator::Eval(std::shared_ptr<ListObject> exp, std::shared_ptr<Environment> env) {
    if (isNumber(exp)) { // is number
        return Value(std::stold(exp->asAtom()));
    } else if (isBoolean(exp)) { // is boolean
        std::string token = exp->asAtom();
        if (token == "TRUE")
            return Value(true);
        else
            return Value(false);
    } else if (isString(exp)) { // is string
        return Value(exp->asAtom());
    } else if (isVariable(exp, env)) { // is variable
        Value result = Value(env->get(exp->asAtom()));
        return result;
    } if (isLambda(exp)) {
        auto lambdaList = exp->asList();
        auto paramsListObj = lambdaList[1];

        std::vector<std::string> args;
        for (auto& param : paramsListObj->asList()) {
            args.push_back(param->asAtom());
        }

        std::vector<std::shared_ptr<ListObject>> bodyExprs(
            lambdaList.begin() + 2, lambdaList.end()
            );
        auto bodyList = std::make_shared<ListObject>(bodyExprs);

        auto lambda = std::make_shared<Lambda>(args, bodyList);
        return Value(lambda);
    } else if(isApplication(exp, env, *this)) { // is application
        const auto& list = exp->asList();
        std::shared_ptr<ListObject> funcExp = list[0];

        std::vector<std::shared_ptr<ListObject>> args;
        for (size_t i = 1; i < list.size(); ++i)
            args.push_back(list[i]);

        return Apply(funcExp, args, env, *this);
    } else {
        std::cout << "No found type" << std::endl;
    }

    return Value();
}

Value Evaluator::Apply(std::shared_ptr<ListObject> proccedure, std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (proccedure->isAtom() && eval.isPrimitive(proccedure->asAtom())) {
        auto prim = eval.getPrimitive(proccedure->asAtom());
        Value result = prim(args, env, eval);
        return result;
    } else {
        auto lambda = eval.Eval(proccedure, env);
        auto procArgs = lambda.asLambda()->getArgs();
        auto procBody = lambda.asLambda()->getBody();
        std::shared_ptr<Environment> newEnv = std::make_shared<Environment>(env);
        for (size_t i = 0; i < procArgs.size(); ++i) {
            std::string argName = procArgs[i];
            Value argValue = eval.Eval(args[i], env);
            newEnv->define(argName, argValue);
        }
        Value result;
        if (!procBody->isAtom() && procBody->asList().size() >= 1) {
            for (auto& expr : procBody->asList()) {
                result = eval.Eval(expr, newEnv);
            }
            return result;
        } else {
            return eval.Eval(procBody, newEnv);
        }
        return result;
    }
}

Evaluator::Evaluator() {
    primitives["DEFINE"] = Primitive::std_define;
    primitives["BEGIN"] = Primitive::std_begin;
    primitives["COND"] = Primitive::std_cond;

    primitives["="] =  Primitive::std_equal;
    primitives[">"] =  Primitive::std_gt;
    primitives["<"] =  Primitive::std_lt;
    primitives[">="] =  Primitive::std_ge;
    primitives["<="] =  Primitive::std_le;
    primitives["AND"] = Primitive::std_and;
    primitives["OR"] = Primitive::std_or;
    primitives["NOT"] = Primitive::std_not;
    primitives["NUMBER?"] = Primitive::std_is_number;
    primitives["STRING?"] = Primitive::std_is_string;
    primitives["BOOL?"] = Primitive::std_is_bool;
    primitives["LAMBDA?"] = Primitive::std_is_lambda;

    primitives["+"] = Primitive::std_plus;
    primitives["-"] = Primitive::std_minus;
    primitives["*"] = Primitive::std_mul;
    primitives["/"] = Primitive::std_div;
    primitives["SQRT"] = Primitive::std_sqrt;
    primitives["POW"] = Primitive::std_pow;
    primitives["SIN"] = Primitive::std_sin;
    primitives["COS"] = Primitive::std_cos;
    primitives["TAN"] = Primitive::std_tan;
    primitives["ASIN"] = Primitive::std_asin;
    primitives["ACOS"] = Primitive::std_acos;
    primitives["ATAN"] = Primitive::std_atan;

    primitives["EXIT"] = Primitive::std_exit;
    primitives["LOAD-FILE"] = Primitive::std_load_file;
}

bool Evaluator::isPrimitive(const std::string& name) const {
    return primitives.find(name) != primitives.end();
}

std::function<Value(std::vector<std::shared_ptr<ListObject>> args, std::shared_ptr<Environment> env, Evaluator& eval)> Evaluator::getPrimitive(const std::string& name) const {
    auto it = primitives.find(name);
    if (it == primitives.end())
        throw std::runtime_error(" Unknown primitive: " + name);
    return it->second;
}
