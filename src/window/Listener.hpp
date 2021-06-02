#pragma once

#include <entt/entt.hpp>

namespace entt {

template <typename Event, typename Listener>
concept Listenable = requires(Listener listener, const Event &event) {
  listener.on_event(event);
};

template <typename Event, typename Listener>
concept Callable = requires(Listener listener, const Event &event) {
  listener.operator()(event);
};

template <typename... Ts>
struct listener : Ts... {
  template <typename T>
  requires Callable<T, listener<Ts...>>
  auto on_event(T &&args) { return operator()(std::forward<T>(args)); }

private:
  using Ts::operator()...;
};

template <class... Ts>
listener(Ts...) -> listener<Ts...>;

template <typename Event, typename Listener>
requires Listenable<Event, Listener>
constexpr auto _get_ptr() {
  using Ptr = void (Listener::*)(const Event &);
  return static_cast<Ptr>(&Listener::on_event);
}

template <typename Event, typename Listener>
requires Listenable<Event, Listener>
void add_listener(entt::dispatcher &dispatcher, Listener *listener) {
  constexpr auto ptr = _get_ptr<Event, Listener>();
  dispatcher.sink<Event>().template connect<ptr>(listener);
}

template <typename Event, typename Listener>
requires Listenable<Event, Listener>
void remove_listener(entt::dispatcher &dispatcher, Listener *listener) {
  constexpr auto ptr = _get_ptr<Event, Listener>();
  dispatcher.sink<Event>().template disconnect<ptr>(listener);
}

} // namespace entt
