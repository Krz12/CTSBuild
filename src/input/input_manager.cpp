#include "key_state.hpp"
#include "key.hpp"
#include <vector>

using namespace std;

namespace input_manager
{
    vector<key_state> key_states;
    void init()
    {
        key_states.resize(static_cast<size_t>(key::Z) + 1);
    }
    void update();
    const key_state &get_key_state(const key &k)
    {
        return key_states[static_cast<size_t>(k)];
    }
}