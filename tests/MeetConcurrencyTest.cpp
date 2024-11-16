#include <gtest/gtest.h>
#include <future>

#include "../src/Meet.h"
#include "../src/Lifter.h"

TEST(MeetConcurrencyTest, SimultaneousImportExport) {
    Meet meet;
    Lifter lifter("Lifter X", Gender::MALE, 30, 100.0, 200.0, 150.0, 250.0, Unit::KG);
    meet.addLifter(lifter);

    auto importTask = std::async(std::launch::async, [&meet] {
        meet.clearLifters();
    });

    auto exportTask = std::async(std::launch::async, [&meet] {
        return meet.getLifters();
    });

    importTask.wait();
    auto exportedLifters = exportTask.get();

    EXPECT_TRUE(exportedLifters.empty());
}

TEST(MeetConcurrencyTest, AsyncSorting) {
    Meet meet;
    Lifter lifter1("Lifter Y", Gender::MALE, 30, 100.0, 200.0, 150.0, 250.0, Unit::KG);
    Lifter lifter2("Lifter Z", Gender::FEMALE, 25, 80.0, 150.0, 100.0, 200.0, Unit::KG);

    meet.addLifter(lifter1);
    meet.addLifter(lifter2);

    auto sortingTask = std::async(std::launch::async, [&meet] {
        meet.sortLifters("Total");
    });

    sortingTask.wait();

    EXPECT_EQ(meet.getLifters()[0].getName(), "Lifter Y");
}
