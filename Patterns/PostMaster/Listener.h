#pragma once
#include "EventMessage.h"

namespace Tga
{
	class Listener
	{
	public:
		Listener() {};
		virtual ~Listener() {};

		virtual void OnNotify(Message* /*aMsg*/) { __noop; };
	};
}