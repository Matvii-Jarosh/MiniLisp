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
#include "utils.h"
#include "environment.h"
#include "listobject.h"
#include "value.h"
#include <iostream>
#include <string>

int main() {
    std::shared_ptr<Environment> env0 = std::make_shared<Environment>();

    Evaluator interp = Evaluator();

    std::cout << "Mini Math Lisp REPL (write 'q' for exit)\n";

    while (true) {
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "q")
            break;
        try {
            auto tokens = tokenizeLisp(input);
            TokenStream ts(tokens);
            auto exp = ListObject::parse_tokens(ts);
            Value result = interp.Eval(exp, env0);
            result.print();
            std::cout << std::endl;
        } catch (const std::exception& e) {
            std::cerr << " Error: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << " Unknown error occurred" << std::endl;
        }
    }

    std::cout << "Bye!" << std::endl;
    return 0;
}

