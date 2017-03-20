#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include <sstream>

enum Instructions {
	PSH, 
	ADD, 
	POP, 
	SET, 
  SUB,
  MUL,
  DIV,
	HLT
};

const std::unordered_map<std::string, Instructions> instruction_set {
	{ "PSH", PSH },
	{ "ADD", ADD },
	{ "POP", POP },
	{ "SET", SET },
  { "SUB", SUB },
  { "MUL", MUL },
  { "DIV", DIV },
	{ "HLT", HLT }
};

enum Registers {
	A, B, C, D, E, F,
	SP, IP,
	LEN
};

std::vector<int> stack;
std::vector<int> registers(LEN);

std::vector<int> parse_file() {
	std::vector<std::string> program_lines;
	std::ifstream source_file("run.vasm");
	std::string source_line;

	if(source_file.is_open()) {
		while (getline(source_file, source_line, '\n')) {
			program_lines.push_back(source_line);
		}
	}

	std::vector<int> program_tokens;
	std::for_each(begin(program_lines), end(program_lines), [&program_tokens](std::string s)
	{
		std::stringstream ss;
		ss.str(s);
		std::string token;
		while (getline(ss, token, ' ')) {
			if(instruction_set.find(token) != instruction_set.end()) {
				program_tokens.push_back(instruction_set.at(token));
			} else {
				// TODO: validate
				program_tokens.push_back(std::stoi(token));
			}
		}
	});

	return program_tokens;
}

int main()
{
	auto program = parse_file();

	for(size_t ip=0; ip != program.size(); ++ip) {
		switch (program[ip]) {
		case PSH:
			++ip;
			stack.push_back(program[ip]);
			break;
		case ADD:
			registers[B] = stack.back();
			stack.pop_back();
			registers[A] = stack.back();
			stack.pop_back();
			registers[C] = registers[A] + registers[B];
			stack.push_back(registers[C]);
			break;
    case SUB:
      registers[B] = stack.back();
      stack.pop_back();
      registers[A] = stack.back();
      stack.pop_back();
      registers[C] = registers[A] - registers[B];
      stack.push_back(registers[C]);
      break;
    case MUL:
      registers[B] = stack.back();
      stack.pop_back();
      registers[A] = stack.back();
      stack.pop_back();
      registers[C] = registers[A] * registers[B];
      stack.push_back(registers[C]);
      break;
    case DIV:
      registers[B] = stack.back();
      stack.pop_back();
      registers[A] = stack.back();
      stack.pop_back();
      registers[C] = registers[A] / registers[B];
      stack.push_back(registers[C]);
      break;
		case POP:
			registers[A] = stack.back();
			stack.pop_back();
			printf("%d\n", registers[A]);
			break;
		case HLT:
			std::cin.get();
			break;
			default: ;
		}
	}

	return 0;
}
