#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2List.h"
#include "SDL\include\SDL.h"

class ModuleTextures : public Module
{
public:
	ModuleTextures(Application* app, bool start_enabled = true);
	~ModuleTextures();

	bool Init();
	bool CleanUp();

	SDL_Texture* const Load(const char* path);
	void Unload(SDL_Texture* texture);

	void ModuleTextures::GetTextureSize(const SDL_Texture* texture, uint& width, uint& height) const { SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*)&width, (int*)&height); }

public:
	p2List<SDL_Texture*> textures;
};