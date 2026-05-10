#include "Expression.h"

#include <cassert>

int main() {
    ExpressionFactory factory;

    auto c2 = factory.CreateConstant(2);
    auto x = factory.CreateVariable("x");
    auto y = factory.CreateVariable("y");

    auto expr = std::make_shared<Addition>(
        c2,
        std::make_shared<Multiplication>(x, std::make_shared<Subtraction>(y, c2)));

    Context ctx;
    ctx["x"] = 3.0;
    ctx["y"] = 7.0;

    assert(expr->Calculate(ctx) == 17.0);
    assert(factory.CreateConstant(2).get() == c2.get());
    assert(factory.CreateVariable("x").get() == x.get());
    return 0;
}
