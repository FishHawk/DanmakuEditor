#include "Ui.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include "../Game.hpp"
#include "../SpellManager.hpp"
#include "../components/Launchable.hpp"
#include "../components/Livetime.hpp"
#include "../components/Moveable.hpp"
#include "../components/Renderable.hpp"
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
  static auto &game = Game::get_instance();
  static auto &spell_names = game.spell_manager().spell_names;

  append_component_number("Entity", registry.size());
  append_component_number("Renderable", registry.view<Renderable>().size());
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

  static int spell_index = 0;
  if (ImGui::Button("Run")) {
    game.run_spell(spell_index);
  }
  ImGui::SameLine();

  if (ImGui::BeginCombo("Spell", spell_names[spell_index])) {
    for (int i = 0; i < spell_names.size(); ++i) {
      const bool is_selected = (spell_index == i);
      if (ImGui::Selectable(spell_names[i], is_selected))
        spell_index = i;
      if (is_selected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }

  ImPlot::SetNextPlotLimits(0, record_time, -0.001, 0.03);
  if (ImPlot::BeginPlot("System time", "", "", ImVec2(500, 200))) {
    ImPlot::SetLegendLocation(ImPlotLocation_::ImPlotLocation_NorthEast);
    for (auto &[name, index] : _system_names) {
      auto &queue = _system_times[index];
      plot_line(name, &queue);
    }
    ImPlot::EndPlot();
  }

  ImPlot::SetNextPlotLimits(0, record_time, -0.001, 100000);
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