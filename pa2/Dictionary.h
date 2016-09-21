#pragma once
#include <unordered_map>
#include <string>

class Dictionary
{
public:
	Dictionary();
	~Dictionary();
	void Load(std::ifstream& fileName);
	std::string* FindMatch(std::string& key);
private:
	std::unordered_map<std::string, std::string> mDictionaryMap;
};