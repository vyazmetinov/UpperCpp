#include "crop_box_filter.hpp"

#include <cmath>
#include <stdexcept>
#include <utility>
#include <vector>

namespace pointcloud_preprocessor
{

CropBoxFilter::CropBoxFilter() : Filter("CropBoxFilter") {}

bool CropBoxFilter::IsValidBox(const CropBoxParam& param) {
    return param.min_x < param.max_x && param.min_y < param.max_y && param.min_z < param.max_z;
}

bool CropBoxFilter::IsInsideBox(double x, double y, double z, const CropBoxParam& param) {
    return x > param.min_x && x < param.max_x && y > param.min_y && y < param.max_y && z > param.min_z &&
           z < param.max_z;
}

bool CropBoxFilter::ShouldKeepPoint(double x, double y, double z, const CropBoxParam& param) {
    const bool inside = IsInsideBox(x, y, z, param);
    return param.negative ? !inside : inside;
}

PointCloud* CropBoxFilter::Apply(PointCloud* pc) {
    if (pc == nullptr) {
        throw std::invalid_argument("input point cloud is null");
    }

    std::vector<double> output;
    output.reserve(pc->size_ * pc->point_size_);

    for (size_t point_index = 0; point_index < pc->size_; ++point_index) {
        const size_t offset = point_index * pc->point_size_;
        const double x = pc->points_[offset + 0];
        const double y = pc->points_[offset + 1];
        const double z = pc->points_[offset + 2];

        if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z)) {
            logger_.log("Ignoring point containing NaN values");
            continue;
        }

        if (!ShouldKeepPoint(x, y, z, param_)) {
            continue;
        }

        const auto begin = pc->points_.begin() + static_cast<std::ptrdiff_t>(offset);
        output.insert(output.end(), begin, begin + static_cast<std::ptrdiff_t>(pc->point_size_));
    }

    auto* output_pc = new PointCloud();
    output_pc->pointcloud_type_ = pc->pointcloud_type_;
    output_pc->point_size_ = pc->point_size_;
    output_pc->size_ = output.empty() ? 0 : output.size() / pc->point_size_;
    output_pc->points_ = std::move(output);
    return output_pc;
}

void CropBoxFilter::SetParams(const FilterParametr& param) {
    CropBoxParam new_param{};

    new_param.min_x = param.GetParam("min_x", new_param.min_x);
    new_param.max_x = param.GetParam("max_x", new_param.max_x);
    new_param.min_y = param.GetParam("min_y", new_param.min_y);
    new_param.max_y = param.GetParam("max_y", new_param.max_y);
    new_param.min_z = param.GetParam("min_z", new_param.min_z);
    new_param.max_z = param.GetParam("max_z", new_param.max_z);
    new_param.negative = static_cast<bool>(param.GetParam("negative", param_.negative ? 1.0 : 0.0));

    if (!IsValidBox(new_param)) {
        logger_.log("[paramCallback] Wrong crop box params");
        Filter::SetParams(param);
        return;
    }

    if (new_param.min_x != param_.min_x || new_param.max_x != param_.max_x || new_param.min_y != param_.min_y ||
        new_param.max_y != param_.max_y || new_param.min_z != param_.min_z || new_param.max_z != param_.max_z ||
        new_param.negative != param_.negative) {
        logger_.log("[paramCallback] Setting the minimum point to: " + std::to_string(new_param.min_x) + " " +
                    std::to_string(new_param.min_y) + " " + std::to_string(new_param.min_z));
        logger_.log("[paramCallback] Setting the maximum point to: " + std::to_string(new_param.max_x) + " " +
                    std::to_string(new_param.max_y) + " " + std::to_string(new_param.max_z));
        logger_.log("[paramCallback] Setting the filter negative flag to: " +
                    std::string(new_param.negative ? "true" : "false"));
        param_ = new_param;
    }

    Filter::SetParams(param);
}

}  // namespace pointcloud_preprocessor
