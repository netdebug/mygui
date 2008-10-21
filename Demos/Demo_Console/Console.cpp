/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "Console.h"

namespace demo
{

	Console::Console() :
		BaseLayout("Console.layout")
	{
	}

	void Console::initialise()
	{
		loadLayout();

		assignWidget(mListHistory, "list_History");
		assignWidget(mComboCommand, "combo_Command");
		assignWidget(mButtonSubmit, "button_Submit");

		MyGUI::WindowPtr window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != null) window->eventWindowButtonPressed = newDelegate(this, &Console::notifyWindowButtonPressed);

		mStringCurrent = mMainWidget->getUserString("Current");
		mStringError = mMainWidget->getUserString("Error");
		mStringSuccess = mMainWidget->getUserString("Success");
		mStringUnknow = mMainWidget->getUserString("Unknow");
		mStringFormat = mMainWidget->getUserString("Format");

		mAutocomleted = false;

		mComboCommand->eventComboAccept = newDelegate(this, &Console::notifyComboAccept);
		mComboCommand->eventKeyButtonPressed = newDelegate(this, &Console::notifyButtonPressed);
		mButtonSubmit->eventMouseButtonClick = newDelegate(this, &Console::notifyMouseButtonClick);
	}

	void Console::notifyWindowButtonPressed(MyGUI::WidgetPtr _sender, const std::string & _button)
	{
		if (_button == "close") {
			mMainWidget->hide();
		}
	}

	void Console::notifyMouseButtonClick(MyGUI::WidgetPtr _sender)
	{
		notifyComboAccept(mComboCommand);
	}

	void Console::notifyComboAccept(MyGUI::WidgetPtr _sender)
	{
		const Ogre::UTFString & command = _sender->getCaption();
		if (command == "") return;

		Ogre::UTFString key = command;
		Ogre::UTFString value;

		size_t pos = command.find(' ');
		if (pos != Ogre::UTFString::npos) {
			key = command.substr(0, pos);
			value = command.substr(pos + 1);
		}

		MapDelegate::iterator iter = mDelegates.find(key);
		if (iter != mDelegates.end()) {
			iter->second(key, value);
		}
		else {
			if (eventConsoleUnknowCommand.empty()) {
				addToConsole(mStringUnknow + "'" + key + "'");
			}
			else {
				eventConsoleUnknowCommand(key, value);
			}
		}

		_sender->setCaption("");
	}


	void Console::notifyButtonPressed(MyGUI::WidgetPtr _sender, MyGUI::KeyCode _key, MyGUI::Char _char)
	{
		size_t len = _sender->getCaption().length();
		MyGUI::EditPtr edit = _sender->castType<MyGUI::Edit>();
		if ((_key == MyGUI::KC_BACK) && (len > 0) && (mAutocomleted))
		{
			edit->deleteTextSelect();
			len = _sender->getCaption().length();
			edit->eraseText(len-1);
		}

		Ogre::UTFString command = _sender->getCaption();
		if (command.length() == 0) return;

		for (MapDelegate::iterator iter = mDelegates.begin(); iter != mDelegates.end(); ++iter)
		{
			if (iter->first.find(command) == 0)
			{
				if (command == iter->first) break;
				edit->setCaption(iter->first);
				edit->setTextSelect(command.length(), iter->first.length());
				mAutocomleted = true;
				return;
			}
		}
		mAutocomleted = false;
	}

	void Console::addToConsole(const Ogre::UTFString & _line)
	{
		mListHistory->addItem(_line);
		mListHistory->beginToItemLast();
	}

	void Console::clearConsole()
	{
		mListHistory->removeAllItems();
	}

	void Console::registerConsoleDelegate(const Ogre::UTFString & _command, DelegatePtr _delegate)
	{
		mComboCommand->addItem(_command);
		MapDelegate::iterator iter = mDelegates.find(_command);
		if (iter != mDelegates.end()) {
			MYGUI_LOG(Warning, "console - command '" << _command << "' already exist");
		}
		mDelegates[_command] = _delegate;
	}

	void Console::internalCommand(MyGUI::WidgetPtr _sender, const Ogre::UTFString & _key, const Ogre::UTFString & _value)
	{
		if (_key == "clear") {
			clearConsole();
		}
	}

} // namespace demo
