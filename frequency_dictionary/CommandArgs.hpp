#pragma once
#include <string>
#include <fstream>

namespace CommandArgs {
	struct CommandArgs {
		std::string InputFileName;
		std::string OutputFileName;
	};

	struct CommandArgs ParseCommandArgs(int argc, char* argv[]);
}