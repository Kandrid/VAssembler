#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>

std::string arg[3];
std::ifstream in;
int args = 0;
int line = 0;
bool success = true;

bool is_arg(const std::string& s)
{
	if (s[0] == '#' && s[s.size() - 1] == '#' && s.size() > 2) {
		return true;
	}
	std::string::const_iterator it = s.begin();
	if (s.size() != 2 && ::toupper(*it) == 'R') {
		return false;
	}
	while (it != s.end() && (std::isdigit(*it) || (it == s.begin() && (::toupper(*it) == 'R' || *it == '-')))) ++it;
	return !s.empty() && it == s.end();
}

bool is_str(const std::string& s) {
	return (s[0] == '#' && s[s.size() - 1] == '#' && s.size() > 2);
}

bool is_reg(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	if (s.size() != 2 || ::toupper(*it) != 'R') {
		return false;
	}
	while (it != s.end() && (std::isdigit(*it) || (it == s.begin() && ::toupper(*it) == 'R'))) ++it;
	return !s.empty() && it == s.end();
}

bool is_num(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && (std::isdigit(*it) || (it == s.begin() && *it == '-'))) ++it;
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
	args = 0;
	for (int i = 0; i < 3; i++) {
		if (argument_test()) {
			in >> arg[i];
			if (!is_arg(arg[i])) {
				std::cout << "Error - Unexpected Argument '" << arg[i] << "' at line " << line << std::endl;
				return false;
			}
		}
		else {
			return true;
		}
		args++;
	}
	return true;
}

