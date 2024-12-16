#include "stdafx.h"
#include "PostMaster.h"
#include <cassert>
#include "Listener.h"
namespace Tga
{
	PostMaster::PostMaster()
	{
	}

	PostMaster::~PostMaster()
	{
	}

	void PostMaster::SendEventMessage(Message* aMsg)
	{
		for (Listener* listener : myListeners[aMsg->event])
		{
			listener->OnNotify(aMsg);
		}
	}

	void PostMaster::SubscribeListenerToEvent(Listener* aListener, Event aEvent)
	{

		for (Listener* listener : myListeners[aEvent])
		{
			if (listener == aListener)
			{
				assert(false && "Listener has already been subscribed to event");
			}
		}

		myListeners[aEvent].push_back(aListener);
	}

	void PostMaster::RemoveListenerFromEvent(Listener* aListener, Event aEvent)
	{
		for (int i = 0; i < myListeners[aEvent].size(); i++)
		{
			if (myListeners[aEvent][i] == aListener)
			{
				myListeners[aEvent].erase(myListeners[aEvent].begin() + i);
			}
		}
	}

	void PostMaster::ClearEventFromListeners(Event aEvent)
	{
		myListeners[aEvent].clear();
	}
}