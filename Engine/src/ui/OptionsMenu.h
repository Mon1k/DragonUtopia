#pragma once

#include "../../main.h"

#include "../../../../engine/Engine/ui/dialogs/window.h"
#include "../../../../engine/Engine/ui/button.h"
#include "../../../../engine/Engine/ui/checkbox.h"

class OptionsMenu
{
public:
	OptionsMenu::OptionsMenu(App* app)
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
		m_window->addHeader("Options", Window::HEADER_BUTTON_CLOSE);
		m_window->addBody();
		m_window->setId(m_app->m_uiManager->getNextId());
		m_window->addEventHandler(Window::EventType::WINDOW_CLOSE, [this] {
			m_app->m_uiManager->remove(m_window->getId());
		});
		shiftY = m_window->m_y + m_window->getHeader()->m_height + 5;

		Checkbox* shadowCheckbox = new Checkbox;
		m_window->addChild(shadowCheckbox);
		shadowCheckbox->initialize();
		shadowCheckbox->Add("Is shadow", m_window->m_x + m_window->m_width / 2 - shadowCheckbox->m_width / 2, shiftY + 20);
		shadowCheckbox->setMarked(Options::shadow_enabled);
		shadowCheckbox->addEventHandler(AbstractGui::EventType::MOUSE_DOWN, [this, shadowCheckbox] {
			Options::shadow_enabled = shadowCheckbox->getIsMarked();
		});
		shiftY += shadowCheckbox->m_height + 20;

		Checkbox* reflectionCheckbox = new Checkbox;
		m_window->addChild(reflectionCheckbox);
		reflectionCheckbox->initialize();
		reflectionCheckbox->Add("Reflection", m_window->m_x + m_window->m_width / 2 - reflectionCheckbox->m_width / 2, shiftY + 20);
		reflectionCheckbox->setMarked(Options::reflectionLevel == 0);
		reflectionCheckbox->addEventHandler(AbstractGui::EventType::MOUSE_DOWN, [this, reflectionCheckbox] {
			Options::reflectionLevel = reflectionCheckbox->getIsMarked() ? 0 : 2;
		});
		shiftY += reflectionCheckbox->m_height + 20;

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