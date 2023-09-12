#include "ArgsParser.h"

#include <stdexcept>

bool ArgumentsParser::Parameter::operator==(const Parameter& p){
	if(m_short_name != p.m_short_name){
		return false;
	}

	if(m_full_name != p.m_full_name){
		return false;
	}

	return true;
}

bool ArgumentsParser::Parameter::operator<(const Parameter& p){
	if(m_short_name >= p.m_short_name){
		return false;
	}

	if(m_full_name >= p.m_full_name){
		return false;
	}

	return true;
}

bool ArgumentsParser::Parameter::operator>(const Parameter& p){
	return !(operator==(p) || operator<(p));
}

ArgumentsParser::Parameter::Parameter(char short_name, const std::string& full_name, PARAM_PRESENCE parameter_presence, VALUE_PRESENCE value_presence){
	if(short_name == Parameter::EMPTY_SHORT_NAME && full_name == Parameter::EMPTY_FULL_NAME){
		throw std::invalid_argument("Both names (short and full) are empty");
	}

	m_short_name = short_name;
	m_full_name = full_name;
	m_param_presence = parameter_presence;
	m_value_presence = value_presence;
}

ArgumentsParser::ArgumentsParser(int argc, char* argv[]){
	if(argc < 1){
		throw std::invalid_argument("argc < 1");
	}

	if(!argv){
		throw std::invalid_argument("argv is nullptr");
	}

	m_argc = argc;
	m_argv = argv;

	m_parse_status = PARSE_STATUS::NOT_PARSED;
	m_arguments = std::map<Parameter, std::pair<bool, const char*>>{};
}

void ArgumentsParser::AddParam(const Parameter& param){
	m_arguments[param] = std::pair<bool, const char*>{false, nullptr};
}

std::pair<bool, const char*> ArgumentsParser::FindValueOfParam(const Parameter& param){

	std::pair<bool, const char*> result{false, nullptr};

	//Start from 1 to skip programm name
	for(uint32_t i = 1; i < m_argc; ++i){
		if	(//! (param.CompareFullName() 
			 //|| param.CompareShortName())
			)
		{
			continue;
		}

		result.first = true;

		if(/*param.GetValuePresence() == Parameter::VALUE_PRESENCE::NOT_REQUIRED*/){
			result.second = nullptr;
			return result;
		}

		//check if value missing when it optional or required
		if(    (i == m_argc - 1)
		  	|| (m_argv[i+1][0] == '-')
		  )
		{
			result.second = nullptr;
			return result;
		}

		result.second = m_argv[i + 1];
		return result;
	}

	return result;
}

ArgumentsParser::PARSE_STATUS ArgumentsParser::Parse(){
	m_parse_status = PARSE_STATUS::ERROR;

	for(auto cur_pair : m_arguments){
			cur_pair.second = FindValueOfParam(cur_pair.first);
	}
}
