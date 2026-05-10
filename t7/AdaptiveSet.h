#pragma once

#include <algorithm>
#include <memory>
#include <unordered_set>
#include <vector>

class ISetStorage {
public:
    virtual ~ISetStorage() = default;
    virtual void Add(int value) = 0;
    virtual void Remove(int value) = 0;
    virtual bool Contains(int value) const = 0;
    virtual std::vector<int> Elements() const = 0;
    virtual std::size_t Size() const = 0;
};

class VectorSetStorage : public ISetStorage {
public:
    void Add(int value) override {
        if (!Contains(value)) {
            data_.push_back(value);
        }
    }

    void Remove(int value) override {
        data_.erase(std::remove(data_.begin(), data_.end(), value), data_.end());
    }

    bool Contains(int value) const override {
        return std::find(data_.begin(), data_.end(), value) != data_.end();
    }

    std::vector<int> Elements() const override { return data_; }
    std::size_t Size() const override { return data_.size(); }

private:
    std::vector<int> data_;
};

class HashSetStorage : public ISetStorage {
public:
    void Add(int value) override { data_.insert(value); }
    void Remove(int value) override { data_.erase(value); }
    bool Contains(int value) const override { return data_.find(value) != data_.end(); }
    std::size_t Size() const override { return data_.size(); }

    std::vector<int> Elements() const override {
        return {data_.begin(), data_.end()};
    }

private:
    std::unordered_set<int> data_;
};

class Set {
public:
    explicit Set(std::size_t threshold = 8) : threshold_(threshold), storage_(std::make_unique<VectorSetStorage>()) {}

    void Add(int value) {
        storage_->Add(value);
        Rebalance();
    }

    void Remove(int value) {
        storage_->Remove(value);
        Rebalance();
    }

    bool Contains(int value) const { return storage_->Contains(value); }
    std::size_t Size() const { return storage_->Size(); }

    Set Union(const Set& other) const {
        Set result{threshold_};
        for (int v : storage_->Elements()) {
            result.Add(v);
        }
        for (int v : other.storage_->Elements()) {
            result.Add(v);
        }
        return result;
    }

    Set Intersection(const Set& other) const {
        Set result{threshold_};
        for (int v : storage_->Elements()) {
            if (other.Contains(v)) {
                result.Add(v);
            }
        }
        return result;
    }

private:
    void Rebalance() {
        const bool should_be_hash = storage_->Size() > threshold_;
        const bool is_hash = dynamic_cast<HashSetStorage*>(storage_.get()) != nullptr;
        if (should_be_hash == is_hash) {
            return;
        }

        std::vector<int> elements = storage_->Elements();
        storage_ = should_be_hash ? std::unique_ptr<ISetStorage>(std::make_unique<HashSetStorage>())
                                  : std::unique_ptr<ISetStorage>(std::make_unique<VectorSetStorage>());
        for (int v : elements) {
            storage_->Add(v);
        }
    }

    std::size_t threshold_;
    std::unique_ptr<ISetStorage> storage_;
};
