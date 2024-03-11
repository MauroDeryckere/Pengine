#include "TrashCacheExercise.h"
#include <imgui_plot.h>
#include <algorithm>
#include <numeric>

namespace dae 
{
    TrashCacheExercise::GameObject* TrashCacheExercise::arrGameobj = new TrashCacheExercise::GameObject[arrSize]{};
    TrashCacheExercise::GameObjectAlt* TrashCacheExercise::arrGameobjAlt = new TrashCacheExercise::GameObjectAlt[arrSize]{};
    int* TrashCacheExercise::arrInt = new int[arrSize] {};
    std::vector<float> TrashCacheExercise::averageTimesGameObject;
    std::vector<float> TrashCacheExercise::averageTimesGameObjectAlt;
    std::vector<float> TrashCacheExercise::averageTimesIntArray;

    void TrashCacheExercise::Render() const {

        ImGui::Begin("Trash Cache Exercise - Window 1 (Integer Array)");

        static int numRunsIntArray = 10;
        ImGui::SliderInt("Num Runs (Int Array)", &numRunsIntArray, 1, 20);

        if (ImGui::Button("Calculate Averages (Int Array)")) {
            CalculateAverages(arrInt, numRunsIntArray, averageTimesIntArray);
        }

        DisplayPlot("Average Times (Int Array)", averageTimesIntArray);

        ImGui::End();

        ImGui::Begin("Trash Cache Exercise - Window 2 (GameObject and GameObjectAlt Arrays)");

        static int numRunsGameObjectArray = 10;
        ImGui::SliderInt("Num Runs (GameObject Array)", &numRunsGameObjectArray, 1, 20);

        static int numRunsGameObjectAltArray = 10;
        ImGui::SliderInt("Num Runs (GameObjectAlt Array)", &numRunsGameObjectAltArray, 1, 20);

        if (ImGui::Button("Calculate Averages (GameObject Array)")) {
            CalculateAverages(arrGameobj, numRunsGameObjectArray, averageTimesGameObject);
        }

        if (ImGui::Button("Calculate Averages (GameObjectAlt Array)")) {
            CalculateAverages(arrGameobjAlt, numRunsGameObjectAltArray, averageTimesGameObjectAlt);
        }

        DisplayPlot("Average Times (GameObject Array)", averageTimesGameObject);
        DisplayPlot("Average Times (GameObjectAlt Array)", averageTimesGameObjectAlt);

        if (!averageTimesGameObject.empty() && !averageTimesGameObjectAlt.empty()) {

            DisplayCombinedPlot("Average Times (Combined)", averageTimesGameObject, averageTimesGameObjectAlt);
        }

        ImGui::End();
    }

    void TrashCacheExercise::CalculateAverages(int* arr, int numRuns, std::vector<float>& averageTimes) const {
        averageTimes.clear();

        for (int stepIndex = 0; stepIndex < 11; ++stepIndex) {
            const int stepSize{ 1 << stepIndex };

            std::vector<long long> timings(numRuns);
            for (int i = 0; i < numRuns; ++i) {
                auto start = std::chrono::high_resolution_clock::now();

                for (size_t j = 0; j < arrSize; j += stepSize) {
                    arr[j] *= 2;
                }

                auto end = std::chrono::high_resolution_clock::now();
                timings[i] = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            }

            std::sort(timings.begin(), timings.end());
            timings.erase(timings.begin());
            timings.pop_back();

            float averageTime = std::accumulate(timings.begin(), timings.end(), 0.0f) / timings.size();
            averageTimes.push_back(averageTime);
        }
    }

    void TrashCacheExercise::CalculateAverages(GameObject* arr, int numRuns, std::vector<float>& averageTimes) const {
        averageTimes.clear();

        for (int stepIndex = 0; stepIndex < 11; ++stepIndex) {
            const int stepSize{ 1 << stepIndex };

            std::vector<long long> timings(numRuns);
            for (int i = 0; i < numRuns; ++i) {
                auto start = std::chrono::high_resolution_clock::now();

                for (size_t j = 0; j < arrSize; j += stepSize) {
                    arr[j].id *= 2; 
                }

                auto end = std::chrono::high_resolution_clock::now();
                timings[i] = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            }

            std::sort(timings.begin(), timings.end());
            timings.erase(timings.begin());
            timings.pop_back();

            float averageTime = std::accumulate(timings.begin(), timings.end(), 0.0f) / timings.size();
            averageTimes.push_back(averageTime);
        }
    }

    void TrashCacheExercise::CalculateAverages(GameObjectAlt* arr, int numRuns, std::vector<float>& averageTimes) const {
        averageTimes.clear();

        for (int stepIndex = 0; stepIndex < 11; ++stepIndex) {
            const int stepSize{ 1 << stepIndex };

            std::vector<long long> timings(numRuns);
            for (int i = 0; i < numRuns; ++i) {
                auto start = std::chrono::high_resolution_clock::now();

                for (size_t j = 0; j < arrSize; j += stepSize) {
                    arr[j].id *= 2;
                }

                auto end = std::chrono::high_resolution_clock::now();
                timings[i] = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            }

            std::sort(timings.begin(), timings.end());
            timings.erase(timings.begin());
            timings.pop_back();

            float averageTime = std::accumulate(timings.begin(), timings.end(), 0.0f) / timings.size();
            averageTimes.push_back(averageTime);
        }
    }

    void TrashCacheExercise::DisplayPlot(const char* title, const std::vector<float>& data) const {
        if (!data.empty()) {
            ImGui::PlotConfig plotConf;
            plotConf.values.ys = data.data();
            plotConf.values.count = static_cast<int>(data.size());
            plotConf.scale.min = 0.0f;
            plotConf.scale.max = *std::max_element(data.begin(), data.end()) + 100.0f;
            plotConf.tooltip.show = true;
            plotConf.grid_x.show = true;
            plotConf.grid_y.show = true;
            plotConf.frame_size = ImVec2(500, 200);
            ImGui::Plot(title, plotConf);
        }
    }

    void TrashCacheExercise::DisplayCombinedPlot(const char* title, const std::vector<float>& data1, const std::vector<float>& data2) const {
        if (!data1.empty() && !data2.empty()) {
            ImGui::PlotConfig combinedPlotConf;
            combinedPlotConf.values.ys_list = new const float* [2] { data1.data(), data2.data() };
            combinedPlotConf.values.count = static_cast<int>(std::min(data1.size(), data2.size()));
            combinedPlotConf.values.ys_count = 2;
            combinedPlotConf.scale.min = 0.0f;
            combinedPlotConf.scale.max = std::max(*std::max_element(data1.begin(), data1.end()), *std::max_element(data2.begin(), data2.end())) + 100.0f;
            combinedPlotConf.tooltip.show = true;
            combinedPlotConf.grid_x.show = true;
            combinedPlotConf.grid_y.show = true;
            combinedPlotConf.frame_size = ImVec2(500, 200);

            ImU32 color1 = IM_COL32(255, 0, 0, 255);
            ImU32 color2 = IM_COL32(0, 255, 0, 255);

            combinedPlotConf.values.colors = new const ImU32[2]{ color1, color2 };

            ImGui::Plot(title, combinedPlotConf);

            delete[] combinedPlotConf.values.ys_list;
            delete[] combinedPlotConf.values.colors;
        }
    }

}
