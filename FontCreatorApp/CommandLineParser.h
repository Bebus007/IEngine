#pragma once

#include <string>
#include <vector>

class CCommandLineParser
{
public:
  struct data_t
  {
    std::string fontName;
    int fontHeight;
    std::string outFileName;
  };

  static data_t Parse(int argc, const char* argv[]);
  static data_t Parse(std::vector<std::string>& args);
private:
  enum ArgumentType_e
  {
    AT_FONT_NAME,
    AT_FONT_HEIGHT,
    AT_OUT_FILENAME,
    AT_UNDEFINED
  };

  static ArgumentType_e GetArgumentType(const std::string& argument);
  static std::string GetStringValue(const std::string& value);
  static int GetIntegerValue(const std::string& value);
};