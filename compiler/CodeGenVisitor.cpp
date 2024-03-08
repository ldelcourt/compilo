#include "CodeGenVisitor.h"

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx) 
{
    std::cout<< ".globl _main\n" ;
    std::cout<< " _main: \n" ;

    this->visitChildren(ctx);

    std::cout << "    ret\n";

    return 0;
}


antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    int retval = stoi(ctx->CONST()->getText());

    std::cout << "    movl $"<<retval<<", %eax\n" ;

    return 0;
}


antlrcpp::Any CodeGenVisitor::visitVar_declaration(ifccParser::Var_declarationContext *ctx)
{
    std::string declaredVarName = ctx->VARNAME(0)->getText();
    int declaredVarIndex = symbolTable.getSymbolIndex(declaredVarName);

    // Cas VARNAME = VARNAME
    if (ctx->VARNAME(1) != nullptr) {
        std::string assignedVarName = ctx->VARNAME(1)->getText();
        int assignedVarIndex = symbolTable.getSymbolIndex(assignedVarName);
        std::cout << "    movl " << assignedVarName << "(%rbp), %eax\n";
        std::cout << "    movl %eax, " << declaredVarIndex << "(%rbp)\n";
    }

    // Cas VARNAME = CONST
    if(ctx->CONST() != nullptr) {
        string tempVar = symbolTable.autoAddSymbol();
        int tempVarIndex = symbolTable.getSymbolIndex(tempVar);

        std::string assignedConst = ctx->CONST()->getText();

        std::cout << "    movl $" << assignedConst << ", " << tempVarIndex << "(%rbp)\n";
        std::cout << "    movl " << tempVarIndex << "(%rbp), %eax\n";
        std::cout << "    movl %eax, " << declaredVarIndex << "(%rbp)\n";

    }

    return 0;
}