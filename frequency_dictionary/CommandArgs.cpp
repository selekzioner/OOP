#include "CommandArgs.hpp"

struct CommandArgs::CommandArgs CommandArgs::ParseCommandArgs(int argc, char* argv[]) {
	struct CommandArgs::CommandArgs args;
	if (argc > 1) {
		args.InputFileName = argv[1];
	}
	else {
		args.InputFileName = "input.txt";
	}
	if (argc > 2) {
		args.OutputFileName = argv[2];
	}
	else {
		args.OutputFileName = "output.txt";
	}
	return args;
}