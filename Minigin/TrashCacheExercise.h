#pragma once

#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>

#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl2.h>
#include <imgui_plot.h>


namespace dae {
    class TrashCacheExercise {
    public:
        void Render() const;

        ~TrashCacheExercise()
        {
            delete[] arrGameobj;
            delete[] arrGameobjAlt;
            delete[] arrInt;
        }
    private:
        static constexpr size_t arrSize{ 50'000'00 }; //Much larger wont work in debug

        struct Transform {
            float matrix[16]{
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };
        };

        class GameObject {
        public:
            Transform local{};
            int id{ 1 };
        };

        class GameObjectAlt {
        public:
            Transform* local;
            int id{ 1 };
        };

        // Declaration of static member variable
        static GameObjectAlt* arrGameobjAlt;
        static GameObject* arrGameobj;
        static int* arrInt;

        static std::vector<float> averageTimesGameObject;
        static std::vector<float> averageTimesGameObjectAlt;
        static std::vector<float> averageTimesIntArray;

        void CalculateAverages(GameObjectAlt* arr, int numRuns, std::vector<float>& averageTimes) const;
        void CalculateAverages(GameObject* arr, int numRuns, std::vector<float>& averageTimes) const;
        void CalculateAverages(int* arr, int numRuns, std::vector<float>& averageTimes) const;

        void DisplayPlot(const char* title, const std::vector<float>& data) const;
        void DisplayCombinedPlot(const char* title, const std::vector<float>& data1, const std::vector<float>& data2) const;
    };
}
