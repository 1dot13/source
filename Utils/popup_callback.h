#ifndef POPUP_CALLBACK_CLASS
	#define POPUP_CALLBACK_CLASS

	#include "sgp.h"	

	class popupCallback;

	template <typename R, typename P1>	class popupCallbackFunction;
	template <typename R, typename P1, typename P2>	class popupCallbackFunction2;

	class popupCallback{
	public:
		popupCallback(void){}
		~popupCallback(void){}

		virtual void bind(void* newFun) = 0;
		virtual bool call(void) = 0;
	};

// returns something, takes three parameters

	template<typename R,typename P1,typename P2,typename P3>
	class popupCallbackFunction3: public popupCallback{
	protected:
		R (*fun)(P1,P2,P3);
		P1 param_1;
		P2 param_2;
		P3 param_3;
	public:
		popupCallbackFunction3(void);
		popupCallbackFunction3(void * newFun, P1 param, P2 param2, P3 param3);
		virtual void bind(void * newFun);
		virtual bool call(void);	
	};

	template<typename R,typename P1,typename P2,typename P3>
	popupCallbackFunction3<R,P1,P2,P3>::popupCallbackFunction3(void){
		this->fun = 0;
		this->param_1 = P1();
		this->param_2 = P2();
		this->param_3 = P3();
	};

	template<typename R,typename P1,typename P2,typename P3>
	popupCallbackFunction3<R,P1,P2,P3>::popupCallbackFunction3(void * newFun, P1 param, P2 param2, P3 param3){
		this->fun = static_cast< R(*)(P1,P2,P3)>(newFun);
		this->param_1 = param;
		this->param_2 = param2;
		this->param_3 = param3;
	};

	template<typename R,typename P1,typename P2,typename P3>
	void popupCallbackFunction3<R,P1,P2,P3>::bind(void * newFun){
			this->fun = static_cast< R(*)(P1,P2,P3)>(newFun);
	};

	template<typename R,typename P1,typename P2,typename P3>
	bool popupCallbackFunction3<R,P1,P2,P3>::call(void){
		return (bool)(this->fun)(this->param_1,this->param_2,this->param_3);
	};

	// three params, no return val

	template<typename P1,typename P2,typename P3>
	class popupCallbackFunction3<void,typename P1,typename P2,typename P3>: public popupCallback{
	protected:
		void (*fun)(P1,P2,P3);
		P1 param_1;
		P2 param_2;
		P3 param_3;
	public:
		popupCallbackFunction3(void);
		popupCallbackFunction3(void * newFun, P1 param, P2 param2, P3 param3);
		virtual void bind(void * newFun);
		virtual bool call(void);	
	};

	template<typename P1,typename P2,typename P3>
	popupCallbackFunction3<void,P1,P2,P3>::popupCallbackFunction3(void){
		this->fun = 0;
		this->param_1 = P1();
		this->param_2 = P2();
		this->param_3 = P3();
	};

	template<typename P1,typename P2,typename P3>
	popupCallbackFunction3<void,P1,P2,P3>::popupCallbackFunction3(void * newFun, P1 param, P2 param2, P3 param3){
		this->fun = static_cast< void(*)(P1,P2,P3)>(newFun);
		this->param_1 = param;
		this->param_2 = param2;
		this->param_3 = param3;
	};

	template<typename P1,typename P2,typename P3>
	void popupCallbackFunction3<void,P1,P2,P3>::bind(void * newFun){
		this->fun = static_cast< void(*)(P1,P2,P3)>(newFun);
	};

	template<typename P1,typename P2,typename P3>
	bool popupCallbackFunction3<void,P1,P2,P3>::call(void){
		(this->fun)(this->param_1,this->param_2,this->param_3);
		return 1;
	};

// returns something, takes two parameters

	template<typename R,typename P1,typename P2>
	class popupCallbackFunction2: public popupCallback{
	protected:
		R (*fun)(P1,P2);
		P1 param_1;
		P2 param_2;
	public:
		popupCallbackFunction2(void);
		popupCallbackFunction2(void * newFun, P1 param, P2 param2);
		virtual void bind(void * newFun);
		virtual bool call(void);	
	};

	template<typename R,typename P1,typename P2>
	popupCallbackFunction2<R,P1,P2>::popupCallbackFunction2(void){
			this->fun = 0;
			this->param_1 = P1();
			this->param_2 = P2();
	};

	template<typename R,typename P1,typename P2>
	popupCallbackFunction2<R,P1,P2>::popupCallbackFunction2(void * newFun, P1 param, P2 param2){
			this->fun = static_cast< R(*)(P1,P2)>(newFun);
			this->param_1 = param;
			this->param_2 = param2;
	};

	template<typename R,typename P1,typename P2>
	void popupCallbackFunction2<R,P1,P2>::bind(void * newFun){
			this->fun = static_cast< R(*)(P1,P2)>(newFun);
	};

	template<typename R,typename P1,typename P2>
	bool popupCallbackFunction2<R,P1,P2>::call(void){
		return (bool)(this->fun)(this->param_1,this->param_2);
	};

	// two params, no return val

	template<typename P1,typename P2>
	class popupCallbackFunction2<void, typename P1,typename P2>: public popupCallback{
	protected:
		void (*fun)(P1,P2);
		P1 param_1;
		P2 param_2;
	public:
		popupCallbackFunction2(void);
		popupCallbackFunction2(void * newFun, P1 param, P2 param2);
		virtual void bind(void * newFun);
		virtual bool call(void);	
	};

	template<typename P1,typename P2>
	popupCallbackFunction2<void,P1,P2>::popupCallbackFunction2(void){
			this->fun = 0;
			this->param_1 = P1();
			this->param_2 = P2();
	};

	template<typename P1,typename P2>
	popupCallbackFunction2<void,P1,P2>::popupCallbackFunction2(void * newFun, P1 param, P2 param2){
			this->fun = static_cast< void(*)(P1,P2)>(newFun);
			this->param_1 = param;
			this->param_2 = param2;
	};

	template<typename P1,typename P2>
	void popupCallbackFunction2<void,P1,P2>::bind(void * newFun){
			this->fun = static_cast< void(*)(P1,P2)>(newFun);
	};

	template<typename P1,typename P2>
	bool popupCallbackFunction2<void,P1,P2>::call(void){
		(this->fun)(this->param_1,this->param_2);
		return 1;
	};

