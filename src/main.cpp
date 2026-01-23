#include "ast/visitor.hpp"
#include "parser.hpp"

#include <iostream>
#include <lexer.hpp>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/SMLoc.h>
#include <llvm/Support/SourceMgr.h>
#include <print>
#include <system_error>

int
main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Expected some file bruh\n";
		return 1;
	}

	char*			filename = argv[1];
	llvm::SourceMgr src_mgr;

	auto buff = llvm::MemoryBuffer::getFile(filename);

	if (std::error_code err = buff.getError())
	{
		std::print("Could not open file: %s", filename);
		return 1;
	}

	src_mgr.AddNewSourceBuffer(std::move(*buff), llvm::SMLoc());

	Lexer lexer(&src_mgr);
	Token token;

	Parser parser(&lexer);
	AST	   tree;
	parser.Parse(tree);

	ASTPrinter printer(&tree);
	printer.VisitAll();

	return 0;
}
