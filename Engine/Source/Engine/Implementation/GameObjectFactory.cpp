#include "GameObjectFactory.h"

GameObjectFactory::GameObjectFactory(std::shared_ptr<Scene> scene) : scene(scene)
{
	gameObject = std::make_shared<GameObject>();
	SetupRandom();
}

GameObjectFactory::GameObjectFactory(std::shared_ptr<Scene> scene, std::string name) : scene(scene)
{
	gameObject = std::make_shared<GameObject>(name);
	SetupRandom();
}

void GameObjectFactory::SetupRandom()
{
	if (random == false)
	{
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		random = true;
	}
}

GameObjectFactory& GameObjectFactory::AddPosition(Vec3 vec)
{
	gameObject->Transform()->Position = vec;
	return *this;
}

GameObjectFactory& GameObjectFactory::AddRotation(Vec3 vec)
{
	gameObject->Transform()->Rotation = vec;
	return *this;
}

GameObjectFactory& GameObjectFactory::AddName(std::string name)
{
	gameObject->Name = name;
	return *this;
}

std::shared_ptr<GameObject> GameObjectFactory::Build()
{
	scene->AddObject(gameObject);
	return gameObject;
}

GameObjectFactory& GameObjectFactory::AddSpriteRenderable(std::shared_ptr<IRenderable> renderable)
{
	auto component = std::make_shared<SpriteRenderable>(gameObject, renderable);
	if (component != nullptr)
	{
		gameObject->AddComponent(std::move(component));
		return *this;
	}
	return *this;
}

GameObjectFactory& GameObjectFactory::AddMeshRenderable(std::shared_ptr<IRenderable> renderable)
{
	auto component = std::make_shared<MeshRenderable>(gameObject, renderable);
	if (component != nullptr)
	{
		gameObject->AddComponent(std::move(component));
		return *this;
	}
	return *this;
}

GameObjectFactory& GameObjectFactory::AddRandomRotation()
{
	gameObject->Transform()->Rotation = Vec3(static_cast<float>(std::rand() % 361),
		static_cast<float>(std::rand() % 361),
		static_cast<float>(std::rand() % 361));
	return *this;
}

GameObjectFactory& GameObjectFactory::AddPerspectiveCamera()
{
	auto perspective = std::make_shared<PerspectiveCamera>(gameObject->Transform(), 0, 0);
	auto component = std::make_shared<CameraComponent>(gameObject, perspective);
	if (component != nullptr)
	{
		gameObject->AddComponent(std::move(component));
		return *this;
	}
	return *this;
}

GameObjectFactory& GameObjectFactory::AddOrthoCamera()
{
	auto ortho = std::make_shared<OrthographicCamera>(gameObject->Transform(), 0, 0);

	auto component = std::make_shared<CameraComponent>(gameObject, ortho);
	if (component != nullptr)
	{
		gameObject->AddComponent(std::move(component));
		return *this;
	}
	return *this;
}