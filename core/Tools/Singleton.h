#pragma once

template <class T>
class Singleton
{
    public:
        static T& get();
    protected:

		static T _Data;
};

/*** EXAMPLE
>>HPP
	class Window : public Singleton<Window>
	{
		friend class Singleton<Window>;

		public:

		protected:
			Window();
			~Window();
	}

>>CPP
	template <class T> T Singleton<T>::_Data = T();

	template <class T> T& Singleton<T>::Get()
	{
		return _Data;
	}
***/

template <class T> T Singleton<T>::_Data = T();

template <class T> T& Singleton<T>::get()
{
    return _Data;
}


