#ifndef POINTCLOUD_PREPROCESSOR__FILTER_HPP_
#define POINTCLOUD_PREPROCESSOR__FILTER_HPP_

#include "Logger.h"
#include "PointCloud.h"

#include <cmath>
#include <string>
#include <unordered_map>

namespace pointcloud_preprocessor
{

class FilterParametr {
public:
    FilterParametr() = default;
    FilterParametr(std::unordered_map<std::string, double> params) : params_(std::move(params)) {}

    void ChangeParam(const std::string& param_name, double param_val) { params_[param_name] = param_val; }

    double GetParam(const std::string& param_name) const { return params_.at(param_name); }

    double GetParam(const std::string& param_name, double default_val) const noexcept {
        const auto it = params_.find(param_name);
        return it == params_.end() ? default_val : it->second;
    }

private:
    std::unordered_map<std::string, double> params_;
};

/** \brief @b Filter represents the base filter class. */
class Filter {
public:
    explicit Filter(const std::string& filter_name = "pointcloud_preprocessor_filter")
        : filter_name_(filter_name), logger_(filter_name_)
    {}

    virtual ~Filter() = default;

    const std::string& GetFilterName() const { return filter_name_; }

    double GetDistance(const PointCloud* pc, size_t index) const {
        const size_t offset = pc->PointOffset(index);
        if (pc->pointcloud_type_ == PointCloud::kTypeXyzir) {
            const double x = pc->points_[offset + 0];
            const double y = pc->points_[offset + 1];
            const double z = pc->points_[offset + 2];
            return std::hypot(x, y, z);
        }
        return pc->points_[offset + 5];
    }

    double GetAzimuth(const PointCloud* pc, size_t index) const {
        const size_t offset = pc->PointOffset(index);
        if (pc->pointcloud_type_ == PointCloud::kTypeXyzir) {
            const double x = pc->points_[offset + 0];
            const double y = pc->points_[offset + 1];
            return std::atan2(y, x);
        }
        return pc->points_[offset + 6];
    }

    virtual PointCloud* Apply(PointCloud* pc) = 0;

    virtual void SetParams(const FilterParametr& param) { params_ = param; }

    const FilterParametr& GetFilterParam() const { return params_; }

protected:
    std::string filter_name_ = "pointcloud_preprocessor_filter";
    Logger logger_;
    FilterParametr params_;
};

}  // namespace pointcloud_preprocessor

#endif  // POINTCLOUD_PREPROCESSOR__FILTER_HPP_
