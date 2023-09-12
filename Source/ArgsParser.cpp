#include "ArgsParser.h"

bool ArgumentsParser::Parameter::operator==(const Parameter& p){
    if(m_short_name != p.m_short_name){
        return false;
    }

    if(m_full_name != p.m_full_name){
        return false;
    }

    if(m_param_presence != p.m_param_presence){
        return false;
    }

    if(m_value_presence != p.m_value_presence){
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
