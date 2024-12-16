#pragma once
#include <vector>
#include "EventMessage.h"
namespace Tga
{
	class Listener;

	class PostMaster
	{
	public:
		PostMaster();
		~PostMaster();

		void SendEventMessage(Message* aMsg);

		void SubscribeListenerToEvent(Listener* aListener, Event aEvent);
		void RemoveListenerFromEvent(Listener* aListener, Event aEvent);

		void ClearEventFromListeners(Event aEvent);
	private:
		std::vector<Listener*> myListeners[Event::COUNT];
	};
}