#include <string>
#include <map>
#include <any>

class Parameter final{
	public:
	enum class PARAM_PRESENCE {
		OPTIONAL = 0,
		REQUIRED
	};

	enum class VALUE_PRESENCE{
		OPTIONAL = 0,
		REQUIRED
	};

	private:
	char 			m_short_name = '\0';
	std::string 	m_full_name{};
	PARAM_PRESENCE	m_param_presence = PARAM_PRESENCE::OPTIONAL;
	VALUE_PRESENCE	m_value_presence = VALUE_PRESENCE::OPTIONAL;

	public:
	Parameter(char short_name, std::string full_name, PARAM_PRESENCE parameter_presence, VALUE_PRESENCE value_presence);
};

class ArgumentsParser final{
	public:
	enum class PARSE_STATUS{
		SUCCESS = 0,
		ERROR,
		NOT_PARSED
	};

	private:

	std::map<std::string, std::any> m_arguments{};
	PARSE_STATUS					m_parse_status = PARSE_STATUS::NOT_PARSED;

	public:
	explicit ArgumentsParser(uint32_t argc, char** argv);
	void AddParam(Parameter& const param);
	void Parse();
	std::any GetValueOfParameter(std::string& const param_name);
};
