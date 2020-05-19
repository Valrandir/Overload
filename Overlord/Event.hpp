template<typename... TArgs>
class Event {
	using TFunc = void (*)(TArgs..., void* userdata);
	TFunc func{};
	void* userdata{};

public:
	Event() = default;

	void SetHandler(TFunc func, void* userdata = nullptr)
	{
		this->func = func;
		this->userdata = userdata;
	}

	void operator()(TArgs... args)
	{
		if(func)
			func(args..., userdata);
	}
};
