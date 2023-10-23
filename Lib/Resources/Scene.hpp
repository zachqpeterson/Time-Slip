#pragma once

#include "ResourceDefines.hpp"

struct Scene
{
public:
	void Create();
	~Scene();
	void Destroy();

	bool Update();

	void AddModel(Model* model);
	void SetSkybox(Skybox* newSkybox);

public:
	String				name{};
	HashHandle			handle;

	Camera				camera{};

	Skybox*				skybox{ nullptr };

	Vector<Model*>		models{}; //TODO: Objects
};