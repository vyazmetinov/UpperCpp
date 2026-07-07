#ifndef POINTCLOUD_H_
#define POINTCLOUD_H_

#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>


class PointCloud {
public:
    static constexpr const char* kTypeXyzir = "XYZIR";
    static constexpr const char* kTypeXyzirdat = "XYZIRDAT";

    void Init(std::string pc_type, size_t points_count) {
        pointcloud_type_ = std::move(pc_type);
        point_size_ = PointSizeForType(pointcloud_type_);
        size_ = points_count;
        points_.assign(size_ * point_size_, 0.0);
    }

    void AddPoint(const std::vector<double>& point, size_t index) {
        if (point.size() != point_size_) {
            throw std::runtime_error(
                "Point has unsupported size = " + std::to_string(point.size()) +
                " for type " + pointcloud_type_);
        }
        if (index >= size_) {
            throw std::runtime_error("Point index out of range");
        }

        const size_t offset = index * point_size_;
        for (size_t i = 0; i < point_size_; ++i) {
            points_[offset + i] = point[i];
        }
    }

    size_t PointOffset(size_t point_index) const {
        if (point_index >= size_) {
            throw std::out_of_range("Point index out of range");
        }
        return point_index * point_size_;
    }

    std::string pointcloud_type_;
    size_t size_ = 0;
    size_t point_size_ = 0;
    std::vector<double> points_;

private:
    static size_t PointSizeForType(const std::string& type) {
        if (type == kTypeXyzir) {
            return 5;
        }
        if (type == kTypeXyzirdat) {
            return 8;
        }
        throw std::runtime_error("unknown pointcloud type: " + type);
    }
};

inline void FillPointCloud(
    PointCloud* pc,
    size_t points_count,
    const std::string& point_type,
    const std::vector<double>& data)
{
    const size_t point_size = (point_type == PointCloud::kTypeXyzir) ? 5 : 8;
    if (data.size() != points_count * point_size) {
        throw std::runtime_error("data size does not match points count");
    }

    pc->Init(point_type, points_count);
    for (size_t i = 0; i < points_count; ++i) {
        pc->AddPoint(
            std::vector<double>(data.begin() + i * point_size, data.begin() + (i + 1) * point_size),
            i);
    }
}

#endif
