#include "main.h"

#include "../../engine/Engine/ui/UIManager.h"
#include "../../engine/Engine/ui/label.h"
#include "../../engine/Engine/ui/cursor.h"
#include "../../engine/Engine/ui/checkbox.h"

#include "src/ui/MainMenu.h"

#include "../../engine/Engine/models/ModelManager.h"
#include "../../engine/Engine/modelclass.h"

#include "../../engine/Engine/map/MapEntity.h"
#include "../../engine/Engine/reader/xml/XmlReader.h"
#include "../../engine/Engine/map/XmlLoad.h"

#include "../../engine/Engine/lightclass.h"

#include "../../engine/Engine/lightshaderclass.h"


bool App::init()
{
	if (!SystemClass::init()) {
		return false;
	}

	m_Position->SetPosition(173.0f, 26.0f, 154.0f);
	m_Position->SetRotation(15.0f, 50.0f, 0.0f);
	m_modelManager = m_Graphics->getModelManager();
	m_uiManager = m_Graphics->getUiManager();

	m_mapEntities = new MapEntity;

	loadUI();
	loadScene();

	return true;
}

void App::loadUI()
{
	Label* label = new Label;
	m_uiManager->add(label);
	label->Initialize(76, 28);
	label->Add("Hello World", 10, 0);
	label->setId(4);

	Label* label2 = new Label;
	m_uiManager->add(label2);
	label2->Initialize(76, 28);
	label2->Add("", 10, 20, 1.0f, 0.3f, 0.3f);
	label2->setId(5);

	Checkbox* checkbox = new Checkbox;
	m_uiManager->add(checkbox);
	checkbox->initialize();
	checkbox->Add("Free look camera", 10, 50);
	checkbox->setMarked(false);
	checkbox->setId(7);

	Cursor* cursor = new Cursor;
	m_uiManager->add(cursor);
	cursor->Initialize();
	cursor->setId(6);

	m_mainMenu = new MainMenu(this);
}

void App::loadScene()
{
	XmlLoad* xmlLoad = new XmlLoad(new XmlReader);
	xmlLoad->load("demotest.map", m_mapEntities, m_modelManager);
}

bool App::frame()
{
	if (!SystemClass::frame()) {
		return false;
	}

	frameUI();
	frameScene();

	return true;
}

void App::frameUI()
{
	int mouseX, mouseY;
	m_Input->GetMouseLocation(mouseX, mouseY);
	int mouseButtonPress = m_Input->getMouseButtonPress();
	bool freeLook = true;
	bool isUiFocued = m_uiManager->isFocused();

	if (!isUiFocued) {
		if (m_Input->IsKeyDown(DIK_ESCAPE)) {
			m_mainMenu->initialize();
			return;
		}
	}

	std::vector<AbstractGui*> elements = m_uiManager->getElements();
	int size = elements.size();
	for (int i = 0; i < size; i++) {
		if (elements[i]->getId() == 4) {
			Label* label = dynamic_cast<Label*>(elements[i]);
			char mouseString[128];
			sprintf(mouseString, "Fps: %d, Cpu: %3.2f%%, MouseX: %d, MouseY: %d, MouseButton: %d", m_Fps->GetFps(), m_Fps->GetCpuPercentage(), mouseX, mouseY, mouseButtonPress);
			label->Add(mouseString);
		}
		if (elements[i]->getId() == 5) {
			Label* label = dynamic_cast<Label*>(elements[i]);
			char string[128];
			sprintf(string, "Coord: %.2f:%.2f:%.2f, Render objects: %d, triangles: %d", m_Graphics->getCamera()->GetPosition().x, m_Graphics->getCamera()->GetPosition().y, m_Graphics->getCamera()->GetPosition().z, m_modelManager->getRenderCount(), m_modelManager->getTriangleCount());
			label->Add(string);
		}
		if (elements[i]->getId() == 6) {
			Cursor* cursor = dynamic_cast<Cursor*>(elements[i]);
			if (mouseButtonPress == MOUSE_BUTTON2 || isUiFocued) {
				cursor->setPosition(mouseX, mouseY);
				cursor->show();
			} else {
				cursor->hide();
			}
		}
		if (elements[i]->getId() == 7) {
			Checkbox* checkbox = dynamic_cast<Checkbox*>(elements[i]);
			freeLook = checkbox->getIsMarked();
		}
	}

	if (!isUiFocued) {
		if (!freeLook && (m_Input->IsKeyDown(DIK_W) || m_Input->IsKeyDown(DIK_D) || m_Input->IsKeyDown(DIK_S) || m_Input->IsKeyDown(DIK_A))) {
			D3DXVECTOR3 position = m_Position->getPosition();
			TerrainClass* terrain = dynamic_cast<TerrainClass*>(m_modelManager->getById(2));
			float height;
			if (terrain->getQuadTree()->GetHeightAtPosition(position.x, position.z, height)) {
				m_Position->SetPosition(position.x, height + 1.83f, position.z);
			}
		}
	}
}

void App::frameScene()
{
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	App* app = new App;
	
	if (app->init()) {
		app->run();
	}

	app->shutdown();
	delete app;
	app = 0;

	return 0;
}