// returns something, takes one parameter

	template <typename R, typename P1>
	class popupCallbackFunction : public popupCallback{
	protected:
		R (*fun)(P1);
		P1 param_1;
	public:
		popupCallbackFunction(void);
		popupCallbackFunction(void * newFun, P1 param);
		virtual void bind(void * newFun);
		virtual bool call(void);	
	};

	template<typename R,typename P1>
	popupCallbackFunction<R,P1>::popupCallbackFunction(void){
			this->fun = 0;
			this->param_1 = P1();
	};

	template<typename R,typename P1>
	popupCallbackFunction<R,P1>::popupCallbackFunction(void * newFun, P1 param){
			this->fun = static_cast< R(*)(P1)>(newFun);
			this->param_1 = param;
	};

	template<typename R,typename P1>
	void popupCallbackFunction<R,P1>::bind(void * newFun){
			this->fun = static_cast< R(*)(P1)>(newFun);
	};

	template<typename R,typename P1>
	bool popupCallbackFunction<R,P1>::call(void){
		return (bool)(this->fun)(this->param_1);
	}

	// no return type, one parameter
	template <typename P1>
	class popupCallbackFunction<void, typename P1> : public popupCallback{
	protected:
		void (*fun)(P1);
		P1 param_1;
	public:
		popupCallbackFunction(void);
		popupCallbackFunction(void * newFun, P1 param);
		virtual void bind(void * newFun);
		virtual bool call(void);	
	};

	template<typename P1>
	popupCallbackFunction<void,P1>::popupCallbackFunction(void){
			this->fun = 0;
			this->param_1 = P1();
	};

	template<typename P1>
	popupCallbackFunction<void,P1>::popupCallbackFunction(void * newFun, P1 param){
			this->fun = static_cast< void(*)(P1)>(newFun);
			this->param_1 = param;
	};

	template<typename P1>
	void popupCallbackFunction<void,P1>::bind(void * newFun){
			this->fun = static_cast< void(*)(P1)>(newFun);
	};


	template<typename P1>
	bool popupCallbackFunction<void,P1>::call(void){
		try { 
			(this->fun)(this->param_1);
		} catch (...) {
			return false;
		}
		return true;
	}

	// returns something, no parameter
	template <typename R>
	class popupCallbackFunction<typename R, void> : public popupCallback{
	protected:
		R (*fun)(void);
	public:
		popupCallbackFunction(void);
		popupCallbackFunction(void * newFun);
		virtual void bind(void * newFun);
		virtual bool call(void);	
	};

	template<typename R>
	popupCallbackFunction<R,void>::popupCallbackFunction(void){
			this->fun = 0;
	};

	template<typename R>
	popupCallbackFunction<R,void>::popupCallbackFunction(void * newFun){
			this->fun = static_cast< R(*)(void)>(newFun);
	};

	template<typename R>
	void popupCallbackFunction<R,void>::bind(void * newFun){
			this->fun = static_cast< R(*)(void)>(newFun);
	};

	template<typename R>
	bool popupCallbackFunction<R,void>::call(void){
		return (bool)(this->fun)();
	}


	//template<>
	//bool popupCallbackFunction<bool,void>::call(void){
	//	return (this->fun)();
	//}

	// no parameters/return types
	template <>
	class popupCallbackFunction<void, void> : public popupCallback{
	protected:
		void (*fun)(void);
	public:
		popupCallbackFunction(void){
			this->fun = 0;
		};
		popupCallbackFunction(void * newFun){
			this->fun = static_cast< void(*)(void)>(newFun);
		};
		virtual void bind(void * newFun){
			this->fun = static_cast< void(*)(void)>(newFun);
		};
		virtual bool call(void){
			try { 
				(this->fun)();
			} catch (...) {
				return false;
			}
			return true;
		};	
	};

	class popupCallbackLua : public popupCallback{
	protected:
		CHAR16 * functionName;
	public:
		popupCallbackLua(void);
		popupCallbackLua(CHAR16 * newFunctionName){
			this->functionName = newFunctionName;
		};
		virtual void bind(void * newFun){
			this->functionName = static_cast< CHAR16* >(newFun);
		};
		virtual bool call(void){
			return true;
		};	
	};

#endif