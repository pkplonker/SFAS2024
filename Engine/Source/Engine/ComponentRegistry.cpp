#include "ComponentRegistry.h"

#include "Implementation/CameraComponent.h"
#include "Implementation/MeshComponent.h"
#include "implementation/SpriteComponent.h"


void ComponentRegistry::RegisterComponents()
{
    RegisterComponent<MeshComponent>("Mesh");
    RegisterComponent<SpriteComponent>("Sprite");
    RegisterComponent<CameraComponent>("Camera");
}
