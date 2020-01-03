#ifndef NREL_SPAWN_COMPILER_HPP
#define NREL_SPAWN_COMPILER_HPP

#include "JIT.hpp"
#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include <boost/filesystem.hpp>

namespace nrel {

class Compiler
{
public:
  static std::string get_target_triple()
  {
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    return llvm::sys::getDefaultTargetTriple();
  }

  static const llvm::Target *get_target(const std::string &target_triple)
  {
    std::string error;
    const llvm::Target *target = llvm::TargetRegistry::lookupTarget(target_triple, error);
    if (!target) {
      throw std::runtime_error("Unable to get target: " + error);
    }
    return target;
  }

  static std::unique_ptr<llvm::Module> initialize_module(llvm::orc::ThreadSafeContext ctx, llvm::TargetMachine *target_machine)
  {
    assert(ctx.getContext());
    auto module = llvm::make_unique<llvm::Module>("Module", *ctx.getContext());
    module->setDataLayout(target_machine->createDataLayout());
    return module;
  }

  static std::string get_CPU()
  {
    return "generic";
  }

  static std::string get_features()
  {
    return "";
  }

  static llvm::TargetOptions get_OPT()
  {
    return llvm::TargetOptions{};
  }

  static llvm::Optional<llvm::Reloc::Model> get_reloc_model()
  {
    return {};
  }

  static llvm::TargetMachine *get_target_machine(const llvm::Target *target,
                                                 const std::string &triple,
                                                 const std::string &cpu,
                                                 const std::string &features,
                                                 const llvm::TargetOptions opt,
                                                 const llvm::Optional<llvm::Reloc::Model> &reloc_model)
  {
    llvm::TargetMachine *machine = target->createTargetMachine(triple, cpu, features, opt, reloc_model);
    //    machine->
    return machine;
  }

  explicit Compiler(std::vector<boost::filesystem::path> include_paths, std::vector<std::string> flags)
      : m_include_paths{std::move(include_paths)}, m_flags{std::move(flags)}
  {
  }

  void compile_and_link(const boost::filesystem::path &source);

  std::unique_ptr<llvm::Module> take_compilation()
  {
    return std::move(m_currentCompilation);
  }

  auto context()
  {
    return m_context;
  }

  auto move_to_jit()
  {
    auto jit = llvm::orc::SimpleJIT::Create();
    if (!jit) {
      throw std::runtime_error("Some error happened, but it's too much of a pain to get it out of the Expected object");
    }
    move_to_jit(*jit.get());
    return std::move(*jit);
  }

  void move_to_jit(llvm::orc::SimpleJIT &jit)
  {
    jit.addModule(take_compilation(), context());
  }

  void write_bitcode(const boost::filesystem::path &loc);
  void write_object_file(const boost::filesystem::path &loc);

private:
  std::string m_target_triple{get_target_triple()};
  const llvm::Target *m_target{get_target(m_target_triple)};
  llvm::TargetMachine *m_target_machine{get_target_machine(m_target, m_target_triple, get_CPU(), get_features(), get_OPT(), get_reloc_model())};
  std::vector<boost::filesystem::path> m_include_paths;
  std::vector<std::string> m_flags;
  llvm::orc::ThreadSafeContext m_context{std::make_unique<llvm::LLVMContext>()};
  std::unique_ptr<llvm::Module> m_currentCompilation{initialize_module(m_context, m_target_machine)};

  static std::unique_ptr<llvm::Module> compile(const boost::filesystem::path &source,
                                               llvm::LLVMContext &ctx,
                                               const std::vector<boost::filesystem::path> &include_paths,
                                               const std::vector<std::string> &flags);
};

} // namespace nrel

#endif
