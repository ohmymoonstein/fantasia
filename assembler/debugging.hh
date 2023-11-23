#pragma once

#include <iostream>
#include <fasm/ast.hh>

std::ostream& operator<<(std::ostream &, const std::shared_ptr<Program> &);
std::ostream& operator<<(std::ostream &, const std::shared_ptr<Variable> &);
std::ostream& operator<<(std::ostream &, const std::shared_ptr<Parameter> &);
std::ostream& operator<<(std::ostream &, const std::shared_ptr<Function> &);
std::ostream& operator<<(std::ostream &, const std::shared_ptr<Instruction> &);