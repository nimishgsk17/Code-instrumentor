#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "clang/Frontend/CompilerInstance.h"

#include <iostream>

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

class BlockVisitor : public MatchFinder::MatchCallback {
public:
  BlockVisitor(Rewriter &R) : Rewrite(R) {}

  virtual void run(const MatchFinder::MatchResult &Result) {
    SourceManager &SM = *Result.SourceManager;

    if (const CompoundStmt *CS = Result.Nodes.getNodeAs<CompoundStmt>("block")) {
      unsigned int lineNo = SM.getSpellingLineNumber(CS->getBeginLoc());
      std::string coutStmt = "std::cout << \"Visiting block at line: " + std::to_string(lineNo) + "\" << std::endl; ";
      SourceLocation loc = CS->getBeginLoc().getLocWithOffset(1);
      Rewrite.InsertText(loc, coutStmt, true, true);
    } else if (const IfStmt *If = Result.Nodes.getNodeAs<IfStmt>("ifStmt")) {
      unsigned int lineNo = SM.getSpellingLineNumber(If->getBeginLoc());
      std::string coutStmt = "std::cout << \"Visiting if statement at line: " + std::to_string(lineNo) + "\" << std::endl; ";
      SourceLocation loc = If->getBeginLoc();
      Rewrite.InsertText(loc, coutStmt, true, true);
    } else if (const ForStmt *For = Result.Nodes.getNodeAs<ForStmt>("forStmt")) {
      unsigned int lineNo = SM.getSpellingLineNumber(For->getBeginLoc());
      std::string coutStmt = "std::cout << \"Visiting for loop at line: " + std::to_string(lineNo) + "\" << std::endl; ";
      SourceLocation loc = For->getBeginLoc();
      Rewrite.InsertText(loc, coutStmt, true, true);
    } else if (const WhileStmt *While = Result.Nodes.getNodeAs<WhileStmt>("whileStmt")) {
      unsigned int lineNo = SM.getSpellingLineNumber(While->getBeginLoc());
      std::string coutStmt = "std::cout << \"Visiting while loop at line: " + std::to_string(lineNo) + "\" << std::endl; ";
      SourceLocation loc = While->getBeginLoc();
      Rewrite.InsertText(loc, coutStmt, true, true);
    } else if (const DoStmt *Do = Result.Nodes.getNodeAs<DoStmt>("doStmt")) {
      unsigned int lineNo = SM.getSpellingLineNumber(Do->getBeginLoc());
      std::string coutStmt = "std::cout << \"Visiting do-while loop at line: " + std::to_string(lineNo) + "\" << std::endl; ";
      SourceLocation loc = Do->getBeginLoc();
      Rewrite.InsertText(loc, coutStmt, true, true);
    } else if (const FunctionDecl *Func = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
      if (Func->hasBody()) {
        unsigned int lineNo = SM.getSpellingLineNumber(Func->getBody()->getBeginLoc());
        std::string coutStmt = "std::cout << \"Visiting function '" + Func->getNameInfo().getName().getAsString() + "' at line: " + std::to_string(lineNo) + "\" << std::endl; ";
        SourceLocation loc = Func->getBody()->getBeginLoc().getLocWithOffset(1);
        Rewrite.InsertText(loc, coutStmt, true, true);
      }
    } else if (const CXXConstructorDecl *Ctor = Result.Nodes.getNodeAs<CXXConstructorDecl>("constructor")) {
      if (Ctor->hasBody()) {
        unsigned int lineNo = SM.getSpellingLineNumber(Ctor->getBody()->getBeginLoc());
        std::string coutStmt = "std::cout << \"Visiting constructor '" + Ctor->getNameAsString() + "' at line: " + std::to_string(lineNo) + "\" << std::endl; ";
        SourceLocation loc = Ctor->getBody()->getBeginLoc().getLocWithOffset(1);
        Rewrite.InsertText(loc, coutStmt, true, true);
      }
    }
  }

private:
  Rewriter &Rewrite;
};

class BlockVisitorAction : public ASTFrontendAction {
public:
  virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
    RewriterForFile.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    MatchFinder *Finder = new MatchFinder();
    BlockVisitor *Visitor = new BlockVisitor(RewriterForFile);

    Finder->addMatcher(compoundStmt().bind("block"), Visitor);
    Finder->addMatcher(ifStmt().bind("ifStmt"), Visitor);
    Finder->addMatcher(forStmt().bind("forStmt"), Visitor);
    Finder->addMatcher(whileStmt().bind("whileStmt"), Visitor);
    Finder->addMatcher(doStmt().bind("doStmt"), Visitor);
    Finder->addMatcher(functionDecl(isDefinition()).bind("function"), Visitor);
    Finder->addMatcher(cxxConstructorDecl(isDefinition()).bind("constructor"), Visitor);

    return Finder->newASTConsumer();
  }

  void EndSourceFileAction() override {
    SourceManager &SM = RewriterForFile.getSourceMgr();
    llvm::errs() << "** EndSourceFileAction for: "
                 << SM.getFileEntryForID(SM.getMainFileID())->tryGetRealPathName() << "\n";
    RewriterForFile.getEditBuffer(SM.getMainFileID()).write(llvm::outs());
  }

private:
  Rewriter RewriterForFile;
};

static llvm::cl::OptionCategory MyToolCategory("block-visitor options");

int main(int argc, const char **argv) {
  auto OptionsParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
  if (!OptionsParser) {
    llvm::errs() << OptionsParser.takeError();
    return 1;
  }

  ClangTool Tool(OptionsParser->getCompilations(), OptionsParser->getSourcePathList());

  return Tool.run(newFrontendActionFactory<BlockVisitorAction>().get());
}
