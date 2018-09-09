#include "CommandLineParser.h"

CCommandLineParser::data_t CCommandLineParser::Parse(int argc, const char * argv[])
{
  std::vector<std::string> args(argc);

  for (int i = 0; i < argc; i++)
    args[i] = argv[i];

  return Parse(args);
}

CCommandLineParser::data_t CCommandLineParser::Parse(std::vector<std::string>& args)
{
  data_t result;

  for (auto it = args.begin(); it != args.end(); it++)
  {
    ArgumentType_e argumentType = GetArgumentType(*it);
    switch (argumentType)
    {
    case CCommandLineParser::AT_FONT_NAME:
    {
      it++;
      result.fontName = GetStringValue(*it);
      break;
    }
    case CCommandLineParser::AT_OUT_FILENAME:
    {
      it++;
      result.outFileName = GetStringValue(*it);
      break;
    }
    case CCommandLineParser::AT_FONT_HEIGHT:
    {
      it++;
      result.fontHeight = GetIntegerValue(*it);
      break;
    }
    case CCommandLineParser::AT_UNDEFINED:
    default:
      break;
    }
  }

  return result;
}

CCommandLineParser::ArgumentType_e CCommandLineParser::GetArgumentType(const std::string& argument)
{
  if (argument == "-fontname")
    return AT_FONT_NAME;
  if (argument == "-outfile")
    return AT_OUT_FILENAME;
  if (argument == "-fontheight")
    return AT_FONT_HEIGHT;

  return AT_UNDEFINED;
}

std::string CCommandLineParser::GetStringValue(const std::string& value)
{
  return value;
}

int CCommandLineParser::GetIntegerValue(const std::string & value)
{
  return atoi(value.c_str());
}
