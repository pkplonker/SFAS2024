#include "ComponentDrawer.h"

ComponentDrawer::ComponentDrawer(std::weak_ptr<IComponent> component) : component(component)
{
}

ComponentDrawer::~ComponentDrawer()
{
}