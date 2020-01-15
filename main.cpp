#include <iostream>
#include <fstream>
#include <sstream>

std::string arg[3];
std::ifstream in;
int args = 0;
int line = 0;

bool is_arg(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	if (s.size() == 1 && *it == 'R') {
		return false;
	}
	while (it != s.end() && (std::isdigit(*it) || (it == s.begin() && (*it == 'R' || *it == '-')))) ++it;
	return !s.empty() && it == s.end();
}

bool is_reg(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	if (s.size() != 2 || *it != 'R') {
		return false;
	}
	while (it != s.end() && (std::isdigit(*it) || (it == s.begin() && *it == 'R'))) ++it;
	return !s.empty() && it == s.end();
}

bool argument_test() {
	char c;
	in.get(c);
	if (c == '\n') {
		return false;
	}
	in.unget();
	return true;
}

bool get_args() {
	for (int i = 0; i < 3; i++) {
		if (argument_test()) {
			in >> arg[i];
			if (!is_arg(arg[i])) {
				std::cout << "Error - Unexpected Argument '" << arg[i] << "' at line " << line << std::endl;
				return false;
			}
			std::cout << " Argument " << arg[i] << std::endl;
		}
		else {
			return true;
		}
		args++;
	}
	return true;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		return 1;
	}
	else {
		in = std::ifstream(argv[1]);
		std::ofstream out("assembly.txt");
		if (!in.good()) {
			return 1;
		}
		else {
			std::string chars;
			std::string opcode = "";
			line = 0;
			uint16_t code = 0;
			while (in >> chars) {
				args = 0;
				line++;
				std::cout << "Opcode " << chars << std::endl;
				if (!get_args()) {
					in.close();
					out.close();
					return 1;
				} else if (chars == "ADD") {
					if (args == 3 && is_reg(arg[0]) && is_reg(arg[1])) {
						if (is_reg(arg[2])) {
							code = 2048 + ((uint16_t)(arg[0][1] - '0') << 8) + ((uint16_t)(arg[1][1] - '0') << 5) + (arg[2][1] - '0');
						}
						else {
							code = 2048 + ((uint16_t)(arg[0][1] - '0') << 8) + ((uint16_t)(arg[1][1] - '0') << 5) + ((uint16_t)std::stoi(arg[2]) & 0b1111) + 16;
						}
						std::cout << code << std::endl;
						out << code;
					}
					else {
						std::cout << "Error - Invalid Arguments at line " << line << std::endl;
						break;
					}
				}
				else if (chars == "SUB") {

				}
				else if (chars == "MUL") {
				
				}
				else if (chars == "DIV") {

				}
				else if (chars == "INC") {

				}
				else if (chars == "DEC") {

				}
				else if (chars == "SHR") {

				}
				else if (chars == "SHL") {

				}
				else if (chars == "NOT") {

				}
				else if (chars == "OR") {

				}
				else if (chars == "AND") {

				}
				else if (chars == "XOR") {

				}
				else if (chars == "LDR") {

				}
				else if (chars == "LD") {

				}
				else if (chars == "LDI") {

				}
				else if (chars == "ST") {

				}
				else if (chars == "STI") {

				}
				else if (chars == "BRn") {

				}
				else if (chars == "BRz") {

				}
				else if (chars == "BRp") {

				}
				else if (chars == "BRnz") {

				}
				else if (chars == "BRzp") {

				}
				else if (chars == "BRnzp") {

				}
				else if (chars == "BRnzp") {

				}
				else if (chars == "JMP") {

				}
				else if (chars == "JSR") {

				}
				else if (chars == "JSRR") {

				}
				else if (chars == "RET") {

				}
				else if (chars == "CLR") {

				}
				else if (chars == "IN") {

				}
				else if (chars == "OUT") {

				}
				else {
					std::cout << "Error - Unknown Opcode '" << chars << "' at line " << line << std::endl;
					in.close();
					out.close();
					return 1;
				}
			}
			out.close();
		}
	}
	return 0;
}