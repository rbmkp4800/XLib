#pragma once

namespace XLib
{
	class RawDelegate
	{
		template <typename ReturnType, typename ... ArgsTypes>
		friend class Delegate;

	private:
		void *object, *method;

	public:
		inline RawDelegate() : object(nullptr), method(nullptr) {}
		inline RawDelegate(void* _object, void* _method) : object(_object), method(_method) {}

		inline void* getObject() { return object; }
		inline void* getMethod() { return method; }
	};

	template <typename ReturnType, typename ... ArgsTypes>
	class Delegate
	{
	public:
		template <class Class>
		using Method = ReturnType(Class::*)(ArgsTypes ...);

	private:
		class DummyClass { int dummy; };

		DummyClass *object;
		Method<DummyClass> method;

	public:
		inline Delegate() : object(nullptr), method(nullptr) {}
		inline Delegate(const RawDelegate& raw)
		{
			object = (DummyClass*)raw.object;
			*((void**)&method) = raw.method;
		}

		template <class Class>
		inline Delegate(Class& _object, Method<Class> _method) : object((DummyClass*)&_object)
			{ *((Method<Class>*)&method) = _method; }

		template <class Class>
		inline Delegate(Class* _object, Method<Class> _method) : object((DummyClass*)_object)
			{ *((Method<Class>*)&method) = _method; }

		inline RawDelegate toRaw()
		{
			RawDelegate raw;
			raw.object = object;
			*((Method<DummyClass>*)&raw.method) = method;
			return raw;
		}

		inline ReturnType call(ArgsTypes ... args) { return (object->*method)(args ...); }

		inline bool isInitialized() { return method ? true : false; }
	};
}