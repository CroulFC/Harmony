#include <string>
#include <map>
#include <any>

class ArgumentsParser final{
	public:
	enum class PARSE_STATUS{
		SUCCESS = 0,
		ERROR,
		NOT_PARSED
	};

	class Parameter final{
		public:
		enum class PARAM_PRESENCE {
			OPTIONAL = 0,
			REQUIRED
		};

		enum class VALUE_PRESENCE{
			NOT_REQUIRED = 0,
			REQUIRED,
			OPTIONAL
		};

		static constexpr char 			EMPTY_SHORT_NAME = '\0';
		inline static const std::string EMPTY_FULL_NAME = std::string("");

		private:
		char 			m_short_name = '\0';
		std::string 	m_full_name{};
		PARAM_PRESENCE	m_param_presence = PARAM_PRESENCE::OPTIONAL;
		VALUE_PRESENCE	m_value_presence = VALUE_PRESENCE::OPTIONAL;

		public:
		bool CompareShortName(const char ch) {return m_short_name == ch;}
		bool ComareFullName(const std::string &str) {return m_full_name == str;}
		PARAM_PRESENCE GerParameterPresence() {return m_param_presence;}
		VALUE_PRESENCE GetValuePresence() {return m_value_presence;}

		Parameter() = delete;
		Parameter(char short_name, const std::string &full_name, PARAM_PRESENCE parameter_presence, VALUE_PRESENCE value_presence);
		bool operator ==(const Parameter &p);
		bool operator <(const Parameter &p);
		bool operator >(const Parameter &p);
	};

	ArgumentsParser() = delete;
	explicit ArgumentsParser(int argc, char** argv);
	void AddParam(const Parameter& param);
	PARSE_STATUS Parse();
	std::pair<bool, const char*> GetValueOfParameter(const Parameter &param);

	private:
	int		m_argc{};
	char** 	m_argv{};
	//Pair to support params what not require value (flags like --help)
	std::map<Parameter, std::pair<bool, const char*>> 	m_arguments{};
	PARSE_STATUS										m_parse_status = PARSE_STATUS::NOT_PARSED;

	bool IsParseResultCorrect();
	std::pair<bool, const char*> FindValueOfParam(const Parameter &param, const int index);
};
