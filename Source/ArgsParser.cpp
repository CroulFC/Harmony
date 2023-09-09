#include <string>

class Parameter{
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
};
