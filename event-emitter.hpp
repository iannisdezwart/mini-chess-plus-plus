#ifndef EVENT_EMITTER_HEADER
#define EVENT_EMITTER_HEADER

#include <bits/stdc++.h>

namespace events
{
	template <typename... Args>
	class EventEmitter
	{
		private:
			typedef std::function<void(Args...)> callback_t;
			callback_t callback = NULL;

		public:
			void add_listener(callback_t&& callback)
			{
				this->callback = std::move(callback);
			}

			void trigger(Args... args)
			{
				if (callback != NULL)
				{
					callback(args...);
				}
			}
	};
};

#endif