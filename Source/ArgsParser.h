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

		const char 			EMPTY_SHORT_NAME = '\0';
		const std::string 	EMPTY_FULL_NAME = std::string("");

		private:
		char 			m_short_name = '\0';
		std::string 	m_full_name{};
		PARAM_PRESENCE	m_param_presence = PARAM_PRESENCE::OPTIONAL;
		VALUE_PRESENCE	m_value_presence = VALUE_PRESENCE::OPTIONAL;

		public:
		Parameter() = delete;
		Parameter(char short_name, std::string full_name, PARAM_PRESENCE parameter_presence, VALUE_PRESENCE value_presence);
	};

	private:
	std::map<const Parameter&, char*> 	m_arguments{};
	PARSE_STATUS						m_parse_status = PARSE_STATUS::NOT_PARSED;

	public:
	ArgumentsParser() = delete;
	explicit ArgumentsParser(int argc, char** argv);
	void AddParam(const Parameter& param);
	PARSE_STATUS Parse();
	std::any GetValueOfParameter(const std::string& param_name);
};
