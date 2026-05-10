#pragma once

#include <cmath>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

using Context = std::unordered_map<std::string, double>;

class Expression {
public:
    virtual ~Expression() = default;
    virtual double Calculate(const Context& ctx) const = 0;
    virtual std::string Print() const = 0;
};

class Constant final : public Expression {
public:
    explicit Constant(double value) : value_(value) {}

    double Calculate(const Context&) const override { return value_; }
    std::string Print() const override { return std::to_string(value_); }

private:
    double value_;
};

class Variable final : public Expression {
public:
    explicit Variable(std::string name) : name_(std::move(name)) {}

    double Calculate(const Context& ctx) const override {
        auto it = ctx.find(name_);
        if (it == ctx.end()) {
            throw std::runtime_error("Variable is missing in context: " + name_);
        }
        return it->second;
    }

    std::string Print() const override { return name_; }

private:
    std::string name_;
};

class BinaryOperation : public Expression {
public:
    BinaryOperation(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right)
        : left_(std::move(left)), right_(std::move(right)) {}

protected:
    const std::shared_ptr<Expression>& Left() const { return left_; }
    const std::shared_ptr<Expression>& Right() const { return right_; }

private:
    std::shared_ptr<Expression> left_;
    std::shared_ptr<Expression> right_;
};

class Addition final : public BinaryOperation {
public:
    using BinaryOperation::BinaryOperation;
    double Calculate(const Context& ctx) const override { return Left()->Calculate(ctx) + Right()->Calculate(ctx); }
    std::string Print() const override { return "(" + Left()->Print() + " + " + Right()->Print() + ")"; }
};

class Subtraction final : public BinaryOperation {
public:
    using BinaryOperation::BinaryOperation;
    double Calculate(const Context& ctx) const override { return Left()->Calculate(ctx) - Right()->Calculate(ctx); }
    std::string Print() const override { return "(" + Left()->Print() + " - " + Right()->Print() + ")"; }
};

class Multiplication final : public BinaryOperation {
public:
    using BinaryOperation::BinaryOperation;
    double Calculate(const Context& ctx) const override { return Left()->Calculate(ctx) * Right()->Calculate(ctx); }
    std::string Print() const override { return "(" + Left()->Print() + " * " + Right()->Print() + ")"; }
};

class Division final : public BinaryOperation {
public:
    using BinaryOperation::BinaryOperation;
    double Calculate(const Context& ctx) const override {
        const double rhs = Right()->Calculate(ctx);
        if (std::fabs(rhs) < 1e-12) {
            throw std::runtime_error("Division by zero");
        }
        return Left()->Calculate(ctx) / rhs;
    }
    std::string Print() const override { return "(" + Left()->Print() + " / " + Right()->Print() + ")"; }
};

class ExpressionFactory {
public:
    ExpressionFactory() {
        for (int i = -5; i <= 256; ++i) {
            constants_[static_cast<double>(i)] = std::make_shared<Constant>(static_cast<double>(i));
        }
    }

    std::shared_ptr<Constant> CreateConstant(double value) {
        auto it = constants_.find(value);
        if (it != constants_.end()) {
            return it->second;
        }
        auto created = std::make_shared<Constant>(value);
        constants_[value] = created;
        return created;
    }

    std::shared_ptr<Variable> CreateVariable(const std::string& name) {
        auto it = variables_.find(name);
        if (it != variables_.end()) {
            return it->second;
        }
        auto created = std::make_shared<Variable>(name);
        variables_[name] = created;
        return created;
    }

    void RemoveConstant(double value) {
        if (value >= -5.0 && value <= 256.0 && std::floor(value) == value) {
            return;
        }
        constants_.erase(value);
    }

    void RemoveVariable(const std::string& name) { variables_.erase(name); }

private:
    std::unordered_map<double, std::shared_ptr<Constant>> constants_;
    std::unordered_map<std::string, std::shared_ptr<Variable>> variables_;
};
