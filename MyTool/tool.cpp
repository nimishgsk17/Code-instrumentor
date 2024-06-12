#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include <unordered_set>

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

namespace {

class FunctionPrinterVisitor : public RecursiveASTVisitor<FunctionPrinterVisitor> {
public:
    explicit FunctionPrinterVisitor(Rewriter &R) : TheRewriter(R) {}

    bool VisitStmt(Stmt *S) {
        if (isa<CompoundStmt>(S)) {
            return true; // Handled in VisitCompoundStmt
        }

        if (!TheRewriter.getSourceMgr().isInSystemHeader(S->getBeginLoc())) {
            SourceLocation SL = S->getBeginLoc();
            if (SL.isValid() && !SL.isMacroID()) {
                unsigned Line = TheRewriter.getSourceMgr().getSpellingLineNumber(SL);
                if (LinesProcessed.insert(Line).second) {
                    std::string printStmt = "std::cout << \"Line " + std::to_string(Line) + " executed.\" << std::endl;";
                    TheRewriter.InsertTextBefore(SL, printStmt);
                }
            }
        }
        return true;
    }

    bool VisitCompoundStmt(CompoundStmt *S) {
        if (!TheRewriter.getSourceMgr().isInSystemHeader(S->getBeginLoc())) {
            for (auto it = S->body_begin(); it != S->body_end(); ++it) {
                Stmt *stmt = *it;
                TraverseStmt(stmt); // Ensure all statements are visited
            }
        }
        return true;
    }

    bool VisitFunctionDecl(FunctionDecl *FD) {
        if (FD->hasBody()) {
            Stmt *Body = FD->getBody();
            if (!isa<CompoundStmt>(Body)) {
                TraverseStmt(Body); // Explicitly visit the single statement body
            }
        }
        return true;
    }

    bool VisitCXXMethodDecl(CXXMethodDecl *MD) {
        if (MD->hasBody()) {
            Stmt *Body = MD->getBody();
            if (!isa<CompoundStmt>(Body)) {
                TraverseStmt(Body); // Explicitly visit the single statement body
            }
        }
        return true;
    }

    bool VisitForStmt(ForStmt *FS) {
        Stmt *Body = FS->getBody();
        if (Body && !isa<CompoundStmt>(Body)) {
            TraverseStmt(Body); // Explicitly visit the single statement body
        }
        return true;
    }

    bool VisitWhileStmt(WhileStmt *WS) {
        Stmt *Body = WS->getBody();
        if (Body && !isa<CompoundStmt>(Body)) {
            TraverseStmt(Body); // Explicitly visit the single statement body
        }
        return true;
    }

    bool VisitDoStmt(DoStmt *DS) {
        Stmt *Body = DS->getBody();
        if (Body && !isa<CompoundStmt>(Body)) {
            TraverseStmt(Body); // Explicitly visit the single statement body
        }
        return true;
    }

private:
    Rewriter &TheRewriter;
    std::unordered_set<unsigned> LinesProcessed;
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
