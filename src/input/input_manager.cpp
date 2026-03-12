#include "input_manager.hpp"
#include "key_state.hpp"
#include "key.hpp"
#include "mouse_state.hpp"
#include "action.hpp"
#include <vector>

using namespace std;

namespace input_manager
{
    vector<key_state> key_states;
    vector<action> action_mappings;
    mouse_state mouse;

    void init()
    {
        key_states.resize(static_cast<size_t>(key::COUNT));
        action_mappings.resize(static_cast<size_t>(action_type::last_action) + 1);
    }
    void update(double delta_time)
    {
        GLFWwindow* win = glfwGetCurrentContext();
        if (!win) return;

        for (size_t i = 0; i < static_cast<size_t>(key::COUNT); ++i) {
            key k = static_cast<key>(i);
            bool pressed = false;

            if (is_mouse_button(k)) {
                int button = glfw_mouse_button(k);
                pressed = (button >= 0 && glfwGetMouseButton(win, button) == GLFW_PRESS);
            } else {
                pressed = glfwGetKey(win, glfw_key(k)) == GLFW_PRESS;
            }

            key_states[i].update(pressed, delta_time);
        }

        mouse.update(delta_time);
    }
    const key_state &get_key_state(const key &k)
    {
        return key_states[static_cast<size_t>(k)];
    }

    const mouse_state &get_mouse_state()
    {
        return mouse;
    }

    const key_state &get_action_state(const action &a)
    {
        return key_states[static_cast<size_t>(a.get_trigger())];
    }

    void map_action(const action_type &a,const key &k)
    {
        action_mappings[static_cast<size_t>(a)].set_trigger(k);
    }

    const key get_action_mapping(const action_type &a)
    {
        return action_mappings[static_cast<size_t>(a)].get_trigger();
    }
}
