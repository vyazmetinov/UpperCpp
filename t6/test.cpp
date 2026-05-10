#include "CheckpointBuilder.h"

#include <cassert>

int main() {
    std::vector<Checkpoint> cps{
        {"CP1", 55.75, 37.61, true, std::nullopt},
        {"CP2", 56.00, 38.00, false, 1.5},
        {"CP3", 57.20, 39.20, false, 0.75},
    };

    CheckpointDirector director{cps};

    TextReportBuilder text_builder;
    director.Build(text_builder);
    const auto text = text_builder.Result();
    assert(text.find("CP1") != std::string::npos);
    assert(text.find("незачёт СУ") != std::string::npos);

    PenaltySumBuilder penalty_builder;
    director.Build(penalty_builder);
    assert(penalty_builder.Result() == 2.25);

    TableWidgetBuilder table_builder;
    director.Build(table_builder);
    assert(table_builder.Result().size() == 3);
    assert(table_builder.Result()[1].name == "CP2");
    return 0;
}
