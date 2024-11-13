#include "MeetWindow.h"
#include "../MeetIO.h"

#include "imgui.h"
#include "imgui_stdlib.h"
#include "ImGuiFileDialog.h"

#include <iostream>

constexpr size_t MAX_BUFFER_SIZE = 256;
constexpr float COMBO_WIDTH_RATIO = 0.25f;
constexpr float INPUT_WIDTH_RATIO = 0.5f;
constexpr float BUTTON_WIDTH = 97.0f;
constexpr int COLUMN_COUNT = 13;

MeetWindow::MeetWindow(Meet &meet) : meet_(meet) {
    config_.path = ".";
    config_.flags = ImGuiFileDialogFlags_Modal;

    filepath_ = "Powerlifting_Meet.json";
}

void CenterFormattedText(const char *fmt, ...) {
    // Prepare a variable argument list to format the string
    va_list args;
    va_start(args, fmt);

    // Allocate a buffer to hold the formatted text
    char buffer[MAX_BUFFER_SIZE];
    vsnprintf(buffer, IM_ARRAYSIZE(buffer), fmt, args);

    // End the use of the variable argument list
    va_end(args);

    // Calculate padding
    const ImVec2 textSize = ImGui::CalcTextSize(buffer);
    const ImVec2 cellSize = ImGui::GetContentRegionAvail();
    const float paddingX = (cellSize.x - textSize.x) * 0.5f;
    if (paddingX > 0) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + paddingX);

    ImGui::TextUnformatted(buffer);
}

bool isValidDate(const std::string &date) {
    if (date.size() != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;

    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (date[i] < '0' || date[i] > '9') return false;
    }

    return true;
}

