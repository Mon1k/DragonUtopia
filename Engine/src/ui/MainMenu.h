#pragma once

#include "../../main.h"

#include "../../../../engine/Engine/reader/xml/XmlReader.h"
#include "../../../../engine/Engine/map/XmlLoad.h"

#include "../../../../engine/Engine/ui/dialogs/window.h"
#include "../../../../engine/Engine/ui/dialogs/OpenDialog.h"
#include "../../../../engine/Engine/ui/button.h"

#include "OptionsMenu.h"

class MainMenu
{
public:
	MainMenu::MainMenu(App* app)
	{
		m_app = app;
		m_window = 0;
	}

	void initialize()
	{
		int width = 640;
		int height = 480;
		int shiftY = 0;

		m_window = new Window;
		m_app->m_uiManager->add(m_window);
		m_window->Initialize(width, height, Options::screen_width / 2 - width / 2, Options::screen_height / 2 - height / 2);
		m_window->addBody();
		m_window->setId(m_app->m_uiManager->getNextId());
		m_window->addEventHandler(Window::EventType::WINDOW_CLOSE, [this] {
			m_app->m_uiManager->remove(m_window->getId());
			m_window = 0;
		});
		shiftY = m_window->m_y + 10;

		Button* resumeButton = new Button;
		m_window->addChild(resumeButton);
		resumeButton->initialize(80, 28);
		resumeButton->Add("Resume", m_window->m_x + m_window->m_width / 2 - resumeButton->m_width / 2, shiftY);
		resumeButton->addEventHandler(AbstractGui::EventType::MOUSE_DOWN, [this] {
			m_window->hide();
			m_app->m_uiManager->remove(m_window->getId());
		});
		shiftY += resumeButton->m_height + 10;

		Button* loadMapButton = new Button;
		m_window->addChild(loadMapButton);
		loadMapButton->initialize(80, 28);
		loadMapButton->Add("Load", m_window->m_x + m_window->m_width / 2 - loadMapButton->m_width / 2, shiftY);
		loadMapButton->addEventHandler(AbstractGui::EventType::MOUSE_DOWN, [this] {
			OpenDialog* dialog = new OpenDialog;
			m_app->m_uiManager->add(dialog);
			dialog->initialize();
			dialog->addFilter("map");
			dialog->setId(m_app->m_uiManager->getNextId());
			dialog->addEventHandler(FileChooser::EventType::FILE_CHOOSE, [this, dialog] {
				dialog->hide();
				m_app->m_uiManager->remove(dialog->getId());
				XmlLoad* xmlLoad = new XmlLoad(new XmlReader);
				xmlLoad->load(dialog->getCurrentFilePath(), m_app->m_mapEntities, m_app->m_modelManager);
			});
			dialog->show();
			dialog->proccesedEventHandlers(Window::EventType::WINDOW_OPEN);
		});
		shiftY += loadMapButton->m_height + 10;

		Button* optionsButton = new Button;
		m_window->addChild(optionsButton);
		optionsButton->initialize(80, 28);
		optionsButton->Add("Options", m_window->m_x + m_window->m_width / 2 - optionsButton->m_width / 2, shiftY);
		optionsButton->addEventHandler(AbstractGui::EventType::MOUSE_DOWN, [this] {
			OptionsMenu* menuOptions = new OptionsMenu(m_app);
			menuOptions->initialize();
			m_window->hide();
			menuOptions->getWindow()->addEventHandler(Window::EventType::WINDOW_CLOSE, [this] {
				m_window->show();
			});
		});
		shiftY += optionsButton->m_height + 10;

		Button* exitButton = new Button;
		m_window->addChild(exitButton);
		exitButton->initialize(80, 28);
		exitButton->Add("Exit", m_window->m_x + m_window->m_width / 2 - exitButton->m_width / 2, shiftY);
		exitButton->addEventHandler(AbstractGui::EventType::MOUSE_DOWN, [this] {
			m_app->m_Done = true;
		});
		shiftY += exitButton->m_height + 10;

		m_window->show();
	}

	Window* getWindow()
	{
		return m_window;
	}

protected:
	App* m_app;
	Window* m_window;
};