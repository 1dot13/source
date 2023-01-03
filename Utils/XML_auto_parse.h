#ifndef XML_AUTO_PARSE_H
#define XML_AUTO_PARSE_H

#include <vfs/Core/vfs_debug.h>
#include <vfs/Core/Interface/vfs_file_interface.h>
#include "XML_Parser.h"
#include <stack>
#include <algorithm>
#include <map>

#if !defined(TRUE)
#	define TRUE		1
#endif

namespace xml_auto
{
	class ITransition
	{
	public:
		void Register()		{ _ref_count++; }
		void UnRegister()	{ if(_ref_count > 0) _ref_count--; }
		virtual void Delete()
		{
			UnRegister();
			if(_ref_count == 0) delete this;
		}
		virtual void enter(const char* tag_name, IXMLParser::Attributes const& atts) {};
		virtual void leave(const char* tag_name, std::string const& data) {};
		virtual bool handleText() {return false;};
	protected:
		ITransition() : _ref_count(0) {};
		~ITransition() {};
	private:
		int _ref_count;
	};

	template<typename T>
	class TTransition : public ITransition
	{
	public:
		typedef typename T::States state_t;
		const state_t StartPoint, EndPoint;
		T* P_DATA; // parser data
	protected:
		TTransition(const state_t start, const state_t end, T* p_data = NULL)
			: StartPoint(start), EndPoint(end), P_DATA(p_data)
		{
			Register();
		};
		virtual ~TTransition()
		{};
	};

	template<typename T, typename States>
	class TBaseStructure
	{
	public:
		typedef TTransition<T>	transition_t;
		typedef States			state_t;
	private:
		// map : transition trigger (string) -> pointing at next state
		typedef std::map<std::string, transition_t*>	tr_state_t;

		// map : state -> outgoing transitions (of that state)
		typedef std::map<state_t,tr_state_t>			tr_map_t;
	public:
		transition_t* getTransition(state_t current_state, std::string const& str)
		{
			typename tr_map_t::iterator it = _state_tr_map.find(current_state);
			if(it != _state_tr_map.end())
			{
				typename tr_state_t::iterator trit = it->second.find(str);
				if( trit != it->second.end() )
				{
					return trit->second;
				}
				else if( (trit=it->second.find("")) != it->second.end() )
				{
					return trit->second;
				}
				else
				{
					return NULL;
				}
			}
			return NULL;
		}
		~TBaseStructure()
		{
			typename tr_map_t::iterator it = _state_tr_map.begin();
			for(; it != _state_tr_map.end(); ++it)
			{
				typename tr_state_t::iterator trit = it->second.begin();
				for(;trit != it->second.end(); trit++)
				{
					trit->second->Delete();
				}
			}
		}
	protected:
		void newTransition(std::string const& tag, transition_t* tr)
		{
			_state_tr_map[tr->StartPoint][tag] = tr;
			_state_tr_map[tr->EndPoint][tag] = tr;
			// we use one transition twice -> increment ref counter
			tr->Register(); 
		}

		tr_map_t _state_tr_map;
	};

	template<typename T>
	class TGenericXMLParser : public IXMLParser
	{
	public:
		typedef TBaseStructure<T, typename T::States> struct_t;
		TGenericXMLParser(struct_t* base, IXMLParser* caller = NULL)
			: IXMLParser("",NULL,caller), m_base(base), m_state(T::STATE_NONE)
		{
			parser = XML_ParserCreate(NULL);
			setParser(&parser);
		};
		~TGenericXMLParser()
		{
			if(parser)
			{
				XML_ParserFree(parser);
			}
		}
		virtual void onStartElement(const XML_Char* name, const XML_Char** atts)
		{
			typename struct_t::transition_t* tr = m_base->getTransition(m_state.state(), name);
			if(tr && m_state.stateTransition(tr->StartPoint, LazyTrue(), tr->EndPoint))
			{
				m_tr_stack.push(tr);
				tr->enter(name, IXMLParser::Attributes(atts));
			}
			sCharData = "";
		}
		virtual void onEndElement(const XML_Char* name)
		{
			typename struct_t::transition_t* tr = m_base->getTransition(m_state.state(), name);
			if(tr && m_state.stateTransition(tr->EndPoint, LazyTrue(), tr->StartPoint))
			{
				SGP_THROW_IFFALSE(tr == m_tr_stack.top(),
					_BS(L"Transition associated to tag [") << name << L"] doesn't correspond to saved transition" << _BS::wget);
				tr->leave(name, sCharData);
				m_tr_stack.pop();
			}
		};
		virtual void onTextElement(const XML_Char *str, int len)
		{
			if(!m_tr_stack.empty())
			{
				typename struct_t::transition_t* tr = m_tr_stack.top();
				if(tr && tr->handleText())
				{
					sCharData.append(str,len);
				}
			}
		};

