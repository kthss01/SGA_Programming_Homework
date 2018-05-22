#pragma once
// Json.h

namespace Json {
	void GetValue(Json::Value& parent, string name, float& vec);
	void SetValue(Json::Value& parent, string name, float& vec);
}