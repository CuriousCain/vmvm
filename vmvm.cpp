#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include <sstream>
#include <chrono>

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

void dump_stack() {
  for(auto& i:stack) {
    printf("StackVal: %d\n", i);
  }
}

std::vector<int> program;

void push() {
  ++registers[IP];
  stack.push_back(program[registers[IP]]);
}

void add() {
  registers[ACC] = stack.back();
  stack.pop_back();
  registers[ACC] = stack.back() + registers[ACC];
  stack.pop_back();
  stack.push_back(registers[ACC]);
}

void sub() {
  registers[ACC] = stack.back();
  stack.pop_back();
  registers[ACC] = stack.back() - registers[ACC];
  stack.pop_back();
  stack.push_back(registers[ACC]);
}

void mul() {
  registers[ACC] = stack.back();
  stack.pop_back();
  registers[ACC] = stack.back() * registers[ACC];
  stack.pop_back();
  stack.push_back(registers[ACC]);
}

void div() {
  registers[ACC] = stack.back();
  stack.pop_back();
  registers[ACC] = stack.back() / registers[ACC];
  stack.pop_back();
  stack.push_back(registers[ACC]);
}

void pop() {
  printf("POP: %d\n", stack.back());
  stack.pop_back();
}

void ldr() {
  ++registers[IP];
  registers[program[registers[IP]]] = program[registers[IP]+1];
  ++registers[IP];
}

void hlt() {
  dump_registers();
  //exit(0);
}

typedef void(*vasm_func)(void);
std::unordered_map<int, vasm_func> vasm_functions {
  {PSH, &push},
  {ADD, &add},
  {SUB, &sub},
  {MUL, &mul},
  {DIV, &div},
  {POP, &pop},
  {LDR, &ldr},
  {HLT, &hlt}
};

int main()
{
  program = parse_file();

  auto start = std::chrono::steady_clock::now();
  for(auto x=0;x<1001;++x){
    for(; registers[IP] != (int)program.size(); ++registers[IP]) {
      vasm_functions[program[registers[IP]]]();
    }
    registers[IP] = 0;
  }
  auto end = std::chrono::steady_clock::now();
  auto diff = end-start;
  std::cout << std::chrono::duration<double, std::milli> (diff).count() << "ms" << std::endl;

  return 0;
}

