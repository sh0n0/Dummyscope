#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

#include <iostream>
#include <memory>
#include <string>

#include "codegen.hpp"
#include "parser.hpp"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Just one file name is allowed as input" << std::endl;
    return 1;
  }

  std::string input_file_name = argv[1];

  // parse
  auto parser = std::make_unique<Parser>(input_file_name);
  if (!parser->parse()) {
    std::cout << "parse failed" << std::endl;
    return 1;
  }

  // generate
  auto generator = std::make_unique<CodeGen>();
  if (!generator->generate(parser->getTranslationUnitAST())) {
    std::cout << "generate failed" << std::endl;
    return 1;
  }

  auto module = generator->getModule();
  //  module->print(llvm::errs(), nullptr);

  // output executable file

  // setup
  llvm::InitializeNativeTarget();
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  std::string target_triple = llvm::sys::getDefaultTargetTriple();
  std::string err;
  const llvm::Target* target = llvm::TargetRegistry::lookupTarget(llvm::sys::getDefaultTargetTriple(), err);
  if (!target) {
    std::cerr << "Failed to lookup target " + target_triple + ": " + err;
    return 1;
  }

  llvm::TargetOptions opt;
  llvm::TargetMachine* target_machine =
      target->createTargetMachine(target_triple, "generic", "", opt, llvm::Optional<llvm::Reloc::Model>());
  module->setTargetTriple(target_triple);
  module->setDataLayout(target_machine->createDataLayout());

  std::string tmp_output_file_name = "tmp_output.o";
  std::error_code err_code;
  llvm::raw_fd_ostream output_file(tmp_output_file_name, err_code, llvm::sys::fs::OpenFlags::F_None);
  if (err_code) {
    std::cerr << "Could not open file: " << err_code.message();
    return 1;
  }

  llvm::legacy::PassManager passManager;
  if (target_machine->addPassesToEmitFile(passManager, output_file, nullptr, llvm::CGFT_ObjectFile)) {
    std::cerr << "target_machine can't emit a file of this type\n";
    return 1;
  }
  passManager.run(*module);
  output_file.close();

  // link builtin function and generated code, and output executable file
  std::vector<std::string> commands;
  int path_idx = input_file_name.find_last_of('/') + 1;
  int ext_idx = input_file_name.find_last_of('.');
  std::string executable_file_name = input_file_name.substr(path_idx, ext_idx - path_idx);
  commands.push_back(std::string("gcc ") + std::string("./lib/builtin.o ") + tmp_output_file_name + std::string(" -o") +
                     executable_file_name);
  commands.push_back(std::string("rm " + tmp_output_file_name));
  for (auto& command : commands) {
    std::system(command.c_str());
  }
  return 0;
}
