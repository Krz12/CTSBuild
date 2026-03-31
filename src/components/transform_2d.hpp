#include "math/vector2.hpp"

class transform_2d {
    private:
    class transform_2d_internal {
    protected:
        vector2<double> __position;
        double __rotation;
        vector2<double> __scale;
        
    public:
        transform_2d_internal(vector2<double> position = vector2<double>::ZERO) :
            __position(position),
            __rotation(0),
            __scale({1, 1}) {}
        
        void position(vector2<double> new_pos) { __position = new_pos; }
        vector2<double> position() const { return __position; }
        
        void rotation(double new_rotation) { __rotation = new_rotation; }
        double rotation() const { return __rotation; }
        
        void scale(vector2<double> new_scale) { __scale = new_scale; }
        vector2<double> scale() const { return __scale; }
    };
    public: 
    transform_2d_internal local, global;
    transform_2d_internal calculate_global(transform_2d_internal parent_global)
    {
        parent_global.position(parent_global.position() + local.position().rotate(parent_global.rotation()));
        parent_global.rotation(parent_global.rotation() + local.rotation());
        parent_global.scale(parent_global.scale() * local.scale());
        global = parent_global;
        return parent_global;
    }
};