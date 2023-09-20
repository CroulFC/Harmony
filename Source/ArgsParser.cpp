#include "ArgsParser.h"

#include <stdexcept>
#include <memory>
#include <string.h>

bool ArgumentsParser::Parameter::operator==(const Parameter &p){
	if(m_short_name != p.m_short_name){
		return false;
	}

	if(m_full_name != p.m_full_name){
		return false;
	}

	return true;
}

bool ArgumentsParser::Parameter::operator<(const Parameter &p){
	if(m_short_name >= p.m_short_name){
		return false;
	}

	if(m_full_name >= p.m_full_name){
		return false;
	}

	return true;
}

bool ArgumentsParser::Parameter::operator>(const Parameter &p){
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

void ArgumentsParser::AddParam(const Parameter &param){
	m_arguments[param] = std::pair<bool, const char*>{false, nullptr};
}

std::pair<bool, const char*> ArgumentsParser::FindValueOfParam(const ArgumentsParser::Parameter &param, const int index){
	std::pair<bool, const char*>  return_val{true, nullptr};

	if(index == m_argc - 1){
		return return_val;
	}

	std::string next_string = m_argv[index + 1];

	if(std::all_of(next_string.begin(), next_string.end(), ::isdigit)){
		return_val.second = m_argv[index + 1];
	}

	return return_val;
}

bool ArgumentsParser::IsParseResultCorrect(){
	for (auto p : m_arguments){
		if(  	p.first.GetValuePresence() == Parameter::VALUE_PRESENCE::REQUIRED
		  	&&	p.second.second == nullptr
		  )
		{
			return false;
		}

		if(  	p.first.GerParameterPresence() == Parameter::PARAM_PRESENCE::REQUIRED
		  	&&	p.second.first == false
		  )
		{
			return false;
		}
	}

	return true;
}

ArgumentsParser::PARSE_STATUS ArgumentsParser::Parse(){
	for(int index = 0; index < m_argc; ++index){

		if(m_argv[0][0] != '-'){
			return PARSE_STATUS::ERROR;
		}
		
		if(m_argv[index][0] != '-'){
			continue;
		}

		Parameter cur(	m_argv[index][1],
						std::string(m_argv[index]),
						Parameter::PARAM_PRESENCE::OPTIONAL,
						Parameter::VALUE_PRESENCE::OPTIONAL
						);

		auto matched_param = m_arguments.find(cur);
		if(matched_param == m_arguments.end()){
			continue;
		}

		matched_param->second = FindValueOfParam(matched_param->first, index);
	}

	if(IsParseResultCorrect()){
		return PARSE_STATUS::SUCCESS;
	}

	return PARSE_STATUS::ERROR;
}

std::pair<bool, const char*> ArgumentsParser::GetValueOfParameter(const Parameter &param){
	return m_arguments[param];
}
