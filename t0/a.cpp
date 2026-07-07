#include <fstream>

class JsonFileWriter {
public:
    explicit JsonFileWriter(const std::string& filename);
    ~JsonFileWriter();

    void Add(const std::string& key, const std::string& value);

    JsonFileWriter(const JsonFileWriter&) = delete;
    JsonFileWriter& operator=(const JsonFileWriter&) = delete;

    JsonFileWriter(JsonFileWriter&& other) noexcept;
    JsonFileWriter& operator=(JsonFileWriter&& other) noexcept;

private:
    std::ofstream file_;
    bool first_;
    bool valid_;

};
JsonFileWriter::JsonFileWriter(JsonFileWriter &&other) noexcept
    : file_(std::move(other.file_)),
    first_(other.first_),
    valid_(other.valid_) {
    other.valid_ = false;
}

JsonFileWriter& JsonFileWriter::operator=(JsonFileWriter &&other) noexcept {
    if (this != &other) {
        if (valid_ && file_.is_open()) {
            file_ << "}";
            file_.close();
        }

        file_ = std::move(other.file_);
        first_ = other.first_;
        valid_ = other.valid_;

        other.valid_ = false;
    }
    return *this;
}


JsonFileWriter::JsonFileWriter(const std::string &filename) : first_(true), valid_(true) {
    file_.open(filename);
    if (!file_.is_open()) {
        valid_ = false;
    }
    else {
        file_ << "{";
    }
}

JsonFileWriter::~JsonFileWriter() {
    if (valid_ && file_.is_open()) {
        file_ << "}";
        file_.close();
    }
}

void JsonFileWriter::Add(const std::string &key, const std::string &value) {
    if (valid_ && file_.is_open()) {
        if (!first_) {
            file_ << ",";
        }
        file_ << "\"" << key << "\":\"" << value << "\"";
        first_ = false;
    }
}


class JsonEntity {
public:
    explicit JsonEntity(const std::string& filename) : writer_(filename) {}
    virtual ~JsonEntity() = default;

    void Add(const std::string& key, const std::string& value) {
        writer_.Add(key, value);
    }

    JsonEntity(JsonEntity&&) noexcept = default;
    JsonEntity& operator=(JsonEntity&&) noexcept = default;

    JsonEntity(const JsonEntity&) = delete;
    JsonEntity& operator=(const JsonEntity&) = delete;

protected:
    JsonFileWriter writer_;
};


class User : public JsonEntity {
public:
    explicit User(int id) : JsonEntity(MakeFileName(id)) {}
    void Serialize(const std::string& name, const std::string& age) {
        Add("Name", name);
        Add("age", age);
    }

private:
    static std::string MakeFileName(int id) {
        return "user_" + std::to_string(id) + ".json";
    }
};


class Product : public JsonEntity {
public:
    explicit Product(int id) : JsonEntity(MakeFileName(id)) {}

    void Serialize(const std::string& name, const std::string& price) {
        Add("Name", name);
        Add("price", price);
    }

private:
    static std::string MakeFileName(int id) {
        return "product_" + std::to_string(id) + ".json";
    }
};

int main() {
    User u(1);
    u.Add("Name", "Ivan");
    Product p(2);
    p.Add("age", "20");
}