void MeetWindow::displayMenuBar() {
    static bool isDateValid = true;

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Menu")) {
            ImGui::BeginDisabled(isImporting_);
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                ImGuiFileDialog::Instance()->OpenDialog("ImportFileDlgKey", "Choose File", ".json", config_);
            }
            ImGui::EndDisabled();

            ImGui::BeginDisabled(isExporting_);
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                startSaveFile();
            }
            if (ImGui::MenuItem("Save As..")) {
                ImGuiFileDialog::Instance()->OpenDialog("ExportFileDlgKey", "Choose File", ".json", config_);
            }
            ImGui::EndDisabled();

            ImGui::Separator();
            if (ImGui::BeginMenu("Meet Options")) {
                ImGui::Text("Name:");
                ImGui::SameLine(100);
                ImGui::InputText("##01", &state_.meetName);
                ImGui::Text("Date:");
                ImGui::SameLine(100);
                ImGui::InputTextWithHint("##02", "MM-DD-YYYY", &state_.date);
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    isDateValid = isValidDate(state_.date);
                }
                ImGui::Text("Location:");
                ImGui::SameLine(100);
                ImGui::InputText("##03", &state_.location);
                ImGui::Text("Federation:");
                ImGui::SameLine(100);
                ImGui::Combo("##04", &state_.fedIndex, state_.fedOptions, IM_ARRAYSIZE(state_.fedOptions));
                ImGui::Checkbox("Bench Only", &state_.benchOnly);
                ImGui::Checkbox("Equipped", &state_.equipped);
                ImGui::Checkbox("Drug Tested", &state_.drugTested);

                if (!isDateValid) {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Invalid date format! Use MM-DD-YYYY.");
                }
                ImGui::BeginDisabled(!isDateValid);

                if (ImGui::Button("Save", ImVec2(155, 0))) {
                    meet_.setName(state_.meetName);
                    meet_.setDate(state_.date);
                    meet_.setLocation(state_.location);
                    meet_.setFederation(static_cast<Federation>(state_.fedIndex));
                    meet_.setDrugTested(state_.drugTested);
                    meet_.setBenchOnly(state_.benchOnly);
                    meet_.setEquipClass(state_.equipped == false ? EquipmentClass::RAW : EquipmentClass::EQUIPPED);
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndDisabled();

                ImGui::SameLine();
                if (ImGui::Button("Close", ImVec2(155, 0))) {
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void MeetWindow::displayAddLifterSection() {
    const float inputWidth = width_ * INPUT_WIDTH_RATIO;

    ImGui::Text("Add Lifter:");
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputText("Name", &state_.lifterName);
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputInt("Age", &state_.age);
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputDouble("Bodyweight", &state_.bodyweight, 0, 0, "%.3f");
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputDouble("Squat", &state_.squat, 0, 0, "%.3f");
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputDouble("Bench", &state_.bench, 0, 0, "%.3f");
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputDouble("Deadlift", &state_.deadlift, 0, 0, "%.3f");
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::Combo("Gender", &state_.genderIndex, state_.genders, IM_ARRAYSIZE(state_.genders));
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::Combo("Units", &state_.unitIndex, state_.units, IM_ARRAYSIZE(state_.units));

    if (ImGui::Button("Add Lifter", ImVec2(BUTTON_WIDTH, 0))) {
        const Gender gender = (state_.genderIndex == 0) ? Gender::MALE : Gender::FEMALE;
        const Unit unit = (state_.unitIndex == 0) ? Unit::KG : Unit::LBS;
        auto lifter = Lifter(state_.lifterName, gender, state_.age, state_.bodyweight, state_.squat, state_.bench,
                             state_.deadlift, unit);

        if (unit != meet_.getUnits()) {
            lifter.convertUnits();
        }
        meet_.addLifter(lifter);
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear", ImVec2(BUTTON_WIDTH, 0))) {
        clearLifterState();
    }
}

void MeetWindow::displayTableModifiers() {
    const float comboWidth = width_ * COMBO_WIDTH_RATIO;

    ImGui::Text("Sort Lifters By:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(comboWidth);
    if (ImGui::Combo("##SortOptions", &state_.sortingIndex, state_.sortOptions, IM_ARRAYSIZE(state_.sortOptions))) {
        if (state_.sortingIndex != state_.previousSortingIndex) {
            state_.previousSortingIndex = state_.sortingIndex;
            startSortingLifters(state_.sortOptions[state_.sortingIndex]);
        }
    }

    ImGui::SameLine();
    ImGui::Text("Units:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(comboWidth);
    ImGui::Combo("##MeetUnits", &state_.meetUnitIndex, state_.units, IM_ARRAYSIZE(state_.units));
    if (state_.meetUnitIndex != state_.previousMeetUnitIndex) {
        meet_.setUnits(static_cast<Unit>(state_.meetUnitIndex));
        meet_.convertAllUnits();
        state_.previousMeetUnitIndex = state_.meetUnitIndex;
    }
}

void MeetWindow::displayLifterRow(Lifter &lifter, const size_t index) {
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    CenterFormattedText("%zu", index + 1);
    ImGui::TableSetColumnIndex(1);
    CenterFormattedText("%s", lifter.getName().c_str());
    ImGui::TableSetColumnIndex(2);
    CenterFormattedText("%c", lifter.getGender() == Gender::MALE ? 'M' : 'F');
    ImGui::TableSetColumnIndex(3);
    CenterFormattedText("%d", lifter.getAge());
    ImGui::TableSetColumnIndex(4);
    CenterFormattedText("%.1f", lifter.getBodyweight());
    ImGui::TableSetColumnIndex(5);
    CenterFormattedText("%.1f", lifter.getSquat());
    ImGui::TableSetColumnIndex(6);
    CenterFormattedText("%.1f", lifter.getBench());
    ImGui::TableSetColumnIndex(7);
    CenterFormattedText("%.1f", lifter.getDeadlift());
    ImGui::TableSetColumnIndex(8);
    CenterFormattedText("%.1f", lifter.getTotal());
    ImGui::TableSetColumnIndex(9);
    CenterFormattedText("%.2f", lifter.getDOTSScore());
    ImGui::TableSetColumnIndex(10);
    CenterFormattedText("%.2f", lifter.getIPFGLScore());
    ImGui::TableSetColumnIndex(11);
    CenterFormattedText("%.2f", lifter.getWILKSScore());

    displayLifterActions(lifter, index);
}

void MeetWindow::displayLifterActions(Lifter &lifter, const size_t index) {
    ImGui::TableSetColumnIndex(12);
    const std::string updateId = "Update##" + std::to_string(index);
    const std::string removeId = "Remove##" + std::to_string(index);
    const std::string popupId = "Update Lifter##" + std::to_string(index);

    if (ImGui::SmallButton(updateId.c_str())) {
        ImGui::OpenPopup(popupId.c_str());
        state_.isDataLoaded = false;
    }

    ImGui::SameLine();
    if (ImGui::SmallButton(removeId.c_str())) {
        meet_.removeLifter(lifter);
        return;
    }

    if (ImGui::BeginPopupModal(popupId.c_str(), nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |
                               ImGuiWindowFlags_NoMove)) {
        displayLifterUpdateForm(lifter);
        ImGui::EndPopup();
    }
}

void MeetWindow::displayLifterUpdateForm(Lifter &lifter) {
    const float inputWidth = width_ * INPUT_WIDTH_RATIO;
    if (!state_.isDataLoaded) {
        state_.updateName = lifter.getName();
        state_.updateAge = lifter.getAge();
        state_.updateBodyweight = lifter.getBodyweight();
        state_.updateSquat = lifter.getSquat();
        state_.updateBench = lifter.getBench();
        state_.updateDeadlift = lifter.getDeadlift();
        state_.updateGenderIndex = static_cast<int>(lifter.getGender());
        state_.updateUnitIndex = static_cast<int>(lifter.getUnits());
        state_.isDataLoaded = true;
    }

    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputText("Name", &state_.updateName);
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputInt("Age", &state_.updateAge);
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputDouble("Bodyweight", &state_.updateBodyweight, 0, 0, "%.3f");
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputDouble("Squat", &state_.updateSquat, 0, 0, "%.3f");
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputDouble("Bench", &state_.updateBench, 0, 0, "%.3f");
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputDouble("Deadlift", &state_.updateDeadlift, 0, 0, "%.3f");
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::Combo("Gender", &state_.updateGenderIndex, state_.genders, IM_ARRAYSIZE(state_.genders));
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::Combo("Units", &state_.updateUnitIndex, state_.units, IM_ARRAYSIZE(state_.units));

    if (ImGui::Button("Save", ImVec2(BUTTON_WIDTH, 0))) {
        updateLifter(lifter);
        ImGui::CloseCurrentPopup();
    }
    ImGui::SetItemDefaultFocus();
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(BUTTON_WIDTH, 0))) { ImGui::CloseCurrentPopup(); }
}

void MeetWindow::setupLifterTableColumns(const std::string &unitLabel) {
    ImGui::TableSetupColumn("Rank");
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Sex");
    ImGui::TableSetupColumn("Age");
    ImGui::TableSetupColumn(("Bodyweight " + unitLabel).c_str());
    ImGui::TableSetupColumn(("Squat " + unitLabel).c_str());
    ImGui::TableSetupColumn(("Bench " + unitLabel).c_str());
    ImGui::TableSetupColumn(("Deadlift " + unitLabel).c_str());
    ImGui::TableSetupColumn(("Total " + unitLabel).c_str());
    ImGui::TableSetupColumn("DOTS Score");
    ImGui::TableSetupColumn("GL Points");
    ImGui::TableSetupColumn("Wilks Score");
    ImGui::TableSetupColumn("Action");

    // Custom Centered Headers
    ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
    for (int column = 0; column < COLUMN_COUNT; column++) {
        ImGui::TableSetColumnIndex(column);
        const char *column_name = ImGui::TableGetColumnName(column); // Retrieve name passed to TableSetupColumn()
        const ImVec2 textSize = ImGui::CalcTextSize(column_name);
        const ImVec2 cellSize = ImGui::GetContentRegionAvail();
        const float paddingX = (cellSize.x - textSize.x) * 0.5f;
        if (paddingX > 0) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + paddingX);
        ImGui::TableHeader(column_name);
    }
}

void MeetWindow::displayLifterTable() {
    std::lock_guard<std::mutex> lock(meetMutex_);

    ImGui::BeginChild("LiftersList", ImVec2(0, 300), true);
    std::vector<Lifter> &lifters = meet_.getLifters();
    static ImGuiTableFlags flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg |
                                   ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable |
                                   ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
    const std::string unitLabel = meet_.getUnits() == Unit::KG ? "(kg)" : "(lbs)";

    if (ImGui::BeginTable("LiftersTable", COLUMN_COUNT, flags)) {
        setupLifterTableColumns(unitLabel);

        ImGuiListClipper clipper;
        clipper.Begin(static_cast<int>(lifters.size()));

        while (clipper.Step()) {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {
                displayLifterRow(lifters[i], i);
            }
        }

        clipper.End();
        ImGui::EndTable();
    }
    ImGui::EndChild();
}

void MeetWindow::displayLifterButtons() {
    if (ImGui::Button("Calculate Scores", ImVec2(BUTTON_WIDTH, 0))) {
        meet_.getResults();
        startSortingLifters(state_.sortOptions[state_.sortingIndex]);
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear Lifters", ImVec2(BUTTON_WIDTH, 0))) {
        meet_.clearLifters();
    }
}

void MeetWindow::displayStatusMessage() {
    ImGui::OpenPopup("Status");

    if (ImGui::BeginPopupModal("Status", nullptr, ImGuiWindowFlags_NoMove)) {
        ImGui::TextWrapped("%s", statusMessage_.c_str());
        const ImVec2 popupWidth = ImGui::GetContentRegionAvail();
        constexpr float buttonSizeX = 50.0f;
        const float paddingX = (popupWidth.x - buttonSizeX) * 0.5f;

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + paddingX);
        if (ImGui::Button("Dismiss", ImVec2(buttonSizeX, 0))) {
            statusMessage_.clear();
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void MeetWindow::handleImportDialog() {
    if (ImGuiFileDialog::Instance()->Display("ImportFileDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            startImportFile(ImGuiFileDialog::Instance()->GetFilePathName());
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void MeetWindow::handleExportDialog() {
    if (ImGuiFileDialog::Instance()->Display("ExportFileDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            startSaveFile(ImGuiFileDialog::Instance()->GetFilePathName());
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void MeetWindow::updateMeetState() {
    state_.meetName = meet_.getName();
    state_.date = meet_.getDate();
    state_.location = meet_.getLocation();
    state_.fedIndex = static_cast<int>(meet_.getFederation());
    state_.benchOnly = meet_.getBenchOnly();
    state_.equipped = static_cast<bool>(meet_.getEquipClass());
    state_.drugTested = meet_.getDrugTested();
    state_.meetUnitIndex = static_cast<int>(meet_.getUnits());
}

void MeetWindow::clearLifterState() {
    state_.lifterName.clear();
    state_.bodyweight = 0.0;
    state_.squat = 0.0;
    state_.bench = 0.0;
    state_.deadlift = 0.0;
    state_.age = 0;
    state_.unitIndex = 0;
    state_.genderIndex = 0;
}

void MeetWindow::updateLifter(Lifter &lifter) {
    lifter.setName(state_.updateName);
    lifter.setAge(state_.updateAge);
    lifter.setBodyweight(state_.updateBodyweight);
    lifter.setSquat(state_.updateSquat);
    lifter.setBench(state_.updateBench);
    lifter.setDeadlift(state_.updateDeadlift);
    lifter.setGender(static_cast<Gender>(state_.updateGenderIndex));
    lifter.setUnits(static_cast<Unit>(state_.updateUnitIndex));
    lifter.calculateScores(static_cast<bool>(meet_.getEquipClass()));

    if (lifter.getUnits() != meet_.getUnits()) {
        lifter.convertUnits();
    }
    startSortingLifters(state_.sortOptions[state_.sortingIndex]);
}

void MeetWindow::detectPlatformKeystrokes() {
    static bool ctrlPressed = false;

#ifdef _WIN32
    ctrlPressed = ImGui::GetIO().KeyCtrl;
#elif __APPLE__
            ctrlPressed = ImGui::GetIO().KeySuper;
#elif __linux__
        ctrlPressed = ImGui::GetIO().KeyCtrl;
#endif

    if (!isExporting_) {
        if (ctrlPressed && ImGui::IsKeyPressed(ImGuiKey_S)) {
            startSaveFile();
        }
    }

    if (!isImporting_) {
        if (ctrlPressed && ImGui::IsKeyPressed(ImGuiKey_O)) {
            ImGuiFileDialog::Instance()->OpenDialog("ImportFileDlgKey", "Choose File", ".json", config_);
        }
    }
}

void MeetWindow::setStatusMessage(const std::string &message) {
    std::lock_guard<std::mutex> lock(statusMutex_);
    statusMessage_ = message;
}

template<typename Task>
void MeetWindow::startAsyncTask(Task task, std::atomic<bool> &flag, std::future<bool> &future) {
    if (flag.exchange(true)) return;

    future = std::async(std::launch::async, [this, task = std::move(task), &flag]() mutable -> bool {
        struct TaskGuard {
            std::atomic<bool> &flag;
            ~TaskGuard() { flag = false; }
        } guard{flag};

        try {
            return task();
        } catch (const std::exception &e) {
            setStatusMessage(std::string("Task error: ") + e.what());
            return false;
        }
    });
}

void MeetWindow::startSortingLifters(const std::string &criteria) {
    Meet tempMeet; {
        std::lock_guard<std::mutex> lock(meetMutex_);
        tempMeet = meet_;
    }

    startAsyncTask([this, criteria, tempMeet = std::move(tempMeet)]() mutable -> bool {
        const bool success = tempMeet.sortLifters(criteria);
        if (success) {
            std::lock_guard<std::mutex> lock(meetMutex_);
            meet_ = std::move(tempMeet);
        } else {
            setStatusMessage("Failed to sort lifters!");
        }
        return success;
    }, isSorting_, sortFuture_);
}

void MeetWindow::startSaveFile(std::string filename) {
    if (filename.empty()) {
        filename = filepath_;
    }

    Meet snapshotMeet; {
        std::lock_guard<std::mutex> lock(meetMutex_);
        snapshotMeet = meet_;
    }

    startAsyncTask([this, filename = std::move(filename), snapshotMeet = std::move(snapshotMeet)]() -> bool {
        const bool success = MeetIO::exportMeetToJSON(snapshotMeet, filename);
        if (success) {
            std::lock_guard<std::mutex> lock(meetMutex_);
            filepath_ = filename;
        }
        setStatusMessage(success ? "Exported data to " + filename : "Failed to export data!");
        return success;
    }, isExporting_, exportFuture_);
}

void MeetWindow::startImportFile(const std::string &filename) {
    startAsyncTask([this, filename]() -> bool {
        Meet tempMeet;
        const bool success = MeetIO::importMeetFromJSON(tempMeet, filename);

        if (success) {
            tempMeet.sortLifters(state_.sortOptions[state_.sortingIndex]);
            std::lock_guard<std::mutex> lock(meetMutex_);
            meet_ = std::move(tempMeet);
            filepath_ = filename;
            updateMeetState();
        }

        setStatusMessage(success ? "Imported data from " + filename : "Failed to import data!");
        return success;
    }, isImporting_, importFuture_);
}

template<typename Future>
void MeetWindow::checkAsyncTask(std::atomic<bool>& isTaskRunning, Future& asyncTask) {
    if (isTaskRunning && asyncTask.valid() &&
        asyncTask.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
        try {
            asyncTask.get();
        } catch (const std::exception &e) {
            setStatusMessage(std::string("Failed to get async task: ") + e.what());
        }
    }
}

void MeetWindow::checkAsyncTasks() {
    checkAsyncTask(isImporting_, importFuture_);
    checkAsyncTask(isExporting_, exportFuture_);
    checkAsyncTask(isSorting_, sortFuture_);
}

void MeetWindow::display() {
    width_ = ImGui::GetWindowWidth();
    ImGui::Begin("Powerlifting Meet Calculator", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar);

    detectPlatformKeystrokes();

    handleImportDialog();
    handleExportDialog();

    displayMenuBar();
    displayAddLifterSection();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    displayTableModifiers();
    displayLifterTable();
    displayLifterButtons();

    checkAsyncTasks();
    if (!statusMessage_.empty()) {
        displayStatusMessage();
    }

    ImGui::End();
}