void arg_error() {
	std::cout << "Error - Invalid Arguments at line " << line << std::endl;
	success = false;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		return 1;
	}
	else {
		in = std::ifstream(argv[1]);
		std::ofstream out("data2.bin", std::ios_base::binary);
		std::ofstream out_t("data.txt");
		if (!in.good()) {
			return 1;
		}
		else {
			std::string chars;
			std::string opcode = "";
			line = 0;
			uint16_t code = 0;
			while (in >> chars) {
				transform(chars.begin(), chars.end(), chars.begin(), ::toupper);
				bool single = true;
				args = 0;
				line++;
				if (!get_args()) {
					in.close();
					out.close();
					out_t.close();
					return 1;
				}
				else if (chars == "ADD") {
					if (args == 3 && is_reg(arg[0]) && is_reg(arg[1])) {
						if (is_reg(arg[2])) {
							code = (1 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5) + (arg[2][1] - '0');
						}
						else {
							code = (1 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5) + ((uint16_t)std::stoi(arg[2]) & 0b1111) + 16;
						}
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "SUB") {
					if (args == 3 && is_reg(arg[0]) && is_reg(arg[1])) {
						if (is_reg(arg[2])) {
							code = (2 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5) + (arg[2][1] - '0');
						}
						else {
							code = (2 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5) + ((uint16_t)std::stoi(arg[2]) & 0b1111) + 16;
						}
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "MUL") {
					if (args == 3 && is_reg(arg[0]) && is_reg(arg[1])) {
						if (is_reg(arg[2])) {
							code = (3 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5) + (arg[2][1] - '0');
						}
						else {
							code = (3 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5) + ((uint16_t)std::stoi(arg[2]) & 0b1111) + 16;
						}
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "DIV") {
					if (args == 3 && is_reg(arg[0]) && is_reg(arg[1])) {
						if (is_reg(arg[2])) {
							code = (4 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5) + (arg[2][1] - '0');
						}
						else {
							code = (4 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5) + ((uint16_t)std::stoi(arg[2]) & 0b1111) + 16;
						}
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "INC") {
					if (args == 2 && is_reg(arg[0]) && is_num(arg[1])) {
						code = (5 << 11) + ((arg[0][1] - '0') << 8) + ((uint16_t)std::stoi(arg[1]) & 0b1111) + 16;
					}
					else if (args == 1 && is_reg(arg[0])) {
						code = (5 << 11) + ((arg[0][1] - '0') << 8);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "DEC") {
					if (args == 2 && is_reg(arg[0]) && is_num(arg[1])) {
						code = (6 << 11) + ((arg[0][1] - '0') << 8) + ((uint16_t)std::stoi(arg[1]) & 0b1111) + 16;
					}
					else if (args == 1 && is_reg(arg[0])) {
						code = (6 << 11) + ((arg[0][1] - '0') << 8);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "SHR") {
					if (args == 2 && is_reg(arg[0]) && is_reg(arg[1])) {
						code = (7 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "SHL") {
					if (args == 2 && is_reg(arg[0]) && is_reg(arg[1])) {
						code = (8 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "NOT") {
					if (args == 2 && is_reg(arg[0]) && is_reg(arg[1])) {
						code = (9 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "OR") {
					if (args == 3 && is_reg(arg[0]) && is_reg(arg[1])) {
						if (is_reg(arg[2])) {
							code = (10 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5) + (arg[2][1] - '0');
						}
						else {
							code = (10 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5) + ((uint16_t)std::stoi(arg[2]) & 0b1111) + 16;
						}
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "AND") {
					if (args == 3 && is_reg(arg[0]) && is_reg(arg[1])) {
						if (is_reg(arg[2])) {
							code = (11 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5) + (arg[2][1] - '0');
						}
						else {
							code = (11 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5) + ((uint16_t)std::stoi(arg[2]) & 0b1111) + 16;
						}
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "XOR") {
					if (args == 3 && is_reg(arg[0]) && is_reg(arg[1])) {
						if (is_reg(arg[2])) {
							code = (12 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5) + (arg[2][1] - '0');
						}
						else {
							code = (12 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5) + ((uint16_t)std::stoi(arg[2]) & 0b1111) + 16;
						}
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "LDR") {
					if (args == 2 && is_reg(arg[0]) && is_reg(arg[1])) {
						code = (13 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "LD") {
					if (args == 2 && is_reg(arg[0]) && is_num(arg[1])) {
						code = (14 << 11) + ((arg[0][1] - '0') << 8) + ((uint16_t)std::stoi(arg[1]) & 0xff);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "LDI") {
					if (args == 2 && is_reg(arg[0]) && is_num(arg[1])) {
						code = (15 << 11) + ((arg[0][1] - '0') << 8) + ((uint16_t)std::stoi(arg[1]) & 0xff);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "ST") {
					if (args == 2 && is_reg(arg[0]) && is_num(arg[1])) {
						code = (16 << 11) + ((arg[0][1] - '0') << 8) + ((uint16_t)std::stoi(arg[1]) & 0xff);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "STI") {
					if (args == 2 && is_reg(arg[0]) && is_num(arg[1])) {
						code = (17 << 11) + ((arg[0][1] - '0') << 8) + ((uint16_t)std::stoi(arg[1]) & 0xff);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "STR") {
					if (args == 2 && is_reg(arg[0]) && is_reg(arg[1])) {
						code = (18 << 11) + ((arg[0][1] - '0') << 8) + ((arg[1][1] - '0') << 5);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "BRN") {
					if (args == 1 && is_num(arg[0])) {
						code = (19 << 11) + (4 << 8) +((uint16_t)std::stoi(arg[0]) & 0xff);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "BRZ") {
					if (args == 1 && is_num(arg[0])) {
						code = (19 << 11) + (2 << 8) + ((uint16_t)std::stoi(arg[0]) & 0xff);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "BRP") {
					if (args == 1 && is_num(arg[0])) {
						code = (19 << 11) + (1 << 8) + ((uint16_t)std::stoi(arg[0]) & 0xff);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "BRNZ") {
					if (args == 1 && is_num(arg[0])) {
						code = (19 << 11) + (6 << 8) + ((uint16_t)std::stoi(arg[0]) & 0xff);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "BRZP") {
					if (args == 1 && is_num(arg[0])) {
						code = (19 << 11) + (3 << 8) + ((uint16_t)std::stoi(arg[0]) & 0xff);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "BRNP") {
					if (args == 1 && is_num(arg[0])) {
						code = (19 << 11) + (5 << 8) + ((uint16_t)std::stoi(arg[0]) & 0xff);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "BRNZP") {
					if (args == 1 && is_num(arg[0])) {
						code = (19 << 11) + (7 << 8) + ((uint16_t)std::stoi(arg[0]) & 0xff);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "JMP") {
					if (args == 1) {
						if (is_reg(arg[0])) {
							code = (20 << 11) + ((arg[0][1] - '0') << 5);
						}
						else {
							code = (20 << 11) + ((uint16_t)std::stoi(arg[0]) & 0b1111) + 16;
						}
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "JSR") {
					if (args == 1 && is_num(arg[0])) {
						code = (21 << 11) + (4 << 8) + ((uint16_t)std::stoi(arg[0]) & 0x3ff);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "JSRR") {
					if (args == 1 && is_reg(arg[0])) {
						code = (22 << 11) + ((arg[0][1] - '0') << 5);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "RET") {
					if (args == 0) {
						code = (23 << 11) + (7 << 5);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "CLR") {
					if (args == 1 && is_reg(arg[0])) {
						code = (24 << 11) + ((arg[0][1] - '0') << 8);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "IN") {
					if (args == 1 && is_reg(arg[0])) {
						code = (25 << 11) + ((arg[0][1] - '0') << 8);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "OUT") {
					if (args == 1 && is_reg(arg[0])) {
						code = (26 << 11) + ((arg[0][1] - '0') << 5);
					}
					else if (args == 2 && is_reg(arg[0]) && arg[1] == "1") {
						code = (26 << 11) + (1 << 10) + ((arg[0][1] - '0') << 5);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "PUT") {
					if (args == 1 && is_reg(arg[0])) {
						code = (26 << 11) + (2 << 8) + ((arg[0][1] - '0') << 5);
					}
					else if (args == 2 && is_reg(arg[0]) && arg[1] == "1") {
						code = (26 << 11) + (1 << 10) + (2 << 8) + ((arg[0][1] - '0') << 5);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "SET") {
					if (args == 2 && is_reg(arg[0]) && is_num(arg[1])) {
						code = (27 << 11) + ((arg[0][1] - '0') << 8) + ((uint16_t)std::stoi(arg[1]) & 0xff);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "GETS") {
					if (args == 1 && is_reg(arg[0])) {
						code = (28 << 11) + ((arg[0][1] - '0') << 5);
					}
					else {
						arg_error();
						break;
					}
				}
				else if (chars == "PUTS") {
					if (args == 1 && is_reg(arg[0])) {
						code = (29 << 11) + (2 << 8) + ((arg[0][1] - '0') << 5);
					}
					else if (args == 2 && is_reg(arg[0]) && arg[1] == "1") {
						code = (29 << 11) + (1 << 10) + ((arg[0][1] - '0') << 5);
					}
					else {
						arg_error();
						break;
					}
				}
				else {
					std::cout << "Error - Unknown Opcode '" << chars << "' at line " << line << std::endl;
					in.close();
					out.close();
					out_t.close();
					success = false;
					return 1;
				}
				if (single) {
					out.write((char*)&code, sizeof(code));
					out_t << std::bitset<16>(code).to_string() << " " << std::hex << code << std::endl;
				}
			}
			out.close();
			out_t.close();
			if (success) {
				std::cout << "Compiled binary to 'data.bin'" << std::endl;
				std::cout << "Compiled text to 'data.txt'" << std::endl;
			}
		}
	}
	return 0;
}