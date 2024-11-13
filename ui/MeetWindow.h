#pragma once

#include <future>
#include <mutex>
#include "ImGuiFileDialog.h"
#include "MeetWindowState.h"
#include "../Meet.h"

class MeetWindow {
private:
    Meet &meet_;
    MeetWindowState state_;
    std::mutex meetMutex_;
    std::mutex statusMutex_;
    std::future<bool> importFuture_;
    std::future<bool> exportFuture_;
    std::future<bool> sortFuture_;
    std::atomic<bool> isImporting_{false};
    std::atomic<bool> isExporting_{false};
    std::atomic<bool> isSorting_{false};
    std::string statusMessage_;
    std::string filepath_;
    IGFD::FileDialogConfig config_;
    float width_ = 0.0f;

public:
    explicit MeetWindow(Meet &meet);

    // Display Methods
    void display();
    void displayMenuBar();
    void displayAddLifterSection();
    void displayTableModifiers();
    void displayLifterRow(Lifter &lifter, size_t index);
    void displayLifterActions(Lifter &lifter, size_t index);
    void displayLifterUpdateForm(Lifter &lifter);
    void displayLifterTable();
    void displayLifterButtons();
    void displayStatusMessage();
    void handleImportDialog();
    void handleExportDialog();
    static void setupLifterTableColumns(const std::string &unitLabel);

    // State Management
    void updateMeetState();
    void clearLifterState();
    void updateLifter(Lifter &lifter);
    void detectPlatformKeystrokes();

    // Async
    template<class Task>
    void startAsyncTask(Task task, std::atomic<bool> &flag, std::future<bool> &future);
    template<class Future>
    void checkAsyncTask(std::atomic<bool>& isTaskRunning, Future& asyncTask);
    void startSortingLifters(const std::string &criteria);
    void setStatusMessage(const std::string& message);
    void checkAsyncTasks();

    // File Management (also Async)
    void startSaveFile(std::string filename = "");
    void startImportFile(const std::string &filename);
};
