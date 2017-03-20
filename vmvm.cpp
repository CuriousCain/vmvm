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
  LDR,
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
  { "LDR", LDR },
	{ "HLT", HLT }
};

enum Registers {
	A, B, C, D, E, F,
  ACC,
	SP, IP,
	LEN
};

const std::unordered_map<std::string, Registers> register_set {
  { "A", A },
  { "B", B },
  { "C", C },
  { "D", D },
  { "E", E },
  { "F", F }
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

  for(auto current_line:program_lines) {
    std::stringstream ss;
    ss.str(current_line);
    std::string token;
    while(getline(ss, token, ' ')) {
      if(instruction_set.find(token) != instruction_set.end()) {
        program_tokens.push_back(instruction_set.at(token));
      } else {
        // TODO: Validate
        if(program_tokens.back() == LDR) {
          program_tokens.push_back(register_set.at(token));
        } else {
          program_tokens.push_back(std::stoi(token));
        }
      }
    }
  }

  return program_tokens;
}

void dump_registers() {
  for(auto& i:registers) {
    printf("RegVal: %d\n", i);
  }
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
        registers[ACC] = stack.back();
        stack.pop_back();
        registers[ACC] = stack.back() + registers[ACC];
        stack.pop_back();
        stack.push_back(registers[ACC]);
        break;
      case SUB:
        registers[ACC] = stack.back();
        stack.pop_back();
        registers[ACC] = stack.back() - registers[ACC];
        stack.pop_back();
        stack.push_back(registers[ACC]);
        break;
      case MUL:
        registers[ACC] = stack.back();
        stack.pop_back();
        registers[ACC] = stack.back() * registers[ACC];
        stack.pop_back();
        stack.push_back(registers[ACC]);
        break;
      case DIV:
        registers[ACC] = stack.back();
        stack.pop_back();
        registers[ACC] = stack.back() / registers[ACC];
        stack.pop_back();
        stack.push_back(registers[ACC]);
        break;
      case POP:
        printf("POP: %d\n", stack.back());
        stack.pop_back();
        break;
      case LDR:
        ++ip;
        registers[program[ip]] = program[ip+1];
        ++ip;
        break;
      case HLT:
        dump_registers();
        std::cin.get();
        exit(0);
        break;
      default: ;
    }
  }

  return 0;
}
