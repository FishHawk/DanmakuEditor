#include "Ui.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include "../Game.hpp"
#include "../TestCase.hpp"
#include "../components/Launchable.hpp"
#include "../components/Livetime.hpp"
#include "../components/Moveable.hpp"
#include "../components/Sprite.hpp"
#include "../util/Timer.hpp"
#include "../util/debug.hpp"

template <typename T>
void append_number(std::deque<T> &queue, const T value) {
  queue.pop_back();
  queue.push_front(value);
}

template <typename T>
void plot_line(const std::string &name, std::deque<T> *data) {
  ImPlot::PlotLineG(
      name.c_str(),
      [](void *data, int idx) {
        auto queue = static_cast<std::deque<T> *>(data);
        return ImPlotPoint{(double)idx / 60, queue->at(idx)};
      },
      (void *)data,
      data->size());
}

Ui::Ui() {
  for (auto &q : _system_times) {
    q.resize(record_time * 60, 0);
  }

  for (auto &q : _component_numbers) {
    q.resize(record_time * 60, 0);
  }
};

void Ui::update(entt::registry &registry) {
  auto &game = Game::get_instance();

  append_component_number("Entity", registry.size());
  append_component_number("Sprite", registry.view<Sprite>().size());
  append_component_number("Moveable", registry.view<Moveable>().size());
  append_component_number("Launchable", registry.view<Launchable>().size());
  append_component_number("Livetime", registry.view<Livetime>().size());

  // Start new imgui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();



  // Define gui
  ImGui::Begin("Console");
  ImGui::SetWindowPos(ImVec2(600.f, 0.f));
  ImGui::SetWindowSize(ImVec2(510.f, 800.f));

  static int current_case = 0;
  if (ImGui::Button("Run")) {
    game.run_test_case(current_case);
  }
  ImGui::SameLine();
  ImGui::Combo(
      "Test case",
      &current_case,
      TestCase::case_name.data(),
      TestCase::case_name.size());

  ImPlot::SetNextPlotLimits(0, record_time, -0.001, 0.03);
  if (ImPlot::BeginPlot("System time", "", "", ImVec2(500, 200))) {
    ImPlot::SetLegendLocation(ImPlotLocation_::ImPlotLocation_NorthEast);
    for (auto &[name, index] : _system_names) {
      auto &queue = _system_times[index];
      plot_line(name, &queue);
    }
    ImPlot::EndPlot();
  }

  ImPlot::SetNextPlotLimits(0, record_time, -0.001, 10000);
  if (ImPlot::BeginPlot("Component number", "", "", ImVec2(500, 200))) {
    ImPlot::SetLegendLocation(ImPlotLocation_::ImPlotLocation_NorthEast);
    for (auto &[name, index] : _component_names) {
      auto &queue = _component_numbers[index];
      plot_line(name, &queue);
    }
    ImPlot::EndPlot();
  }

  ImGui::End();

  // Render gui
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Ui::append_system_time(const std::string &name, double value) {
  auto index = _system_names.at(name);
  auto &queue = _system_times.at(index);
  append_number(queue, value);
}

void Ui::append_component_number(const std::string &name, unsigned int value) {
  auto index = _component_names.at(name);
  auto &queue = _component_numbers.at(index);
  append_number(queue, value);
}