		void parseBuffer(vfs::Byte* buffer, vfs::size_t length)
		{
			this->grabParser();
			XML_Parser& _parser = this->getParser();
			if(!XML_Parse(_parser, buffer, length, TRUE))
			{
				std::wstringstream wss;
				wss << L"XML Parser Error : "
					<< vfs::String::as_utf16(XML_ErrorString(XML_GetErrorCode(_parser)))
					<< L" in line "
					<< XML_GetCurrentLineNumber(_parser);
				SGP_THROW(wss.str().c_str());
			}
		}

		void parseFile(vfs::tReadableFile* pFile)
		{
			if(!pFile)
			{
				return;
			}
			vfs::COpenReadFile rfile(pFile);

			vfs::size_t size = rfile->getSize();
			std::vector<vfs::Byte> buffer(size+1);

			SGP_TRYCATCH_RETHROW( rfile->read(&buffer[0],size), L"" );
			buffer[size] = 0;
			
			SGP_TRYCATCH_RETHROW( this->parseBuffer(&buffer[0], size),
				_BS(L"error in file : ") << pFile->getPath() << _BS::wget);
		}

		void parseFile(vfs::Path const& sFile)
		{
			vfs::tReadableFile* file = getVFS()->getReadFile(sFile);
			SGP_THROW_IFFALSE(file, _BS(L"Could not find file : ") << sFile << _BS::wget);

			SGP_TRYCATCH_RETHROW( this->parseFile(file),
				_BS(L"error in file : ") << sFile << _BS::wget);
		}

	private:
		XML_Parser parser;

		typedef IXMLParser::ParserState<typename T::States> ParserState_t;
		struct_t*			m_base;

		typedef std::stack<typename struct_t::transition_t*> tr_stack_t;
		tr_stack_t			m_tr_stack;
		ParserState_t		m_state;
	};
}

#define PARSER_CLASS_MACRO(class_name, states) \
	class class_name : public states, public xml_auto::TBaseStructure<class_name, states::States>

#define PURE_TRANSITION(class_name, main_type) \
	class class_name : public xml_auto::TTransition<main_type>{ \
	public: \
		typedef main_type::States state_t; \
		typedef xml_auto::TTransition<main_type> super_t; \
		class_name(const state_t start, const state_t end) : super_t(start, end) \
		{} \
		virtual ~class_name() \
		{} \
	};

#define DEFINE_TRANSITION(class_name, base_type, handle_text) \
	class class_name : public xml_auto::TTransition<base_type>{ \
	public: \
		typedef xml_auto::TTransition<base_type> super_t; \
		typedef base_type::States state_t; \
		class_name(const state_t start, const state_t end, base_type* base_obj) : super_t(start,end,base_obj) {}; \
		virtual ~class_name() {}; \
		virtual bool handleText() {return handle_text;}; \

#define DEFINE_TRANSITION_WDATA(class_name, base_type, handle_text, tr_data_type) \
	class class_name : public xml_auto::TTransition<base_type>{ \
		tr_data_type T_DATA; \
	public: \
		typedef xml_auto::TTransition<base_type> super_t; \
		typedef base_type::States state_t; \
		class_name(const state_t start, const state_t end, base_type* base_obj) : super_t(start,end,base_obj) {}; \
		virtual ~class_name() {}; \
		virtual bool handleText() {return handle_text;}; \

#define FINISH_TRANSITION };

#define TRANSITION_ENTER virtual void enter(const char* tag_name, IXMLParser::Attributes const& atts)
#define TRANSITION_LEAVE virtual void leave(const char* tag_name, std::string const& char_data)

#define TR_HANDLE(from_state, trigger, to_state, handler_class_type)	newTransition((trigger), new handler_class_type((from_state),(to_state),this));
#define TR_PASS(from_state, trigger, to_state)							newTransition((trigger), new NOOP((from_state),(to_state)));

#endif // XML_AUTO_PARSE_H
