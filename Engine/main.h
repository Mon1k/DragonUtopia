#pragma once

#include "../../engine/Engine/systemclass.h"
#include "../../engine/Engine/map/MapEntity.h"

class MainMenu;

class App : public SystemClass
{
public:
	virtual bool init();

	void loadUI();
	void loadScene();

	bool frame();
	void frameUI();
	void frameScene();

public:
	UIManager* m_uiManager;
	ModelManager* m_modelManager;
	MapEntity* m_mapEntities;

private:
	MainMenu* m_mainMenu;
};