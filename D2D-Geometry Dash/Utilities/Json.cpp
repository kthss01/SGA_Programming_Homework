#include "stdafx.h"
#include "Json.h"

void Json::GetValue(Json::Value & parent, string name, float & vec)
{
	vec = parent[name.c_str()].asFloat();
}

void Json::SetValue(Json::Value & parent, string name, float & vec)
{
	parent[name.c_str()] = vec;
}

void Json::GetValue(Json::Value & parent, string name, int & vec)
{
	vec = parent[name.c_str()].asInt();
}

void Json::SetValue(Json::Value & parent, string name, int & vec)
{
	parent[name.c_str()] = vec;
}
