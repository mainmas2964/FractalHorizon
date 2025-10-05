#include <string>
#include <vector>
#include <cstdint>
#include "ECS/component/component.h"
#include "ECS/entity/entity.h"
#include "system.h"
template<typename T>
template<typename Func>
void System<T>::update(Func&& func, int8_t iteration_step) {
    for (size_t i = 0; i < component.size(); i += iteration_step
    ) {
        func(component.comp[i]);
    }
}
template<typename T>
template<typename Func>
void System<T>::updateSingleComp(Func&& func, Entity e) {
    if (component.hasComponent(e)) {
        func(component.getComponent(e));
    }
}
// explicit template instantiation for common types if needed
template class System<int>;
template class System<float>;
template class System<double>; 
template class System<std::string>; 
// add more types as needed