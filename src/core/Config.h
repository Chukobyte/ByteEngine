#ifndef CONFIG_H
#define CONFIG_H

class Config {};

class AssetConfig : public Config {
	public:
		std::string type;
		std::string id;
		std::string filePath;
		int fontSize;
		AssetConfig(std::string type, std::string id, std::string filePath, int fontSize) {
			this->type = type;
			this->id = id;
			this->filePath = filePath;
			this->fontSize = fontSize;
		}
};

class InputConfig : public Config {
	public:
		std::string type;
		std::string actionName;
		std::string inputValue;
		InputConfig(std::string type, std::string actionName, std::string inputValue) {
			this->type = type;
			this->actionName = actionName;
			this->inputValue = inputValue;
		}
};
#endif
