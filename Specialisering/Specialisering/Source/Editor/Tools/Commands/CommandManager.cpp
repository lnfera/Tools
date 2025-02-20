#include "stdafx.h"
#include "AbstractCommand.h"
#include "CommandManager.h"
#include <Tools/ConsoleSystem.h>
#include <stack>

using namespace Tga;

static std::stack<std::shared_ptr<AbstractCommand>> locUndoStack;
static std::stack<std::shared_ptr<AbstractCommand>> locRedoStack;

void CommandManager::Clear()
{
	while (!locUndoStack.empty())
		locUndoStack.pop();
	while (!locRedoStack.empty())
		locRedoStack.pop();
	std::string msg = "Cleared Command Stacks";
	Tga::Console::LogMessage(msg, Tga::Console::Colors::Yellow);
}

void CommandManager::DoCommand(const std::shared_ptr<AbstractCommand>& aCommand) 
{
	std::string msg = "Executed ";
	msg += typeid(*aCommand).name();
	Tga::Console::LogMessage(msg, Tga::Console::Colors::White);

	aCommand->Execute();
	locUndoStack.push(aCommand);
	while (locRedoStack.empty() == false)
	{
		locRedoStack.pop();
	}
}

void CommandManager::Undo() 
{
	if (locUndoStack.empty() == false)
	{
		locUndoStack.top()->Undo();

		std::string msg = "Undid ";
		msg += typeid(*locUndoStack.top()).name();
		Tga::Console::LogMessage(msg, Tga::Console::Colors::White);

		locRedoStack.push(locUndoStack.top());
		locUndoStack.pop();
	}
	else
	{
		std::string msg = "Can't undo, undo stack is empty...";
		Tga::Console::LogMessage(msg, Tga::Console::Colors::Yellow);
	}
}

void CommandManager::Redo() 
{
	if (locRedoStack.empty() == false)
	{
		std::string msg = "Redo ";
		msg += typeid(*locRedoStack.top()).name();
		Tga::Console::LogMessage(msg, Tga::Console::Colors::White);

		locRedoStack.top()->Execute();
		locUndoStack.push(locRedoStack.top());
		locRedoStack.pop();
	}
	else
	{
		std::string msg = "Can't redo, redo stack is empty...";
		Tga::Console::LogMessage(msg, Tga::Console::Colors::Yellow);
	}
}

bool CommandManager::CanUndo()
{
	return !locUndoStack.empty();
}

bool CommandManager::CanRedo()
{
	return !locRedoStack.empty();
}