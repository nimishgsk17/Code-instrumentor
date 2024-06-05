#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

namespace {

class FunctionPrinterVisitor : public RecursiveASTVisitor<FunctionPrinterVisitor> {
public:
    explicit FunctionPrinterVisitor(Rewriter &R) : TheRewriter(R) {}

    bool VisitStmt(Stmt *s) {
        // We avoid inserting before CompoundStmt to not clutter with braces.
        // We focus on more granular statements for precise insertions.
        if (!isa<CompoundStmt>(s)) {
            SourceLocation SL = s->getBeginLoc();

            if (SL.isValid() && !SL.isMacroID()) {
                unsigned currentLine = TheRewriter.getSourceMgr().getSpellingLineNumber(SL);
                // Ensuring we're not inserting multiple times on the same line
                if (linesProcessed.find(currentLine) == linesProcessed.end()) {
                    std::string printStmt = "std::cout << \"Statement executed at line: \" << __LINE__ << std::endl;";
                    TheRewriter.InsertTextBefore(SL, printStmt);
                    linesProcessed.insert(currentLine);
                }
            }
        }
        return true; // Continue traversal
    }
    

private:
    Rewriter &TheRewriter;
    std::set<unsigned> linesProcessed; // Tracks processed lines to avoid duplicates
};

class FunctionPrinterASTConsumer : public ASTConsumer {
public:
    explicit FunctionPrinterASTConsumer(Rewriter &R) : Visitor(R) {}

    void HandleTranslationUnit(ASTContext &Context) override {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }

private:
    FunctionPrinterVisitor Visitor;
};

class FunctionPrinterFrontendAction : public ASTFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<FunctionPrinterASTConsumer>(TheRewriter);
    }

    void EndSourceFileAction() override {
        TheRewriter.getEditBuffer(TheRewriter.getSourceMgr().getMainFileID()).write(llvm::outs());
    }

private:
    Rewriter TheRewriter;
};
} // namespace

static llvm::cl::OptionCategory MyToolCategory("function-printer options");

int main(int argc, const char **argv) {
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
    if (!ExpectedParser) {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }
    CommonOptionsParser &OptionsParser = ExpectedParser.get();
    ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

    return Tool.run(newFrontendActionFactory<FunctionPrinterFrontendAction>().get());
}
