#include "ECS/system/system.cpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include "ECS/component/component.cpp"
#include "ECS/entity/entity.h"
#include "test.h"
// function to execute ECS test
void execute1() {
    // adding structs for components:
    // health component struct
    struct health {
        int hp;
    };
    // attack component struct
    struct attack {
        int damage;
    };

    // starting ECS system test
    std::cout << "start test ECS System" << std::endl;
    const size_t maxEntities = 100;

    // create components:
    // health component
    Component<health> healthComp(maxEntities);
    // attack component
    Component<attack> attackComp(maxEntities);

    // create system:
    SystemECS<attack> attackEntitySystem(attackComp);
    
    // create entities
    Entity e1{0};
    Entity e2{1};

    // add components:
    // health components
    healthComp.addComponent(e1, health{100});
    healthComp.addComponent(e2, health{80});
    // attack components
    attackComp.addComponent(e1, attack{20});
    attackComp.addComponent(e2, attack{15});

    // updating system with function
    attackEntitySystem.update(/*function with lambda body*/[&healthComp](attack& atk, size_t idx) {
        Entity ent{static_cast<uint32_t>(idx)}; // create entity from index
        
        if (healthComp.hasComponent(ent)) {
            auto& h = healthComp.getComponent(ent);
            h.hp -= atk.damage; // apply damage
            std::cout << "entity " << ent.id << " took " << atk.damage << " damage, hp=" << h.hp << '\n';
            // output result
        }
    });
}