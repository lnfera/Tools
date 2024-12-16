#pragma once
namespace Tga
{
	enum Event
	{
		DEFAULT = 0,
		COUNT,
	};

	struct Message
	{
		Event event = Event::DEFAULT;
		void* data;
	};
}