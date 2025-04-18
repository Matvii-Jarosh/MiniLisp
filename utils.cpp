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
#include "utils.h"
#include "evaluator.h"
#include <regex>

std::vector<std::string> tokenizeLisp(const std::string& input) {
    std::vector<std::string> tokens;

    std::regex token_regex(R"((\()|(\))|(\"[^\"]*\")|([^\s()]+))");
    std::smatch match;
    std::string s = input;

    while (std::regex_search(s, match, token_regex)) {
        tokens.push_back(match.str());
        s = match.suffix();
    }

    return tokens;
}

bool isNumber(std::shared_ptr<ListObject> exp) {
    if (!exp->isAtom())
        return false;

    std::string token = exp->asAtom();
    // Дозволяє: -12, 3.14, -0.001, 42
    std::regex number_regex(R"(^-?[0-9]+(\.[0-9]+)?$)");
    return std::regex_match(token, number_regex);
}


bool isBoolean(std::shared_ptr<ListObject> exp) {
    if (!exp->isAtom())
        return false;

    std::string token = exp->asAtom();
    if (token == "TRUE" || token == "FALSE")
        return true;
    else
        return false;
}

bool isString(std::shared_ptr<ListObject> exp) {
    if (!exp->isAtom())
        return false;

    std::string token = exp->asAtom();
    if (token[0] == '"' && token[token.size() - 1] == '"')
        return true;
    else
        return false;

}

bool isVariable(std::shared_ptr<ListObject> exp, std::shared_ptr<Environment> env) {
    if (!exp->isAtom())
        return false;

    if (env->has(exp->asAtom()))
        return true;

    return false;
}

bool isLambda(std::shared_ptr<ListObject> exp) {
    if (!exp->isAtom()) {
        auto token = exp->asList();

        // lambda має мінімум 3 частини: 'lambda', аргументи, тіло
        if (token.size() < 3)
            return false;

        // перший елемент має бути атомом "lambda"
        if (!token[0]->isAtom() || token[0]->asAtom() != "LAMBDA")
            return false;

        // другий елемент має бути списком параметрів
        if (token[1]->isAtom())
            return false;

        for (const auto& param : token[1]->asList()) {
            if (!param->isAtom()) {
                std::cerr << "Lambda param is not atom!\n";
                return false;
            }
        }

        // все валідно
        return true;
    }
    return false;
}


bool isApplication(std::shared_ptr<ListObject> exp, std::shared_ptr<Environment> env, Evaluator& eval) {
    if (exp->isAtom())
        return false;

    const auto& list = exp->asList();
    if (list.empty())
        return false;

    try {
        if (list[0]->isAtom() && eval.isPrimitive(list[0]->asAtom()))
            return true;

        Value first = eval.Eval(list[0], env);
        return first.isLambda();
    } catch (...) {
        return false;
    }
}

Value ensureSingleTypeAndCompare(const std::vector<Value>& vals, std::function<bool(long double, long double)> cmp) {
    for (const auto& val : vals)
        if (!val.isNumber())
            throw std::runtime_error("Comparison requires numeric arguments");

    for (size_t i = 1; i < vals.size(); ++i)
        if (!cmp(vals[i - 1].asNumber(), vals[i].asNumber()))
            return Value(false);

    return Value(true);
}
