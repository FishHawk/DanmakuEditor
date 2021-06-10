#pragma once

#include <entt/entt.hpp>

namespace entt {

template <typename Event, typename Listener>
concept Listenable = requires(Listener listener, const Event &event) {
  listener(event);
};

template <typename... Ts>
struct base_listener : Ts... { using Ts::operator()...; };

template <typename... Ts>
struct listener : protected base_listener<Ts...> {
  listener(Ts... args) : base_listener<Ts...>(args...) {}

  template <typename T>
  requires Listenable<T, base_listener<Ts...>>
  void operator()(T v) { base_listener<Ts...>::operator()(v); }
};

template <typename Event, typename Listener>
constexpr auto _get_ptr() {
  using Ptr = void (Listener::*)(const Event &);
  return static_cast<Ptr>(&Listener::operator());
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
