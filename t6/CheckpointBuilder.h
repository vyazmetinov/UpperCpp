#pragma once

#include <iomanip>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

struct Checkpoint {
    std::string name;
    double latitude = 0.0;
    double longitude = 0.0;
    bool mandatory = true;
    std::optional<double> penalty_hours;
};

class ICheckpointBuilder {
public:
    virtual ~ICheckpointBuilder() = default;
    virtual void Start() = 0;
    virtual void Add(std::size_t index, const Checkpoint& cp) = 0;
};

class CheckpointDirector {
public:
    explicit CheckpointDirector(std::vector<Checkpoint> checkpoints) : checkpoints_(std::move(checkpoints)) {}

    void Build(ICheckpointBuilder& builder) const {
        builder.Start();
        for (std::size_t i = 0; i < checkpoints_.size(); ++i) {
            Validate(checkpoints_[i]);
            builder.Add(i + 1, checkpoints_[i]);
        }
    }

private:
    static void Validate(const Checkpoint& cp) {
        if (cp.latitude < -90.0 || cp.latitude > 90.0) {
            throw std::out_of_range("Latitude is out of range");
        }
        if (cp.longitude < -180.0 || cp.longitude > 180.0) {
            throw std::out_of_range("Longitude is out of range");
        }
        if (!cp.mandatory && !cp.penalty_hours.has_value()) {
            throw std::invalid_argument("Optional checkpoint must have penalty");
        }
    }

    std::vector<Checkpoint> checkpoints_;
};

class TextReportBuilder : public ICheckpointBuilder {
public:
    void Start() override { lines_.clear(); }

    void Add(std::size_t index, const Checkpoint& cp) override {
        std::ostringstream oss;
        oss << index << ". " << cp.name << " [lat=" << cp.latitude << ", lon=" << cp.longitude << "] ";
        if (cp.mandatory) {
            oss << "незачёт СУ";
        } else {
            oss << "penalty=" << std::fixed << std::setprecision(2) << *cp.penalty_hours << "h";
        }
        lines_.push_back(oss.str());
    }

    std::string Result() const {
        std::ostringstream out;
        for (const auto& line : lines_) {
            out << line << '\n';
        }
        return out.str();
    }

private:
    std::vector<std::string> lines_;
};

class PenaltySumBuilder : public ICheckpointBuilder {
public:
    void Start() override { sum_ = 0.0; }

    void Add(std::size_t, const Checkpoint& cp) override {
        if (!cp.mandatory) {
            sum_ += *cp.penalty_hours;
        }
    }

    double Result() const { return sum_; }

private:
    double sum_ = 0.0;
};

class TableWidgetBuilder : public ICheckpointBuilder {
public:
    struct Row {
        std::size_t index = 0;
        std::string name;
        double latitude = 0.0;
        double longitude = 0.0;
        std::string penalty_or_required;
    };

    void Start() override { rows_.clear(); }

    void Add(std::size_t index, const Checkpoint& cp) override {
        Row row;
        row.index = index;
        row.name = cp.name;
        row.latitude = cp.latitude;
        row.longitude = cp.longitude;
        row.penalty_or_required = cp.mandatory ? "незачёт СУ" : std::to_string(*cp.penalty_hours) + "h";
        rows_.push_back(std::move(row));
    }

    const std::vector<Row>& Result() const { return rows_; }

private:
    std::vector<Row> rows_;
};
